/*-----------------------------------------------------------------------------
  Marvin - A forward-chaining heuristic-search planner

  $Date: 2004/04/26 15:00:00 $
  $Revision: 1.0.0.0 $

  Andrew Coles and Amanda Smith
  
  andrew.coles@cis.strath.ac.uk
  amanda.smith@cis.strath.ac.uk
  
  Parser courtesy of Stephen Cresswell (stephen.cresswell@cis.strath.ac.uk)
  
    By releasing this code we imply no warranty as to its reliability
    and its use is entirely at your own risk.

  Strathclyde Planning Group
  http://planning.cis.strath.ac.uk/
 ----------------------------------------------------------------------------*/
#include "Action.h"
#include "ActionInstance.h"
#include "staticinformation.h"
#include "State.h"
#include "PredicateInstance.h"

#include "hashdefinitions.h"

#include "ActionSequenceValidator.h"

#include <list>
#include <ext/slist>

using std::list;


const int Action::intIsOne = 1;
const int Action::intIsMinusOne = -1;
const int Action::intIsZero = 0;
const bool Action::boolIsFalse = false;

class MakeGoalListVisitor : public VisitController {

private:
	list<AbstractPredicate>* predicates;
	list<bool>* polarities;
	int * goalCount;
	int * negativeGoalCount;
	
	
	
	
	
	hash_map<parameter_symbol*, int>* parameter_symbolToIndex;
	hash_map<const pred_symbol*, Predicate* > * pred_symbolToPredicate;
	hash_map<const const_symbol*, int> * const_symbolToConstantIndex;
	bool currentPolarity;

public:

	int nextArg;
	
	MakeGoalListVisitor() : currentPolarity(true) {};


	void parseGoal(goal* topGoal,
			list<AbstractPredicate> * destPred,
			list<bool> * destPolarities,
			int * destGoalCount,
			int * destNegativeGoalCount,
			hash_map<parameter_symbol*, int> * conv,
			hash_map<const pred_symbol*, Predicate* > * predConv,
			hash_map<const const_symbol*, int> * constConv) {

		predicates = destPred;
		polarities = destPolarities;
		goalCount = destGoalCount;
		negativeGoalCount = destNegativeGoalCount;
		parameter_symbolToIndex = conv;
		pred_symbolToPredicate = predConv;
		const_symbolToConstantIndex = constConv;
		currentPolarity = true;
		topGoal->visit(this);

	}

	void visit_simple_goal(simple_goal * s) {

		const proposition* prop = s->getProp();

		Predicate* currPredicate = (*pred_symbolToPredicate)[prop->head];
		AbstractPredicate ans(currPredicate);
		

		
		parameter_symbol_list::iterator itr = prop->args->begin();

		for (int i = 0; itr != prop->args->end(); i++, itr++) {

			parameter_symbol* curr = (*itr);
			curr->visit(this);
			ans.parameters[i] = nextArg;

		}
		
		bool p = (s->getPolarity() == E_POS);
		if (!currentPolarity) p = !p;
		
		
		predicates->push_back(ans);
		polarities->push_back(p); 
		if (currentPolarity ) {
			(*goalCount)++;
		} else {
			(*negativeGoalCount)++;	
		}
			

	};
	
	void visit_conj_goal(conj_goal * c) {
	
		c->getGoals()->visit(this);
	
	};
	
	void visit_neg_goal(neg_goal * n) {
	
		currentPolarity = !currentPolarity;
		
		n->getGoal()->visit(this);
		
		currentPolarity = !currentPolarity;
	};

	void visit_const_symbol(const_symbol * c) {
		nextArg = -1 - (*const_symbolToConstantIndex)[c];
	}

	void visit_var_symbol(var_symbol * c) {
		nextArg = (*parameter_symbolToIndex)[c];
	}
};

