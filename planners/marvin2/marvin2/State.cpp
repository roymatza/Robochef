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
#include "State.h"
#include "PredicateInstance.h"
#include "staticinformation.h"
#include <ext/slist>

using __gnu_cxx::slist;

#include <values.h>
#include "SymmetryGroup.h"
#include "MacroAction.h"
#include "relaxedPlanningGraph.h"

#include "ActionSequenceValidator.h"


State::State() : parentState(0), timeStamp(0) {
#ifdef CHECK_STATE_LEAKS
	leakCheck = new int[1];
#endif
}

State::State(StaticInformation * staticInf, const PredicateInstanceList & p) : parentState(0), /*currState(p), */currStateTrie(staticInf, p), timeStamp(0) {

#ifdef CHECK_STATE_LEAKS
	leakCheck = new int[1];
#endif

};

State::State(StaticInformation * staticInf) : parentState(0), currStateTrie(staticInf), timeStamp(0) {

#ifdef CHECK_STATE_LEAKS
	leakCheck = new int[1];
#endif

};


State::~State() {
	
#ifdef CHECK_STATE_LEAKS
	delete [] leakCheck;
#endif
	
}

State::State(const State & s) :
	parentState(s.parentState),
	/*currState(s.currState),*/
	currStateTrie(s.currStateTrie),
	timeStamp(s.timeStamp),
	predicatesToAdd(s.predicatesToAdd),
	predicatesToDelete(s.predicatesToDelete),
	predicatesInvariant(s.predicatesInvariant),
	predicatesNegativeInvariant(s.predicatesNegativeInvariant),
	eventQueue(s.eventQueue),
	achievingActions(s.achievingActions) {

#ifdef CHECK_STATE_LEAKS
	leakCheck = new int[1];
#endif


}


State & State::operator=(const State & s) {


	parentState = s.parentState;
	/*currState = s.currState;*/
	currStateTrie = s.currStateTrie;
	
	timeStamp = s.timeStamp;
	
	predicatesToAdd = s.predicatesToAdd;
	predicatesToDelete = s.predicatesToDelete;
	
	predicatesInvariant = s.predicatesInvariant;
	predicatesNegativeInvariant = s.predicatesNegativeInvariant;
	
	eventQueue = s.eventQueue;
	
	achievingActions = s.achievingActions;
	

	return *this;
}

void State::addPredicateInstance(PredicateInstance * p) {
	
	
	if (currStateTrie.contains(p)) return;
	
	
	currStateTrie.addPredicateInstance(p);
	/*currState.push_back(p);*/
	
}


void State::removePredicateInstance(PredicateInstance * p) {

	currStateTrie.removePredicateInstance(p);
	/*PredicateInstanceList::iterator itr = currState.begin();
	PredicateInstanceList::iterator itrEnd = currState.end();
	for (; itr != itrEnd; ++itr) {
		if ((*itr) == p) {
			currState.erase(itr);
			return;
		}
	}*/
	// p not found!
}

bool State::containsPredicateInstance(PredicateInstance * p) const {
	return currStateTrie.contains(p);
}

bool State::contains(const Predicate * p, Entity** parameters) const {
	return currStateTrie.contains(p, parameters);
};

bool State::containsNegative(const Predicate * p, Entity** parameters) const {
	return currStateTrie.containsNegative(p, parameters);
};

void State::removeForeignPredicateInstance(PredicateInstance * p) {

	PredicateInstance* knownHereAs = currStateTrie.getPredicateInstance(p);
	
	if (knownHereAs != 0) {
		removePredicateInstance(knownHereAs);
	}


}

void State::removePredicateInstance(State::iterator p) {
	currStateTrie.removePredicateInstance(*p);
	
	/*currState.erase(p);*/
	
}

void State::applyActionLocally(ActionInstance * toApply, StaticInformation * staticInf) {
	static RelaxedPlanningGraph* rpg = 0;
	ActionInstanceList toPass;
	toPass.push_front(toApply);
	internalApplyActions(toPass, false, rpg, staticInf, true, false);
	
}

void State::applyActionsLocally(ActionInstanceList & toApply, StaticInformation * staticInf) {
	static RelaxedPlanningGraph* rpg = 0;
	internalApplyActions(toApply, false, rpg, staticInf, true, false);
	
}

void State::applyActionsLocally(ActionInstanceList & toApply, RelaxedPlanningGraph*& rpg, StaticInformation * staticInf) {
	internalApplyActions(toApply, false, rpg, staticInf, true, false);
	
}

void State::applyActionsLocally(ActionInstanceList & toApply, StaticInformation * staticInf, const bool & supressTOs) {
	static RelaxedPlanningGraph* rpg = 0;
	internalApplyActions(toApply, false, rpg, staticInf, true, supressTOs);
	
}



State* State::applyAction(ActionInstance * toApply, StaticInformation * staticInf) {

	static RelaxedPlanningGraph* rpg = 0;
	
	ActionInstanceList toPass;
	toPass.push_front(toApply);
	return internalApplyActions(toPass, false, rpg, staticInf, false, false);
	
}

State* State::applyActions(const ActionInstanceList & toApply, RelaxedPlanningGraph*& rpg, StaticInformation *& staticInf){

	return internalApplyActions(toApply, false, rpg, staticInf, false, false);
	
}

State* State::applyActions(const ActionInstanceList & toApply, RelaxedPlanningGraph*& rpg, StaticInformation *& staticInf, const bool & supressTOs){

	return internalApplyActions(toApply, false, rpg, staticInf, false, supressTOs);
	
}



State* State::applyRelaxedActions(const ActionInstanceList & toApply, StaticInformation * staticInf){

	static RelaxedPlanningGraph* rpg = 0;
	return internalApplyActions(toApply, true, rpg, staticInf, false, false);

}

State* State::copy() {
	State* toReturn = new State(*this);
	return toReturn;
}

