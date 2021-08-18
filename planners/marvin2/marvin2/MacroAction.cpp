#include "MacroAction.h"
#include "State.h"
#include "relaxedPlanningGraph.h"
#include "SymmetryExtractor.h"
#include "SymmetryGroup.h"

#include "ActionInstanceWithSubactions.h"
#include <sys/times.h>
#include <unistd.h>

#include <assert.h>

int MacroAction::calculateParameterCount(Plan* toUse, StaticInformation & problemInformation) {
tms refReturn;
	times(&refReturn);
	
	double secs = ((double)refReturn.tms_utime + (double)refReturn.tms_stime) / ((double) sysconf(_SC_CLK_TCK));

	int twodp = (int) (secs * 100.0);
	int wholesecs = twodp / 100;
	int centisecs = twodp % 100;
	
	cout << "; Time for first macro " << wholesecs << ".";
	if (centisecs < 10) cout << "0";
	cout << centisecs << endl;
	EntitySet consideredParameters;
	Plan::iterator planItr = toUse->begin();

	for(; planItr != toUse->end(); planItr++){

		//work out the parameters...

		//for each precondition, add effect and delete effect



		//for each binding: if we've not made the entity in question into a parameter, do so.



		//work out the preconditions...
		ActionInstance* aci = (*planItr).first;
		for (int i = 0; i < aci->getParameterCount(); i++) {
		
			consideredParameters.insert(aci->getParameter(i)->boundTo());
		
		}
	}
	
	return consideredParameters.size();
	
};


MacroAction::MacroAction(Plan* toUse, StaticInformation & problemInformation) : Action(calculateParameterCount(toUse, problemInformation)) {

	bool verbose = true;
	list<Type*> parameters;
	EntitySet consideredParameters;
//	parameterCount = 0;
	

	isMacroAction = true;
	
	verbose = false;


	Plan::iterator planItr = toUse->begin();

	for(; planItr != toUse->end(); planItr++){

		//work out the parameters...

		//for each precondition, add effect and delete effect



		//for each binding: if we've not made the entity in question into a parameter, do so.



		//work out the preconditions...
		ActionInstance* aci = (*planItr).first;
		for (int i = 0; i < aci->getParameterCount(); i++) {
		
			consideredParameters.insert(aci->getParameter(i)->boundTo());
		
		}
	}
	
	//turn all the lists into arrays
	verbose = false;
	
	//parameterCount = consideredParameters.size();
	//parameterTypes = new Type*[parameterCount];
	
	parameterDomains = new EntitySet*[parameterCount];
	ownParameterDomains = new bool[parameterCount];
	
	originalBindings = new Entity*[parameterCount];
	hash_map<Entity*, int> bindings(parameterCount);

	if(verbose) cout << "Parameters:" << endl;

	int i = 0;

	EntitySet::iterator paramItr = consideredParameters.begin();
	
	for(; paramItr != consideredParameters.end(); paramItr++, i ++){
		Entity* currEnt = *paramItr;
		if(verbose) cout << "Parameter " << i << " is of type " << currEnt->getEntityType()->name() << endl;
		parameterTypes[i] = currEnt->getEntityType();
		
		bindings[currEnt] = i;
		originalBindings[i] = currEnt;
		parameterDomains[i] = 0;
		ownParameterDomains[i] = false;
		
	}

	//create a hash map mapping each entity to its parameter number

	
	
	//int subActionCount = toUse->length();
	
	double frontTime = (*(toUse->begin())).second;
	abstractSteps.push_back(pair<list<AbstractAction>, double>(list<AbstractAction>(), frontTime));
	definedName = "Macro-action: ";

	ownName = true;
	
	for (Plan::iterator csaItr = toUse->begin(); csaItr != toUse->end(); csaItr++) {

		if ((*csaItr).second > frontTime) {
		
			frontTime = (*csaItr).second;
			abstractSteps.push_back(pair<list<AbstractAction>, double>(list<AbstractAction>(), frontTime));
		
		}
		
		int looplimit = (*csaItr).first->getParameterCount();
		
		AbstractAction ab((*csaItr).first->getParentAction());
		
		for (int j = 0; j < looplimit; j++) {
			int rI = bindings[(*csaItr).first->getParameter(j)->boundTo()];
			ab.parameters[j] = rI;
		}
		definedName += ab.actType->name();
		definedName += " ";
		/*cout << "MacroAction: Abstract Step: ";
		cout << ab.actType->name();
		for (int j = 0; j < looplimit; j++) {
			cout << " " << ab.parameters[j];
		}
		cout << " [" << frontTime << "]" << endl;*/
		
		abstractSteps.back().first.push_back(ab);


	}

	/*calculateMutexInformation();
	
	

	
	bool was = problemInformation.verbose;
	problemInformation.verbose = false;

	parameterDomains = new EntitySet*[parameterCount];

	ownParameterDomains = new bool[parameterCount];

	jointParameterDomainConstraints = new hash_map<Entity*, EntitySet**>*[parameterCount];

	for (int i = 0; i < parameterCount; i++) {

		hash_map<Entity*, EntitySet**> * eHash = new hash_map<Entity*, EntitySet**>();


		jointParameterDomainConstraints[i] = eHash;


		parameterDomains[i] = 0;

		ownParameterDomains[i] = false;
	}

	definedName = "Macro-action: ";

	for(int i = 0; i < subActionCount; i++){
		definedName += subActionTypes[i]->name();
		definedName += " ";
	}

	ownName = true;
	calculateParameterDomains(&problemInformation);
	calculateStaticJointParameterDomainConstraints(&problemInformation);
	reorderPreconditions(&problemInformation);

	while (!deleteEffectsConstParams.empty()) {
		delete [] deleteEffectsConstParams.front();
		deleteEffectsConstParams.pop_front();
	}
	
	while (!addEffectsConstParams.empty()) {
		delete [] addEffectsConstParams.front();
		addEffectsConstParams.pop_front();
	}
	
	while (!preconditionsConstParams.empty()) {
		delete [] preconditionsConstParams.front();
		preconditionsConstParams.pop_front();
	}
	
	while (!negativePreconditionsConstParams.empty()) {
		delete [] negativePreconditionsConstParams.front();
		negativePreconditionsConstParams.pop_front();
	}
	problemInformation.verbose = was;

	delete [] subActionTypes;
	for (int i = 0; i < subActionCount; i++) {
		delete [] subActionArguments[i];
	}
	delete [] subActionArguments;*/
	
	timesSinceUse = 0;
	usageCount = 0;
	
	jointParameterDomainConstraints = new hash_map<Entity*, EntitySet**>*[parameterCount];
	

	for (int i = 0; i < parameterCount; i++) {
	
		hash_map<Entity*, EntitySet**> * eHash = new hash_map<Entity*, EntitySet**>();
		
		jointParameterDomainConstraints[i] = eHash;
		
	}
	
	dummyStaticJointParameterDomainConstraints(&problemInformation);
}