Action::Action(operator_ * & o,
		hash_map<pddl_type*, Type* > & pddl_typeToType,
		hash_map<const pred_symbol*, Predicate* > & pred_symbolToPredicate,
		hash_map<const const_symbol*, int > & const_symbolToConstantIndex,
		Entity** constantsIn,
		EntitySet** constantDomainsIn)
	: 	borrowedParameterDomains(false),
		originalOperatorPointer(o),
		ownName(false),
		subActionTypes(0),
		constants(constantsIn),
		constantDomains(constantDomainsIn),
		isMacroAction(false),
		parameterCount(o->parameters->size()),
		parameterTypes(new Type*[parameterCount]),
		parameterDependentCounts(0),
		parameterDependents(0),
		parameterNegativeDependentCounts(0),
		parameterNegativeDependents(0),
		preconditionCount(0),
		negativePreconditionCount(0),
		addEffectCount(0),
		deleteEffectCount(0),
		dummySatCount(new int[1]),
		transformationalOperator(false),
		sequenceValidator(0),
		costs(true),
		isDPAction(false)
		{

	var_symbol_list* parameters = o->parameters;
	
	parameterDomains = new EntitySet*[parameterCount];
	ownParameterDomains = new bool[parameterCount];
	
	jointParameterDomainConstraints = new hash_map<Entity*, EntitySet**>*[parameterCount];
	

	for (int i = 0; i < parameterCount; i++) {
	
		hash_map<Entity*, EntitySet**> * eHash = new hash_map<Entity*, EntitySet**>();
		
		jointParameterDomainConstraints[i] = eHash;
		
	}
	
	hash_map<parameter_symbol*, int> parameter_symbolToIndex;
	
	var_symbol_list::const_iterator itr = parameters->begin();

	for (int i = 0; itr != parameters->end(); itr++, i++) {

		
		parameterTypes[i] = pddl_typeToType[(*itr)->type];
		parameter_symbolToIndex[(*itr)] = i;
		parameterDomains[i] = 0;
		ownParameterDomains[i] = false;
	}

	MakeGoalListVisitor vis;
	list<AbstractPredicate> destPred;
	list<bool> destPolarities;
	
	
	vis.parseGoal(	o->precondition,
				&destPred,
				&destPolarities,
				&preconditionCount,
				&negativePreconditionCount,
				&parameter_symbolToIndex,
				&pred_symbolToPredicate,
				&const_symbolToConstantIndex);
				
	preconditions = new AbstractPredicate[preconditionCount];
	constOnly = new bool[preconditionCount];
			
	negativePreconditions = new AbstractPredicate[negativePreconditionCount];
	negativeConstOnly = new bool[negativePreconditionCount];
	
	dummySatCount[0] = preconditionCount;

	for (int i = 0; i < preconditionCount; i++) {
		constOnly[i] = true;
		preconditionIndices.push_back(i);
	}
	for (int i = 0; i < negativePreconditionCount; i++) {
		negativeConstOnly[i] = true;
		negativePreconditionIndices.push_back(i);
	}
	
	list<AbstractPredicate>::iterator itrPred = destPred.begin();
	
	list<bool>::iterator itrPol = destPolarities.begin();
	
	int ione = 0;
	int itwo = 0;
	
	for (; itrPred != destPred.end(); itrPred++, itrPol++) {
		if (*itrPol) {
			preconditions[ione] = (*itrPred);
			for (int i = 0; constOnly[ione]&& i < preconditions[ione].predType->getArgumentCount(); i++) {
				if (preconditions[ione].parameters[i] >= 0) constOnly[ione] = false;
			}
			ione++;
		} else {
			negativePreconditions[itwo] = (*itrPred);
			for (int i = 0; negativeConstOnly[itwo] && i < preconditions[itwo].predType->getArgumentCount(); i++) {
				if (negativePreconditions[itwo].parameters[i] >= 0) negativeConstOnly[itwo] = false;
			}
			itwo++;
		}
	}
	
	addEffectCount = o->effects->add_effects.size();
	addEffects = new AbstractPredicate[addEffectCount];
	
	pc_list<simple_effect*>::const_iterator plItr = o->effects->add_effects.begin();
	
	for (int i = 0; plItr != o->effects->add_effects.end(); plItr++, i++) {
		proposition* prop = (*plItr)->prop;
		addEffects[i] = AbstractPredicate(pred_symbolToPredicate[prop->head]);
					
		parameter_symbol_list::iterator pslItr = prop->args->begin();

		for (int j = 0; pslItr != prop->args->end(); j++, pslItr++) {

			parameter_symbol* curr = (*pslItr);
			curr->visit(&vis);
			addEffects[i].parameters[j] = vis.nextArg;
			
		}
		
	}
	
	deleteEffectCount = o->effects->del_effects.size();
	deleteEffects = new AbstractPredicate[deleteEffectCount];
			
	plItr = o->effects->del_effects.begin();
	
	for (int i = 0; plItr != o->effects->del_effects.end(); plItr++, i++) {
		proposition* prop = (*plItr)->prop;
		deleteEffects[i] = AbstractPredicate(pred_symbolToPredicate[prop->head]);
		
		parameter_symbol_list::iterator pslItr = prop->args->begin();

		for (int j = 0; pslItr != prop->args->end(); j++, pslItr++) {

			parameter_symbol* curr = (*pslItr);
			curr->visit(&vis);
			deleteEffects[i].parameters[j] = vis.nextArg;
			
		}
		
	}
	
	
		
};
ActionInstance* Action::getInstance() {

	return new ActionInstance(this);

}

void Action::briefWrite(ostream & o) const {

	o << "Action " << name() << endl;

}

void Action::write(ostream & o) const {
	
	o << "Action " << name() << endl;
	o << "- Parameters:" << endl;
	for (int i = 0; i < parameterCount; i++) {
		o << "\t" << i << " of type " << parameterTypes[i]->name() << endl;
	}
	o << "- Preconditions:" << endl;
	for (int i = 0; i < preconditionCount; i++) {
		o << "\t" << preconditions[i];
		o << endl;
	}
	o << "- Negative Preconditions:" << endl;
	for (int i = 0; i < negativePreconditionCount; i++) {
		o << "\t" << negativePreconditions[i];
		o << endl;
	}
	o << "- Effects:" << endl;
	for (int i = 0; i < addEffectCount; i++) {
		o << "\t" << addEffects[i];
		o << endl;
	}
	for (int i = 0; i < deleteEffectCount; i++) {
		o << "\t¬(" << deleteEffects[i];
		o << ")" << endl;
	}

};