State* State::internalApplyActions(const ActionInstanceList & toApply, const bool & relaxedActions, RelaxedPlanningGraph*& rpg, StaticInformation *& staticInf, const bool & local, const bool & supressTOs) {

	const EntitySet* dummyEntSet = 0;
	
	State* toReturn = this;
	
	if (!local) {
		toReturn = new State(*this);
		toReturn->achievingActions.clear();
	}
//	toReturn->parentState = this;
	
	
	
	ActionInstanceList::const_iterator itr = toApply.begin();
	
	for (; itr != toApply.end(); ++itr) {
	
		ActionInstance* curr = (*itr);
		
		Plan* TOPlan = 0;
		if (!supressTOs && curr->getParentAction()->transformationalOperator) {
			const Action* currPA = curr->getParentAction();
			TOPlan = currPA->sequenceValidator->GetActionsToApply(curr->getParameter(currPA->sequenceValidator->getParameterOne())->boundTo(), curr->getParameter(currPA->sequenceValidator->getParameterTwo())->boundTo(), timeStamp, curr);
		}
		
		
		if (!curr->isMacroAction() && (TOPlan == 0 || TOPlan->length() == 1)) {
			
			delete TOPlan;
			
			if (staticInf->verbose) {
				cout << "Applying action ";
				curr->briefWrite(cout);
			}
			
			
			
			//cout << endl;
			PredicateInstance** effArray;
			if (!relaxedActions) {
			
				if (staticInf->verbose) {
					cout << "Applying delete effects" << endl;
				}
				effArray = curr->internalDeleteEffects();
				
				for (int i = 0; i < curr->getDeleteEffectCount(); i++) {
				
					if (staticInf->verbose) cout << "\tDeleting " << *effArray[i] << endl;
					if (toReturn->currStateTrie.contains(effArray[i])) {
						if (staticInf->verbose) cout << "\t\tMarking to be deleted" << endl;
						toReturn->setToDelete(effArray[i]);
					} else {
						if (staticInf->verbose) cout << "\t\tMarking as negative invariant" << endl;
						
						toReturn->setNegativeInvariant(effArray[i]);
						//toReturn->addPredicateInstance(effArray[i]);
						//toReturn->setUnsafe(effArray[i]);
						
					}
					
					
				}
			} else {
				if (staticInf->verbose) {
					cout << "Relaxed action application - no delete effects" << endl;
				}
			}
		
			//bool doneAnything = false;
			
			effArray = curr->internalAddEffects();
			
			for (int i = 0; i < curr->getAddEffectCount(); i++) {
			
				//cout << "\tAdding " << *effArray[i] << endl;
				
				bool notFoundYet = !toReturn->currStateTrie.contains(effArray[i]);
				
					
				
				
				if (notFoundYet) {
					if (staticInf->verbose) cout << "\tAdding " << *effArray[i] << endl;
					toReturn->setToAdd(effArray[i]);
					
				
				} else {
					if (staticInf->verbose) cout << "\tPreserving " << *effArray[i] << endl;
					toReturn->setInvariant(effArray[i]);
					
					
				}		
				
			}
			
			//if (doneAnything) {
			
			PredicateInstance** currPreconditions;
			
			currPreconditions = curr->internalPreconditions();
			
			for (int i = 0; i < curr->getPreconditionCount(); i++) {
			
				if (!curr->getPreconditionType(i)->isStatic()) {
				
					
					if (currPreconditions[i] != 0) {
						bool found = toReturn->currStateTrie.contains(currPreconditions[i]);
						
						
						
						if (found) {
							
							PredicateInstance* toMarkAsSafe = toReturn->currStateTrie.getPredicateInstance(currPreconditions[i]);	
							if (!toReturn->isToDelete(toMarkAsSafe)) toReturn->setInvariant(toMarkAsSafe);
							
						}
					}
									
					
					
					
				}
							
			}
			
			currPreconditions = curr->internalNegativePreconditions();
			
			for (int i = 0; i < curr->getNegativePreconditionCount(); i++) {
			
				if (!curr->getNegativePreconditionType(i)->isStatic()) {
				
					if (currPreconditions[i] != 0) {
						bool found = toReturn->currStateTrie.contains(currPreconditions[i]);	
						
						if (!found) {
							PredicateInstance* toMarkAsSafe = toReturn->currStateTrie.getPredicateInstance(currPreconditions[i]);
							if (!toReturn->isToAdd(toMarkAsSafe)) toReturn->setNegativeInvariant(currPreconditions[i]);
							
							
						}
					}			
									
					
				}
							
			}
			
			
			toReturn->achievingActions.push_back(curr);
			
		} else {
			
			//cout << "m";
			cout.flush();
			Plan* steps;
			
			if (curr->isMacroAction()) {
			
				const MacroAction* currMA = static_cast<const MacroAction*>(curr->getParentAction());
				//cout << "Applying ";
				//curr->briefWrite(cout);
				
				Plan* tSteps = currMA->getConstituentActions(curr);
				
				//cout << "Raw steps: " << endl;
				
				//cout << *tSteps << endl;
				
				steps = tSteps->expandTOs();
				
				
				
				delete tSteps;
				
				//cout << "MA Steps: " << endl;
				//cout << *steps << endl;
				
			} else {
				steps = TOPlan;
				
				cout << "T.O. Plan:" << endl;
				cout << *steps << endl;
				
			}
			
			Plan::iterator pItr = steps->begin();
			Plan::iterator pEnd = steps->end();
			
			//cout << *steps << endl;
			
			for (; pItr != pEnd; ++pItr) {
			
				//cout << "Replacing ";
				//(*pItr).first->briefWrite(cout);
				//cout << "Referring to master RPG at " << rpg << endl;
				ActionInstance* replacement = rpg->getMasterAI((*pItr).first);
				//cout << "with ";
				if ((*pItr).first->decrUsage()) delete (*pItr).first;
				(*pItr).first = replacement;
				replacement->incrUsage();
				//replacement->briefWrite(cout);
					
			}
			
			if (staticInf->verbose) cout << "Standardised plan" << endl;
			
			if (staticInf->verbose) cout << *steps << endl;
			
						
			pItr = steps->begin();
			pEnd = steps->end();
			
			double frontTime = (*pItr).second;
			
			ActionInstanceList firstStep;
			
			rpg->applicableRelaxedActions(*staticInf, toReturn, dummyEntSet);
			
			for (;pItr != pEnd && (*pItr).second <= frontTime; ++pItr) {
				rpg->getApplicableSubActions((*pItr).first, firstStep);
			}
			
			//cout << "Actions to apply now:" << endl;
		
			//ActionInstanceList::iterator fsI = firstStep.begin();
			//ActionInstanceList::iterator fsE = firstStep.end();
			
			//for (;fsI != fsE; ++fsI) (*fsI)->briefWrite(cout);
			
			toReturn->applyActionsLocally(firstStep, rpg, staticInf);
			
			/*PredicateInstanceSet predicatesToAdd;
			PredicateInstanceSet predicatesToDelete;
			PredicateInstanceSet predicatesInvariant;
			PredicateInstanceSet predicatesNegativeInvariant;*/
		
			ActionInstanceList layerQueue;
			State* appTestState = toReturn->copy();
			slist<State*> statesToDelete;
			statesToDelete.push_front(appTestState);
			
			double prevTime = 0;
			
			list<EventQueueItem>::iterator currEqi = toReturn->eventQueue.begin();
			
			bool finished = false;
			
			for (;!finished; ++pItr) {
				ActionInstance* currAI;
				double actTime;
				
				if (pItr != pEnd) {
					currAI = (*pItr).first;
					actTime = (*pItr).second;
				} else {
					finished = true;
				}
				
				
				list<EventQueueItem>::iterator eqItr;
				list<EventQueueItem>::iterator eqEnd;
				
				if (finished || actTime > prevTime) {
				
					
					
					
					
					
				
					if (!layerQueue.empty()) {
						State* newState = statesToDelete.front()->copy();
						newState->advanceTime();
						statesToDelete.push_front(newState);
						
						rpg->applicableRelaxedActions(*staticInf, newState, dummyEntSet);
						
						ActionInstanceList allToApply;
						
						ActionInstanceList::iterator ataItr = layerQueue.begin();
						ActionInstanceList::iterator ataEnd = layerQueue.end();
						
						for (;ataItr != ataEnd; ++ataItr) {
							rpg->getApplicableSubActions(*ataItr, allToApply);
						}
						
						ataItr = allToApply.begin();
						ataEnd = allToApply.end();
						
						for (; ataItr != ataEnd; ataItr++) {
							PredicateInstance** effs = (*ataItr)->internalAddEffects();
							int effLimit = (*ataItr)->getAddEffectCount();
							
							for (int ea = 0; ea < effLimit; ++ea) {
								(*currEqi).addEffects.insert(effs[ea]);
							}
							
							effs = (*ataItr)->internalDeleteEffects();
							effLimit = (*ataItr)->getDeleteEffectCount();
							
							for (int ea = 0; ea < effLimit; ++ea) {
								(*currEqi).deleteEffects.insert(effs[ea]);
							}
							
							effs = (*ataItr)->internalPreconditions();
							effLimit = (*ataItr)->getPreconditionCount();
							
							for (int ea = 0; ea < effLimit; ++ea) {
								if (effs[ea] != 0 && !effs[ea]->getParentPredicate()->isStatic()) {
								 
									double addFromTime = 1.0;
									
									list<EventQueueItem>::iterator addFrom = toReturn->eventQueue.begin();
									bool addToStart = (toReturn->containsPredicateInstance(effs[ea]) && !toReturn->isToDelete(effs[ea]));
									list<EventQueueItem>::iterator eqSearch = toReturn->eventQueue.begin();
										
									bool keepSafe = addToStart;
									double countTime = 1.0;
																	
									for (;eqSearch != currEqi; ++eqSearch, ++countTime) {
									
										if ((*eqSearch).deleteEffects.find(effs[ea]) != (*eqSearch).deleteEffects.end()) {
											keepSafe = false;
											addToStart = false;	
										}
										
										if ((*eqSearch).addEffects.find(effs[ea]) != (*eqSearch).addEffects.end()) {
											keepSafe = true;
											addToStart = false;
											addFrom = eqSearch;	
											addFromTime = countTime;
										}
									
									}
									
									if (addToStart) {
										//cout << *effs[ea] << " needed as an invariant at the start" << endl;
										toReturn->setInvariant(effs[ea]);
									}
									if (keepSafe) {
										if (!addToStart) ++addFrom;
										for (; addFrom != currEqi; ++addFrom) {
											//cout << *effs[ea] << " needed as an invariant from time " << addFromTime << endl;
											(*addFrom).invariants.insert(effs[ea]);
										}
									}
									
									(*currEqi).invariants.insert(effs[ea]);
								}
							}
							
							
							
							effs = (*ataItr)->internalNegativePreconditions();
							effLimit = (*ataItr)->getNegativePreconditionCount();
							
							for (int ea = 0; ea < effLimit; ++ea) {
								if (effs[ea] != 0 && !effs[ea]->getParentPredicate()->isStatic()) {
								 
									list<EventQueueItem>::iterator addFrom = toReturn->eventQueue.begin();
									bool addToStart = (!toReturn->containsPredicateInstance(effs[ea]) && !toReturn->isToAdd(effs[ea]));
									
									list<EventQueueItem>::iterator eqSearch = toReturn->eventQueue.begin();
										
									bool keepSafe = addToStart;
																	
									for (;eqSearch != currEqi; ++eqSearch) {
									
										if ((*eqSearch).deleteEffects.find(effs[ea]) != (*eqSearch).deleteEffects.end()) {
											keepSafe = true;
											addToStart = false;	
											addFrom = eqSearch;	
										}
										
										if ((*eqSearch).addEffects.find(effs[ea]) != (*eqSearch).addEffects.end()) {
											keepSafe = false;
											addToStart = false;
											
										}
									
									}
									
									if (addToStart) {
										//cout << *effs[ea] << " needed as a negative invariant at the start" << endl;
										toReturn->setNegativeInvariant(effs[ea]);
									}
									if (keepSafe) {
										if (!addToStart) ++addFrom;
										for (; addFrom != currEqi; ++addFrom) {
											//cout << *effs[ea] << " needed as a negative invariant" << endl;
											(*addFrom).negativeInvariants.insert(effs[ea]);
										}
									}
									
									(*currEqi).negativeInvariants.insert(effs[ea]);
								}
							
								
							}
						}
												
						layerQueue.clear();	
					}
					
					prevTime = actTime;
					
					if (!finished) {
					
						currEqi++;
						
						if (currEqi == toReturn->eventQueue.end()) {
							toReturn->eventQueue.push_back(EventQueueItem());
							currEqi = toReturn->eventQueue.end();
							currEqi--;
						}
					}
					
				}
				
				if (!finished) {
					layerQueue.push_back(currAI);
				}
				
				
			}
			
			
			
			while (!statesToDelete.empty()) {
				delete statesToDelete.front();
				statesToDelete.pop_front();
			}
			
			delete steps;
			
			/*dumpEqiItr = toReturn->eventQueue.begin();
			dumpEqiEnd = toReturn->eventQueue.end();
			
			cout << "Invariants After:" << endl;
			cout << "(queue of size " << toReturn->eventQueue.size() << ")" << endl;
			if (true == true) {
				cout << "--" << endl;
				PredicateInstanceSet::const_iterator dpItr;
				PredicateInstanceSet::const_iterator dpEnd;
				
				dpItr = toReturn->predicatesInvariant.begin();
				dpEnd = toReturn->predicatesInvariant.end();
				for (; dpItr != dpEnd; ++dpItr) {
					cout << *(*dpItr) << endl;
				}
			}
			
			for (; dumpEqiItr != dumpEqiEnd; ++dumpEqiItr) {
				cout << "--" << endl;
				PredicateInstanceSet::const_iterator dpItr;
				PredicateInstanceSet::const_iterator dpEnd;
				
				dpItr = (*dumpEqiItr).invariants.begin();
				dpEnd = (*dumpEqiItr).invariants.end();
				
				for (; dpItr != dpEnd; ++dpItr) {
					cout << *(*dpItr) << endl;
				}
			}
			cout << "-----------" << endl;*/
			
			/*
			cout << "ToReturn now has:" << endl;
			
			
			cout << "--- Time n + 0 ---" << endl;
			
			cout << "\tTo add:" << endl;
			
			PredicateInstanceSet::const_iterator dumpItr = toReturn->predicatesToAdd.begin();
			PredicateInstanceSet::const_iterator dumpEnd = toReturn->predicatesToAdd.end();
			
			for (; dumpItr != dumpEnd; dumpItr++) {
				cout << "\t\t" << *(*dumpItr) << endl;
			}
			
			dumpItr = toReturn->predicatesToDelete.begin();
			dumpEnd = toReturn->predicatesToDelete.end();
			
			cout << "\tTo delete:" << endl;
			
			for (; dumpItr != dumpEnd; dumpItr++) {
				cout << "\t\t" << *(*dumpItr) << endl;
			}
			
			dumpItr = toReturn->predicatesInvariant.begin();
			dumpEnd = toReturn->predicatesInvariant.end();
			
			cout << "\tInvariant:" << endl;
			
			for (; dumpItr != dumpEnd; dumpItr++) {
				cout << "\t\t" << *(*dumpItr) << endl;
			}
			
			dumpItr = toReturn->predicatesNegativeInvariant.begin();
			dumpEnd = toReturn->predicatesNegativeInvariant.end();
			
			cout << "\tNegative invariant:" << endl;
			
			for (; dumpItr != dumpEnd; dumpItr++) {
				cout << "\t\t" << *(*dumpItr) << endl;
			}
			
			
			
			list<EventQueueItem>::const_iterator eqItr = toReturn->eventQueue.begin();
			list<EventQueueItem>::const_iterator eqEnd = toReturn->eventQueue.end();
			
			for (int i = 1; eqItr != eqEnd; ++i, ++eqItr) {
			
				cout << "--- Time n + " << i << " ---" << endl;
			
				cout << "\tTo add:" << endl;
				
				dumpItr = (*eqItr).addEffects.begin();
				dumpEnd = (*eqItr).addEffects.end();
				
				for (; dumpItr != dumpEnd; dumpItr++) {
					cout << "\t\t" << *(*dumpItr) << endl;
				}
				
				dumpItr = (*eqItr).deleteEffects.begin();
				dumpEnd = (*eqItr).deleteEffects.end();
				
				cout << "\tTo delete:" << endl;
				
				for (; dumpItr != dumpEnd; dumpItr++) {
					cout << "\t\t" << *(*dumpItr) << endl;
				}
				
				dumpItr = (*eqItr).invariants.begin();
				dumpEnd = (*eqItr).invariants.end();
				
				cout << "\tInvariant:" << endl;
				
				for (; dumpItr != dumpEnd; dumpItr++) {
					cout << "\t\t" << *(*dumpItr) << endl;
				}
				
				dumpItr = (*eqItr).negativeInvariants.begin();
				dumpEnd = (*eqItr).negativeInvariants.end();
				
				cout << "\tNegative invariant:" << endl;
				
				for (; dumpItr != dumpEnd; dumpItr++) {
					cout << "\t\t" << *(*dumpItr) << endl;
				}
			
			
			}
			cout << "Carry on!" << endl;
			exit(0);
			*/
			//Plan* tPlan = currMA->getConstituentActions(curr);
			//cout << *tPlan << endl;
			//delete tPlan;
			
			// rewrite this bit
			/*list<EventQueueItem> mergeEQ = currMA->getMutexInformation(curr);
			
			bool firstTime = true;
			list<EventQueueItem>::iterator reqItr = toReturn->eventQueue.begin();
			
			for (	list<EventQueueItem>::iterator eqItr = mergeEQ.begin();
				eqItr != mergeEQ.end();
				eqItr++) {
			
				if (firstTime) {
				
					EventQueueItem * eqi = &(*eqItr);
					PredicateInstanceSet::iterator psItr;
					PredicateInstanceSet::iterator psEnd;
					if (!relaxedActions) {
					
						psItr = eqi->deleteEffects.begin();
						psEnd = eqi->deleteEffects.end();
						for (; psItr != psEnd; psItr++) {
						
							PredicateInstance* toRemove = toReturn->currStateTrie.getPredicateInstance(*psItr);
							toReturn->setToDelete(toRemove);
							//cout << "Marking " << *toRemove << " as unsafe" << endl;
						}
					
					}
					
					psItr = eqi->addEffects.begin();
					psEnd = eqi->addEffects.end();
					
					for (; psItr != psEnd; psItr++) {
					
						PredicateInstance* toAdd = toReturn->currStateTrie.getPredicateInstance(*psItr);
						if (toAdd == 0) { // actually new
							toReturn->setToAdd(*psItr);
						} else {
							toReturn->setInvariant(toAdd);
						}
					}
				
					psItr = eqi->invariants.begin();
					psEnd = eqi->invariants.end();
					for (; psItr != psEnd; psItr++) {
					
						PredicateInstance* toMarkAsSafe = toReturn->currStateTrie.getPredicateInstance(*psItr);
						
						toReturn->setInvariant(toMarkAsSafe);
						//if (!toReturn->isUnsafe(toMarkAsSafe)) toReturn->setSafe(toMarkAsSafe);
					}
					
					psItr = eqi->negativeInvariants.begin();
					psEnd = eqi->negativeInvariants.end();
					for (; psItr != psEnd; psItr++) {
					
						
						toReturn->setNegativeInvariant(*psItr);
						//if (!toReturn->isUnsafe(toMarkAsSafe)) toReturn->setSafe(toMarkAsSafe);
					}
					
					firstTime = false;
				} else {
					//cout << "Queueing effects" << endl;
					if (reqItr == toReturn->eventQueue.end()) {
						//cout << "No other queued effects; queueing just these" << endl;
						toReturn->eventQueue.push_back(*eqItr);
						if (toReturn->eventQueue.empty()) {
							//cout << "Event queue is empty!!!" << endl;
						}
						reqItr = toReturn->eventQueue.end();
					} else {
					
						EventQueueItem * eqi = &(*eqItr);
						EventQueueItem * meqi = &(*reqItr);
						
						PredicateInstanceSet::iterator psItr;
						if (!relaxedActions) {
						
							psItr = eqi->deleteEffects.begin();
							for (; psItr != eqi->deleteEffects.end(); psItr++) {
								bool found = false;
								for (	PredicateInstanceSet::iterator caiItr = meqi->deleteEffects.begin();
									!found && caiItr != meqi->deleteEffects.end();
									caiItr++ ) {
									
									found = (*(caiItr) == *psItr);
									
								}
								if (!found) meqi->deleteEffects.insert(*psItr);
								
							}
						
						}
						
						psItr = eqi->addEffects.begin();
						for (; psItr != eqi->addEffects.end(); psItr++) {
						
							bool found = false;
							for (	PredicateInstanceSet::iterator caiItr = meqi->addEffects.begin();
								!found && caiItr != meqi->addEffects.end();
								caiItr++ ) {
								
								found = (*(caiItr) == *psItr);
								
							}
							if (!found) meqi->addEffects.insert(*psItr);
							
							
						}
					
						psItr = eqi->invariants.begin();
						for (; psItr != eqi->invariants.end(); psItr++) {
						
							bool found = false;
							for (	PredicateInstanceSet::iterator caiItr = meqi->invariants.begin();
								!found && caiItr != meqi->invariants.end();
								caiItr++ ) {
								
								found = (*(caiItr) == *psItr);
								
							}
							if (!found) meqi->invariants.insert(*psItr);
						}
						
						reqItr++;
					
					}
					
				}
					
			}*/
		
		}
		
	}
	
	
	if (staticInf->verbose) {
	
		cout << "Actions applied:" << endl;
		ActionInstanceList::const_iterator  aiItr = toReturn->achievingActions.begin();
	
		for (; aiItr != toReturn->achievingActions.end(); aiItr++) {
			cout << "\t";
			(*aiItr)->briefWrite(cout);
			cout << endl;
		
		}
		
	}
	//toReturn->checkEventQueueConsistency();
	
	return toReturn;

}