void MacroAction::write(ostream & o) const {

	o << definedName << endl;

};


/*3 PredicateInstanceLists:

- stuff that is added by actions in this step of the macro op
- stuff that is deleted " " " ""
- stuff that must be held true at this time point because a later step needs it
  and it isn't reachieved before then*/
/*
AbstractPredicate* MacroAction::getAbstractPreconditions(const AbstractAction & a) const {

	AbstractPredicate * toReturn = new AbstractPredicate[a.actType->preconditionCount];
	
	for (int i = 0; i < a.actType->preconditionCount; i++) {
		const Predicate* currPred = a.actType->preconditions[i].predType;
		toReturn[i] = AbstractPredicate(currPred);
		for (int j = 0; j < currPred->getArgumentCount(); j++) {
			int rfi = a.actType->preconditions[i].parameters[j];
			if (rfi >= 0) {
				toReturn[i].parameters[j] = a.parameters[rfi];
			} else {
				toReturn[i].parameters[j] = rfi;
			}
			
		}
	}
	
	return toReturn;

};

AbstractPredicate* MacroAction::getAbstractNegativePreconditions(const AbstractAction & a) const {

	AbstractPredicate * toReturn = new AbstractPredicate[a.actType->negativePreconditionCount];
	
	for (int i = 0; i < a.actType->negativePreconditionCount; i++) {
		const Predicate* currPred = a.actType->negativePreconditions[i].predType;
		toReturn[i] = AbstractPredicate(currPred);
		for (int j = 0; j < currPred->getArgumentCount(); j++) {
			int rfi = a.actType->negativePreconditions[i].parameters[j];
			if (rfi >= 0) {
				toReturn[i].parameters[j] = a.parameters[rfi];
			} else {
				toReturn[i].parameters[j] = rfi;
			}
			
		}
	}
	
	return toReturn;

};

AbstractPredicate* MacroAction::getAbstractAddEffects(const AbstractAction & a) const {

	AbstractPredicate * toReturn = new AbstractPredicate[a.actType->addEffectCount];
	
	for (int i = 0; i < a.actType->addEffectCount; i++) {
		const Predicate* currPred = a.actType->addEffects[i].predType;
		toReturn[i] = AbstractPredicate(currPred);
		for (int j = 0; j < currPred->getArgumentCount(); j++) {
			int rfi = a.actType->addEffects[i].parameters[j];
			if (rfi >= 0) {
				toReturn[i].parameters[j] = a.parameters[rfi];
			} else {
				toReturn[i].parameters[j] = rfi;
			}
			
		}
	}
	
	return toReturn;

};

bool MacroAction::abstractActionAchieves(const AbstractAction & a, const AbstractPredicate & p) const {

	for (int i = 0; i < a.actType->addEffectCount; i++) {
		const Predicate* currPred = a.actType->addEffects[i].predType;
		if (currPred == p.predType) {
			bool matches = true;
			for (int j = 0; j < currPred->getArgumentCount(); j++) {
				Entity* entOne;
				Entity* entTwo;
				
				if (p.parameters[j] >= 0) {
					entOne = originalBindings[p.parameters[j]];
				} else {
					entOne = a.actType->constants[-1 - (p.parameters[j])];
				}
				
				if (a.actType->addEffects[i].parameters[j] >= 0) {
					entTwo = originalBindings[a.parameters[a.actType->addEffects[i].parameters[j]]];
				} else {
					entTwo = a.actType->constants[-1 - (a.actType->addEffects[i].parameters[j])];
				}
				matches = (entOne == entTwo);
			}
			
			if (matches) return true;
		}
	}
	
	return false;

};

bool MacroAction::abstractActionDeletes(const AbstractAction & a, const AbstractPredicate & p) const {

	for (int i = 0; i < a.actType->deleteEffectCount; i++) {
		const Predicate* currPred = a.actType->deleteEffects[i].predType;
		if (currPred == p.predType) {
			bool matches = true;
			for (int j = 0; j < currPred->getArgumentCount(); j++) {
				Entity* entOne;
				Entity* entTwo;
				
				if (p.parameters[j] >= 0) {
					entOne = originalBindings[p.parameters[j]];
				} else {
					entOne = a.actType->constants[-1 - (p.parameters[j])];
				}
				
				if (a.actType->deleteEffects[i].parameters[j] >= 0) {
					entTwo = originalBindings[a.parameters[a.actType->deleteEffects[i].parameters[j]]];
				} else {
					entTwo = a.actType->constants[-1 - (a.actType->deleteEffects[i].parameters[j])];
				}
				matches = (entOne == entTwo);
				
			}
			
			if (matches) return true;
		}
	}
	
	return false;

};


AbstractPredicate* MacroAction::getAbstractDeleteEffects(const AbstractAction & a) const {

	AbstractPredicate * toReturn = new AbstractPredicate[a.actType->deleteEffectCount];
	
	for (int i = 0; i < a.actType->deleteEffectCount; i++) {
		const Predicate* currPred = a.actType->deleteEffects[i].predType;
		toReturn[i] = AbstractPredicate(currPred);
		for (int j = 0; j < currPred->getArgumentCount(); j++) {
			int rfi = a.actType->deleteEffects[i].parameters[j];
			if (rfi >= 0) {
				toReturn[i].parameters[j] = a.parameters[rfi];
			} else {
				toReturn[i].parameters[j] = rfi;
			}
		}
	}
	
	return toReturn;

};
*/
Plan* MacroAction::getConstituentActions(const ActionInstance * a) const {
	
	Plan* toReturn = new Plan();
	list<pair<list<AbstractAction>, double> >::const_iterator itr = abstractSteps.begin();
	list<pair<list<AbstractAction>, double> >::const_iterator itrEnd = abstractSteps.end();
	
	for ( ; itr != itrEnd; ++itr) {
		
		double layerTime = (*itr).second;
		
		for (list<AbstractAction>::const_iterator aItr = (*itr).first.begin(); aItr != (*itr).first.end(); aItr++) {
		
			const AbstractAction * currAA = &(*aItr);
			
			//cout << "-- Adding step for abstract action at time " << layerTime << ":" << currAA->actType->name() << endl;
			
			int pSize = currAA->actType->parameterCount;
			
			Entity** params = new Entity*[pSize];
			
			for (int i = 0; i < pSize; i++) {
				params[i] = a->getParameter(currAA->parameters[i])->boundTo();
			}
			
			ActionInstance* nextStep = new ActionInstance(currAA->actType, params);
			
			delete [] params;
			
			toReturn->blindlyAddStepAtEnd(nextStep, layerTime);
		
		}
	}
	
	//cout << "Returning plan:" << endl;
	//cout << *toReturn;
	
	//cout << endl;
	
	return toReturn;

}
/*
void MacroAction::calculateMutexInformation() {  

	bool verbose = false;
	list<pair<list<AbstractAction>, double> >::const_iterator itr = abstractSteps.begin();

	for (; itr != abstractSteps.end(); itr++) {
	
		pair<AbstractEventQueueItem, double> tp;
		tp.second = (*itr).second;
		abstractEventQueue.push_back(tp);
		AbstractEventQueueItem * eventQueueEntry = &(abstractEventQueue.back().first);  // fiddle - save having to type .back() all the time
		
		if(verbose) cout << "considering actions at offset " << ((*itr).second) << endl;
		for (	list<AbstractAction>::const_iterator taItr = (*itr).first.begin();
			taItr != (*itr).first.end();
			taItr++) {
			
			AbstractPredicate * addEffects = getAbstractAddEffects(*taItr);
			
			int limit = (*taItr).getAddEffectCount();

			for(int i = 0; i < limit; i++){
				eventQueueEntry->addEffects.insert(addEffects[i]);
			}
			
			delete [] addEffects;
			
			AbstractPredicate * taPreconditions = getAbstractPreconditions(*taItr);
			
			limit = (*taItr).getPreconditionCount();

			for(int i = 0; i < limit; i++){
				if (!taPreconditions[i].predType->isStatic()) {
					eventQueueEntry->invariants.insert(taPreconditions[i]);
				}
			}
			
			delete [] taPreconditions;
			
			AbstractPredicate * taNegativePreconditions = getAbstractNegativePreconditions(*taItr);
			
			limit = (*taItr).getNegativePreconditionCount();

			for(int i = 0; i < limit; i++){
				if (!taNegativePreconditions[i].predType->isStatic()) {
					eventQueueEntry->negativeInvariants.insert(taNegativePreconditions[i]);
				}
			}
			
			delete [] taNegativePreconditions;
			
			AbstractPredicate * deleteEffects = getAbstractDeleteEffects(*taItr);
		
			limit = (*taItr).getDeleteEffectCount();

			for(int i = 0; i < limit; i++){
				eventQueueEntry->deleteEffects.insert(deleteEffects[i]);
			}
			
			delete [] deleteEffects;
			
			list<pair<list<AbstractAction>, double> >::const_iterator checkSafe = itr;
			
			checkSafe++;
				
			for(; checkSafe != abstractSteps.end(); checkSafe++){
				for (	list<AbstractAction>::const_iterator intItr = (*checkSafe).first.begin();
					intItr != (*checkSafe).first.end();
					intItr++) {
					
					const AbstractAction * currAA = &(*intItr);
					
					AbstractPredicate* currPreconditions = getAbstractPreconditions(*currAA);
					
					limit = currAA->getPreconditionCount();
					
					//for each future precondition
		
					for(int i = 0; i < limit; i++){
						if (!currPreconditions[i].predType->isStatic()) {
							
							if (	eventQueueEntry->invariants.find(currPreconditions[i]) == eventQueueEntry->invariants.end()
							     &&	eventQueueEntry->addEffects.find(currPreconditions[i]) == eventQueueEntry->addEffects.end()   ) {
								list<pair<list<AbstractAction>, double> >::const_iterator potentialAdder = itr;
								potentialAdder++;
								//double timeRequired = (*checkSafe).second;

								//consider all actions from immediatley after the current time until the time at which this precondition is required
								bool added = false;
								for(; potentialAdder != checkSafe; potentialAdder++){

									for (	list<AbstractAction>::const_iterator paIntItr = (*potentialAdder).first.begin();
										paIntItr != (*potentialAdder).first.end();
										paIntItr++) {

									//find a future action that adds it or mark it as safe
										
										
										if (added) {
											if (abstractActionDeletes(*paIntItr, currPreconditions[i])) {
												added = false;
											}
										}
										if (!added){
											if (abstractActionAchieves(*paIntItr, currPreconditions[i])) {
												added = true;
											}
											
										}
									}
								}
				
								if(!added) {
									eventQueueEntry->invariants.insert(currPreconditions[i]); 
								}
							}
						}
						
					}
					
					delete [] currPreconditions;
				}	
			}
			
			checkSafe = itr;
			
			checkSafe++;
				
			for(; checkSafe != abstractSteps.end(); checkSafe++){
				for (	list<AbstractAction>::const_iterator intItr = (*checkSafe).first.begin();
					intItr != (*checkSafe).first.end();
					intItr++) {
					
					const AbstractAction * currAA = &(*intItr);
					
					AbstractPredicate* currNegativePreconditions = getAbstractNegativePreconditions(*currAA);
					
					limit = currAA->getNegativePreconditionCount();
					
					//for each future negative precondition
		
					for(int i = 0; i < limit; i++){
						if (!currNegativePreconditions[i].predType->isStatic()) {
							
							if (	eventQueueEntry->negativeInvariants.find(currNegativePreconditions[i]) == eventQueueEntry->negativeInvariants.end()
							     &&	eventQueueEntry->deleteEffects.find(currNegativePreconditions[i]) == eventQueueEntry->deleteEffects.end()   ) {
								list<pair<list<AbstractAction>, double> >::const_iterator potentialDeleter = itr;
								potentialDeleter++;
								//double timeRequired = (*checkSafe).second;

								//consider all actions from immediatley after the current time until the time at which this precondition is required
								bool deleted = false;
								for(; potentialDeleter != checkSafe; potentialDeleter++){

									for (	list<AbstractAction>::const_iterator paIntItr = (*potentialDeleter).first.begin();
										paIntItr != (*potentialDeleter).first.end();
										paIntItr++) {

									//find a future action that deletes it or mark it as unsafe
										
										if (!deleted){
											if (abstractActionDeletes(*paIntItr, currNegativePreconditions[i])) {
												deleted = true;
											}
											
										}
										
										if (deleted) {
											if (abstractActionAchieves(*paIntItr, currNegativePreconditions[i])) {
												deleted = false;
											}
										}
										
									}
								}
				
								if(!deleted) {
									eventQueueEntry->negativeInvariants.insert(currNegativePreconditions[i]); 
								}
							}
						}
						
					}
					
					delete [] currNegativePreconditions;
				}	
			}
			
		}
	
	}
	
	for (	list<pair<AbstractEventQueueItem, double> >::iterator aeqItr = abstractEventQueue.begin();
		aeqItr != abstractEventQueue.end();
		aeqItr++) {

		cout << (*aeqItr).second << ":" << endl;
		cout << "Add: " << endl;
		for (	set<AbstractPredicate>::iterator pItr = (*aeqItr).first.addEffects.begin();
			pItr != (*aeqItr).first.addEffects.end();
			pItr++) {
			
			cout << "\t";
			cout << (*pItr).predType->name();
			for (int i = 0; i < (*pItr).predType->getArgumentCount(); i++) {
				cout << " " << (*pItr).parameters[i];
			}
			cout << endl;
		}
		cout << "Delete: " << endl;
		for (	set<AbstractPredicate>::iterator pItr = (*aeqItr).first.deleteEffects.begin();
			pItr != (*aeqItr).first.deleteEffects.end();
			pItr++) {
			
			cout << "\t";
			cout << (*pItr).predType->name();
			for (int i = 0; i < (*pItr).predType->getArgumentCount(); i++) {
				cout << " " << (*pItr).parameters[i];
			}
			cout << endl;
		}
		cout << "Invariant: " << endl;
		for (	set<AbstractPredicate>::iterator pItr = (*aeqItr).first.invariants.begin();
			pItr != (*aeqItr).first.invariants.end();
			pItr++) {
			
			cout << "\t";
			cout << (*pItr).predType->name();
			for (int i = 0; i < (*pItr).predType->getArgumentCount(); i++) {
				cout << " " << (*pItr).parameters[i];
			}
			cout << endl;
		}
		for (	set<AbstractPredicate>::iterator pItr = (*aeqItr).first.negativeInvariants.begin();
			pItr != (*aeqItr).first.negativeInvariants.end();
			pItr++) {
			
			cout << "\t�(";
			cout << (*pItr).predType->name();
			for (int i = 0; i < (*pItr).predType->getArgumentCount(); i++) {
				cout << " " << (*pItr).parameters[i];
			}
			cout << ")" << endl;
		}
	}
	

}

  
*/
ostream & operator <<(ostream & o, const MacroAction & a){
	a.write(o);
	return o;

};
/*
PredicateInstance* MacroAction::groundWRT(const AbstractPredicate & p, const ActionInstance * realAction) const {

	Entity** params = new Entity*[p.predType->getArgumentCount()];
	for (int i = 0; i < p.predType->getArgumentCount(); i++) {
		params[i] = realAction->getParameter(p.parameters[i])->boundTo();
	}
	
	
	PredicateInstance* toReturn = new PredicateInstance(p.predType, params);
	
	delete [] params;
	
	return toReturn;

};

list<EventQueueItem> MacroAction::getMutexInformation(ActionInstance * realAction) const {

	list<EventQueueItem> toReturn;
	
	list<pair<AbstractEventQueueItem, double> >::const_iterator aeqItr = abstractEventQueue.begin();	
	
	for (; aeqItr != abstractEventQueue.end(); aeqItr++) {
	
		const AbstractEventQueueItem * aeqiToUse = &((*aeqItr).first);
		
		toReturn.push_back(EventQueueItem());
		
		EventQueueItem * eqiToUse = &(toReturn.back());
		
		for (int pass = 0; pass < 4; pass++) {
		
			set<AbstractPredicate>::const_iterator apItr;
			set<AbstractPredicate>::const_iterator apEnd;
			PredicateInstanceSet * insertInto;
			
			
			if (pass == 0) {
				apItr = aeqiToUse->addEffects.begin();
				apEnd = aeqiToUse->addEffects.end();
				insertInto = &(eqiToUse->addEffects);
			} else if (pass == 1) {
				apItr = aeqiToUse->deleteEffects.begin();
				apEnd = aeqiToUse->deleteEffects.end();
				insertInto = &(eqiToUse->deleteEffects);
			} else if (pass == 2) {
				apItr = aeqiToUse->invariants.begin();
				apEnd = aeqiToUse->invariants.end();
				insertInto = &(eqiToUse->invariants);
			} else {
				apItr = aeqiToUse->negativeInvariants.begin();
				apEnd = aeqiToUse->negativeInvariants.end();
				insertInto = &(eqiToUse->negativeInvariants);
			}
		
			for (; apItr != apEnd; apItr++) {
			
				insertInto->insert(groundWRT(*apItr, realAction));
				
			}
		
		}

	}
	
	return toReturn;

};

*/
double MacroAction::duration() const {

	double toReturn = 0.0;
	
	list<pair<list<AbstractAction>, double> >::const_iterator itr = abstractSteps.begin();
	
	for (; itr != abstractSteps.end(); itr++) {
	
		if ((*itr).second > toReturn) {
			toReturn = (*itr).second;
		}
	
	}
	
	toReturn += 1.0;

	return toReturn;

}