void Action::reorderPreconditions(StaticInformation * staticInf) {

	AbstractPredicate* tmpPreconditions = new AbstractPredicate[preconditionCount];
	
		
	list<AbstractPredicate> staticPreconditions;
	list<int> instancesOfStatic;
	list<AbstractPredicate> nonStaticPreconditions;
	

	for (int i = 0; i < preconditionCount; i++) {
	
		int currInstances = 0;
		
		if (constOnly[i]) {
			currInstances = 1;
		} else {
			if (preconditions[i].predType->isStatic()) {
				
				currInstances = 0;
				
				State::const_iterator staticStateItr = staticInf->StaticInitialState->begin();
				State::const_iterator staticStateEnd = staticInf->StaticInitialState->begin();
				
				for (; staticStateItr != staticStateEnd; staticStateItr++) {
				
					PredicateInstance* currPredToCheck = (*staticStateItr);
					
					if (currPredToCheck->isOfType(preconditions[i].predType)) {
						bool okToCount = true;
						for (int j = 0; okToCount && j < currPredToCheck->getParentPredicate()->getArgumentCount(); j++) {
							Entity* currToLookup = currPredToCheck->getParameter(j)->boundTo();
							const EntitySet* currDomain = getParameterDomain(preconditions[i].parameters[j]);
							okToCount = (currDomain->find(currToLookup) != currDomain->end());
							
							
						}
						if (okToCount) currInstances++;
					}
				}
				
				
				
				
				
				
			} else {
			
				currInstances = 1;
				
				for (int j = 0; j < preconditions[i].predType->getArgumentCount(); j++) {
				
					currInstances *= getParameterDomain(preconditions[i].parameters[j])->size();

				}
				
				
				
			}
		}
			
		list<AbstractPredicate>::iterator mergeStart = staticPreconditions.begin();
		list<AbstractPredicate>::iterator mergeEnd = staticPreconditions.end();
		list<int>::iterator instancesOfStaticStart = instancesOfStatic.begin();
		list<int>::iterator instancesOfStaticEnd = instancesOfStatic.end();
			
		while (mergeStart != mergeEnd && currInstances >= (*instancesOfStaticStart)) {
			mergeStart++;
			instancesOfStaticStart++;
		}
			
		staticPreconditions.insert(mergeStart, preconditions[i]);
		instancesOfStatic.insert(instancesOfStaticStart, currInstances);
	}
	
	
	
	list<AbstractPredicate>::iterator mergeStart = staticPreconditions.begin();
	list<AbstractPredicate>::iterator mergeEnd = staticPreconditions.end();
	
	
	for (int i = 0; mergeStart != mergeEnd; mergeStart++, i++) {
	
		tmpPreconditions[i] = (*mergeStart);
				
	}
	
	delete [] preconditions;
	
	
	preconditions = tmpPreconditions;
	

}


const EntitySet* Action::getParameterDomain(const int & i) const {

	if (i < 0) {
		return constantDomains[-1 - i];
	
	}
	
	if (ownParameterDomains[i]) {
		return parameterDomains[i];
	}

	return parameterTypes[i]->getEntitySet();
};