bool State::checkEventQueueConsistency() const {
	
#ifndef STRIPS_PARANOIA
	return true;
#endif
	
	list<EventQueueItem>::const_iterator reqItr = eventQueue.begin();
	unsigned int eqTS = 0;
	
	for (; reqItr != eventQueue.end(); ++reqItr, ++eqTS) {
	
		const EventQueueItem * eqi = &(*reqItr);
		
		PredicateInstanceSet::const_iterator itr = eqi->invariants.begin();
		PredicateInstanceSet::const_iterator itrEnd = eqi->invariants.end();
		
				
		for (; itr != itrEnd; ++itr) {
			
			PredicateInstance* currPI = getPredicateInstance(*itr);
			bool consistent;
			string synopsis;
			if (currPI == 0 && !isToAdd(*itr)) {
				consistent = false;
				synopsis = "Not true to start with;";
			} else { 
				consistent = !isToDelete(currPI);
				if (consistent) {
					synopsis = "True to start with;";
				} else {
					synopsis = "Deleted to start With;";
				}
			}
			
			
			list<EventQueueItem>::const_iterator dItr = eventQueue.begin();
			bool anyBetween = false;
			for (; dItr != reqItr; ++dItr) {
				anyBetween = true;
				bool isdel = ((*dItr).deleteEffects.find(*itr) != (*dItr).deleteEffects.end());
		
							
				bool isadded = ((*dItr).addEffects.find(*itr) != (*dItr).addEffects.end());
				
								
				if (isdel) {
					if (isadded) {
						synopsis += " deleted but added again straight away;";	
					} else {
						synopsis += " deleted;";	
					}
				} else {
					if (isadded) {
						synopsis += " added;";	
					} else {
						synopsis += " left alone;";
					}
				}
				consistent = isadded || (consistent && !isdel);
			
				
			}
			if (!anyBetween) synopsis += " no steps after that;";
			if (consistent ) {
				/*cout << "Event queue is consistent" << endl;
				cout << "The predicate " << *(*itr) << " is an invariant that will be there in time" << endl;
				cout << synopsis << endl;*/
			} else {
				cout << "Oh boy, the event queue is inconsistent" << endl;
				cout << "Time = " << eqTS << endl;
				cout << "The predicate " << *(*itr) << " is an invariant but won't be there in time" << endl;
				cout << synopsis << endl;
				
				exit(0);
				return false;
			}
			
		}
		
		
	
	}
	
	cout << "k";
	cout.flush();
	return true;
	

}