void MacroAction::writeAbstractActions(ostream & o) const {

	o << "; " << *this;
	list<pair<list<AbstractAction>, double> >::const_iterator itr = abstractSteps.begin();
	for (; itr != abstractSteps.end(); itr++) {
	
		double currTS = (*itr).second;
		for (	list<AbstractAction>::const_iterator aItr = (*itr).first.begin();
			aItr != (*itr).first.end();
			aItr++) {
			
			const AbstractAction* currAA = &(*aItr);
			//o << "; Action at " << currAA->actType << endl;
			o << "; " << currTS << ": " << currAA->actType->name();
			for (int i = 0; i < currAA->actType->parameterCount; i++) {
				int tc = currAA->parameters[i];
				if (tc >= 0) {
					o << " " << tc;
				} else {
					o << " " << currAA->actType->constants[-1 -tc]->name();
				}
			}
			o << " [1]" << endl;
		}
		
	
	}
	o << ";" << endl;

};

struct GroundOutActionData {

	AbstractAction soFar;
	State* stateToUse;
	ActionInstanceWithSubActionsList* unit;

	list<pair<list<AbstractAction>, double> >::iterator itr;
	list<AbstractAction>::iterator subStepItr;
	
	double timeStamp;
	int actOnThisLayer;

	ActionInstanceList toApply;
	
	
		