void Action::calculateParameterDomains(StaticInformation * staticInf) {

	for (int i = 0; i < parameterCount; i++) {
		if (ownParameterDomains[i]) {
			ownParameterDomains[i] = false;
			delete parameterDomains[i];
			parameterDomains[i] = 0;
		}
	}
	
	for (int i = 0; i < preconditionCount; i++) {
		const Predicate* currPred = preconditions[i].predType;
		if (currPred->isStatic()) {
			int currPredArgCount = currPred->getArgumentCount();
			if (staticInf->verbose) {
				cout << "Processing precondition " << i << " of type " << currPred->name() << endl;
			}
			EntitySet** domains = new EntitySet*[currPred->getArgumentCount()];
			
			for (int j = 0; j < currPredArgCount; j++) {
				if (preconditions[i].parameters[j] >= 0) {
					domains[j] = new EntitySet();
				}
			}
				  
			State::const_iterator itr = staticInf->StaticInitialState->begin();
			State::const_iterator itrEnd = staticInf->StaticInitialState->end();

			for (; itr != itrEnd; itr++) {
				
				PredicateInstance* curr = (*itr);
				
				if (curr->isOfType(currPred)) {
				
					//cout << *curr << " in initial state is of the right type" << endl;
					
					bool constantsMatch = true;
					for (int j = 0; constantsMatch && j < currPred->getArgumentCount(); j++) {
						if (preconditions[i].parameters[j] < 0) {
							Parameter* param =  curr->getParameter(j);
							Entity* toPush = param->boundTo();
							constantsMatch = (constants[-1-preconditions[i].parameters[j]] == toPush);
							
						}
					
					}

					if (constantsMatch) {
						for (int j = 0; j < currPred->getArgumentCount(); j++) {
						
							if (preconditions[i].parameters[j] >= 0) {
								Parameter* param =  curr->getParameter(j);
								Entity* toPush = param->boundTo();
						
								domains[j]->insert(toPush);	
							}
						
						}
					}
				
				}
				
			}
			
			
			 
			for (int j = 0; j < currPred->getArgumentCount(); j++) {
		
				int argRef = preconditions[i].parameters[j];	
				if (argRef >= 0) {
				 
					if (!ownParameterDomains[argRef]) {
					
						if (staticInf->verbose) {
							cout << "Domain for argument " << argRef << " previously unrestricted" << endl;
						}
						ownParameterDomains[argRef] = true;
						
						parameterDomains[argRef] = domains[j]; 
						if (staticInf->verbose) {
							cout << "Domain for argument " << argRef << " now:" << endl;
							
							for (EntitySet::iterator esi = parameterDomains[argRef]->begin(); esi != parameterDomains[argRef]->end(); esi++) {
								cout << " " << (*esi)->name();
								
							}
							cout << endl;
						}
											
					} else {
						EntitySet* newES = intersection(domains[j], parameterDomains[argRef]);
						
						delete domains[j];
						delete parameterDomains[argRef];
						
						parameterDomains[argRef] = newES;
						
						if (staticInf->verbose) {
							cout << "Domain for argument " << argRef << " now:" << endl;
							
							for (EntitySet::iterator esi = parameterDomains[argRef]->begin(); esi != parameterDomains[argRef]->end(); esi++) {
								cout << " " << (*esi)->name();
								
							}
							cout << endl;
						}

						
					}
				}
			}
			 
			delete [] domains;
			
		}
	}
	
	

};

void Action::intersection(	Entity** & firstArray, int & firstSize,
				Entity** & secondArray, int & secondSize,
				Entity** & destArray, int & destSize) {

	if (firstSize == -1 || secondSize == -1) {
	
		Entity** copyFrom;
		if (firstSize == -1) {
			copyFrom = secondArray;
			destArray = new Entity*[secondSize];
			destSize = secondSize;
		} else {
			copyFrom = firstArray;
			destArray = new Entity*[firstSize];
			destSize = firstSize;
		}
		
		for (int i = 0; i < destSize; i++) {
		
			destArray[i] = copyFrom[i];
		
		}
		
		return;
	
	}
	
	slist<Entity*> isect;
	int counter = 0;
	
	for (int i = firstSize - 1; i >=0 ; i--) {
		bool found = false;
		for (int j = secondSize - 1; !found && j >= 0; j--) {
			found = (firstArray[i] == secondArray[j]);
		}

		if (found) {
			isect.push_front(firstArray[i]);
			counter++;
		}
	}

	destArray = new Entity*[counter];
	destSize = counter;

	counter = 0;
	slist<Entity*>::iterator itr = isect.begin();
	for (; itr != isect.end(); itr++) {
		destArray[counter] = (*itr);
		counter++;
	}
}

void Action::intersection(	Entity** & firstArray, const int & firstSize,
				slist<Entity*> * secondArray) {

	if (firstSize == -1) return;



	slist<Entity*>::iterator cItr = secondArray->begin();
	slist<Entity*>::iterator pItr = secondArray->begin();

	while (cItr != secondArray->end()) {

		bool found = false;
		for (int j = 0; !found && j < firstSize; j++) {
			found = (firstArray[j] == *cItr);
		}

		if (found) {
			pItr = cItr;
			cItr++;
		} else {


			if (cItr == secondArray->begin()) {

				secondArray->pop_front();
				cItr = secondArray->begin();
				pItr = secondArray->begin();

			} else {

				secondArray->erase_after(pItr);

				cItr = pItr;
				cItr++;

			}
		}

	}


}

EntitySet * Action::intersection(const EntitySet * first, const EntitySet * second) {

	EntitySet * toReturn;
	if (first == 0 && second == 0) {
		toReturn = 0;
		return toReturn;
	}
	if (first == 0) {
		toReturn = new EntitySet(*second);
		return toReturn;
	}
	if (second == 0) {
		toReturn = new EntitySet(*first);
		return toReturn;
	}
	toReturn = new EntitySet();
	
	/*EntitySet::const_iterator fStart = first->begin();
	EntitySet::const_iterator fEnd = first->end();
	EntitySet::const_iterator sStart = second->begin();
	EntitySet::const_iterator sEnd = second->end();
	
	//set_intersection(fStart, fEnd, sStart, sEnd, output);
	while (fStart != fEnd && sStart != sEnd) {
		if (*fStart < *sStart) {
			fStart++;
		} else if (*sStart < *sStart) {
			sStart++;
		} else {
			toReturn->insert(*fStart);
			fStart++;
			sStart++;
		}
	}*/
	
	EntitySet::const_iterator fStart = first->begin();
	EntitySet::const_iterator fEnd = first->end();
	
	for (;fStart != fEnd; fStart++) {
		if (second->find(*fStart) != second->end()) {
			toReturn->insert(*fStart);
		}
		
	}
	return toReturn;
};