void State::applyRelaxedActionsLocally(	ActionInstanceList & toApply,
					StaticInformation * staticInf,
					bool popNonApplied,
					bool deleteNonApplied) {

	achievingActions.clear();
	
	ActionInstanceList::iterator itr = toApply.begin();
	ActionInstanceList::iterator prevItr = toApply.begin();
		
	while (itr != toApply.end()) {
	
		ActionInstance* curr = (*itr);
	
		if (staticInf->verbose) {
			cout << "Applying action ";
			curr->briefWrite(cout);
		}
			
		bool doneAnything = false;
		
		PredicateInstance** aeffects = curr->internalAddEffects();
		
		for (int i = 0; i < curr->getAddEffectCount(); i++) {
		
			
			
			
			
			bool notFoundYet = !currStateTrie.contains(aeffects[i]);
				
			
			
				
			
			if (notFoundYet) {
			
				
				if (staticInf->verbose) {
					cout << "Creating new predicate " << *aeffects[i] << endl;
				}
				
				doneAnything = true;
				if (!isToAdd(aeffects[i])) {
					setToAdd(aeffects[i]);
				}
				
				
			
			}
		}
		
		if (doneAnything) {
		
			achievingActions.push_back(curr);
			prevItr = itr;
			itr++;
			
		} else {
			if (popNonApplied) {
				
				
				if (itr == toApply.begin()) {
					toApply.pop_front();
					itr = toApply.begin();
					prevItr = toApply.begin();
				} else {
					toApply.erase(itr);
					itr = prevItr;
					itr++;
				}
			} else {
			
			prevItr = itr;
			itr++;
			
			}
		}
		
		
	}
	
	clearPredicateFlags();
	
	if (staticInf->verbose) {
	
		cout << "Actions applied:" << endl;
		ActionInstanceList::const_iterator  aiItr = achievingActions.begin();
	
		for (; aiItr != achievingActions.end(); aiItr++) {
			cout << "\t";
			(*aiItr)->briefWrite(cout);
			cout << endl;
		
		}
		
	}
	

}


ActionInstanceList State::applicableActions(StaticInformation * staticInf) {

	hash_map<Type*, EntitySet* > * exemplarMap = 0;
	return internalApplicableActions(staticInf, exemplarMap, false, true);
}

ActionInstanceList State::applicableActions(StaticInformation * staticInf, list<SymmetryGroup*> symGroups) {

	hash_map<Type*, EntitySet* > * exemplarMap = new hash_map<Type*, EntitySet* >(staticInf->numberOfTypes);
	
	slist<EntitySet* > toDelete;
	list<SymmetryGroup*>::iterator itr = symGroups.begin();
	
	for (; itr != symGroups.end(); itr++) {

		SymmetryGroup * currGroup = *itr;
		
		Entity* exemplar = currGroup->getExemplar();
		list<Type*> allTypes;
		
		
		
		
		
		Type * p = exemplar->getEntityType();
		while ( p != 0) {
			allTypes.push_back(p);
			p = p->getParent();				
		}
		
		
		
		for (list<Type*>::iterator tItr = allTypes.begin(); tItr != allTypes.end(); tItr++) {
		
			hash_map<Type*, EntitySet* >::iterator hmItr = exemplarMap->find(*tItr);
			if (hmItr == exemplarMap->end()) {
				
				EntitySet* newList = new EntitySet();
				toDelete.push_front(newList);
				newList->insert(exemplar);
				(*exemplarMap)[*tItr] = newList;
			} else {
				(*hmItr).second->insert(exemplar);
			}
		
		}
		
		
	}
	
	
	ActionInstanceList toReturn = internalApplicableActions(staticInf, exemplarMap, false, true);
	
	delete exemplarMap;
	while (!toDelete.empty()) {
		delete toDelete.front();
		toDelete.pop_front();
	}
	
	return toReturn;
}



ActionInstanceList State::applicableRelaxedActions(StaticInformation * staticInf) {
	hash_map<Type*, EntitySet* > * exemplarMap = 0;
	return internalApplicableActions(staticInf, exemplarMap,true,false);
}

ActionInstanceList State::applicableRelaxedUnitActions(StaticInformation * staticInf) {
	hash_map<Type*, EntitySet* > * exemplarMap = 0;
	return internalApplicableActions(staticInf, exemplarMap,true,false);
}


struct AssignmentQueueComparator {

	bool operator()(const pair<int, int> & a, const pair<int, int> & b) const {
		return (a.second < b.second);
	}
	

};

class ActionSearchData {

private:

	//typedef priority_queue<pair<int, int>, vector<pair<int, int> >, AssignmentQueueComparator > AssignmentQueue;
	
	pair<EntitySet*, int>** domains;
	Entity** argAssignments;
	bool* assignedTo;
	bool relaxedGoal;
	bool normalGoal;
	//AssignmentQueue assignmentOrder;
	slist<pair<int,int> > stillToAssign;
	const Action* parentAction;
	
	int assignToNext;
	int assignToNextDomSize;	
	
public:

	ActionSearchData(const Action* & a, hash_map<Type*, EntitySet*>* & exemplarMap, const bool & needToCheckStatic, State * dstate, State * sstate)
	: domains(new pair<EntitySet*, int>*[a->parameterCount]),
	  argAssignments(new Entity*[a->parameterCount]),
	  assignedTo(new bool[a->parameterCount]),
	  relaxedGoal(false),
	  normalGoal(false),
	  parentAction(a),
	  assignToNext(-1),
	  assignToNextDomSize(INT_MAX)
	