	GroundOutActionData(	const AbstractAction & s,
				State *& c,
				ActionInstanceWithSubActionsList* u,
				list<pair<list<AbstractAction>, double> >::iterator & aii,
				list<AbstractAction>::iterator & ssiIn,
				const double & t,
				const int & ao,
				ActionInstanceList & toApplyIn)
	 : soFar(s), stateToUse(c), unit(u), itr(aii), subStepItr(ssiIn), timeStamp(t), actOnThisLayer(ao), toApply(toApplyIn) {};


};


auto_ptr<ActionInstanceList> MacroAction::derivedActions(ActionInstanceWithSubActionsList & unitActions, State *& startAt, RelaxedPlanningGraph *& masterRPG, StaticInformation *& staticInf) {

//	set<AbstractAction>* possibleBindings = new set<AbstractAction>();
	
	auto_ptr<ActionInstanceList> toReturn(new ActionInstanceList() );
	
	list<pair<list<AbstractAction>, double> >::iterator asItr = abstractSteps.begin();
	const list<pair<list<AbstractAction>, double> >::iterator asEnd = abstractSteps.end();
	
	
	list<pair<list<AbstractAction>, double> >::iterator asStart = asItr;
	
	slist<State*> toDelete;
	slist<ActionInstanceWithSubActionsList*> falToDelete;
	
	
	slist<GroundOutActionData*> asQueue;
	AbstractAction tmpAA(this);
	ActionInstanceList tmpList;
	static const double tmpZeroDouble = 0.0;
	static const int tmpZeroInt = 0;
	
	list<AbstractAction>::iterator tmpTmpItr = (*asItr).first.begin();
	
	asQueue.push_front(new GroundOutActionData(	tmpAA,
							startAt,
							&unitActions,
							asStart,
							tmpTmpItr,
							tmpZeroDouble,
							tmpZeroInt,
							tmpList));
	
	while (!asQueue.empty()) {
		
		GroundOutActionData * curr = asQueue.front();
		asQueue.pop_front();
		
		if (curr->subStepItr == (*(curr->itr)).first.end()) {
		
		
			
			bool abort = false;
			
			
				
				
			ActionInstanceList::iterator mutexCheckItr = curr->toApply.begin();
			ActionInstanceList::iterator mutexCheckEnd = curr->toApply.end();
				
			for (; !abort && mutexCheckItr != mutexCheckEnd; ++mutexCheckItr)  {
			
				ActionInstanceList::iterator rest = mutexCheckItr;
				++rest;
					
				for (; !abort && rest != mutexCheckEnd; ++rest) {
					abort = (*rest)->isMutexWith(*(*mutexCheckItr));
				}
			}
				
			if (!abort) {
				for (list<AbstractAction>::const_iterator cseItr = (*(curr->itr)).first.begin(); cseItr != (*(curr->itr)).first.end(); ++cseItr) {
				
					Entity** parameters = new Entity*[(*cseItr).actType->parameterCount];
					for (int i = 0; i < (*cseItr).actType->parameterCount; ++i) {
						parameters[i] = curr->soFar.hardParameters[(*cseItr).parameters[i]];
					}
					ActionInstance* tmpAI = new ActionInstance((*cseItr).actType, parameters);
					masterRPG->getMasterAI(tmpAI);
					delete [] parameters;
					delete tmpAI;
				}
					
				curr->itr++;
				if (curr->itr == asEnd) {
					ActionInstance* newAI = new ActionInstance(curr->soFar.actType, curr->soFar.hardParameters);
					toReturn->push_back(newAI);
					delete curr;
				} else {
					static RelaxedPlanningGraph* rpg = 0;
						
					State* nextState  = curr->stateToUse->applyActions(curr->toApply, rpg, staticInf, true);
	
					toDelete.push_front(nextState);
					
					double nextTime = (*(curr->itr)).second;
					
					while (curr->timeStamp < nextTime) {
						nextState->advanceTime();
						curr->timeStamp+=1.0;
					}
					
					if (staticInf->hasDerivedPredicates) {
					
						auto_ptr<ActionInstanceList> freeActions = masterRPG->freeActions(*staticInf, nextState);
						while (!freeActions->empty()) {
							nextState->applyActionsLocally(*(freeActions.get()), staticInf);
							nextState->advanceTime();
							nextState->timeBackOne();
							freeActions = masterRPG->freeActions(*staticInf, nextState);
						}
						
					
					}
					
					curr->stateToUse = nextState;
					curr->toApply.clear();
						
					//SymmetryExtractor symm;
				
					//list<SymmetryGroup*> symGroups = symm.extractSymmetryGroups(*staticInf, true, curr->stateToUse);
						
					const EntitySet* exemplars = 0;
					
					//EntitySet* exemplars = new EntitySet();
						
					/*for (list<SymmetryGroup*>::iterator itr = symGroups.begin();
						itr != symGroups.end();
						itr++) {
						
						exemplars->insert((*itr)->getExemplar());
								
					}*/
						//ActionInstanceList applicableActions = stateToUse->applicableActions(staticInf, symGroups);
						
					auto_ptr<ActionInstanceWithSubActionsList> rawApplicableActions = masterRPG->applicableRelaxedActions(*staticInf, nextState, exemplars);
					auto_ptr<ActionInstanceWithSubActionsList> applicableActions = nextState->filterApplicableActions(staticInf, *(rawApplicableActions.get()));
						
					/*	
					while (!symGroups.empty()) {
						delete symGroups.front();
						symGroups.pop_front();
					}
						
					delete exemplars;
					*/	
					curr->unit = applicableActions.release();
					falToDelete.push_front(curr->unit);
					curr->actOnThisLayer = 0;
					curr->subStepItr = (*(curr->itr)).first.begin();
					asQueue.push_front(curr);
					
					
				}
				
				
			} else {
				delete curr;
			}
				
			
			
			
		} else { // not at end of current layer
		
			list<AbstractAction>::iterator nextStepItr = curr->subStepItr;
			const int nextStepID =  curr->actOnThisLayer + 1;
			
			const Action* toMatch = (*(curr->subStepItr)).actType;
			nextStepItr++;
			ActionInstanceWithSubActionsList::iterator uiiItr = curr->unit->begin();
			ActionInstanceWithSubActionsList::iterator uiiEnd = curr->unit->end();
			
			for (; uiiItr != uiiEnd; ++uiiItr) {
				const ActionInstance* currAI = (*uiiItr)->getFirst();
				//masterRPG->getMasterAI(currAI);
				if (currAI->getParentAction() == toMatch) {
					bool carryOn = true;
					AbstractAction newAA(curr->soFar);
					for (int i = 0; carryOn && i < (*(curr->subStepItr)).actType->parameterCount; i++) {
						int lui = (*(curr->subStepItr)).parameters[i];
						const Entity* ce;
						if (lui < 0) {
							ce = constants[-1 - lui];
						} else {
							ce = newAA.hardParameters[lui];
						}
						if (ce == 0) {
							newAA.hardParameters[lui] = currAI->getParameter(i)->boundTo();
						} else {
							if (ce != currAI->getParameter(i)->boundTo()) carryOn = false;
						}
						
						
						
					}
					if (carryOn) {
						/*cout << "Using as source action: ";
						currAI->briefWrite(cout);
						AbstractAction newAA(curr->soFar);
						for (int i = 0; i < (*(curr->subStepItr)).actType->parameterCount; i++) {
							cout << "Binding p
							int lui = (*(curr->subStepItr)).parameters[i];
							if (lui >= 0) {
								newAA.hardParameters[lui] = currAI->getParameter(i)->boundTo();
							}
						}*/
						
						if (false) { //debug branch
						
							list<AbstractAction>::iterator cseItr = (*(curr->itr)).first.begin();
						
							for (; cseItr != nextStepItr; ++cseItr) {
						
								Entity** parameters = new Entity*[(*cseItr).actType->parameterCount];
								for (int i = 0; i < (*cseItr).actType->parameterCount; ++i) {
									parameters[i] = newAA.hardParameters[(*cseItr).parameters[i]];
								}
								ActionInstance* tmpAI = new ActionInstance((*cseItr).actType, parameters);
								masterRPG->getMasterAI(tmpAI);
								delete [] parameters;
								delete tmpAI;
						
							}
						}
						
						//cout << "Successfully bound up to action " << curr->actOnThisLayer << " out of " << (*(curr->itr)).first.size() << " on this layer" << endl;
						
						ActionInstanceList newToApply(curr->toApply);
						
						newToApply.push_back(const_cast<ActionInstance*>(currAI));
						ActionInstanceList::iterator csaItr = (*uiiItr)->getSecond().begin();
						ActionInstanceList::iterator csaEnd = (*uiiItr)->getSecond().end();
						
						for (;csaItr != csaEnd; ++csaItr) newToApply.push_back(*csaItr);
						
						asQueue.push_front(new GroundOutActionData(newAA, curr->stateToUse, curr->unit, curr->itr, nextStepItr, curr->timeStamp, nextStepID,  newToApply));
						
					}
				}
			}
			
			delete curr;
				
		}
		
		
	}
	
	while (!toDelete.empty()) {
		delete toDelete.front();
		toDelete.pop_front();
	}
	
	while (!falToDelete.empty()) {
		delete falToDelete.front();
		falToDelete.pop_front();
	}
	if (false) { //debug branch
		for (ActionInstanceList::iterator crItr = toReturn->begin(); crItr != toReturn->end(); ++crItr) {
		
			Plan* tmp = getConstituentActions(*crItr);
			
			for (Plan::iterator tmpItr = tmp->begin(); tmpItr != tmp->end(); ++tmpItr) {
				masterRPG->getMasterAI((*tmpItr).first);
			}
			
			delete tmp;
		
		}
	}

	
	return toReturn;
	

}