void Action::intersection(	slist<Entity*> & firstArray, 
				slist<Entity*> * secondArray) {

	
	

	slist<Entity*>::iterator cItr = secondArray->begin();
	slist<Entity*>::iterator pItr = secondArray->begin();

	while (cItr != secondArray->end()) {

		bool found = false;
		for (slist<Entity*>::iterator j = firstArray.begin(); !found && j != firstArray.end(); j++) {
			found = (*j == *cItr);
		}

		if (found) {
			pItr = cItr;
			cItr++;
		} else {


			if (cItr == secondArray->begin()) {

				secondArray->pop_front();
				cItr = secondArray->begin();
				pItr = secondArray->begin();

			} else {

				secondArray->erase_after(pItr);

				cItr = pItr;
				cItr++;

			}
		}

	}


}
void Action::calculateStaticJointParameterDomainConstraints(StaticInformation * staticInf) {

	if (staticInf->verbose) {
		cout << "Calculating static joint parameter domain constraints" << endl;
	}
	
	//jointParameterDomainConstraints = new hash_map<Entity*, Entity***>*[parameterCount];
	//parameterDomainConstraintSizes = new hash_map<Entity*, int*>*[parameterCount];
	
	for (int i = 0; i < parameterCount; i++) {
		
		hash_map<Entity*, EntitySet**> * eHash = jointParameterDomainConstraints[i];
		
		
		for (EntitySet::iterator j = getParameterDomain(i)->begin(); j != getParameterDomain(i)->end(); j++) {
		
			EntitySet** epArray = new EntitySet*[parameterCount];
						
			Entity* currEnt = *j;
			
			for (int k = 0; k < parameterCount; k++) {
				epArray[k] = 0;
			}
			hash_map<Entity*, EntitySet**>::iterator ae = eHash->find(currEnt);
			if (ae != eHash->end()) {
				EntitySet** oldEP = (*ae).second;
				
				for (int k = 0; k < parameterCount; k++) {
					if (oldEP[k] != 0) delete oldEP[k];
				}
				
				delete [] oldEP;
				
				(*ae).second = epArray;
			} else {
				
				(*eHash)[currEnt] = epArray;	
			}
			
			
		}
		
			
	}
	
	for (int masterParameter = 0; masterParameter < parameterCount; masterParameter++) {
	
		if (ownParameterDomains[masterParameter]) {
	
			if (staticInf->verbose) {
				cout << " Processing parameter " << masterParameter << endl;
			}		

			for (int currPreconditionIndex = 0; currPreconditionIndex < preconditionCount; currPreconditionIndex++) {
				if (preconditions[currPreconditionIndex].predType->isStatic()) {
					const Predicate* currPrecondition = preconditions[currPreconditionIndex].predType;
					
					for (int i = 0; i < currPrecondition->getArgumentCount(); i++) {
						if (preconditions[currPreconditionIndex].parameters[i] == masterParameter) {
						
							if (staticInf->verbose) {
								cout << "  Parameter interacts with static precondition " << currPreconditionIndex << " of type " << currPrecondition->name() << endl;
							}
							
							
								 
							for (EntitySet::iterator domainEntryIndex = getParameterDomain(masterParameter)->begin(); domainEntryIndex != getParameterDomain(masterParameter)->end(); domainEntryIndex++) {
								EntitySet** restrictionAsList = new EntitySet*[currPrecondition->getArgumentCount()];
								for (int zeroArray = 0; zeroArray < currPrecondition->getArgumentCount(); zeroArray++) {
									if (zeroArray != i  && preconditions[currPreconditionIndex].parameters[zeroArray] >= 0) {
										
										restrictionAsList[zeroArray] = new EntitySet();	
										//cout << "Creating Entity Set at " << restrictionAsList[zeroArray] << endl;
									} else {
										restrictionAsList[zeroArray] = 0;
									}
						
								}
								
								
										
								State::const_iterator stateItr = staticInf->StaticInitialState->begin();
										
										
										
								for (; stateItr != staticInf->StaticInitialState->end(); stateItr++) {
										
									const PredicateInstance * currStaticPred = (*stateItr);
									if (currStaticPred->isOfType(currPrecondition)) {
																				
										bool matches = (currStaticPred->getParameter(i)->boundTo() == (*domainEntryIndex));
												
										for (int matchIndex = 0; matches && matchIndex < currPrecondition->getArgumentCount(); matchIndex++) {
										
											if (preconditions[currPreconditionIndex].parameters[matchIndex] < 0) {
												matches = (currStaticPred->getParameter(matchIndex)->boundTo() == constants[-1 - preconditions[currPreconditionIndex].parameters[matchIndex]]);
											}	
																				
										}
											
										if (matches) {
										
											for (int matchIndex = 0; matchIndex < currPrecondition->getArgumentCount(); matchIndex++) {
										
												if (matchIndex != i && preconditions[currPreconditionIndex].parameters[matchIndex] >=0) {
											
													restrictionAsList[matchIndex]->insert(currStaticPred->getParameter(matchIndex)->boundTo());
													
												}
										
											}	
										
										}
										
									}
										
								}
								
								for (int matchIndex = 0; matchIndex < currPrecondition->getArgumentCount(); matchIndex++) {
									
							
									if (matchIndex != i && preconditions[currPreconditionIndex].parameters[matchIndex] >= 0) {
								
										int correspondingParameter = preconditions[currPreconditionIndex].parameters[matchIndex];
									
									
										
										
										hash_map<Entity*, EntitySet**> * mpHash = jointParameterDomainConstraints[masterParameter];
										
										//Entity** oldJPDC = (*mpHash)[argAssignments[i]][matchIndex];
										EntitySet* oldJPDC = (*mpHash)[(*domainEntryIndex)][correspondingParameter];
										if (oldJPDC == 0) {
											//cout << "Null old JPDC, transferring responsibility for entity set at " << restrictionAsList[matchIndex] << endl;
											(*mpHash)[(*domainEntryIndex)][correspondingParameter] = restrictionAsList[matchIndex];
										} else {
											EntitySet* newJPDC = intersection(oldJPDC, restrictionAsList[matchIndex]);
											delete oldJPDC;
											//cout << "Merged, deleting entity set at " << restrictionAsList[matchIndex] << endl;
											delete restrictionAsList[matchIndex];
											(*mpHash)[(*domainEntryIndex)][correspondingParameter] = newJPDC;
										}
										
										
																		
										
									}			
								}
								
								delete [] restrictionAsList;
								
							}
							
							
							
							
						
						};
					}
					
				}
			}
							
		} else {
			if (staticInf->verbose) {
				cout << " Parameter " << masterParameter << " is unrestricted; moving on." << endl;
			}	
		}
		
		if (staticInf->verbose) {
		
			cout << " Done processing parameter " << masterParameter << endl;
		}
	
	}
	
	

};