	{
		
		
		const int argCount = a->parameterCount;
		int i;
		
		
		if (exemplarMap != 0) {
			for (i = 0; i < argCount; i++) {
				argAssignments[i] = 0;
				assignedTo[i] = false;
				domains[i] = new pair<EntitySet*, int>(Action::intersection((*exemplarMap)[a->parameterTypes[i]], a->getParameterDomain(i)), 0);
				incrUsage(domains[i]);

			}
		} else {
			for (i = 0; i < argCount; i++) {
				argAssignments[i] = 0;
				assignedTo[i] = false;
				domains[i] = new pair<EntitySet*, int>(new EntitySet(*(a->getParameterDomain(i))), 0);
				incrUsage(domains[i]);
			
			}
		}
		/*
		cout << "Initial domains:" << endl;
		
		for (i = 0; i < argCount; i++) {
			cout << i << ":";
			EntitySet* currES = domains[i]->first;
			for (EntitySet::iterator printES = currES->begin(); printES != currES->end(); printES++) {
				cout << " " << (*printES)->name();
			}
			cout << "\n";
		
		}
		
		*/
		
		bool doSearch = true;
		
		for (i = 0; doSearch && i < a->preconditionCount; i++) {
		
			if (a->isConstOnly(i)) {
				
				const Predicate* currPred = a->preconditions[i].predType;
				
				Entity** tmpPIE = new Entity*[currPred->getArgumentCount()];
				
				for (int j = 0; j < currPred->getArgumentCount(); j++) {
				
					tmpPIE[j] = a->constants[-1 - a->preconditions[i].parameters[j]];
				
				}
				
				PredicateInstance* tmpPI = new PredicateInstance(currPred, tmpPIE);
				
				delete tmpPIE;
				
				if (a->preconditions[i].predType->isStatic()) {
					PredicateInstance* cref = sstate->getPredicateInstance(tmpPI);
					if (cref == 0) {
						doSearch = false;
					} else {
						doSearch = !sstate->isToDelete(cref);
					}
				} else {
					PredicateInstance* cref = dstate->getPredicateInstance(tmpPI);
					if (cref == 0) {
						doSearch = false;
					} else {
						doSearch = !dstate->isToDelete(cref);
					}
				}
				
				delete tmpPI;
			}		
		
		}
		
		if (!doSearch) {
			assignToNext = -1;
		}
		
		for (i = 0; doSearch && i < argCount; i++) {
			int currDomSize = domains[i]->first->size();
			if (currDomSize == 0) { // empty domain - really easy as one doesn't have to search at all!
				doSearch = false;
				assignToNext = -1;
			} else { 
				pair<int, int> data(i, currDomSize);
				
				if (currDomSize < assignToNextDomSize) {
					assignToNext = i;
					assignToNextDomSize = currDomSize;
				}
				
				stillToAssign.push_front(data);
				
			}
		}
		
	}
	ActionSearchData(const ActionSearchData & oldASD, const int & argAss, Entity* & entAss)
	    :	domains(new pair<EntitySet*, int>*[oldASD.parentAction->parameterCount]),
		argAssignments(new Entity*[oldASD.parentAction->parameterCount]),
		assignedTo(new bool[oldASD.parentAction->parameterCount]),
		relaxedGoal(false),
		normalGoal(false),
		parentAction(oldASD.parentAction),
		assignToNext(-1),
		assignToNextDomSize(INT_MAX) {
		
		
		const int argCount = parentAction->parameterCount;
		int i;
		
		for (i = 0; i < argCount; i++) {
			if (i != argAss) {
				argAssignments[i] = oldASD.argAssignments[i];
				assignedTo[i] = oldASD.assignedTo[i];
				if (assignedTo[i]) {
					//cout << "Copying parent's domain of parameter " << i << endl; 
					domains[i] = oldASD.domains[i];
					incrUsage(domains[i]);
					
				} else {
					//cout << "Creating null domain for parameter " << i << endl;
					domains[i] = 0;
				}
			} else {
				argAssignments[i] = entAss;
				assignedTo[i] = true;
				EntitySet* newUnitES = new EntitySet();
				newUnitES->insert(entAss);
				domains[i] = new pair<EntitySet*, int>(newUnitES, 0);
				incrUsage(domains[argAss]);
				//cout << "Creating custom domain for parameter " << i << endl;
			}
			
		}
		
		
		EntitySet** currJPDC = 0;
		parentAction->getDomainConstraints(argAss, entAss, currJPDC);
		
		bool doSearch = true;
		
		for (	slist<pair<int, int> >::const_iterator itr = oldASD.stillToAssign.begin();
			doSearch && itr != oldASD.stillToAssign.end();
			itr++) {
			
			i = (*itr).first;
			if (i != argAss) {
				EntitySet* currES = currJPDC[i];
				if (currES != 0) {
					/*cout << "JPDCs from parameter " << argAss << " being set to " << entAss->name() << " restricts parameter " << i << " to one of" << endl;
					for(EntitySet::iterator newESitr = currES->begin(); newESitr != currES->end(); newESitr++)  {
						cout << (*newESitr)->name() << " ";
					}*/
					/*cout << "Previously our domain was:" << endl;
					for(EntitySet::iterator newESitr = oldASD.domains[i]->first->begin(); newESitr != oldASD.domains[i]->first->end(); newESitr++)  {
						cout << (*newESitr)->name() << " ";
					}*/
					EntitySet* newES = Action::intersection(oldASD.domains[i]->first, currES);
					domains[i] = new pair<EntitySet*, int>(newES, 0);
					
					incrUsage(domains[i]);
					/*cout << "In our case it restricts parameter " << i << " to one of" << endl;
					for(EntitySet::iterator newESitr = newES->begin(); newESitr != newES->end(); newESitr++)  {
						cout << (*newESitr)->name() << " ";
					}
					cout << endl;
					*/
					int domSize = newES->size();
					if (domSize == 0) {
						doSearch = false;
						assignToNext = -1;
					} else {
						pair<int,int> data(i,domSize);
						if (domSize < assignToNextDomSize) {
							assignToNext = i;
							assignToNextDomSize = domSize;
						}
						
						
						stillToAssign.push_front(data);		
					}
				} else {
					domains[i] = oldASD.domains[i];
					incrUsage(domains[i]);
					
					if ((*itr).second < assignToNextDomSize) {
						assignToNext = (*itr).first;
						assignToNextDomSize = (*itr).second;
					}
					
					stillToAssign.push_front(*itr);
				}
			}
			
		}
		
		relaxedGoal = (doSearch && stillToAssign.empty());
		
		/*if (relaxedGoal) {
			cout << "I think that " << parentAction->name();
			for (i = 0; i < argCount; i++) {
				cout << " " << argAssignments[i]->name();	
				
			}
			cout << " is a relaxedly achievable action" << endl;
		}*/
		
	}
	
	~ActionSearchData() {
		const int argCount = parentAction->parameterCount;
		int i;
		for (i = 0; i < argCount; i++) {
			maybeDelete(domains[i]);
			
		}
		
		delete [] argAssignments;
		delete [] assignedTo;
		delete [] domains;
		
	}
	
		
	void dynamicRestriction(StaticInformation * staticInf, State * currState) {
		if (assignToNext == -1) {
		
			return;
		}
		
		
		bool doSearch = true;
		bool domSizeChanged = false;
		
		int preconditionCount = parentAction->preconditionCount;
		
		for (	slist<pair<int, int> >::iterator itr = stillToAssign.begin();
			doSearch && itr != stillToAssign.end();
			itr++) {
		
			
			int tmpAss = (*itr).first;
						
			for (int i = 0; doSearch && i < preconditionCount; i++) {
				
				const Predicate* currPred = parentAction->preconditions[i].predType;
				
				int argCount = currPred->getArgumentCount();
				
				State * stateToUse = currState;
				
				if (currPred->isStatic()) {
					if (argCount > 2 || staticInf->needToCheckStatic) {
						stateToUse = staticInf->StaticInitialState;
					} else {
						stateToUse = 0;
					}
				} 
				if (stateToUse != 0) {
					int interacts = -1;
					
					for (int j = 0; (interacts == -1) && j < argCount; j++) {
						if (parentAction->preconditions[i].parameters[j] == tmpAss) {
							interacts = j;
						}
					}
					
					if (interacts != -1) {
					
						Entity** params = new Entity*[argCount];
						for (int j = 0; j < argCount; j++) {
							int tP = parentAction->preconditions[i].parameters[j];
							if (tP >= 0) {
								params[j] = argAssignments[tP];
							} else {
								params[j] = parentAction->constants[-1-tP];
							}
						}
					
						
						EntitySet::iterator oldDomBegin = domains[tmpAss]->first->begin();
						EntitySet::iterator oldDomEnd = domains[tmpAss]->first->end();
						
						
						EntitySet* reducedSet = new EntitySet();
						
						doSearch = false;
						for (; oldDomBegin != oldDomEnd; oldDomBegin++) {
						
							params[interacts] = *oldDomBegin;
							
			
							
							if (stateToUse->currStateTrie.contains(currPred, params, currState->predicatesToDelete)) {
								//cout.flush();
								reducedSet->insert(*oldDomBegin);
								doSearch = true;
							} else {
								domSizeChanged = true;
							}
						
							
						}
						
						
						delete [] params;
						
						maybeDelete(domains[tmpAss]);
						
						domains[tmpAss] = new pair<EntitySet*, int>(reducedSet, 0);
						
						incrUsage(domains[tmpAss]);
						
						(*itr).second = reducedSet->size();
						
						
					}
					
				}
			}
			
			(*itr).second = domains[tmpAss]->first->size();
			
			
			
		}
		
		
		if (!doSearch) {
		
			assignToNext = -1;
		
		} else {
		
			if (domSizeChanged) {
				
				assignToNextDomSize = INT_MAX;
				
				
				for (	slist<pair<int, int> >::iterator itr = stillToAssign.begin();
					itr != stillToAssign.end();
					itr++) {
				
					if ((*itr).second < assignToNextDomSize) {
						assignToNext = (*itr).first;
						assignToNextDomSize = (*itr).second;
					}
				
				}
			}
			
		}
	};
	