bool MacroAction::operator ==(const MacroAction & o) const {

	if (abstractSteps.size() != o.abstractSteps.size()) return false;
	if (parameterCount != o.parameterCount) return false;
	
	list<pair<list<AbstractAction>, double> >::const_iterator itr = abstractSteps.begin();
	list<pair<list<AbstractAction>, double> >::const_iterator oItr = o.abstractSteps.begin();
	list<pair<list<AbstractAction>, double> >::const_iterator itrEnd = abstractSteps.end();
	
	for (; itr != itrEnd; ++itr, ++oItr) {
	
		if ((*itr).first.size() != (*oItr).first.size()) return false;
		list<AbstractAction>::const_iterator asItr = (*itr).first.begin();
		list<AbstractAction>::const_iterator asEnd = (*itr).first.end();
		
		for (; asItr != asEnd; asItr++) {
		
			list<AbstractAction>::const_iterator oasItr = (*oItr).first.begin();
			list<AbstractAction>::const_iterator oasEnd = (*oItr).first.end();
			
			bool found = false;
			
			for (; !found && oasItr != oasEnd; ++oasItr) {
				found = ((*asItr).actType == (*oasItr).actType);
			}
			
			if (!found) return false;
		
		}
		
	
	}
	
	return true;

}

void MacroAction::updateActions(hash_map<Action*, Action*> & convertActions) {
	list<pair<list<AbstractAction>, double> >::iterator itr =  abstractSteps.begin();
	const list<pair<list<AbstractAction>, double> >::iterator itrEnd =  abstractSteps.end();
	
	for (;itr != itrEnd; ++itr) {
		list<AbstractAction>::iterator asItr = (*itr).first.begin();
		const list<AbstractAction>::iterator asEnd = (*itr).first.end();
		
		for (;asItr != asEnd; ++asItr) {
			Action* was = const_cast<Action*>((*asItr).actType);
			(*asItr).actType = convertActions[was];
			
//			cout << "Action was at " << was << "; now at " << (*asItr).actType << endl;
		}
	}
};