void Action::dummyStaticJointParameterDomainConstraints(StaticInformation * staticInf) {

	if (staticInf->verbose) {
		cout << "Dummy static joint parameter domain constraints" << endl;
	}
	
	//jointParameterDomainConstraints = new hash_map<Entity*, Entity***>*[parameterCount];
	//parameterDomainConstraintSizes = new hash_map<Entity*, int*>*[parameterCount];
	
	for (int i = 0; i < parameterCount; i++) {
		
		hash_map<Entity*, EntitySet**> * eHash = jointParameterDomainConstraints[i];
		
		
		for (EntitySet::iterator j = getParameterDomain(i)->begin(); j != getParameterDomain(i)->end(); j++) {
		
			EntitySet** epArray = new EntitySet*[parameterCount];
						
			Entity* currEnt = *j;
			
			for (int k = 0; k < parameterCount; k++) {
				epArray[k] = 0;
			}
			
			(*eHash)[currEnt] = epArray;
		}
		
			
	}
}

void Action::getDomainConstraints(const int & parameter,
				  Entity* assignment,
				  EntitySet** & domains) const {
	
	
	hash_map<Entity*, EntitySet**> * parHash = jointParameterDomainConstraints[parameter];
	
	
	domains = (*parHash)[assignment];
	
	
			
}

Action::~Action() {

	//cout << "Deleting action" << endl;
	if (jointParameterDomainConstraints != 0) {
		for (int i = 0; i < parameterCount; i++) {
		
			hash_map<Entity*, EntitySet**>* currDomHash = jointParameterDomainConstraints[i];
			
	
			if (currDomHash != 0) {
				hash_map<Entity*, EntitySet**>::iterator itr = currDomHash->begin();
			
				for (; itr != currDomHash->end(); itr++) {
			
					EntitySet** toDelete = (*itr).second;
					
					for (int j = 0; j < parameterCount; j++) {
						if (toDelete[j] != 0) {
							//cout << "Destructor deleting entity set at " << toDelete[j] << endl;
							delete toDelete[j];
						}
					}
					
					delete [] toDelete;
				
				}
			}
			
			
			delete currDomHash;
			jointParameterDomainConstraints[i] = 0;
			
		}
	}
	
	
	
	if (!borrowedParameterDomains) {
		delete [] jointParameterDomainConstraints;
		jointParameterDomainConstraints = 0;
		
		if (ownParameterDomains != 0) {
			for (int i = 0; i < parameterCount; i++) {
			
				if (ownParameterDomains[i]) {
				
					delete parameterDomains[i];
				}
			
			}
		}
		delete [] parameterDomains;
		delete [] ownParameterDomains;
		delete [] parameterTypes;
	}
	
	deleteParameterDependencies();	
	
	delete [] preconditions;
	delete [] negativePreconditions;
	delete [] addEffects;
	delete [] deleteEffects;
	
	delete [] constOnly;
	delete [] negativeConstOnly;
	
	delete [] dummySatCount;
	
	delete sequenceValidator;
	
	
		
}