	void addChildrenToSearchQueue(slist<ActionSearchData*> & searchQueue) {

		if (assignToNext == -1) return;
		
		int tmpAss = assignToNext;
		
		EntitySet::iterator oldDomBegin = domains[tmpAss]->first->begin();
		EntitySet::iterator oldDomEnd = domains[tmpAss]->first->end();
		
		
		for (; oldDomBegin != oldDomEnd; oldDomBegin++) {
		
	
			
			Entity* tmpVal = *oldDomBegin;
			ActionSearchData* newASD = new ActionSearchData(*this, tmpAss, tmpVal);
			searchQueue.push_front(newASD);
			
		}
	
	}
	
	void incrUsage(pair<EntitySet*, int> * p) {
	
		(p->second)++;
		
	}
	
	void maybeDelete(pair<EntitySet*, int> * p) {
	
		if (p == 0) return;
		if (--(p->second) == 0) {
			
			delete p->first;
			delete p;
		}
	
	}
	
	
	
	bool isRelaxedGoal(State * d, State * s) {
		if (!relaxedGoal) return false;
		
		ActionInstance* tInstance = getActionInstance();
		
		
		PredicateInstance** tArray = tInstance->getPreconditionsAsPredicateInstances();
		
		relaxedGoal = true;
		
		for (int i = 0; relaxedGoal && i < parentAction->preconditionCount; i++) {
			PredicateInstance * currPI = tArray[i];
			if (currPI->getParentPredicate()->isStatic()) {
				
				PredicateInstance * cRef = s->getPredicateInstance(currPI);
				
				if (cRef == 0) {
					relaxedGoal = false;
				}
				
				
			} else {
				
				PredicateInstance * cRef = d->getPredicateInstance(currPI);
				
				if (cRef == 0) {
					relaxedGoal = false;
				} else {
					relaxedGoal = !d->isToDelete(cRef);
				}
				
				
				
				
			}
		}
		
		for (int i = 0; i < parentAction->preconditionCount; i++) {
			delete tArray[i];
		}
		delete [] tArray;
		
		tArray = tInstance->getAddEffectsAsPredicateInstances();
		
		for (int i = 0; relaxedGoal && i < parentAction->addEffectCount; i++) {
			PredicateInstance * currPI = tArray[i];
				
			PredicateInstance * cRef = d->getPredicateInstance(currPI);
				
			if (cRef != 0) {
				relaxedGoal = (!d->isToDelete(cRef) || d->isToAdd(cRef));
			}
				
		}
		for (int i = 0; i < parentAction->addEffectCount; i++) {
			delete tArray[i];
		}
		delete [] tArray;
		
		delete tInstance;
		
		return relaxedGoal;
	}
	
	bool isNormalGoal(State * s) { 
		normalGoal = true;
		if (!parentAction->isMacroAction) {
			for (int i = 0; i < parentAction->deleteEffectCount; i++) {
			
				const Predicate* currPred = parentAction->deleteEffects[i].predType;
				Entity** params = new Entity*[currPred->getArgumentCount()];
				
				for (int j = 0; j < currPred->getArgumentCount(); j++) {
					int tP = parentAction->deleteEffects[i].parameters[j];
					if (tP >= 0) {
						params[j] = argAssignments[tP];
					} else {
						params[j] = parentAction->constants[-1-tP];
					}
				}
				
				PredicateInstance* toCheck = s->getPredicateInstance(currPred, params);
				
				/*if (toCheck == 0) {
					cout << "Cannot find delete effect, i.e. precondition, in domain" << endl;
					cout << "This shouldn't happen"  << endl;
					cout << *toCheck << endl;
					exit(0);
				}*/
				
				delete [] params;
				
				if (toCheck != 0) normalGoal = !s->isInvariant(toCheck);
						
			}
			
			return normalGoal;
		} else {
			
			//cout << "Testing whether macro-action is really applicable" << endl;
			//ActionInstance* tmpInstance = getActionInstance();
			cout << "Code not yet implemented; it's deprecated" << endl;
			exit(0);
			/*list<EventQueueItem> checkQueue = ((const MacroAction*)parentAction)->getMutexInformation(tmpInstance);
			
			bool firstTime = true;
			
			list<EventQueueItem>::iterator rqItr = s->eventQueue.begin();
			
			
			for (	list<EventQueueItem>::iterator cqItr = checkQueue.begin();
				cqItr != checkQueue.end();
				cqItr++) {
				
			
				if (firstTime) {
				
					for (	PredicateInstanceSet::iterator deItr = (*cqItr).deleteEffects.begin();
						deItr != (*cqItr).deleteEffects.end();
						deItr++) {
					
						PredicateInstance* toCheck = s->getPredicateInstance(*deItr);
						
						if ((toCheck != 0 && s->isInvariant(toCheck)) || s->isToAdd(*deItr)) {
							delete tmpInstance;
							//cout << "No, it isn't" << endl;
							return false;
						}
						
							
					}
					
					for (	PredicateInstanceSet::iterator deItr = (*cqItr).addEffects.begin();
						deItr != (*cqItr).addEffects.end();
						deItr++) {
					
						PredicateInstance* toCheck = s->getPredicateInstance(*deItr);
						if ((toCheck != 0 && s->isToDelete(toCheck)) || s->isNegativeInvariant(*deItr)) {
							delete tmpInstance;
							//cout << "No, it isn't" << endl;
							return false;
						}
						
						
							
					}
					
					for (	PredicateInstanceSet::iterator deItr = (*cqItr).invariants.begin();
						deItr != (*cqItr).invariants.end();
						deItr++) {
					
						PredicateInstance* toCheck = s->getPredicateInstance(*deItr);
						if (toCheck == 0 || s->isToDelete(toCheck) || s->isNegativeInvariant(toCheck)) {
							delete tmpInstance;
							//cout << "No, it isn't" << endl;
							return false;
						}
						
						
							
					}
					
					firstTime = false;
				
				} else {
					if (rqItr == s->eventQueue.end()) {
						delete tmpInstance;
						//cout << "Yes, it is" << endl;
						return true;
					}
					
					for (	PredicateInstanceSet::iterator deItr = (*cqItr).deleteEffects.begin();
						deItr != (*cqItr).deleteEffects.end();
						deItr++) {
					
						
						for (int pass = 0; pass < 2; pass++) {
						
							PredicateInstanceSet::iterator csItr;
							PredicateInstanceSet::iterator csEnd;
							if (pass == 0) {
								csItr = (*rqItr).addEffects.begin();
								csEnd = (*rqItr).addEffects.end();
							} else {
								csItr = (*rqItr).invariants.begin();
								csEnd = (*rqItr).invariants.end();
							}
						
							for (; csItr != csEnd; csItr++) {
							
								if (*(*csItr) == *(*deItr)) {
									delete tmpInstance;
									//cout << "No, it isn't" << endl;
									return false;
								}
							}
						}
							
					}
					
					for (	PredicateInstanceSet::iterator deItr = (*rqItr).deleteEffects.begin();
						deItr != (*rqItr).deleteEffects.end();
						deItr++) {
					
						
						for (int pass = 0; pass < 2; pass++) {
						
							PredicateInstanceSet::iterator csItr;
							PredicateInstanceSet::iterator csEnd;
							if (pass == 0) {
								csItr = (*cqItr).addEffects.begin();
								csEnd = (*cqItr).addEffects.end();
							} else {
								csItr = (*cqItr).invariants.begin();
								csEnd = (*cqItr).invariants.end();
							}
						
							for (; csItr != csEnd; csItr++) {
							
								if (*(*csItr) == *(*deItr)) {
									delete tmpInstance;
									//cout << "No, it isn't" << endl;
									return false;
								}
							}
						}
							
					}
					
					rqItr++;
					
					
				}
			
			}
			
			delete tmpInstance;
			*/
			//cout << "Yes, it is" << endl;
			
			return true;
		}
	};
	
	ActionInstance* getActionInstance() const {
		ActionInstance* toReturn = new ActionInstance(parentAction, argAssignments);
		return toReturn;
	}
	
	

};