void MacroAction::save(ostream & o) const {

	o << parameterCount << endl;
	for (int i = 0; i < parameterCount; ++i) {
		o << parameterTypes[i]->name() << endl;
	}
	list<pair<list<AbstractAction>, double> >::const_iterator itr = abstractSteps.begin();
	const list<pair<list<AbstractAction>, double> >::const_iterator itrEnd = abstractSteps.end();
	
	o << abstractSteps.size() << endl;
	
	for (; itr != itrEnd; ++itr) {
	
		o << (*itr).first.size() << endl;
		o << (*itr).second << endl;
		
		list<AbstractAction>::const_iterator aItr = (*itr).first.begin();
		const list<AbstractAction>::const_iterator aEnd = (*itr).first.end();
		
		for (; aItr != aEnd; ++aItr) {
		
			const AbstractAction & aa = (*aItr);
			o << aa.actType->name() << endl;
			const int lim = aa.actType->parameterCount;
			o << lim << endl;
			for (int i = 0; i < lim; ++i) {
				o << aa.parameters[i] << endl;
			};
			
		
		};
	
	};
	

};

MacroAction* MacroAction::loadMacroAction(istream & o, StaticInformation & problemInformation, const unsigned int & tsu, const unsigned int & uc) {
	
	const bool verbose = true;
	if (verbose) cout << "Loading macro action" << endl;
	int parameterCount;
	o >> parameterCount;
	if (verbose) cout << "Action has " << parameterCount << " parameters" << endl;
	
	vector<Type*> parameterTypes(parameterCount);
	
	for (int i = 0; i < parameterCount; ++i) {
	
		string tname;
		o >> tname;
		
		bool foundYet = false;
		for (int j = 0; !foundYet && j < problemInformation.numberOfTypes; ++j) {
		
			const string cw = problemInformation.Types[j]->name();
			if (cw == tname) {
				if (verbose) {
					cout << "Parameter " << i << ", type " << tname << " matches " << cw << endl;
				}
				foundYet = true;
				parameterTypes[i] = problemInformation.Types[j];
			}
		
		};
		if (verbose) {
			if (!foundYet) {
				cout << "Parameter " << i << ", type " << tname << ", does not match any type in this domain" << endl;
			}
		}
		assert(foundYet);
		
	};
	
	list<pair<list<AbstractAction>, double> > abstractSteps;
	
	int lim; 
	o >> lim;
	
	for (int i = 0; i < lim; ++i) {
	
		list<AbstractAction> aaList;
		
		int jlim;
		o >> jlim;
		
		double ts;
		o >> ts;
		
		
		for (int j = 0; j < jlim; ++j) {
		
			string stepName;
			o >> stepName;
			int argCount;
			o >> argCount;
			
			vector<int> args(argCount);
			
			for (int k = 0; k < argCount; ++k) {
				o >> args[k];
			};
			
			bool foundYet = false;
			for (int k = 0; !foundYet && k < problemInformation.numberOfActions; ++k) {
			
				const string cw = problemInformation.Actions[k]->name();
				if (cw == stepName && problemInformation.Actions[k]->parameterCount == argCount) {
					bool matches = true;
					for (int c = 0; matches && c < argCount; ++c) {
						Type* const t1 = parameterTypes[args[c]];
						Type* const t2 = problemInformation.Actions[k]->parameterTypes[c];
						matches = ((t1 == t2) || (t1->isASubTypeOf(t2)));
					};
					
					if (matches) {
						foundYet = true;
						AbstractAction newAA(problemInformation.Actions[k]);
						for (int c = 0; c < argCount; ++c) newAA.parameters[c] = args[c];
						aaList.push_back(newAA);
					}
				};
			
			};
			assert(foundYet);
		
		};
		
		abstractSteps.push_back(make_pair(aaList, ts));
	
	};
	return new MacroAction(parameterCount, parameterTypes, abstractSteps, problemInformation, tsu, uc);
	

};