ostream & operator <<(ostream & o, const Action & a) {
	a.write(o);
	return o;
};

bool Action::isTransformationalOperator(const Predicate* & staticTransPred, int & loc1, int & loc2) {

	
	list<pair<const Predicate*, int> > candidates;	
	const bool verbose = false;
	if (verbose) cout << "Assessing whether " << name() << " is a transformational operator" << endl;
	if (preconditionCount > 2) {
		if (verbose) cout << "Not bothering - has more than two preconditions" << endl;
		return false;
	}
	
	for(int i = 0; i < preconditionCount; i++){
	
		const Predicate* currPrecond = preconditions[i].predType;
		if(currPrecond->isStatic()) {
			candidates.push_back(pair<const Predicate*, int>(currPrecond, i));
			if(verbose) cout << "found static precondition " << currPrecond->name() << " for action " << name() << endl;
		}
		
	}
	
	if(candidates.empty()) {
		if(verbose) cout << "action " << name() << " has no static preconditions" << endl;
		return false;
	}
	
	list<pair <const Predicate*, int> >::const_iterator itr = candidates.begin();
	
	//list< pair< pair<Predicate*, int>, pair<Predicate*, int> > > potentialPairedLocations;
	
	for(; itr != candidates.end(); itr++){
		
	
		const Predicate* currPrecond = (*itr).first;
		
		for(int i = 0; i < currPrecond->getArgumentCount(); i++){
			
			//int potentialLocation = preconditionArguments[(*itr).second][i];
			//we need to check the other entities in this predicate to see:
			
			//if they are of the same type
			//start from i+1 to aviod getting duplicate pairs
			for(int j = i+1; j < currPrecond->getArgumentCount(); j++){
				if(verbose) cout << "Checking whether precondition number " << j << " is of the same type as precondition number " << i << endl;
				if(currPrecond->getArgumentType(j) == currPrecond->getArgumentType(i)){
					if(verbose) cout << "...it is" << endl;
					int potentialLinkedLocation = preconditions[(*itr).second].parameters[i];
					int potentialSecondLinkedLocation = preconditions[(*itr).second].parameters[j];//j will never be checked...
					
					if(verbose) cout << "looking for argument with number " << potentialLinkedLocation << endl;
					//we've found two parameters of a static predicate that are of the same type 
					//now we need to see if there's a dynamic predicate with this argument as any argument
					
					//list<Predicate*> potentialDynamicMatches;
					
					for(int k = 0; k < preconditionCount; k++){
						
						if(!preconditions[k].predType->isStatic()){
							if(verbose) cout << "Checking dynamic precondition " << preconditions[k].predType->name() << endl;
							for(int m = 0; m < preconditions[k].predType->getArgumentCount(); m++){
								//if we've found a dynamic predicate with this as an argument...
								if(preconditions[k].parameters[m] == potentialLinkedLocation /*|| preconditionArguments[k][m] == potentialSecondLinkedLocation*/){
									if(verbose) cout << " found precondition " << preconditions[k].predType->name() << " with potential location as argument" << endl;
									//find whether or not this predicate is an add effect and a delete effect
									
									list<AbstractPredicate> addedIndices = addCases(preconditions[k].predType);
									list<AbstractPredicate> deletedIndices = deleteCases(preconditions[k].predType);
 									//we now know the predicate is added and deleted so we need to check the bindings
									
									if(!addedIndices.empty() && !deletedIndices.empty()){
									
										//****this code assumes that the predicate in question is only added once and deleted once***	
										//we need to check if /exactly/ currPrecond is a delete effect
										//and its arguments are reversed as a delete effect
										//if(added < 1) cout << "WARNING: potential predicate added more than once this may cause a transformational operator to be missed" << endl;
										
										///if(deleted < 1) cout << "WARNING: potential predicate deleted more than once this may cause a transformational operator to be missed" << endl;
										
										bool in = false;
										int notFound = 0;
										//int notFoundWas = 0;
										//int foundAt = -1;
										int mis = 0;
										
										list<AbstractPredicate>::iterator itr = deletedIndices.begin();
										list<AbstractPredicate>::iterator itrEnd = deletedIndices.end();
										
										for(; itr != itrEnd && !in; ++itr){
											//in = true;
											in = ((*itr) == preconditions[k]);
											
											/*int currDeleted = (*itr);
										
											//if the predicate we're looking at is deleted
											for(int n = 0; n < currPrecond->getArgumentCount(); n++){
												if(preconditionArguments[k][n] != deleteEffectArguments[currDeleted][n]){
													notFound++;
												}
						
												
											}*/

										}
										
										//in = (notFound != deletedIndices.size());
										
										if(verbose && in) cout << "the exact precondition is deleted" << endl;
										itr = addedIndices.begin();
										notFound = 0;
										
										itrEnd = addedIndices.end();
										
										bool inAdd = false;
										for(; !inAdd && itr != itrEnd; ++itr){
											/*mis = 0;
											//and the same one is added with only one argument different
											for(int n = 0; n < currPrecond->getArgumentCount(); n++){
												//this line assumes the predicates are 
												if(preconditionArguments[k][n] != addEffectArguments[added][n]){
													mis++;
													if(mis > 1 || addEffectArguments[added][currPrecond->getArgumentCount() - n] != potentialSecondLinkedLocation) notFound++;
												}
											}*/
											mis = 0;
										
											for (int n = 0; n < currPrecond->getArgumentCount(); n++){
												
												if(preconditions[k].parameters[n] != (*itr).parameters[n]){
													mis++;
												}
											}
										
											inAdd = (mis == 1);
											
											
										}
										//in = (notFound != addedIndices.size());
										
										if(in && inAdd){
												
											staticTransPred = currPrecond;
											transformationalOperator = true;
											loc1 = potentialLinkedLocation;
											loc2 = potentialSecondLinkedLocation;
											return true;
										}
										
									}
									
								} else {
								
									if(verbose) cout << "argument " << m << " of " << preconditions[k].predType->name() << "Does not match ";
									if(verbose) cout << "potential linked-location " << potentialLinkedLocation << endl;
								}
							}
							
							if(verbose) cout << preconditions[k].predType->name() << " has arguments numbered: ";
							
							for(int m = 0; m < preconditions[k].predType->getArgumentCount(); m++){
							
								if(verbose) cout << preconditions[k].parameters[m] << " ";
							
							}
							
							if(verbose) cout << endl;
						}
					
					}
				} else	{
					if(verbose) cout << "precondition " << j << " of " << currPrecond->name() << " is not of the appropriate type" << endl;
				
				}
			
			}
		}
	}
	
	return false;


}