ActionInstanceList State::internalApplicableActions(StaticInformation * staticInf, hash_map<Type*, EntitySet* > * exemplarMap, const bool & relaxedActions, const bool & tryMacroActions) {

	
	ActionInstanceList toReturn;
	
	bool needToCheckStatic = staticInf->needToCheckStatic;
	
	for (int i = 0; i < staticInf->numberOfActions; i++) {
	
		const Action* a = staticInf->Actions[i];
		
		if (staticInf->verbose) {
		
			cout << "  Attempting to apply actions of type " << a->name() << endl;
			
		}
		
		if (!a->isMacroAction || tryMacroActions) {
			bool anyApp = false;
			slist<ActionSearchData*> searchQueue;
			
			searchQueue.push_front(new ActionSearchData(a, exemplarMap, needToCheckStatic, this, staticInf->StaticInitialState));
			
			while (!searchQueue.empty()) {
			
				ActionSearchData* curr = searchQueue.front();
				searchQueue.pop_front();
				
				if (curr->isRelaxedGoal(this, staticInf->StaticInitialState)) {
				
					if (relaxedActions || curr->isNormalGoal(this)) {
						anyApp = true;
						ActionInstance* newAI = curr->getActionInstance();
						
						bool found = false;
						ActionInstanceList::iterator itr = toReturn.begin();
						for (; !found && itr != toReturn.end(); itr++) {
							found = (*(*itr) == *newAI);
						}
						if (!found) {
							
							toReturn.push_back(newAI);
						} else {
							delete newAI;
						}
					}
				} else {
					curr->dynamicRestriction(staticInf, this);
					curr->addChildrenToSearchQueue(searchQueue);
				}
			
				delete curr;
			}
			
			if (a->isMacroAction) {
				if (anyApp) {
					//cout << "Macro-action " << *a << " applicable" << endl;
				} else {
					//cout << "Macro-action " << *a << " not applicable" << endl;
				}
			}
		}
		
	}
	
	/*cout << "Actions returned: " << endl;
	
	for (	ActionInstanceList::iterator ailItr = toReturn.begin();
		ailItr != toReturn.end();
		ailItr++) {
	
		(*ailItr)->briefWrite(cout);	
	
	}*/
	return toReturn;	

}


void State::advanceTimeRelaxedly() {

	exit(0); // do it later if it's still needed - it's now hard....
	
}

void State::advanceTime() {

	//cout << "Advancing time of state normally" << endl;

	timeStamp++;
	
	PredicateInstanceSet::iterator processItr;
	PredicateInstanceSet::iterator processEnd;
	
	processItr = predicatesToDelete.begin();
	processEnd = predicatesToDelete.end();
	
	for (;processItr != processEnd; ++processItr) {
		removePredicateInstance(*processItr);
	}
	
	processItr = predicatesToAdd.begin();
	processEnd = predicatesToAdd.end();
	
	for (;processItr != processEnd; ++processItr) {
		addPredicateInstance(*processItr);
	}
	
	predicatesToAdd.clear();
	predicatesToDelete.clear();
	
	predicatesInvariant.clear();
	predicatesNegativeInvariant.clear();
	
	
	
	if (eventQueue.empty()) return;
	
	EventQueueItem * eq = &(eventQueue.front());  // avoid copy-construction
	
	PredicateInstanceSet::iterator eqci;
	PredicateInstanceSet::iterator eqciEnd;
	
	eqci = eq->deleteEffects.begin();
	eqciEnd = eq->deleteEffects.end();
	
	for (; eqci != eqciEnd; eqci++) {
	
		PredicateInstance* toMarkAsUnsafe = currStateTrie.getPredicateInstance(*eqci);
		if (toMarkAsUnsafe == 0) {
#ifdef STRIPS_PARANOIA
			cout << "Trying to delete a non-existant predicate!!!!!!!!" << endl;
			cout << "- " << *(*eqci) << endl;
			exit(0);
#endif
		} else {
			setToDelete(toMarkAsUnsafe);
		}
			
	}
	
	eqci = eq->addEffects.begin();
	eqciEnd = eq->addEffects.end();
	
	for (; eqci != eqciEnd; eqci++) {
	
		PredicateInstance* toAdd = currStateTrie.getPredicateInstance(*eqci);
		
		if (toAdd == 0) {
			
			setToAdd(*eqci);
			setInvariant(*eqci);
			
		} else {
			setInvariant(toAdd);
			
		}
			
	}
	
	eqci = eq->invariants.begin();
	eqciEnd = eq->invariants.end();
	
	for (; eqci != eqciEnd; eqci++) {
	
		PredicateInstance* toSetSafe = currStateTrie.getPredicateInstance(*eqci);
		if (toSetSafe == 0) {
#ifdef STRIPS_PARANOIA
			cout << "Trying to mark a non-existant predicate as being invariant!" << endl;
			exit(0);
#endif
		} else {
			setInvariant(toSetSafe);	
		}
					
	}
	
	eqci = eq->negativeInvariants.begin();
	eqciEnd = eq->negativeInvariants.end();
	
	for (; eqci != eqciEnd; eqci++) {
	
		PredicateInstance* toSetSafe = currStateTrie.getPredicateInstance(*eqci);
		if (toSetSafe != 0) {
#ifdef STRIPS_PARANOIA
			// disjunctive negative invariant?
			cout << *(*eqci) << " found - is a negative invariant!!!" << endl;
			int a = 0;
			int b = 2 / a;
			cout << b << endl;
#endif
		} else {
			setNegativeInvariant(*eqci);	
		}
					
	}
	
	
	eventQueue.pop_front(); // pop thing off, don't delete eq !!!
	
}

void State::clearPredicateFlags() {

	exit(0); // decide later, if it's needed	
	
	

};

bool State::subsumes(const State* s, StaticInformation * staticInf) const {

	if (staticInf != 0 && staticInf->verbose) {
	
		cout << "Is the state:" << endl;
		cout << (*s);
		cout << "subsumed by:" << endl;
		cout << (*this);
		cout << "?" << endl;
	}
	
	/*list<PredicateInstance *>::const_iterator itr = s->begin();
	
	for (; itr != s->end(); itr++) {
	
		
		PredicateInstance* curr = (*itr);
		
		if (!currStateTrie.contains(curr)) {
			if (staticInf->verbose) {
				cout << "No, it isn't" << endl;
			}
			return false;
		}
		
		
	}*/
	bool isSubsumed = currStateTrie.contains(s->currStateTrie, predicatesToDelete);
	if (staticInf != 0 && staticInf->verbose) {
		if (isSubsumed) {
			cout << "Yes, it is" << endl;
		} else {
			cout << "No, it isn't" << endl;
		}
	}
	return isSubsumed;	

};

bool State::intersectsWith(const State* s, StaticInformation * staticInf) const {

	State::const_iterator sItr = s->begin();
	const State::const_iterator sEnd = s->end();
	
	for (;sItr != sEnd; ++sItr) {
		if (containsPredicateInstance(*sItr)) return true;
	};
	return false;
	

};

void State::write(ostream & o) const {

	o << "State at " << timeStamp << endl;
	/*o << "Achieved by:" << endl;

	ActionInstanceList::const_iterator  aiItr = achievingActions.begin();

	for (; aiItr != achievingActions.end(); aiItr++) {
		o << "\t";
		(*aiItr)->briefWrite(o);

	}*/
	o << "Predicates:" << endl;

	/*State::const_iterator itr = currState.begin();

	for (; itr != currState.end(); itr++) {
		o << "\t" << *(*itr);
		if (isToDelete(*itr)) o << " (to delete)";
		if (isInvariant(*itr)) o << " (invariant)";
		o << endl;
	}*/
	o << "Trie:" << endl;
	o << currStateTrie << endl;
	
	PredicateInstanceSet::const_iterator psItr = predicatesToAdd.begin();
	PredicateInstanceSet::const_iterator psEnd = predicatesToAdd.end();
	
	for (;psItr != psEnd; ++psItr) {
		o << "\t" << *(*psItr) << " (to add)" << endl;
	}


};

ostream & operator <<(ostream & o, const State & a) {

	a.write(o);
	return o;

}