MacroAction::MacroAction(const int & pcIn, const vector<Type*> & ptIn, const list<pair<list<AbstractAction>, double> > & asIn, StaticInformation & problemInformation, const unsigned int & tsu, const unsigned int & uc)
 : Action(pcIn), loadedFromDisk(true), timesSinceUse(tsu), usageCount(uc) {


	isMacroAction = true;
	
	parameterDomains = new EntitySet*[parameterCount];
	ownParameterDomains = new bool[parameterCount];
	originalBindings = new Entity*[parameterCount];
	
	for (int i = 0; i < parameterCount; ++i) {
	
		parameterTypes[i] = ptIn[i];
		originalBindings[i] = 0;
		parameterDomains[i] = 0;
		ownParameterDomains[i] = false;
	
	};
	
	abstractSteps = asIn;

	definedName = "Macro-action: ";

	ownName = true;
	
	list<pair<list<AbstractAction>, double> >::iterator mnItr = abstractSteps.begin();
	const list<pair<list<AbstractAction>, double> >::iterator mnEnd = abstractSteps.end();
	
	for (; mnItr != mnEnd; ++mnItr) {
	
		list<AbstractAction>::iterator aaItr = (*mnItr).first.begin();
		const list<AbstractAction>::iterator aaEnd = (*mnItr).first.end();
		
		for (; aaItr != aaEnd; ++aaItr) {
			definedName += (*aaItr).actType->name();
			definedName += " ";
		};
	
	};
	
		
	
	jointParameterDomainConstraints = new hash_map<Entity*, EntitySet**>*[parameterCount];
	

	for (int i = 0; i < parameterCount; i++) {
	
		hash_map<Entity*, EntitySet**> * eHash = new hash_map<Entity*, EntitySet**>();
		
		jointParameterDomainConstraints[i] = eHash;
		
	}
	
	dummyStaticJointParameterDomainConstraints(&problemInformation);

//	createExemplarCounters();
	
};

DASearchStatus::~DASearchStatus(){

	while (!asQueue.empty()) {
		delete asQueue.front();
		asQueue.pop_front();
	}	

	while (!toDelete.empty()) {
		delete toDelete.front();
		toDelete.pop_front();
	}
	
	while (!falToDelete.empty()) {
		delete falToDelete.front();
		falToDelete.pop_front();
	}
};


DASearchStatus MacroAction::beginDerivedActions( ActionInstanceWithSubActionsList & unitActions, State *& startAt) {
	static const double tmpZeroDouble = 0.0;
	static const int tmpZeroInt = 0;
	
	DASearchStatus toReturn(abstractSteps.begin(), abstractSteps.end());
	
	AbstractAction tmpAA(this);
	ActionInstanceList tmpList;
		
	list<AbstractAction>::iterator tmpTmpItr = (*(toReturn.asItr)).first.begin();
	
	toReturn.asQueue.push_front(new GroundOutActionData(	tmpAA,
							startAt,
							&unitActions,
							(toReturn.asItr),
							tmpTmpItr,
							tmpZeroDouble,
							tmpZeroInt,
							tmpList));
							
	return toReturn;

};