void Action::setSequenceValidator(ActionSequenceValidator * a) {
	delete sequenceValidator;
	sequenceValidator = a;
};

void Action::deleteParameterDependencies() {

	for (int pass = 0; pass < 2; ++pass) {
		
		int** depCountArray;
		int*** depArray;
		
	
		if (pass == 0) {
			depCountArray = &parameterDependentCounts;
			depArray = &parameterDependents;
			
		} else {
			depCountArray = &parameterNegativeDependentCounts;
			depArray = &parameterNegativeDependents;
			
		}
	
		if (*depCountArray != 0) {
		
			
			delete [] (*depCountArray);
			(*depCountArray) = 0;
			
			if (*depArray != 0) {
				
				for (int i = 0; i < parameterCount; ++i) {
					delete [] (*depArray)[i];
					(*depArray)[i] = 0;
				}
				
				delete [] *depArray;
				*depArray = 0;
			}
		}
	}

}


void Action::calculateParameterDependencies() {
	
	if (parameterDependentCounts != 0) return;

	parameterDependentCounts = new int[parameterCount];
	parameterDependents = new int*[parameterCount];
	
	parameterNegativeDependentCounts = new int[parameterCount];
	parameterNegativeDependents = new int*[parameterCount];
	
	for (int pass = 0; pass < 2; ++pass) {
	
		
		
		list<int>* dependentList = new list<int>[parameterCount];
		
		int preconditionLimit;
		AbstractPredicate* preconditionArray;
		int* depCountArray;
		int** depArray;

		if (pass == 0) {
			preconditionLimit = preconditionCount;
			preconditionArray = preconditions;
			depCountArray = parameterDependentCounts;
			depArray = parameterDependents;
			
		} else {
			preconditionLimit = negativePreconditionCount;
			preconditionArray = negativePreconditions;
			depCountArray = parameterNegativeDependentCounts;
			depArray = parameterNegativeDependents;
			
		}
		
		
		
		for (int i = 0; i < preconditionLimit; ++i) {
		
			const AbstractPredicate* thisOne = &(preconditionArray[i]);
			
			const int looplimit = thisOne->predType->getArgumentCount();
			
			
			for (int j = 0; j < looplimit; ++j) {
			
				if (thisOne->hardParameters[j] == 0) {
					const int pu = thisOne->parameters[j];
					
					if (pu >= 0) {
						//cout << "Precondition " << i << " interacts with parameter " << pu << endl;
						dependentList[pu].push_back(i);
					}
				}
			
			}
		
		}
	
		for (int i = 0; i < parameterCount; ++i) {
			const int looplimit = dependentList[i].size();
			
			
			depCountArray[i] = looplimit;
			
			if (looplimit == 0) {
			
				depArray[i] = 0;
				
			} else {
				depArray[i] = new int[looplimit];
				
				list<int>::const_iterator itr = dependentList[i].begin();
				const list<int>::const_iterator itrEnd = dependentList[i].end();
				
				for (int j = 0; itr != itrEnd; ++itr, ++j) {
					depArray[i][j] = (*itr);
				}
			}
		}		
	
		delete [] dependentList;
		
	}
	
};