int State::getMarginallyLessRubbishHeuristic(StaticInformation * staticInf) {
	
	int totalExpansions = 0;
	int expansions = 0;
	
	State* stillToFind = staticInf->GoalPredicates->copy();
	
	State* currState = this;
	
	bool firstExpansion = true;
	
	
	//bool oldVerbosity = staticInf->verbose;
	//staticInf->verbose = false;
	
	
	
	State::iterator gpItr;;
	State::iterator gpPrev;
	
	gpItr = stillToFind->begin();
	gpPrev = stillToFind->begin();
	while (gpItr != stillToFind->end()) {
		State* dummy = new State();
		dummy->addPredicateInstance(*gpItr);
		if (currState->subsumes(dummy, staticInf)) {
			if (gpItr == stillToFind->begin()) {
				stillToFind->removePredicateInstance(gpItr);
				gpItr = stillToFind->begin();
				gpPrev = stillToFind->begin();
			} else {
				stillToFind->removePredicateInstance(gpItr);
				gpItr = gpPrev;
				gpItr++;
			}
		} else {
			gpPrev = gpItr;
			gpItr++;
		}
		delete dummy;
	
	}
	
		
	while (!stillToFind->empty()) {
	
				
		ActionInstanceList applicable = currState->applicableRelaxedActions(staticInf);
			
		State* nextLayer = currState->applyRelaxedActions(applicable, staticInf);
		
		while (!applicable.empty()) {
			delete applicable.front();
			applicable.pop_front();
		}
		
		if (nextLayer->achievingActions.empty()) {
			cout << "No useful applicable actions (no predicates added to state)" << endl;
			delete stillToFind;
			delete nextLayer;
			if (!firstExpansion) delete currState;
			
			return INT_MAX;
		}
		
		nextLayer->advanceTime();
		
		if (!firstExpansion) {
			delete currState;
		} else {
			firstExpansion = false;
		}
		currState = nextLayer;
			
		expansions++;
		
		gpItr = stillToFind->begin();
		gpPrev = stillToFind->begin();
		while (gpItr != stillToFind->end()) {
			State* dummy = new State();
			dummy->addPredicateInstance(*gpItr);
			if (currState->subsumes(dummy, staticInf)) {
				totalExpansions += expansions;
				if (gpItr == stillToFind->begin()) {
					stillToFind->removePredicateInstance(gpItr);
					gpItr = stillToFind->begin();
					gpPrev = stillToFind->begin();
				} else {
					stillToFind->removePredicateInstance(gpItr);
					gpItr = gpPrev;
					gpItr++;
				}
			} else {
				gpPrev = gpItr;
				gpItr++;
			}
			delete dummy;
		
		}
			
			
	}
	
	
	if (!firstExpansion) delete currState;
	
	delete stillToFind;
			
	//staticInf->verbose = oldVerbosity;
		
		
	if (staticInf->verbose) {
		cout << "Returning heuristic value of " << totalExpansions << endl;
	}
	return totalExpansions;
}

int State::getHeuristic(StaticInformation * staticInf) {

	State* currState = this->copy();
	
//	bool oldVerbosity = staticInf->verbose;
	//staticInf->verbose= false;
	
	int expansions = 0;
	
	while (!currState->subsumes(staticInf->GoalPredicates, staticInf)) {
		
		ActionInstanceList applicable = currState->applicableRelaxedActions(staticInf);
		
		
		
		State* nextLayer = currState->applyRelaxedActions(applicable, staticInf);
		
		while (!applicable.empty()) {
			delete applicable.front();
			applicable.pop_front();
		}
		
		if (nextLayer->achievingActions.empty()) {
			cout << "No useful applicable actions (no predicates added to state)" << endl;
			delete nextLayer;
			delete currState;
			return INT_MAX;
			
		}
		nextLayer->advanceTime();
		delete currState;
		currState = nextLayer;
		
		expansions++;
		
		
	}
	
	delete currState;
	
	//staticInf->verbose = oldVerbosity;
	//expansions += timeStamp;
	if (staticInf->verbose) {
		cout << "Returning heuristic value of " << expansions << endl;
	}
	return expansions;

};
bool State::isToAdd(PredicateInstance* p) const {

	bool toReturn = (predicatesToAdd.find(p) != predicatesToAdd.end());
	
	return toReturn;
};

bool State::isToDelete(PredicateInstance* p) const {

	bool toReturn = (predicatesToDelete.find(p) != predicatesToDelete.end());
	
	return toReturn;
};


bool State::isInvariant(PredicateInstance* p) const {

	bool toReturn = (predicatesInvariant.find(p) != predicatesInvariant.end());
	
	return toReturn;
};

bool State::isNegativeInvariant(PredicateInstance* p) const {

	bool toReturn = (predicatesNegativeInvariant.find(p) != predicatesNegativeInvariant.end());
	
	return toReturn;
};

void State::setToAdd(PredicateInstance* p) {
	
	predicatesToAdd.insert(p);
	if (p == 0) {
		cout << "Adding zero!!!" << endl;
		int a = 3;
		int b = 0;
		int c = a/b;
		cout << c << endl;
	}
};

void State::setToDelete(PredicateInstance* p) {
	
	predicatesToDelete.insert(p);
	if (p == 0) {
		cout << "Deleting zero!!!" << endl;
		int a = 3;
		int b = 0;
		int c = a/b;
		cout << c << endl;
	}
};

void State::setInvariant(PredicateInstance* p) {
	
	predicatesInvariant.insert(p);
	if (p == 0) {
		cout << "Making zero invariant!!!" << endl;
		int a = 3;
		int b = 0;
		int c = a/b;
		cout << c << endl;
	}
};

void State::setNegativeInvariant(PredicateInstance* p) {
	
	predicatesNegativeInvariant.insert(p);
	if (p == 0) {
		cout << "Making zero negatively invariant!!!" << endl;
		int a = 3;
		int b = 0;
		int c = a/b;
		cout << c << endl;
	}
};

bool State::hasQueuedEffects() const {
	return (!predicatesToAdd.empty() || !predicatesToDelete.empty());
	
};

PredicateInstance* State::getPredicateInstance(const Predicate * p, Entity** parameters) const {
	return currStateTrie.getPredicateInstance(p, parameters);
};

PredicateInstance* State::getPredicateInstance(PredicateInstance * p) const {
	return currStateTrie.getPredicateInstance(p);
};

void State::switchInPredicateInstance(State::iterator itr, PredicateInstance* p) {

	currStateTrie.switchIn(itr,p);
		

};
auto_ptr<ActionInstanceWithSubActionsList> State::filterApplicableActions(StaticInformation * staticInf, ActionInstanceWithSubActionsList & toFilter) {

	auto_ptr<ActionInstanceWithSubActionsList> toReturn(new ActionInstanceWithSubActionsList() );
	
	ActionInstanceWithSubActionsList::iterator itr = toFilter.begin();
	ActionInstanceWithSubActionsList::iterator itrEnd = toFilter.end();
	
	ActionInstanceList::iterator sItr;
	ActionInstanceList::iterator sEnd;
	
	for (; itr != itrEnd; ++itr) {
	
		
		const ActionInstance* currAct = (*itr)->getFirst();
		if (currAct->isMacroAction()) {
			cout << "Not supported yet :-(" << endl;
			exit(0);
		} else {
			
			bool keep = true;
			
			PredicateInstance** effects;
			int limit;
			
			
			
			limit = currAct->getAddEffectCount();
			effects = currAct->internalAddEffects();
			for (int i = 0; keep && i < limit; ++i) {
				keep = (!isToAdd(effects[i]) && !isToDelete(effects[i]) && !isNegativeInvariant(effects[i]));
			}
			
			
			limit = currAct->getDeleteEffectCount();			
			effects = currAct->internalDeleteEffects();
			
			for (int i = 0; keep && i < limit; ++i) {
				keep = (!isToAdd(effects[i]) && !isInvariant(effects[i]) && !isToDelete(effects[i]));
			}
			
			
			limit = currAct->getPreconditionCount();
			effects = currAct->internalPreconditions();
			for (int i = 0; keep && i < limit; ++i) {
				keep = (!isToDelete(effects[i]) && !isToAdd(effects[i]));
			}
			
			
			effects = currAct->internalNegativePreconditions();
			limit = currAct->getNegativePreconditionCount();
			
			for (int i = 0; keep && i < limit; ++i) {
				keep = (!isToDelete(effects[i]) && !isToAdd(effects[i]));
			}
			
			
			sItr = (*itr)->getSecond().begin();
			sEnd = (*itr)->getSecond().end();
			
			for (; keep && sItr != sEnd; sItr++) {
			
				currAct = (*sItr);
				if (currAct->isMacroAction()) {
					cout << "Not supported yet :-(" << endl;
					exit(0);
				} else {
					
					limit = currAct->getAddEffectCount();
					effects = currAct->internalAddEffects();
					for (int i = 0; keep && i < limit; ++i) {
						keep = (!isToAdd(effects[i]) && !isToDelete(effects[i]) && !isNegativeInvariant(effects[i]));
					}
					
					
					effects = currAct->internalDeleteEffects();	
					limit = currAct->getDeleteEffectCount();
					
					for (int i = 0; keep && i < limit; ++i) {
						keep = (!isToAdd(effects[i]) && !isInvariant(effects[i]) && !isToDelete(effects[i]));
					}
					
					limit = currAct->getPreconditionCount();
					effects = currAct->internalPreconditions();
					for (int i = 0; keep && i < limit; ++i) {
						keep = (!isToDelete(effects[i]) && !isToAdd(effects[i]));
					}
					
					
					effects = currAct->internalNegativePreconditions();
					limit = currAct->getNegativePreconditionCount();
					
					for (int i = 0; keep && i < limit; ++i) {
						keep = (!isToDelete(effects[i]) && !isToAdd(effects[i]));
					}
					
				}
			
			}
			
			if (keep) {
				toReturn->push_back((*itr));
			}
				
		}
		
	}
	
	return toReturn;

};