ActionInstance* MacroAction::derivedActions(DASearchStatus & status, RelaxedPlanningGraph *& masterRPG, StaticInformation *& staticInf) {

//	set<AbstractAction>* possibleBindings = new set<AbstractAction>();
	
	ActionInstance* toReturn = 0;
	
	
	
	while (toReturn == 0 && !status.asQueue.empty()) {
		
		GroundOutActionData * curr = status.asQueue.front();
		status.asQueue.pop_front();
		
		if (curr->subStepItr == (*(curr->itr)).first.end()) {
		
		
			
			bool abort = false;
			
			
				
				
			ActionInstanceList::iterator mutexCheckItr = curr->toApply.begin();
			ActionInstanceList::iterator mutexCheckEnd = curr->toApply.end();
				
			for (; !abort && mutexCheckItr != mutexCheckEnd; ++mutexCheckItr)  {
			
				ActionInstanceList::iterator rest = mutexCheckItr;
				++rest;
					
				for (; !abort && rest != mutexCheckEnd; ++rest) {
					abort = (*rest)->isMutexWith(*(*mutexCheckItr));
				}
			}
				
			if (!abort) {
				for (list<AbstractAction>::const_iterator cseItr = (*(curr->itr)).first.begin(); cseItr != (*(curr->itr)).first.end(); ++cseItr) {
				
					Entity** parameters = new Entity*[(*cseItr).actType->parameterCount];
					for (int i = 0; i < (*cseItr).actType->parameterCount; ++i) {
						parameters[i] = curr->soFar.hardParameters[(*cseItr).parameters[i]];
					}
					ActionInstance* tmpAI = new ActionInstance((*cseItr).actType, parameters);
					masterRPG->getMasterAI(tmpAI);
					delete [] parameters;
					delete tmpAI;
				}
					
				curr->itr++;
				if (curr->itr == status.asEnd) {
					ActionInstance* newAI = new ActionInstance(curr->soFar.actType, curr->soFar.hardParameters);
					toReturn = newAI;
					delete curr;
				} else {
					static RelaxedPlanningGraph* rpg = 0;
						
					State* nextState  = curr->stateToUse->applyActions(curr->toApply, rpg, staticInf, true);
	
					status.toDelete.push_front(nextState);
					
					double nextTime = (*(curr->itr)).second;
					
					while (curr->timeStamp < nextTime) {
						nextState->advanceTime();
						curr->timeStamp+=1.0;
					}
					
					if (staticInf->hasDerivedPredicates) {
					
						auto_ptr<ActionInstanceList> freeActions = masterRPG->freeActions(*staticInf, nextState);
						while (!freeActions->empty()) {
							nextState->applyActionsLocally(*(freeActions.get()), staticInf);
							nextState->advanceTime();
							nextState->timeBackOne();
							freeActions = masterRPG->freeActions(*staticInf, nextState);
						}
						//masterRPG->derivePredicates(*staticInf, nextState);
						
					
					}
					
					curr->stateToUse = nextState;
					curr->toApply.clear();
						
					//SymmetryExtractor symm;
				
					//list<SymmetryGroup*> symGroups = symm.extractSymmetryGroups(*staticInf, true, curr->stateToUse);
						
					const EntitySet* const exemplars = 0;
					
					//EntitySet* exemplars = new EntitySet();
						
					/*for (list<SymmetryGroup*>::iterator itr = symGroups.begin();
						itr != symGroups.end();
						itr++) {
						
						exemplars->insert((*itr)->getExemplar());
								
					}*/
						//ActionInstanceList applicableActions = stateToUse->applicableActions(staticInf, symGroups);
						
					auto_ptr<ActionInstanceWithSubActionsList> rawApplicableActions = masterRPG->applicableRelaxedActions(*staticInf, nextState, exemplars);
					auto_ptr<ActionInstanceWithSubActionsList> applicableActions = nextState->filterApplicableActions(staticInf, *(rawApplicableActions.get()));
						
					/*	
					while (!symGroups.empty()) {
						delete symGroups.front();
						symGroups.pop_front();
					}
						
					delete exemplars;
					*/	
					curr->unit = applicableActions.release();
					status.falToDelete.push_front(curr->unit);
					curr->actOnThisLayer = 0;
					curr->subStepItr = (*(curr->itr)).first.begin();
					status.asQueue.push_front(curr);
					
					
				}
				
				
			} else {
				delete curr;
			}
				
			
			
			
		} else { // not at end of current layer
		
			list<AbstractAction>::iterator nextStepItr = curr->subStepItr;
			const int nextStepID =  curr->actOnThisLayer + 1;
			
			const Action* toMatch = (*(curr->subStepItr)).actType;
			nextStepItr++;
			ActionInstanceWithSubActionsList::iterator uiiItr = curr->unit->begin();
			ActionInstanceWithSubActionsList::iterator uiiEnd = curr->unit->end();
			
			for (; uiiItr != uiiEnd; ++uiiItr) {
				const ActionInstance* currAI = (*uiiItr)->getFirst();
				//masterRPG->getMasterAI(currAI);
				if (currAI->getParentAction() == toMatch) {
					bool carryOn = true;
					AbstractAction newAA(curr->soFar);
					for (int i = 0; carryOn && i < (*(curr->subStepItr)).actType->parameterCount; i++) {
						int lui = (*(curr->subStepItr)).parameters[i];
						const Entity* ce;
						if (lui < 0) {
							ce = constants[-1 - lui];
						} else {
							ce = newAA.hardParameters[lui];
						}
						if (ce == 0) {
							Entity* const newE = currAI->getParameter(i)->boundTo();
							/*for (int j = 0; carryOn && j < parameterCount; ++j) {
								carryOn = (newAA.hardParameters[j] != newE);
							}
							if (carryOn) */newAA.hardParameters[lui] = newE;
						} else {
							if (ce != currAI->getParameter(i)->boundTo()) carryOn = false;
						}
						
						
						
					}
					if (carryOn) {
						/*cout << "Using as source action: ";
						currAI->briefWrite(cout);
						AbstractAction newAA(curr->soFar);
						for (int i = 0; i < (*(curr->subStepItr)).actType->parameterCount; i++) {
							cout << "Binding p
							int lui = (*(curr->subStepItr)).parameters[i];
							if (lui >= 0) {
								newAA.hardParameters[lui] = currAI->getParameter(i)->boundTo();
							}
						}*/
						
						if (false) { //debug branch
						
							list<AbstractAction>::iterator cseItr = (*(curr->itr)).first.begin();
						
							for (; cseItr != nextStepItr; ++cseItr) {
						
								Entity** parameters = new Entity*[(*cseItr).actType->parameterCount];
								for (int i = 0; i < (*cseItr).actType->parameterCount; ++i) {
									parameters[i] = newAA.hardParameters[(*cseItr).parameters[i]];
								}
								ActionInstance* tmpAI = new ActionInstance((*cseItr).actType, parameters);
								masterRPG->getMasterAI(tmpAI);
								delete [] parameters;
								delete tmpAI;
						
							}
						}
						
						//cout << "Successfully bound up to action " << curr->actOnThisLayer << " out of " << (*(curr->itr)).first.size() << " on this layer" << endl;
						
						ActionInstanceList newToApply(curr->toApply);
						
						newToApply.push_back(const_cast<ActionInstance*>(currAI));
						ActionInstanceList::iterator csaItr = (*uiiItr)->getSecond().begin();
						ActionInstanceList::iterator csaEnd = (*uiiItr)->getSecond().end();
						
						for (;csaItr != csaEnd; ++csaItr) newToApply.push_back(*csaItr);
						
						status.asQueue.push_front(new GroundOutActionData(newAA, curr->stateToUse, curr->unit, curr->itr, nextStepItr, curr->timeStamp, nextStepID,  newToApply));
						
					}
				}
			}
			
			delete curr;
				
		}
		
		
	}
	
	
	

	
	return toReturn;
	

}

