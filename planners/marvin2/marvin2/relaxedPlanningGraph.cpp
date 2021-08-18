#include "relaxedPlanningGraph.h"
#include "PredicateInstance.h"
#include "PredicateInstanceTrie.h"
#include <values.h>
#include <ext/hash_set>
#include <map>

using __gnu_cxx::hash_set;
using std::map;

#include "ActionApplicabilityTester.h"



RelaxedPlanningGraph::RelaxedPlanningGraph(StaticInformation & problemInformation, State * initialState) : goalIDs(0), supportArray(0), applicableReal(0), actionNeeds(0), achieveArray(0), achieved(0), achievedAt(0), firstAchiever(0), resetAchieved(0), achievedReal(0), predicateInstanceIsGoal(0), containersActually(0), resetContainers(0) {

	calculateRPG(problemInformation, initialState, true);
}

RelaxedPlanningGraph::RelaxedPlanningGraph(StaticInformation & problemInformation, State * initialState, const bool & stopWhenGoalsAppear)  : goalIDs(0), supportArray(0), applicableReal(0), actionNeeds(0), achieveArray(0), achieved(0), achievedAt(0), firstAchiever(0), resetAchieved(0), achievedReal(0), predicateInstanceIsGoal(0),containersActually(0), resetContainers(0) {

	calculateRPG(problemInformation, initialState, stopWhenGoalsAppear);

}

		
		
		
		
			

void RelaxedPlanningGraph::makeSupportStructure(StaticInformation * staticInf) {
	
	resetActionSize = 0;
	for (	ActionInstanceList::iterator oldActItr = actions.begin();
		oldActItr != actions.end();
		oldActItr++) {
		
		resetActionSize += (*oldActItr)->getParentAction()->getContainerCount();
		
	}
	cout << "Number of actions found = " << actions.size() << endl;	
	resetActionSize *= sizeof(int);
	
	actionCount = actions.size();
	predicateCount = allPredicates->size();
	
	resetPredicateSize = predicateCount * sizeof(bool);
	
	slist<unsigned int>* supportList = new slist<unsigned int>[predicateCount];
	slist<unsigned int>* negativeSupportList = new slist<unsigned int>[predicateCount];
	
	slist<unsigned int>* achievedByList = new slist<unsigned int>[predicateCount];
	slist<unsigned int>* negativeAchievedByList = new slist<unsigned int>[predicateCount];
	
	//keepAction = new bool[actionCount];
	
	supportArray = new unsigned int*[predicateCount];
	negativeSupportArray = new unsigned int*[predicateCount];
	
	applicableReal = new ActionInstance*[actionCount];
	
	negativeAchieveArray = new unsigned int*[actionCount];
	achieveArray = new unsigned int*[actionCount];
	actionNeeds = new unsigned int*[actionCount];
	actionNegativeNeeds = new unsigned int*[actionCount];
	
	
	achieved = new bool[predicateCount];
	achievedAt = new unsigned int[predicateCount];
	
	negativeAchieved = new bool[predicateCount];
	negativeInitially = new bool[predicateCount];
	
	negativeAchievedAt = new unsigned int[predicateCount];
	
	resetAchieved = new bool[predicateCount];
	resetNegativeAchieved = new bool[predicateCount];
	
	
	
	
	
	
	
	for (unsigned int i = 0; i < predicateCount; i++) {
		resetAchieved[i] = false;
	}

	for (unsigned int i = 0; i < predicateCount; i++) {
		resetNegativeAchieved[i] = true;
		
	}
	
	achievedReal = new PredicateInstance*[predicateCount];
	predicateInstanceIsGoal = new bool[predicateCount];
	predicateInstanceIsNegativeGoal = new bool[predicateCount];

	firstAchiever = new unsigned int[predicateCount];
	firstNegativeAchiever = new unsigned int[predicateCount];

	containers = new int*[actionCount];
	containersActually = new int[resetActionSize];
	resetContainers = new int[resetActionSize];
	
	unsigned int caIndex = 0;
	unsigned int caCurr = 0;
	
	masterAI = new MasterAIHash(actionCount);
	
	for (	ActionInstanceList::iterator oldActItr = actions.begin();
		oldActItr != actions.end();
		oldActItr++, caCurr++) {
		
		(*masterAI)[*oldActItr] = *oldActItr;
		
		(*oldActItr)->incrUsage();
		
		const Action* ca = (*oldActItr)->getParentAction();
				
		memcpy(&(containersActually[caIndex]), ca->getDefaultSatisfactionCounters(), ca->getContainerCount() * sizeof(int));
		
		containers[caCurr] = &(containersActually[caIndex]);
				
		caIndex += (*oldActItr)->getParentAction()->getContainerCount();
		
	}
	
	
	
	


	unsigned int nextID = 0;
		
	for (	State::iterator osItr = allPredicates->begin();
		osItr != allPredicates->end();
		osItr++, nextID++) {
	
		PredicateInstance* currPred = (*osItr);
		currPred->incrUsage();
		currPred->setID(nextID);
		supportArray[nextID] = new unsigned int[1];
		supportArray[nextID][0] = 0;
		
		negativeSupportArray[nextID] = new unsigned int[1];
		negativeSupportArray[nextID][0] = 0;
		
		achievedReal[nextID] = currPred;
		predicateInstanceIsGoal[nextID] = false;
		predicateInstanceIsNegativeGoal[nextID] = false;
		
			
	}
	
	goalCount = staticInf->GoalPredicates->size();
	goalIDs = new unsigned int[goalCount];
	
	State::const_iterator goalItr = staticInf->GoalPredicates->begin();
	for (int i = 0; i < goalCount; i++, goalItr++) {
	
		goalIDs[i]  = allPredicates->getPredicateInstance(*goalItr)->getID();
		predicateInstanceIsGoal[goalIDs[i]] = true;
		
	
	}
	negativeGoalCount = staticInf->NegativeGoalPredicates->size();
	negativeGoalIDs = new unsigned int[negativeGoalCount];
	
	goalItr = staticInf->NegativeGoalPredicates->begin();
	for (int i = 0; i < negativeGoalCount; i++, goalItr++) {
	
		negativeGoalIDs[i]  = allPredicates->getPredicateInstance(*goalItr)->getID();
		predicateInstanceIsNegativeGoal[negativeGoalIDs[i]] = true;
		
	
	}
	
	nextID = 0;
	
	for (	ActionInstanceList::iterator oldActItr = actions.begin();
		oldActItr != actions.end();
		oldActItr++, nextID++) {
	
		ActionInstance* act = *oldActItr;
		act->setID(nextID);
		act->incrUsage();
		
		applicableReal[nextID] = act;
			
		list<int> needQueue;
		
		PredicateInstance** currPreconditions;
		
		currPreconditions = act->internalPreconditions();
		
		for (int i = 0; i < act->getPreconditionCount(); i++) {
			
			if (act->getPreconditionType(i)->isStatic()) {
			
				
				bool achieved = staticInf->StaticInitialState->containsPredicateInstance(currPreconditions[i]);
				
				if (achieved) {
					PredicateInstance* replaceWith = allPredicates->getPredicateInstance(currPreconditions[i]);
					
					act->replaceCachedPrecondition(i, replaceWith);
					
					int toUpdate = act->getParentAction()->getPreconditionUpdates(i);
					while (toUpdate != -1) {
						int before = containers[nextID][toUpdate];
						if (--containers[nextID][toUpdate] <= 0 && before > 0) {
							toUpdate = act->getParentAction()->getContainerUpdates(toUpdate);
						} else {
							toUpdate = -1;
						}
					
					}
				} else {
					act->replaceCachedPrecondition(i, 0);
					/*cout << "No match found for static precondition: ";
					cout << act->getPreconditionType(i)->name();
					for (int j = 0; j < argCount; j++) {
						cout << " " << parameters[j]->name();	
					}
					cout << endl;*/
					
					//cout << "Parent container contains " << containers[nextID][toUpdate] << endl;
					
				}
					
				
			} else {
				
				
				
				
				PredicateInstance* achiever = allPredicates->getPredicateInstance(currPreconditions[i]);
				act->replaceCachedPrecondition(i, achiever);
				if (achiever != 0) {
					unsigned int tID = achiever->getID();
				
					needQueue.push_back(tID);
				
					supportList[tID].push_front(nextID);
					supportList[tID].push_front(i);
					
					
					supportArray[tID][0]++;
					
					
				
				}
				
				
			}
		}
		
		actionNeeds[nextID] = new unsigned int[needQueue.size() + 1];
		actionNeeds[nextID][0] = needQueue.size();
		
		list<int>::iterator anItr = needQueue.begin();
		
		for (int i = 1; anItr != needQueue.end(); i++, anItr++) {
		
			actionNeeds[nextID][i] = *anItr;
		
		}
	
		
		needQueue.clear();
		
		currPreconditions = act->internalNegativePreconditions();
		
		for (int i = 0; i < act->getNegativePreconditionCount(); i++) {
			
			
			if (act->getNegativePreconditionType(i)->isStatic()) {
				
				
				bool achieved = staticInf->StaticInitialState->containsPredicateInstance(currPreconditions[i]);
								
				if (achieved) {
					
					int toUpdate = act->getParentAction()->getNegativePreconditionUpdates(i);
					while (toUpdate != -1) {
						int before = containers[nextID][toUpdate];
						if (++containers[nextID][toUpdate] > 0 && before <= 0) {
							toUpdate = act->getParentAction()->getContainerUpdates(toUpdate);
						} else {
							toUpdate = -1;
						}
					
					}
					toUpdate = act->getParentAction()->getNegativePreconditionUpdates(i);
					//cout << "Parent container contains " << containers[nextID][toUpdate] << endl;
				} else {
					/*cout << "Negative static precondition holds: ";
					cout << act->getNegativePreconditionType(i)->name();
					for (int j = 0; j < argCount; j++) {
						cout << " " << parameters[j]->name();
					}
					cout << endl;*/
				}
				PredicateInstance* replaceWith = allPredicates->getPredicateInstance(currPreconditions[i]);
				
				act->replaceCachedNegativePrecondition(i, replaceWith);
				
				
			} else {
				
				
				
				PredicateInstance* achiever = allPredicates->getPredicateInstance(currPreconditions[i]);
				
				if (achiever != 0) {
					
					unsigned int tID = achiever->getID();
				
					needQueue.push_back(tID);
				
					negativeSupportArray[tID][0]++;
					
					negativeSupportList[tID].push_front(nextID);
					negativeSupportList[tID].push_front(i);
					
					
					
				}
				
				act->replaceCachedNegativePrecondition(i, achiever);
				
			}
		}
		
		actionNegativeNeeds[nextID] = new unsigned int[needQueue.size()+1];
		actionNegativeNeeds[nextID][0] = needQueue.size();
		
		anItr = needQueue.begin();
		
		for (int i = 1; anItr != needQueue.end(); i++, anItr++) {
		
			actionNegativeNeeds[nextID][i] = *anItr;
		
		}
		
		PredicateInstanceTrie consequences;
		
		PredicateInstance** effToProcess = act->internalAddEffects();
		
		if (true) {
		
			const int limit = act->getAddEffectCount();
					
			for (int i = 0; i < limit; ++i) {
				consequences.addPredicateInstance(effToProcess[i]);
			}
		
		}
				
		unsigned int aCount = (unsigned int)(consequences.size());
		
		achieveArray[nextID] = new unsigned int[aCount + 1];
		achieveArray[nextID][0] = aCount;
		
		
		if (true) {  
			PredicateInstanceTrie::iterator consItr = consequences.begin();
			const PredicateInstanceTrie::iterator consEnd = consequences.end();
			
			
			for (unsigned int arID = 1; consItr != consEnd; ++consItr, ++arID) {
				
				const unsigned int v = (*consItr)->getID();
				achieveArray[nextID][arID] = v;	
							
				achievedByList[v].push_front(nextID);	
				//cout << "Registering as an achiever for " << *achievedReal[toCopy.front()->getID()] << " (id=" << toCopy.front()->getID() << ") action (id=" << nextID << "): ";
				//applicableReal[nextID]->briefWrite(cout);
				
			}
		}
		consequences.clear();
		
		effToProcess = act->internalDeleteEffects();
		
		if (true) {
		
			const int limit = act->getDeleteEffectCount();
		
			for (int i = 0; i < limit; ++i) {
				consequences.addPredicateInstance(effToProcess[i]);
			}	
			
		}
		
		
		aCount = (unsigned int)(consequences.size());
		
		negativeAchieveArray[nextID] = new unsigned int[aCount + 1];
		negativeAchieveArray[nextID][0] = aCount;
		
		if (true) { 
			PredicateInstanceTrie::iterator consItr = consequences.begin();
			const PredicateInstanceTrie::iterator consEnd = consequences.end();
			
			
			for (unsigned int arID = 1; consItr != consEnd; ++consItr, ++arID) {
				
				const unsigned int v = (*consItr)->getID();
				negativeAchieveArray[nextID][arID] = v;	
							
				negativeAchievedByList[v].push_front(nextID);	
				//cout << "Registering as an achiever for " << *achievedReal[toCopy.front()->getID()] << " (id=" << toCopy.front()->getID() << ") action (id=" << nextID << "): ";
				//applicableReal[nextID]->briefWrite(cout);
				
			}
		}
		
		

		/*cout << "Containers contain:";
		for (int i = 0; i < act->getParentAction()->getContainerCount(); i++) {
			cout << " " << containers[nextID][i];
		}
		cout << endl;*/
		
		//keepAction[nextID] = !(applicableReal[nextID]->getParentAction()->rootAction());
		//keepAction[nextID] = false;
		
	}

	
	memcpy(resetContainers, containersActually, resetActionSize);

	/*bool* predicateSeenBackwards = new bool[predicateCount];
	bool* predicateNegativeSeenBackwards = new bool[predicateCount];
	
	for (unsigned int i = 0; i < predicateCount; ++i) predicateSeenBackwards[i] = false;
	for (unsigned int i = 0; i < predicateCount; ++i) predicateNegativeSeenBackwards[i] = false;
	
	slist<unsigned int>* toProcess = new slist<unsigned int>();
	slist<unsigned int>* negativeToProcess = new slist<unsigned int>();
	
	goalItr = staticInf->GoalPredicates->begin();
	for (int i = 0; i < goalCount; i++, goalItr++) {
		toProcess->push_front(allPredicates->getPredicateInstance(*goalItr)->getID());
		predicateSeenBackwards[toProcess->front()] = true;
	}
		
	goalItr = staticInf->NegativeGoalPredicates->begin();
	for (int i = 0; i < negativeGoalCount; i++, goalItr++) {
		negativeToProcess->push_front(allPredicates->getPredicateInstance(*goalItr)->getID());
		predicateNegativeSeenBackwards[negativeToProcess->front()] = true;
	}
	*/
	//bool findingNew = true;
	
	
	subActionOf = new unsigned int[actionCount];
	subActions = new unsigned int*[actionCount];
	
	
	
	parentAchieved = new bool[actionCount];
	resetPA = new bool[actionCount];
	resetPASize = actionCount * sizeof(bool);
	
	for (unsigned int i = 0; i < actionCount; i++) {
		ActionInstance* currAI = applicableReal[i];
		const Action* tac = currAI->getParentAction();
		resetPA[i] = tac->rootAction();
		subActionOf[i] = 0;
		subActions[i] = new unsigned int[1];
		subActions[i][0] = 0;
	}
	
	map<ActionInstance*, ActionInstanceList, lt<ActionInstance> >::iterator dmStart = dependencyMap->begin();
	map<ActionInstance*, ActionInstanceList, lt<ActionInstance> >::iterator dmEnd = dependencyMap->end();
	
	ActionInstanceList::iterator clStart;
	ActionInstanceList::iterator clEnd;
	
	for (;dmStart != dmEnd; ++dmStart) {
	
//		cout << "Adding sub-action data for ";
//		(*dmStart).first->briefWrite(cout);
		unsigned int currID = (*dmStart).first->getID();
		delete [] subActions[currID];
		int lsize = (*dmStart).second.size();
		
		subActions[currID] = new unsigned int[lsize + 1];
		subActions[currID][0] = lsize;
		
		clStart = (*dmStart).second.begin();
		clEnd = (*dmStart).second.end();
		
		
		unsigned int subID;
		for (int i = 1; clStart != clEnd; ++clStart, ++i) {
			
			subID = (*clStart)->getID();
			
			subActions[currID][i] = subID;
			subActionOf[subID] = currID;
		
		}
	
	}
	/*
	while (findingNew) {
	
		findingNew = false;
		
		slist<unsigned int>* newToProcess = new slist<unsigned int>();
		slist<unsigned int>* newNegativeToProcess = new slist<unsigned int>();
		slist<unsigned int>::iterator itr_end = toProcess->end();
		slist<unsigned int>::iterator itrTwo;
		slist<unsigned int>::iterator itrTwoEnd;
		
		for (slist<unsigned int>::iterator itr = toProcess->begin();
			itr != itr_end;
			itr++) {
			
			//cout << *achievedReal[*itr] << " is achieved by:" << endl;
			itrTwo = achievedByList[*itr].begin();
			itrTwoEnd = achievedByList[*itr].end();
			
			for (; itrTwo != itrTwoEnd; itrTwo++) {
				
				slist<unsigned int> tp;
				tp.push_front(*itrTwo);
				
				while (!tp.empty()) {
					unsigned int currA = tp.front();
					tp.pop_front();
					if (!keepAction[currA]) {
						//applicableReal[currA]->briefWrite(cout);
						keepAction[currA] = true;
				
						for (unsigned int i = 1; i <= actionNeeds[currA][0]; i++) {
							nextID = actionNeeds[currA][i];
							if (!predicateSeenBackwards[nextID]) {
								predicateSeenBackwards[nextID] = true;
								newToProcess->push_front(nextID);
								findingNew = true;
							}
						}
						
						for (unsigned int i = 1; i <= actionNegativeNeeds[currA][0]; i++) {
							nextID = actionNegativeNeeds[currA][i];
							if (!predicateNegativeSeenBackwards[nextID]) {
								predicateNegativeSeenBackwards[nextID] = true;
								newNegativeToProcess->push_front(nextID);
								findingNew = true;
							}
						}
						
						if (!applicableReal[currA]->getParentAction()->rootAction()) {
							//cout << currA << " is a sub action of " << subActionOf[currA] << endl;
							tp.push_front(subActionOf[currA]);
						}
					}
				}
			}
			
			
			
			
		}
		
		itr_end = negativeToProcess->end();
		
		for (slist<unsigned int>::iterator itr = negativeToProcess->begin();
			itr != itr_end;
			itr++) {
			itrTwo = negativeAchievedByList[*itr].begin();
			itrTwoEnd = negativeAchievedByList[*itr].end();
			
			for (; itrTwo != itrTwoEnd; itrTwo++) {
				slist<unsigned int> tp;
				tp.push_front(*itrTwo);
				
				while (!tp.empty()) {
					unsigned int currA = tp.front();
					tp.pop_front();
					if (!keepAction[currA]) {
					
						keepAction[currA] = true;
						
						for (unsigned int i = 1; i <= actionNeeds[currA][0]; i++) {
							nextID = actionNeeds[currA][i];
							if (!predicateSeenBackwards[nextID]) {
								predicateSeenBackwards[nextID] = true;
								newToProcess->push_front(nextID);
								findingNew = true;
							}
						}
						
						for (unsigned int i = 1; i <= actionNegativeNeeds[currA][0]; i++) {
							nextID = actionNegativeNeeds[currA][i];
							if (!predicateNegativeSeenBackwards[nextID]) {
								predicateNegativeSeenBackwards[nextID] = true;
								newNegativeToProcess->push_front(nextID);
								findingNew = true;
							}
						}
						
						if (!applicableReal[currA]->getParentAction()->rootAction()) {
							tp.push_front(subActionOf[currA]);
						}
					}
				}
			}
		}
		
		delete toProcess;
		delete negativeToProcess;
		
		toProcess = newToProcess;
		negativeToProcess = newNegativeToProcess;
		
		
		
	}
	
	unsigned int numKept = 0;
	for (unsigned int i = 0; i < actionCount; i++) {
		if (keepAction[i]) {
			numKept++;
		} else {
			keepAction[i] = true;
			//cout << "Discarding ";
			//applicableReal[i]->briefWrite(cout);
			
			
		}
	}
	
	cout << "--------------" << endl;
	cout << numKept << " of " << actionCount << " kept" << endl;
	cout << "--------------" << endl;
	*/
//	delete toProcess;
	//delete negativeToProcess;
		
	list<unsigned int> daList;
	
	for (unsigned int i = 0; i < actionCount; i++) {
		if (containers[i][0] <= 0) {
			daList.push_back(i);
		}
	}
	
	defaultlyApplicable = new unsigned int[daList.size() + 1];
	defaultlyApplicable[0] = daList.size();
	
	list<unsigned int>::iterator dlBegin = daList.begin();
	list<unsigned int>::iterator dlEnd = daList.end();
	
	for (int i = 1; dlBegin != dlEnd; i++, dlBegin++) {
	
		defaultlyApplicable[i] = *dlBegin;
	
	}
	
	
	
	unsigned int newSize;
	unsigned int tVar;
	slist<unsigned int>::iterator itr;
	
	for (unsigned int i = 0; i < predicateCount; i++) {
	
		if (supportArray[i][0] > 0) {
			newSize = supportArray[i][0] << 1;
			
			delete [] supportArray[i];
			
			supportArray[i] = new unsigned int[newSize + 1];
			supportArray[i][0] = newSize;
			
			itr = supportList[i].begin();
			
			for (tVar = newSize; tVar > 0; tVar--, itr++) {
			
				supportArray[i][tVar] = *itr;
			
			}
			
			
		
		}
		
		if (negativeSupportArray[i][0] > 0) {
			newSize = negativeSupportArray[i][0] << 1;
			
			delete [] negativeSupportArray[i];
			
			negativeSupportArray[i] = new unsigned int[newSize + 1];
			negativeSupportArray[i][0] = newSize;
			
			itr = negativeSupportList[i].begin();
			
			for (tVar = newSize; tVar > 0; tVar--, itr++) {
			
				negativeSupportArray[i][tVar] = *itr;
			
			}
			
			
		
		}
	
	}
	
	delete [] supportList;
	delete [] negativeSupportList;
	
	delete [] achievedByList;
	delete [] negativeAchievedByList;
	
	
	//delete [] predicateSeenBackwards;
	//delete [] predicateNegativeSeenBackwards;
	
	memcpy(parentAchieved, resetPA, resetPASize);
	
	
	
}

auto_ptr<ActionInstanceList> RelaxedPlanningGraph::freeActions(StaticInformation & problemInformation, State * initialState) { 
	
	auto_ptr<ActionInstanceList> toReturn(new ActionInstanceList());
	
	list<unsigned int>::iterator spItr;
	list<unsigned int>::iterator spEnd;
	
	unsigned int loopLimit;
	unsigned int loopVar;
	
	unsigned int aLoopLimit;
	unsigned int aLoopVar;
	
	unsigned int itSupports;
	
	int toUpdate;
	int before;
	
	//problemInformation.verbose = true;
	
	miniResetRemaining();
	
	//list<State::iterator> tempLevelBoundaries;
	//list<State::iterator> tempLevelEnds;


		
	//list< ActionInstanceList::iterator > tempActionLevelEnds;
	
	
	//State::iterator predLyrEnd = predicates->end();
	//predLyrEnd--;
	
	//tempLevelEnds.push_back(predLyrEnd);

	list<unsigned int> * toProcess = new list<unsigned int>();
	
//	int kc, kl;
	

	State::iterator istEnd = initialState->end();
	
	for (State::iterator itr = initialState->begin(); itr != istEnd; ++itr) {
		PredicateInstance* replaceWith = allPredicates->getPredicateInstance(*itr);
		if (replaceWith != 0) {
			/*if (replaceWith != (*itr)) {
				if (problemInformation.verbose) cout << "Switching in predicate" << endl;
				predicates->switchInPredicateInstance(itr, replaceWith);
		
			}
			
			if (problemInformation.verbose) cout << "Adding " << *(*itr) << " to predicates to process" << endl;*/
			
			bool keep = !initialState->isToDelete(*itr);
			
			
			if (keep) {
				unsigned int tID = replaceWith->getID();
				toProcess->push_back(tID);
				achievedAt[tID] = 0;
				achieved[tID] = true;
				negativeAchieved[tID] = false;
				
				
				loopLimit = negativeSupportArray[tID][0];
				for (loopVar = 0; loopVar < loopLimit; loopVar+=2) {
					itSupports = negativeSupportArray[tID][loopVar+1];
					int npNo = negativeSupportArray[tID][loopVar + 2];
					toUpdate = applicableReal[itSupports]->getParentAction()->getNegativePreconditionUpdates(npNo);
					while (toUpdate != -1) {
						before = containers[itSupports][toUpdate];
						if (++containers[itSupports][toUpdate] > 0 && before <= 0) {
							toUpdate = applicableReal[itSupports]->getParentAction()->getContainerUpdates(toUpdate);
						} else {
							toUpdate = -1;
						}
					}
				}
				//cout << "Can keep " << *achievedReal[tID] << endl;;	
			} else {
				//unsigned int tID = replaceWith->getID();
				//cout << "Can't keep " << *achievedReal[tID] << endl;	
			}
				
		} else {
			if (problemInformation.verbose) cout << "Not adding " << *(*itr) << " to predicates to process: it is novel" << endl;
			cout << "This shouldn't happen" << endl;
			exit(0);
		}
	}
	
	
	
	//problemInformation.verbose = true;
	slist<unsigned int> caQueue;
	unsigned int caqEntry;
	loopLimit = defaultlyApplicable[0];
	
	for (unsigned int daItem = 1; daItem <= loopLimit; ++daItem) {
	
		itSupports = defaultlyApplicable[daItem];
		
		if (applicableReal[itSupports]->getParentAction()->isDPAction /*&&  keepAction[itSupports] */&& parentAchieved[itSupports] && containers[itSupports][0] <= 0) {
			if (applicableReal[itSupports]->getParentAction()->rootAction()) {
				toReturn->push_back(applicableReal[itSupports]);
			}
			
			aLoopLimit = subActions[itSupports][0];
			for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
				caQueue.push_front(subActions[itSupports][aLoopVar]);
			}
			
			while (!caQueue.empty()) {
				caqEntry = caQueue.front();
				caQueue.pop_front();
				if (!parentAchieved[caqEntry] /*&& keepAction[caqEntry]*/) {
				
					parentAchieved[caqEntry] = true;
					
					
					
					if (containers[caqEntry][0] <= 0) {
						
					
						aLoopLimit = subActions[caqEntry][0];
						for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
							caQueue.push_front(subActions[caqEntry][aLoopVar]);
						}
					}
				}
			}
			
		}
	
	}
	
	
	
	
	
	spItr = toProcess->begin();
	spEnd = toProcess->end();
	

	
	
	for (; spItr != spEnd; spItr++) {
		loopLimit = supportArray[*spItr][0];
		if (problemInformation.verbose) {
			cout << "Predicate " << *(achievedReal[*spItr]) << " supports " << (loopLimit / 2) << " actions" << endl;
			cout << "(" << loopLimit << " entries)" << endl;
		}
		
		for (loopVar = 0; loopVar < loopLimit; loopVar+=2) {
			itSupports = supportArray[*spItr][loopVar + 1];
			//cout << "Action " << itSupports << " of " << oldRPG.actionCount << " is supported" << endl;
			if (applicableReal[itSupports]->getParentAction()->isDPAction/* && keepAction[itSupports]*/ && containers[itSupports][0] > 0) { // if action not achieved already
	
				toUpdate = applicableReal[itSupports]->getParentAction()->getPreconditionUpdates(supportArray[*spItr][loopVar + 2]);
				while (toUpdate != -1) {
					before = containers[itSupports][toUpdate];
					if (--containers[itSupports][toUpdate] == 0 && before > 0) {
						toUpdate = applicableReal[itSupports]->getParentAction()->getContainerUpdates(toUpdate);
					} else {
						toUpdate = -1;
					}
				}						
				
				if (containers[itSupports][0] == 0) {
					

					//cout << "Action " << itSupports << " is now achievable, container = " << containers[itSupports][0] << ":";
					//applicableReal[itSupports]->briefWrite(cout);
					
					if (problemInformation.verbose)cout << "Action " << itSupports << " is now achievable" << endl;
					//actions.push_back(oldRPG.applicableReal[itSupports]);
					
					if (applicableReal[itSupports]->getParentAction()->rootAction()) {
						toReturn->push_back(applicableReal[itSupports]);
					}
					
					aLoopLimit = subActions[itSupports][0];
					for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
						caQueue.push_front(subActions[itSupports][aLoopVar]);
					}
					
					while (!caQueue.empty()) {
						caqEntry = caQueue.front();
						caQueue.pop_front();
						if (!parentAchieved[caqEntry]/* && keepAction[caqEntry]*/) {
						
							parentAchieved[caqEntry] = true;
							
							
							
							if (containers[caqEntry][0] <= 0) {
								
							
								aLoopLimit = subActions[caqEntry][0];
								for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
									caQueue.push_front(subActions[caqEntry][aLoopVar]);
								}
							}
						}
					}
					
				} else {
					//cout << "Action " << itSupports << " is not yet achievable, container = " << containers[itSupports][0] << ":";
					//applicableReal[itSupports]->briefWrite(cout);
				}
			}
		}
	}
	
	
	
	delete toProcess;
	

	//cout << toReturn.size() << " free actions" << endl;
	
	return toReturn;
}

auto_ptr<ActionInstanceWithSubActionsList> RelaxedPlanningGraph::applicableRelaxedActions(StaticInformation & problemInformation, State * initialState, const EntitySet* const & exemplars) {

	auto_ptr<ActionInstanceWithSubActionsList> toReturn(new ActionInstanceWithSubActionsList() );
	list<unsigned int>::iterator spItr;
	list<unsigned int>::iterator spEnd;
	
	unsigned int loopLimit;
	unsigned int loopVar;
	
	unsigned int aLoopLimit;
	unsigned int aLoopVar;
	
	unsigned int itSupports;
	
	int toUpdate;
	int before;
	
	//problemInformation.verbose = true;
	
	miniResetRemaining();
	
	//list<State::iterator> tempLevelBoundaries;
	//list<State::iterator> tempLevelEnds;


		
	//list< ActionInstanceList::iterator > tempActionLevelEnds;
	
	
	//State::iterator predLyrEnd = predicates->end();
	//predLyrEnd--;
	
	//tempLevelEnds.push_back(predLyrEnd);

	list<unsigned int> * toProcess = new list<unsigned int>();
	

	const State::iterator istEnd = initialState->end();
	
	for (State::iterator itr = initialState->begin(); itr != istEnd; ++itr) {
		PredicateInstance* replaceWith = allPredicates->getPredicateInstance(*itr);
		if (replaceWith != 0) {
			/*if (replaceWith != (*itr)) {
				if (problemInformation.verbose) cout << "Switching in predicate" << endl;
				predicates->switchInPredicateInstance(itr, replaceWith);
		
			}
			
			if (problemInformation.verbose) cout << "Adding " << *(*itr) << " to predicates to process" << endl;*/
			
			const bool isToDel = initialState->isToDelete(*itr);
			
			
			
			if (!isToDel) {
				unsigned int tID = replaceWith->getID();
				toProcess->push_back(tID);
				achievedAt[tID] = 0;
				achieved[tID] = true;
				negativeAchieved[tID] = false;
				
				
				loopLimit = negativeSupportArray[tID][0];
				for (loopVar = 0; loopVar < loopLimit; loopVar+=2) {
					itSupports = negativeSupportArray[tID][loopVar+1];
					int npNo = negativeSupportArray[tID][loopVar + 2];
					toUpdate = applicableReal[itSupports]->getParentAction()->getNegativePreconditionUpdates(npNo);
					while (toUpdate != -1) {
						before = containers[itSupports][toUpdate];
						if (++containers[itSupports][toUpdate] > 0 && before <= 0) {
							toUpdate = applicableReal[itSupports]->getParentAction()->getContainerUpdates(toUpdate);
						} else {
							toUpdate = -1;
						}
					}
				}
			
			} else {
			
				const bool isToAdd = initialState->isToAdd(*itr);	
				
				if (isToAdd) {
				
					unsigned int tID = replaceWith->getID();
					
					negativeAchieved[tID] = false;
					
					loopLimit = negativeSupportArray[tID][0];
					for (loopVar = 0; loopVar < loopLimit; loopVar+=2) {
						itSupports = negativeSupportArray[tID][loopVar+1];
						int npNo = negativeSupportArray[tID][loopVar + 2];
						toUpdate = applicableReal[itSupports]->getParentAction()->getNegativePreconditionUpdates(npNo);
						while (toUpdate != -1) {
							before = containers[itSupports][toUpdate];
							if (++containers[itSupports][toUpdate] > 0 && before <= 0) {
								toUpdate = applicableReal[itSupports]->getParentAction()->getContainerUpdates(toUpdate);
							} else {
								toUpdate = -1;
							}
						}
					}	
				
				}
				
			
			}
			
			
				
		} else {
			if (problemInformation.verbose) cout << "Not adding " << *(*itr) << " to predicates to process: it is novel" << endl;
			cout << "This shouldn't happen" << endl;
			exit(0);
		}
	}
	
	
	PredicateInstanceSet::const_iterator toAddItr = initialState->getPredicatesToAdd().begin();
	const PredicateInstanceSet::const_iterator toAddEnd = initialState->getPredicatesToAdd().end();
	
	for (; toAddItr != toAddEnd; ++toAddItr) {
	
		PredicateInstance* replaceWith = allPredicates->getPredicateInstance(*toAddItr);
		
		if (replaceWith != 0) {
	
			const bool cpi = initialState->containsPredicateInstance(replaceWith);
			
			if (!cpi) {
				unsigned int tID = replaceWith->getID();
				
				negativeAchieved[tID] = false;
					
					
				loopLimit = negativeSupportArray[tID][0];
				for (loopVar = 0; loopVar < loopLimit; loopVar+=2) {
					itSupports = negativeSupportArray[tID][loopVar+1];
					int npNo = negativeSupportArray[tID][loopVar + 2];
					toUpdate = applicableReal[itSupports]->getParentAction()->getNegativePreconditionUpdates(npNo);
					while (toUpdate != -1) {
						before = containers[itSupports][toUpdate];
						if (++containers[itSupports][toUpdate] > 0 && before <= 0) {
							toUpdate = applicableReal[itSupports]->getParentAction()->getContainerUpdates(toUpdate);
						} else {
							toUpdate = -1;
						}
					}
				}
			}
			
		} else {
			if (problemInformation.verbose) cout << "Not adding " << *(*toAddItr) << " to predicates to process: it is novel" << endl;
			cout << "This shouldn't happen" << endl;
			exit(0);
		}		
	
	};
	
	//problemInformation.verbose = true;
	slist<unsigned int> caQueue;
	unsigned int caqEntry;
	loopLimit = defaultlyApplicable[0];
	
	for (unsigned int daItem = 1; daItem <= loopLimit; ++daItem) {
	
		itSupports = defaultlyApplicable[daItem];
		
		if (!applicableReal[itSupports]->getParentAction()->isDPAction/* &&  keepAction[itSupports]*/ && parentAchieved[itSupports] && containers[itSupports][0] <= 0) {
			if (applicableReal[itSupports]->getParentAction()->rootAction()) {
				toReturn->push_back(new ActionInstanceWithSubActions(applicableReal[itSupports]));
			}
			
			aLoopLimit = subActions[itSupports][0];
			for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
				caQueue.push_front(subActions[itSupports][aLoopVar]);
			}
			
			while (!caQueue.empty()) {
				caqEntry = caQueue.front();
				caQueue.pop_front();
				if (!parentAchieved[caqEntry] /*&& keepAction[caqEntry]*/) {
				
					parentAchieved[caqEntry] = true;
					
					
					
					if (containers[caqEntry][0] <= 0) {
						
					
						aLoopLimit = subActions[caqEntry][0];
						for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
							caQueue.push_front(subActions[caqEntry][aLoopVar]);
						}
					}
				}
			}
			
		}
	
	}
	
	
	
	
	
	spItr = toProcess->begin();
	spEnd = toProcess->end();
	

	
	
	for (; spItr != spEnd; ++spItr) {
		loopLimit = supportArray[*spItr][0];
		if (problemInformation.verbose) {
			cout << "Predicate " << *(achievedReal[*spItr]) << " supports " << (loopLimit / 2) << " actions" << endl;
			cout << "(" << loopLimit << " entries)" << endl;
		}
		
		for (loopVar = 0; loopVar < loopLimit; loopVar+=2) {
			itSupports = supportArray[*spItr][loopVar + 1];
			//cout << "Action " << itSupports << " of " << oldRPG.actionCount << " is supported" << endl;
			if (!applicableReal[itSupports]->getParentAction()->isDPAction/* && keepAction[itSupports]*/ && containers[itSupports][0] > 0) { // if action not achieved already
			/*bool partOne = !applicableReal[itSupports]->getParentAction()->isDPAction;
			if (partOne) { cout << "partOne = true" << endl; };
			bool partTwo = keepAction[itSupports];
			if (partTwo) { cout << "partTwo = true" << endl; };
			bool partThree = containers[itSupports][0] > 0;
			if (partThree) { cout << "partThree = true" << endl; };
			if (partOne && partTwo && partThree) {*/
			
				toUpdate = applicableReal[itSupports]->getParentAction()->getPreconditionUpdates(supportArray[*spItr][loopVar + 2]);
				while (toUpdate != -1) {
					before = containers[itSupports][toUpdate];
					if (--containers[itSupports][toUpdate] == 0 && before > 0) {
						toUpdate = applicableReal[itSupports]->getParentAction()->getContainerUpdates(toUpdate);
					} else {
						toUpdate = -1;
					}
				}						
				
				if (containers[itSupports][0] == 0) {
					

					//cout << "Action " << itSupports << " is now achievable, container = " << containers[itSupports][0] << ":";
					//applicableReal[itSupports]->briefWrite(cout);
					
					if (problemInformation.verbose)cout << "Action " << itSupports << " is now achievable" << endl;
					//actions.push_back(oldRPG.applicableReal[itSupports]);
					
					if (applicableReal[itSupports]->getParentAction()->rootAction()) {
						toReturn->push_back(new ActionInstanceWithSubActions(applicableReal[itSupports]));
					}
					
					aLoopLimit = subActions[itSupports][0];
					for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
						caQueue.push_front(subActions[itSupports][aLoopVar]);
					}
					
					while (!caQueue.empty()) {
						caqEntry = caQueue.front();
						caQueue.pop_front();
						if (!parentAchieved[caqEntry] /*&& keepAction[caqEntry]*/) {
						
							parentAchieved[caqEntry] = true;
							
							
							
							if (containers[caqEntry][0] <= 0) {
								
							
								aLoopLimit = subActions[caqEntry][0];
								for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
									caQueue.push_front(subActions[caqEntry][aLoopVar]);
								}
							}
						}
					}
					
				} else {
					//cout << "Action " << itSupports << " is not yet achievable, container = " << containers[itSupports][0] << ":";
					//applicableReal[itSupports]->briefWrite(cout);
				}
			}
		}
	}
	
	
	
	delete toProcess;
	
	ActionInstanceWithSubActionsList::iterator trItr = toReturn->begin();
	const ActionInstanceWithSubActionsList::iterator trEnd = toReturn->end();
	
	
	for (; trItr != trEnd; ++trItr) {
	
		const ActionInstance* currAI = (*trItr)->getFirst();
		//cout << "Collecting sub-actions for ";
		//currAI->briefWrite(cout);
		/*ActionInstance* sakeOfIt = getMasterAI(currAI);
		sakeOfIt->incrUsage();
		sakeOfIt->decrUsage();*/
		
		
		
		bool keep = true;
		
		if (exemplars != 0) {
		
			const int argCount = currAI->getParentAction()->parameterCount;
		
			for (int i = 0; keep && i < argCount; ++i) {
		
				keep = (exemplars->find(currAI->getParameter(i)->boundTo()) != exemplars->end());
				
			}
		
		}
		
		if (keep) {
			ActionInstanceList* currList = &((*trItr)->getSecond());
			
			itSupports = currAI->getID();
			
			aLoopLimit = subActions[itSupports][0];
			for (aLoopVar = 1; aLoopVar <= aLoopLimit; ++aLoopVar) {
				caQueue.push_front(subActions[itSupports][aLoopVar]);
			}
			
			while (!caQueue.empty()) {
				caqEntry = caQueue.front();
				caQueue.pop_front();
				if (containers[caqEntry][0] <= 0/* && keepAction[caqEntry]*/) {
				
					currList->push_back(applicableReal[caqEntry]);
					//cout << " - ";
					//applicableReal[caqEntry]->briefWrite(cout);
					
					aLoopLimit = subActions[caqEntry][0];
					for (aLoopVar = 1; aLoopVar <= aLoopLimit; ++aLoopVar) {
						caQueue.push_front(subActions[caqEntry][aLoopVar]);
					}
				
					
				}
			}
		}
	
	}
	
	
	return toReturn;

};

RelaxedPlanningGraph::RelaxedPlanningGraph(RelaxedPlanningGraph & oldRPG, StaticInformation & problemInformation, State * initialState) : goalIDs(0), supportArray(0), applicableReal(0), actionNeeds(0), achieveArray(0), achieved(0), achievedAt(0), firstAchiever(0), resetAchieved(0), achievedReal(0),predicateInstanceIsGoal(0), containersActually(0), resetContainers(0) {


	list<unsigned int>::iterator spItr;
	list<unsigned int>::iterator spEnd;
	
	unsigned int loopLimit;
	unsigned int loopVar;
	
	unsigned int aLoopLimit;
	unsigned int aLoopVar;
	
	unsigned int itSupports;
	unsigned int itAchieves;
	
	int toUpdate;
	int before;
	
	//problemInformation.verbose = true;
	
	levelCount = 1;
	
	if (problemInformation.verbose) cout << "Using incremental constructor" << endl;
	
	oldRPG.resetRemaining();
	
	
	
	
	predicates = initialState;

	
	
	
	
	//list<State::iterator> tempLevelBoundaries;
	//list<State::iterator> tempLevelEnds;


		
	//list< ActionInstanceList::iterator > tempActionLevelEnds;
	
	
	//State::iterator predLyrEnd = predicates->end();
	//predLyrEnd--;
	
	//tempLevelEnds.push_back(predLyrEnd);

	list<unsigned int> * toProcess = new list<unsigned int>();
	list<unsigned int> * negativeToProcess = new list<unsigned int>();
	
	unsigned int unachievedGoals = oldRPG.goalCount;
	
	int actionsIn = 0;

	for (State::iterator itr = predicates->begin(); itr != predicates->end(); itr++) {
		PredicateInstance* replaceWith = oldRPG.allPredicates->getPredicateInstance(*itr);
		if (replaceWith != 0) {
			/*if (replaceWith != (*itr)) {
				if (problemInformation.verbose) cout << "Switching in predicate" << endl;
				predicates->switchInPredicateInstance(itr, replaceWith);
		
			}
			
			if (problemInformation.verbose) cout << "Adding " << *(*itr) << " to predicates to process" << endl;*/
			
			bool keep = !predicates->isToDelete(*itr);
			
			for (	list<EventQueueItem>::const_iterator itrTwo = predicates->getEventQueue().begin();
				itrTwo != predicates->getEventQueue().end();
				itrTwo++) {
			
				const EventQueueItem * currEQI = &(*itrTwo); // avoid having to type (*itr). all the time & make auto-complete work in kdevelop
		
				for (	PredicateInstanceSet::const_iterator iItr = currEQI->deleteEffects.begin();
					iItr != currEQI->deleteEffects.end();
					iItr++) {
		
					PredicateInstance* replaceWithTwo = oldRPG.allPredicates->getPredicateInstance(*iItr);
					if (replaceWithTwo != 0) {
					
						if (replaceWithTwo == replaceWith) keep = false;	
					}
				}
				
				for (	PredicateInstanceSet::const_iterator iItr = currEQI->addEffects.begin();
					iItr != currEQI->addEffects.end();
					iItr++) {
		
					PredicateInstance* replaceWithTwo = oldRPG.allPredicates->getPredicateInstance(*iItr);
					if (replaceWithTwo != 0) {
					
						if (replaceWithTwo == replaceWith) keep = true;	
					}
				}
			}
			
			if (keep) {
				unsigned int tID = replaceWith->getID();
				toProcess->push_back(tID);
				oldRPG.achievedAt[tID] = 0;
				oldRPG.achieved[tID] = true;
				oldRPG.negativeAchieved[tID] = false;
				oldRPG.negativeInitially[tID] = false;
								
				if (oldRPG.predicateInstanceIsGoal[tID]) unachievedGoals--;
				if (oldRPG.predicateInstanceIsNegativeGoal[tID]) unachievedGoals++;
				
				loopLimit = oldRPG.negativeSupportArray[tID][0];
				for (loopVar = 0; loopVar < loopLimit; loopVar+=2) {
					itSupports = oldRPG.negativeSupportArray[tID][loopVar+1];
					//cout << "Predicate is negative precondition of action no. " << itSupports << endl;
					//cout << " of " << oldRPG.actionCount << endl;
					int npNo = oldRPG.negativeSupportArray[tID][loopVar + 2];
					//cout << "(Negative precondition no. " << npNo << ")" << endl;
					const Action* PA = oldRPG.applicableReal[itSupports]->getParentAction();
					toUpdate = PA->getNegativePreconditionUpdates(npNo);
					while (toUpdate != -1) {
						before = oldRPG.containers[itSupports][toUpdate];
						if (++oldRPG.containers[itSupports][toUpdate] > 0 && before <= 0) {
							toUpdate = PA->getContainerUpdates(toUpdate);
						} else {
							toUpdate = -1;
						}
					}
				}
				
			}
				
		} else {
			if (problemInformation.verbose) cout << "Not adding " << *(*itr) << " to predicates to process: it is novel" << endl;
			cout << "This shouldn't happen" << endl;
			exit(0);
		}
	}
	
	PredicateInstanceSet::const_iterator addPredItr = predicates->getPredicatesToAdd().begin();
	PredicateInstanceSet::const_iterator addPredEnd = predicates->getPredicatesToAdd().end();
	
	for (; addPredItr != addPredEnd; addPredItr++) {
		PredicateInstance* replaceWith = oldRPG.allPredicates->getPredicateInstance(*addPredItr);
		if (replaceWith != 0) {
			/*if (replaceWith != (*itr)) {
				if (problemInformation.verbose) cout << "Switching in predicate" << endl;
				predicates->switchInPredicateInstance(itr, replaceWith);
		
			}
			
			if (problemInformation.verbose) cout << "Adding " << *(*itr) << " to predicates to process" << endl;*/
			
			bool keep = true;
			
			for (	list<EventQueueItem>::const_iterator itrTwo = predicates->getEventQueue().begin();
				itrTwo != predicates->getEventQueue().end();
				itrTwo++) {
			
				const EventQueueItem * currEQI = &(*itrTwo); // avoid having to type (*itr). all the time & make auto-complete work in kdevelop
		
				for (	PredicateInstanceSet::const_iterator iItr = currEQI->deleteEffects.begin();
					iItr != currEQI->deleteEffects.end();
					iItr++) {
		
					PredicateInstance* replaceWithTwo = oldRPG.allPredicates->getPredicateInstance(*iItr);
					if (replaceWithTwo != 0) {
					
						if (replaceWithTwo == replaceWith) keep = false;	
					}
				}
				
				for (	PredicateInstanceSet::const_iterator iItr = currEQI->addEffects.begin();
					iItr != currEQI->addEffects.end();
					iItr++) {
		
					PredicateInstance* replaceWithTwo = oldRPG.allPredicates->getPredicateInstance(*iItr);
					if (replaceWithTwo != 0) {
					
						if (replaceWithTwo == replaceWith) keep = true;	
					}
				}
			}
			
			if (keep) {
				unsigned int tID = replaceWith->getID();
				toProcess->push_back(tID);
				oldRPG.achievedAt[tID] = 0;
				oldRPG.achieved[tID] = true;
				oldRPG.negativeAchieved[tID] = false;
				oldRPG.negativeInitially[tID] = false;
								
				if (oldRPG.predicateInstanceIsGoal[tID]) unachievedGoals--;
				if (oldRPG.predicateInstanceIsNegativeGoal[tID]) unachievedGoals++;
				
				loopLimit = oldRPG.negativeSupportArray[tID][0];
				for (loopVar = 0; loopVar < loopLimit; loopVar+=2) {
					itSupports = oldRPG.negativeSupportArray[tID][loopVar+1];
					//cout << "Predicate is negative precondition of action no. " << itSupports << endl;
					//cout << " of " << oldRPG.actionCount << endl;
					int npNo = oldRPG.negativeSupportArray[tID][loopVar + 2];
					//cout << "(Negative precondition no. " << npNo << ")" << endl;
					const Action* PA = oldRPG.applicableReal[itSupports]->getParentAction();
					toUpdate = PA->getNegativePreconditionUpdates(npNo);
					while (toUpdate != -1) {
						before = oldRPG.containers[itSupports][toUpdate];
						if (++oldRPG.containers[itSupports][toUpdate] > 0 && before <= 0) {
							toUpdate = PA->getContainerUpdates(toUpdate);
						} else {
							toUpdate = -1;
						}
					}
				}
				
			}
				
		} else {
			if (problemInformation.verbose) cout << "Not adding " << *(*addPredItr) << " to predicates to process: it is novel" << endl;
			cout << "This shouldn't happen" << endl;
			exit(0);
		}
	}
	
	list<EventQueueItem>::const_iterator eqAddEffItr = predicates->getEventQueue().begin();
	list<EventQueueItem>::const_iterator eqAddEffEnd = predicates->getEventQueue().end();
	
	for (;eqAddEffItr != eqAddEffEnd; ++eqAddEffItr) {
	
		addPredItr = (*eqAddEffItr).addEffects.begin();
		addPredEnd = (*eqAddEffItr).addEffects.end();
		
		for (; addPredItr != addPredEnd; ++addPredItr) {
			PredicateInstance* replaceWith = oldRPG.allPredicates->getPredicateInstance(*addPredItr);
			if (replaceWith != 0) {
				if (!oldRPG.achieved[replaceWith->getID()]) {
					/*if (replaceWith != (*itr)) {
						if (problemInformation.verbose) cout << "Switching in predicate" << endl;
						predicates->switchInPredicateInstance(itr, replaceWith);
				
					}
					
					if (problemInformation.verbose) cout << "Adding " << *(*itr) << " to predicates to process" << endl;*/
					
					bool keep = true;
					
					list<EventQueueItem>::const_iterator itrTwo = eqAddEffItr;
					itrTwo++;
					
					
					for (	;itrTwo != eqAddEffEnd; ++itrTwo) {
					
						const EventQueueItem * currEQI = &(*itrTwo); // avoid having to type (*itr). all the time & make auto-complete work in kdevelop
				
						for (	PredicateInstanceSet::const_iterator iItr = currEQI->deleteEffects.begin();
							iItr != currEQI->deleteEffects.end();
							iItr++) {
				
							PredicateInstance* replaceWithTwo = oldRPG.allPredicates->getPredicateInstance(*iItr);
							if (replaceWithTwo != 0) {
							
								if (replaceWithTwo == replaceWith) keep = false;	
							}
						}
						
						for (	PredicateInstanceSet::const_iterator iItr = currEQI->addEffects.begin();
							iItr != currEQI->addEffects.end();
							iItr++) {
				
							PredicateInstance* replaceWithTwo = oldRPG.allPredicates->getPredicateInstance(*iItr);
							if (replaceWithTwo != 0) {
							
								if (replaceWithTwo == replaceWith) keep = true;	
							}
						}
					}
					
					if (keep) {
						unsigned int tID = replaceWith->getID();
						toProcess->push_back(tID);
						oldRPG.achievedAt[tID] = 0;
						oldRPG.achieved[tID] = true;
						oldRPG.negativeAchieved[tID] = false;
						oldRPG.negativeInitially[tID] = false;
										
						if (oldRPG.predicateInstanceIsGoal[tID]) unachievedGoals--;
						if (oldRPG.predicateInstanceIsNegativeGoal[tID]) unachievedGoals++;
						
						loopLimit = oldRPG.negativeSupportArray[tID][0];
						for (loopVar = 0; loopVar < loopLimit; loopVar+=2) {
							itSupports = oldRPG.negativeSupportArray[tID][loopVar+1];
							//cout << "Predicate is negative precondition of action no. " << itSupports << endl;
							//cout << " of " << oldRPG.actionCount << endl;
							int npNo = oldRPG.negativeSupportArray[tID][loopVar + 2];
							//cout << "(Negative precondition no. " << npNo << ")" << endl;
							const Action* PA = oldRPG.applicableReal[itSupports]->getParentAction();
							toUpdate = PA->getNegativePreconditionUpdates(npNo);
							while (toUpdate != -1) {
								before = oldRPG.containers[itSupports][toUpdate];
								if (++oldRPG.containers[itSupports][toUpdate] > 0 && before <= 0) {
									toUpdate = PA->getContainerUpdates(toUpdate);
								} else {
									toUpdate = -1;
								}
							}
						}
						
					}
				}
				
			} else {
				if (problemInformation.verbose) cout << "Not adding " << *(*addPredItr) << " to predicates to process: it is novel" << endl;
				cout << "This shouldn't happen" << endl;
				exit(0);
			}
		}
	
	}
	
	
	if (problemInformation.verbose) cout << "Begin incremental plan graph generation" << endl;
	
	/*spItr = toProcess->begin();
	spEnd = toProcess->end();
	
	cout << "These predicates are in layer 0:" << endl;
	for (;spItr != spEnd; ++spItr) {
		cout << *(oldRPG.achievedReal[*spItr]) << endl;
	}*/
	
	
	bool findingNew = true;
	//problemInformation.verbose = true;

	slist<unsigned int> caQueue;
	unsigned int caqEntry;
	
	bool checkDaList = true;
	
	if (unachievedGoals == 0) {
		cout << "All goals achieved!" << endl;
	}
	
	while (findingNew && unachievedGoals > 0) {
		int actionsInLayer = 0;
		//problemInformation.verbose = true;
		if (problemInformation.verbose) cout << "Generating action layer " << levelCount << " and inferring subsequent fact layer" << endl;

		/*for (PredicateInstanceList::iterator cdItr = toProcess.begin(); cdItr != toProcess.end(); cdItr++) {
			if (processed.contains(*cdItr)) {
				cout << "Double-processing predicate " << *(*cdItr) << endl;
				exit(0);
			}
			processed.addPredicateInstance(*cdItr);
		}*/
		list<unsigned int> * nextToProcess = new list<unsigned int>();
		list<unsigned int> * nextNegativeToProcess = new list<unsigned int>();
		spItr = toProcess->begin();
		spEnd = toProcess->end();
		
		//problemInformation.verbose = true;
		if (checkDaList) {
			unsigned int daLimit = oldRPG.defaultlyApplicable[0];
			
			for (unsigned int daItem = 1; daItem <= daLimit; daItem++) {
			
				itSupports = oldRPG.defaultlyApplicable[daItem];
				
				if (/*oldRPG.keepAction[itSupports] && */oldRPG.containers[itSupports][0] <= 0 && oldRPG.parentAchieved[itSupports]) {
				
					actionsIn++;
					actionsInLayer++;

					if (problemInformation.verbose) {
						cout << "Action is now achievable: ";
						oldRPG.applicableReal[itSupports]->briefWrite(cout);
					}
					//actions.push_back(oldRPG.applicableReal[itSupports]);
					aLoopLimit = oldRPG.achieveArray[itSupports][0];
					if (problemInformation.verbose)cout << "Action has " << aLoopLimit << " add effects" << endl;
					for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
						itAchieves = oldRPG.achieveArray[itSupports][aLoopVar];
						if (problemInformation.verbose) cout << "Add Effect: Predicate " << aLoopVar << ": " << *(oldRPG.achievedReal[itAchieves]) << endl;
						
						if (!oldRPG.achieved[itAchieves]) {
							if (problemInformation.verbose) cout << " -- Previously not present" << endl;
							oldRPG.achievedAt[itAchieves] = levelCount;
							oldRPG.firstAchiever[itAchieves] = itSupports;
							oldRPG.achieved[itAchieves] = true;
							if (oldRPG.predicateInstanceIsGoal[itAchieves]) unachievedGoals--;
							//predicates->addPredicateInstance(oldRPG.achievedReal[itAchieves]);
							nextToProcess->push_back(itAchieves);
						}else { 
							if (problemInformation.verbose) cout << " -- Previously present" << endl;
						}
					}
					aLoopLimit = oldRPG.negativeAchieveArray[itSupports][0];
					if (problemInformation.verbose)cout << "Action has " << aLoopLimit << " delete effects" << endl;
					for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
						itAchieves = oldRPG.negativeAchieveArray[itSupports][aLoopVar];
						if (problemInformation.verbose) cout << "Delete Effect: Predicate " << aLoopVar << ": " << *(oldRPG.achievedReal[itAchieves]) << endl;
						
						if (!oldRPG.negativeAchieved[itAchieves]) {
							if (problemInformation.verbose) cout << " -- Previously not present" << endl;
							oldRPG.negativeAchievedAt[itAchieves] = levelCount;
							oldRPG.firstNegativeAchiever[itAchieves] = itSupports;
							oldRPG.negativeAchieved[itAchieves] = true;
							if (oldRPG.predicateInstanceIsNegativeGoal[itAchieves]) unachievedGoals--;
							//predicates->addPredicateInstance(oldRPG.achievedReal[itAchieves]);
							nextNegativeToProcess->push_back(itAchieves);
						}else { 
							if (problemInformation.verbose) cout << " -- Previously present" << endl;
						}
					}
					
					//cout << "Action supports " << oldRPG.subActions[itSupports][0] << " sub actions" << endl;
					
					aLoopLimit = oldRPG.subActions[itSupports][0];
					for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
						//cout << "Action activates subaction " << oldRPG.subActions[itSupports][aLoopVar] << endl;
						caQueue.push_front(oldRPG.subActions[itSupports][aLoopVar]);
					}
					
					while (!caQueue.empty()) {
						caqEntry = caQueue.front();
						caQueue.pop_front();
						//cout << "Considering subaction " << caqEntry << endl;
						
						if (oldRPG.parentAchieved[caqEntry]) {
						 	//cout << "parentAchieved already set" << endl;
						} else {
						
							if (/*!oldRPG.keepAction[caqEntry]*/ false) {
							 	//cout << "Action is not to keep" << endl;
							} else {
						
								oldRPG.parentAchieved[caqEntry] = true;
								
								
								
								if (oldRPG.containers[caqEntry][0] <= 0) {
									aLoopLimit = oldRPG.achieveArray[caqEntry][0];
									if (problemInformation.verbose)cout << "Action has " << aLoopLimit << " add effects" << endl;
									
									for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
										itAchieves = oldRPG.achieveArray[caqEntry][aLoopVar];
										if (problemInformation.verbose) cout << "Add Effect: Predicate " << aLoopVar << ": " << *(oldRPG.achievedReal[itAchieves]) << endl;
										
										if (!oldRPG.achieved[itAchieves]) {
											if (problemInformation.verbose) cout << " -- Previously not present" << endl;
											oldRPG.achievedAt[itAchieves] = levelCount;
											oldRPG.firstAchiever[itAchieves] = caqEntry;
											oldRPG.achieved[itAchieves] = true;
											if (oldRPG.predicateInstanceIsGoal[itAchieves]) unachievedGoals--;
											//predicates->addPredicateInstance(oldRPG.achievedReal[itAchieves]);
											nextToProcess->push_back(itAchieves);
										}else { 
											if (problemInformation.verbose) cout << " -- Previously present" << endl;
										}
									}
									aLoopLimit = oldRPG.negativeAchieveArray[caqEntry][0];
									if (problemInformation.verbose)cout << "Action has " << aLoopLimit << " delete effects" << endl;
									for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
										itAchieves = oldRPG.negativeAchieveArray[caqEntry][aLoopVar];
										if (problemInformation.verbose) cout << "Delete Effect: Predicate " << aLoopVar << ": " << *(oldRPG.achievedReal[itAchieves]) << endl;
										
										if (!oldRPG.negativeAchieved[itAchieves]) {
											if (problemInformation.verbose) cout << " -- Previously not present" << endl;
											oldRPG.negativeAchievedAt[itAchieves] = levelCount;
											oldRPG.firstNegativeAchiever[itAchieves] = caqEntry;
											oldRPG.negativeAchieved[itAchieves] = true;
											if (oldRPG.predicateInstanceIsNegativeGoal[itAchieves]) unachievedGoals--;
											//predicates->addPredicateInstance(oldRPG.achievedReal[itAchieves]);
											nextNegativeToProcess->push_back(itAchieves);
										}else { 
											if (problemInformation.verbose) cout << " -- Previously present" << endl;
										}
									}
								
								
									aLoopLimit = oldRPG.subActions[caqEntry][0];
									for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
										caQueue.push_front(oldRPG.subActions[caqEntry][aLoopVar]);
									}
								} else {
									//cout << "Subaction " << caqEntry << " not applicable yet" << endl;
								}
							}
						}
					}
				
				
				}
			
			};
		}
		for (; spItr != spEnd; spItr++) {
			loopLimit = oldRPG.supportArray[*spItr][0];
			if (problemInformation.verbose) {
				cout << "Predicate " << *(oldRPG.achievedReal[*spItr]) << " supports " << (loopLimit / 2) << " actions" << endl;
				cout << "(" << loopLimit << " entries)" << endl;
			}
			
			for (loopVar = 0; loopVar < loopLimit; loopVar+=2) {
				itSupports = oldRPG.supportArray[*spItr][loopVar + 1];
				//cout << "Action " << itSupports << " of " << oldRPG.actionCount << " is supported" << endl;
				if (/*oldRPG.keepAction[itSupports] && */oldRPG.containers[itSupports][0] > 0) { // if action not achieved already
		
					const Action* PA = oldRPG.applicableReal[itSupports]->getParentAction();
					toUpdate = PA->getPreconditionUpdates(oldRPG.supportArray[*spItr][loopVar + 2]);
					while (toUpdate != -1) {
						before = oldRPG.containers[itSupports][toUpdate];
						if (--oldRPG.containers[itSupports][toUpdate] == 0 && before > 0) {
							toUpdate = PA->getContainerUpdates(toUpdate);
						} else {
							toUpdate = -1;
						}
					}						
					
					if (oldRPG.parentAchieved[itSupports] && oldRPG.containers[itSupports][0] == 0) {
						actionsIn++;
						actionsInLayer++;

						if (problemInformation.verbose)cout << "Action " << itSupports << " is now achievable" << endl;
						//actions.push_back(oldRPG.applicableReal[itSupports]);
						aLoopLimit = oldRPG.achieveArray[itSupports][0];
						if (problemInformation.verbose)cout << "Action has " << aLoopLimit << " add effects" << endl;
						for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
							itAchieves = oldRPG.achieveArray[itSupports][aLoopVar];
							if (problemInformation.verbose) cout << "Add Effect: Predicate " << aLoopVar << ": " << *(oldRPG.achievedReal[itAchieves]) << endl;
							
							if (!oldRPG.achieved[itAchieves]) {
								if (problemInformation.verbose) cout << " -- Previously not present" << endl;
								oldRPG.achievedAt[itAchieves] = levelCount;
								oldRPG.firstAchiever[itAchieves] = itSupports;
								oldRPG.achieved[itAchieves] = true;
								if (oldRPG.predicateInstanceIsGoal[itAchieves]) unachievedGoals--;
								//predicates->addPredicateInstance(oldRPG.achievedReal[itAchieves]);
								nextToProcess->push_back(itAchieves);
							}else { 
								if (problemInformation.verbose) cout << " -- Previously present" << endl;
							}
						}
						aLoopLimit = oldRPG.negativeAchieveArray[itSupports][0];
						if (problemInformation.verbose)cout << "Action has " << aLoopLimit << " delete effects" << endl;
						for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
							itAchieves = oldRPG.negativeAchieveArray[itSupports][aLoopVar];
							if (problemInformation.verbose) cout << "Delete Effect: Predicate " << aLoopVar << ": " << *(oldRPG.achievedReal[itAchieves]) << endl;
							
							if (!oldRPG.negativeAchieved[itAchieves]) {
								if (problemInformation.verbose) cout << " -- Previously not present" << endl;
								oldRPG.negativeAchievedAt[itAchieves] = levelCount;
								oldRPG.firstNegativeAchiever[itAchieves] = itSupports;
								oldRPG.negativeAchieved[itAchieves] = true;
								if (oldRPG.predicateInstanceIsNegativeGoal[itAchieves]) unachievedGoals--;
								//predicates->addPredicateInstance(oldRPG.achievedReal[itAchieves]);
								nextNegativeToProcess->push_back(itAchieves);
							}else { 
								if (problemInformation.verbose) cout << " -- Previously present" << endl;
							}
						}
						
						
						
						aLoopLimit = oldRPG.subActions[itSupports][0];
						for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
							caQueue.push_front(oldRPG.subActions[itSupports][aLoopVar]);
						}
						
						while (!caQueue.empty()) {
							caqEntry = caQueue.front();
							caQueue.pop_front();
							if (!oldRPG.parentAchieved[caqEntry]/* && oldRPG.keepAction[caqEntry]*/) {
							
								oldRPG.parentAchieved[caqEntry] = true;
								
								
								
								if (oldRPG.containers[caqEntry][0] <= 0) {
									aLoopLimit = oldRPG.achieveArray[caqEntry][0];
									if (problemInformation.verbose)cout << "Action has " << aLoopLimit << " add effects" << endl;
									
									for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
										itAchieves = oldRPG.achieveArray[caqEntry][aLoopVar];
										if (problemInformation.verbose) cout << "Add Effect: Predicate " << aLoopVar << ": " << *(oldRPG.achievedReal[itAchieves]) << endl;
										
										if (!oldRPG.achieved[itAchieves]) {
											if (problemInformation.verbose) cout << " -- Previously not present" << endl;
											oldRPG.achievedAt[itAchieves] = levelCount;
											oldRPG.firstAchiever[itAchieves] = caqEntry;
											oldRPG.achieved[itAchieves] = true;
											if (oldRPG.predicateInstanceIsGoal[itAchieves]) unachievedGoals--;
											//predicates->addPredicateInstance(oldRPG.achievedReal[itAchieves]);
											nextToProcess->push_back(itAchieves);
										}else { 
											if (problemInformation.verbose) cout << " -- Previously present" << endl;
										}
									}
									aLoopLimit = oldRPG.negativeAchieveArray[caqEntry][0];
									if (problemInformation.verbose)cout << "Action has " << aLoopLimit << " delete effects" << endl;
									for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
										itAchieves = oldRPG.negativeAchieveArray[caqEntry][aLoopVar];
										if (problemInformation.verbose) cout << "Delete Effect: Predicate " << aLoopVar << ": " << *(oldRPG.achievedReal[itAchieves]) << endl;
										
										if (!oldRPG.negativeAchieved[itAchieves]) {
											if (problemInformation.verbose) cout << " -- Previously not present" << endl;
											oldRPG.negativeAchievedAt[itAchieves] = levelCount;
											oldRPG.firstNegativeAchiever[itAchieves] = caqEntry;
											oldRPG.negativeAchieved[itAchieves] = true;
											if (oldRPG.predicateInstanceIsNegativeGoal[itAchieves]) unachievedGoals--;
											//predicates->addPredicateInstance(oldRPG.achievedReal[itAchieves]);
											nextNegativeToProcess->push_back(itAchieves);
										}else { 
											if (problemInformation.verbose) cout << " -- Previously present" << endl;
										}
									}
								
								
									aLoopLimit = oldRPG.subActions[caqEntry][0];
									for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
										caQueue.push_front(oldRPG.subActions[caqEntry][aLoopVar]);
									}
								}
							}
						}
					
						
					} else {
						//cout << "Action has " << oldRPG.containers[itSupports][0] << " in its root container: ";
						//oldRPG.applicableReal[itSupports]->briefWrite(cout);
						
					}
				}
			}
		}
		
		spItr = negativeToProcess->begin();
		spEnd = negativeToProcess->end();
		
		for (; spItr != spEnd; spItr++) {
			loopLimit = oldRPG.negativeSupportArray[*spItr][0];
			if (problemInformation.verbose)cout << "Predicate negatively supports " << (loopLimit / 2) << " actions" << endl;
			for (loopVar = 0; loopVar < loopLimit; loopVar+=2) {
				itSupports = oldRPG.negativeSupportArray[*spItr][loopVar + 1];
				
				if (oldRPG.containers[itSupports][0] > 0) { // if action not achieved already
					const Action* PA = oldRPG.applicableReal[itSupports]->getParentAction();
					toUpdate = PA->getNegativePreconditionUpdates(oldRPG.negativeSupportArray[*spItr][loopVar + 2]);
					while (toUpdate != -1) {
						before = oldRPG.containers[itSupports][toUpdate];
						if (--oldRPG.containers[itSupports][toUpdate] == 0 && before > 0) {
							toUpdate = PA->getContainerUpdates(toUpdate);
						} else {
							toUpdate = -1;
						}
					}						
					
					if (oldRPG.containers[itSupports][0] <= 0) {
						actionsIn++;
						actionsInLayer++;
						if (problemInformation.verbose)cout << "Action " << itSupports << " is now achievable" << endl;
						//actions.push_back(oldRPG.applicableReal[itSupports]);
						aLoopLimit = oldRPG.achieveArray[itSupports][0];
						if (problemInformation.verbose)cout << "Action has " << aLoopLimit << " add effects" << endl;
						for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
							itAchieves = oldRPG.achieveArray[itSupports][aLoopVar];
							if (problemInformation.verbose) cout << "Add Effect: Predicate " << aLoopVar << ": " << *(oldRPG.achievedReal[itAchieves]) << endl;
							
							if (!oldRPG.achieved[itAchieves]) {
								if (problemInformation.verbose) cout << " -- Previously not present" << endl;
								oldRPG.achievedAt[itAchieves] = levelCount;
								oldRPG.firstAchiever[itAchieves] = itSupports;
								oldRPG.achieved[itAchieves] = true;
								if (oldRPG.predicateInstanceIsGoal[itAchieves]) unachievedGoals--;
								//predicates->addPredicateInstance(oldRPG.achievedReal[itAchieves]);
								nextToProcess->push_back(itAchieves);
							}else { 
								if (problemInformation.verbose) cout << " -- Previously present" << endl;
							}
						}
						aLoopLimit = oldRPG.negativeAchieveArray[itSupports][0];
						if (problemInformation.verbose)cout << "Action has " << aLoopLimit << " delete effects" << endl;
						for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
							itAchieves = oldRPG.negativeAchieveArray[itSupports][aLoopVar];
							if (problemInformation.verbose) cout << "Delete Effect: Predicate " << aLoopVar << ": " << *(oldRPG.achievedReal[itAchieves]) << endl;
							
							if (!oldRPG.negativeAchieved[itAchieves]) {
								if (problemInformation.verbose) cout << " -- Previously not present" << endl;
								oldRPG.negativeAchievedAt[itAchieves] = levelCount;
								oldRPG.firstNegativeAchiever[itAchieves] = itSupports;
								oldRPG.negativeAchieved[itAchieves] = true;
								if (oldRPG.predicateInstanceIsNegativeGoal[itAchieves]) unachievedGoals--;
								//predicates->addPredicateInstance(oldRPG.achievedReal[itAchieves]);
								nextNegativeToProcess->push_back(itAchieves);
							}else { 
								if (problemInformation.verbose) cout << " -- Previously present" << endl;
							}
						}
						
						
						
						
						aLoopLimit = oldRPG.subActions[itSupports][0];
						for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
							caQueue.push_front(oldRPG.subActions[itSupports][aLoopVar]);
						}
						
						while (!caQueue.empty()) {
							caqEntry = caQueue.front();
							caQueue.pop_front();
							if (!oldRPG.parentAchieved[caqEntry]/* && oldRPG.keepAction[caqEntry]*/) {
							
								oldRPG.parentAchieved[caqEntry] = true;
								
								
								
								if (oldRPG.containers[caqEntry][0] <= 0) {
									aLoopLimit = oldRPG.achieveArray[caqEntry][0];
									if (problemInformation.verbose)cout << "Action has " << aLoopLimit << " add effects" << endl;
									
									for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
										itAchieves = oldRPG.achieveArray[caqEntry][aLoopVar];
										if (problemInformation.verbose) cout << "Add Effect: Predicate " << aLoopVar << ": " << *(oldRPG.achievedReal[itAchieves]) << endl;
										
										if (!oldRPG.achieved[itAchieves]) {
											if (problemInformation.verbose) cout << " -- Previously not present" << endl;
											oldRPG.achievedAt[itAchieves] = levelCount;
											oldRPG.firstAchiever[itAchieves] = caqEntry;
											oldRPG.achieved[itAchieves] = true;
											if (oldRPG.predicateInstanceIsGoal[itAchieves]) unachievedGoals--;
											//predicates->addPredicateInstance(oldRPG.achievedReal[itAchieves]);
											nextToProcess->push_back(itAchieves);
										}else { 
											if (problemInformation.verbose) cout << " -- Previously present" << endl;
										}
									}
									aLoopLimit = oldRPG.negativeAchieveArray[caqEntry][0];
									if (problemInformation.verbose)cout << "Action has " << aLoopLimit << " delete effects" << endl;
									for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
										itAchieves = oldRPG.negativeAchieveArray[caqEntry][aLoopVar];
										if (problemInformation.verbose) cout << "Delete Effect: Predicate " << aLoopVar << ": " << *(oldRPG.achievedReal[itAchieves]) << endl;
										
										if (!oldRPG.negativeAchieved[itAchieves]) {
											if (problemInformation.verbose) cout << " -- Previously not present" << endl;
											oldRPG.negativeAchievedAt[itAchieves] = levelCount;
											oldRPG.firstNegativeAchiever[itAchieves] = caqEntry;
											oldRPG.negativeAchieved[itAchieves] = true;
											if (oldRPG.predicateInstanceIsNegativeGoal[itAchieves]) unachievedGoals--;
											//predicates->addPredicateInstance(oldRPG.achievedReal[itAchieves]);
											nextNegativeToProcess->push_back(itAchieves);
										}else { 
											if (problemInformation.verbose) cout << " -- Previously present" << endl;
										}
									}
								
								
									aLoopLimit = oldRPG.subActions[caqEntry][0];
									for (aLoopVar = 1; aLoopVar <= aLoopLimit; aLoopVar++) {
										caQueue.push_front(oldRPG.subActions[caqEntry][aLoopVar]);
									}
								}
							}
						}
					
					}
				}
			}
			
			
		}
		
		delete negativeToProcess;
		delete toProcess;
		
		toProcess = nextToProcess;
		negativeToProcess = nextNegativeToProcess;
		
		findingNew = !toProcess->empty() || !negativeToProcess->empty();
		
		//predLyrEnd = predicates->end();
		//predLyrEnd--;
		
		//tempLevelEnds.push_back(predLyrEnd);
		
		//ActionInstanceList::iterator actEnd = actions.end();
		//actEnd--;
			
		//tempActionLevelEnds.push_back(actEnd);
		//cout << "Actions in layer " << levelCount << " = " << actionsInLayer << endl;
		
		levelCount++;
		
		
		//problemInformation.verbose = false;
		
		//cout << unachievedGoals << " goals remaining" << endl;
		
	}
	
	delete toProcess;
	delete negativeToProcess;
	
	//problemInformation.verbose = true;
	//cout << "\nActions in incremental RPG = " << actionsIn << endl;

		/*	
	if (!goalsEarly) { // need more layers
		if (problemInformation.verbose) {
			cout << "Need extra time in planning graph" << endl;
			
		}
		problemInformation.verbose = false;
		while(!predicates->subsumes(problemInformation.GoalPredicates, &problemInformation))  {
	
			
	
	
			list<ActionInstance *> applicable = predicates->applicableRelaxedActions(&problemInformation);
	
			
	
			predicates->applyRelaxedActionsLocally(applicable, &problemInformation, false, true);
	
			if (predicates->achievingActions.empty()) {
				cout << "No actions did anything" << endl;
				exit(0);
			}
			
			list<ActionInstance*>::iterator itr = applicable.begin();
	
			
	
			for(; itr != applicable.end(); itr++) {
				bool found = false;
				list<ActionInstance*>::iterator actItr = actions.begin();
				for (; !found && actItr != actions.end(); actItr++) {
					found = (*(*actItr) == *(*itr));
				}
				if (!found) {
			
					actions.push_back(*itr);
				}
			}
	
			
			predLyrEnd = predicates->end();
			predLyrEnd--;
			
			tempLevelEnds.push_back(predLyrEnd);
			
			list<ActionInstance*>::iterator actEnd = actions.end();
			actEnd--;
				
			tempActionLevelEnds.push_back(actEnd);
	
			levelCount++;
			
	
		}
	}
	*/
	
	
	if (findingNew == false) {
		
		goalsAppeared = false;
		
		if (problemInformation.verbose){
			cout << "Goals didn't appear" << endl;
			for (int i = 0; i < problemInformation.goalCount; ++i) {
				if (!oldRPG.achieved[oldRPG.goalIDs[i]]) {
					cout << "Goal " << *(oldRPG.achievedReal[oldRPG.goalIDs[i]]) << " did not appear" << endl;
				}
			}
			for (int i = 0; i < problemInformation.negativeGoalCount; ++i) {
				if (!oldRPG.negativeInitially[oldRPG.negativeGoalIDs[i]] && !oldRPG.negativeAchieved[oldRPG.negativeGoalIDs[i]]) {
					cout << "Goal �(" << *(oldRPG.achievedReal[oldRPG.negativeGoalIDs[i]]) << ") did not appear" << endl;
					
				}
			}
		}
		
	 
	} else {
		goalsAppeared = true;
		if (problemInformation.verbose) cout << "Goals appeared!" << endl;
	}
	
	levelEnds = 0;
	actionLevelEnds = 0;
	
	//cout << "New graph has " << levelCount << " layers; old one had " << oldRPG.levelCount << endl;
	/*
		list<State::iterator>::iterator itr = tempLevelEnds.begin();

	levelEnds = new State::iterator[tempLevelEnds.size()];

	for(int count = 0; itr != tempLevelEnds.end(); itr++, count++){

		State::iterator levelEnd = (*itr);
		levelEnd++; 
		levelEnds[count] = levelEnd;
	}


	list< ActionInstanceList::iterator >::iterator eitr = tempActionLevelEnds.begin();

	actionLevelEnds = new ActionInstanceList::iterator[tempActionLevelEnds.size()];

	for(int count = 0; eitr != tempActionLevelEnds.end(); eitr++, count++){
		ActionInstanceList::iterator actionLevelEnd = (*eitr);
		actionLevelEnd++;
		actionLevelEnds[count] = actionLevelEnd;
	}

	*/	
	
	predicates = 0;
	
}
/*
ActionInstanceList RelaxedPlanningGraph::extraActions(StaticInformation * staticInf, const PredicateInstanceList & extras) {

	ActionInstanceList toReturn;
	bool needToCheckStatic = staticInf->needToCheckStatic;
	
	for (int i = 0; i < staticInf->numberOfSimpleActions; i++) {
	
		const Action* a = staticInf->Actions[i];
		
		if (staticInf->verbose) {
		
			cout << "  Attempting to apply actions of type " << a->name() << endl;
			
		}
		
		for (int predCheckExtra = 0; predCheckExtra < a->preconditionCount; predCheckExtra++) {
		
			PredicateInstanceList::const_iterator extraItr = extras.begin();
		
			
			
			
			for (; extraItr != extras.end(); extraItr++) {
		
				PredicateInstance* currPI = (*extraItr);
			
				
				if (currPI->isOfType(a->preconditions[predCheckExtra])) {
					if (staticInf->verbose) {
		
						cout << "  Based on predicate " << *currPI << "   for precondition " << predCheckExtra << endl;
				
					}
					
					slist<int> argNum;
					slist<Entity*> argVal;
					
					slist<EntitySet** > domains;
					
					
					
					Entity** argAssignments = new Entity*[a->parameterCount];
					
					bool* argPreset = new bool[a->parameterCount];
						
					int loopLimit = a->parameterCount - 1;
					
					for (int j = 0; j < a->parameterCount; j++) {
						argAssignments[j] = 0;
						argPreset[j] = false;
					}
					
					for (int j = 0; j < currPI->getParentPredicate()->getArgumentCount(); j++) {

					
											
				
						int setParameter = a->preconditionArguments[predCheckExtra][j];
					
						argAssignments[setParameter] = currPI->getParameter(j)->boundTo();
						argPreset[setParameter] = true;
						if (staticInf->verbose) {
							cout << "Parameter " << setParameter << " forced to " << *(currPI->getParameter(j)->boundTo()) << endl;
						}
					}
				
					
					bool doSearch = true;
							
					EntitySet** dummy = new EntitySet*[a->parameterCount];
					
					for (int j = 0; j < a->parameterCount; j++) {
					
						
						if (doSearch && argPreset[j]) {
							
							dummy[j] = new EntitySet();
							Entity* currEnt = argAssignments[j];
							const EntitySet* domArray = a->getParameterDomain(j);
							bool inDomain = (domArray->find(currEnt) != domArray->end());
							
							
							if (inDomain) {
								
								dummy[j]->insert(argAssignments[j]);
							} else {
								doSearch = false;
								if (staticInf->verbose) {
									cout << " Domain of parameter " << j << " does not contain " << currEnt->name() << endl;
									
								}
							}
							
						} else if (doSearch) {
							dummy[j] = new EntitySet(*(a->getParameterDomain(j)));
							
							
						} else {
							dummy[j] = new EntitySet();
						}
					}
					
					list<int> toJPDCfrom;
					
					for (int j = 0; doSearch && j < a->parameterCount; j++) {
					
						if (argPreset[j]) toJPDCfrom.push_back(j);
						
					}
					
					while (doSearch && !toJPDCfrom.empty()) {
					
						int j = toJPDCfrom.front();
						toJPDCfrom.pop_front();
					
						
						if (staticInf->verbose) cout << "Applying JPDCs from parameter " << j << endl;
							
						EntitySet** currJPDC = 0;
											
						a->getDomainConstraints(j, argAssignments[j], currJPDC);
						
						for (int k = 0; doSearch && k < a->parameterCount; k++) {
							
							if (k != j ) {
								
								
								
								EntitySet* newES = Action::intersection(currJPDC[k], dummy[k]);
								delete dummy[k];
								dummy[k] = newES;
								
								doSearch = !(dummy[k]->empty());
								if (!doSearch && staticInf->verbose) cout << "     Domain of parameter " << k << " is now empty" << endl;
								if (!argPreset[k]) {
									EntitySet::iterator checkNotSizeOneItr = dummy[k]->begin();
									int domSize = 0;
									while (domSize != 2 && checkNotSizeOneItr != dummy[k]->end()) {
										domSize++;
										checkNotSizeOneItr++;
									}
									if (domSize == 1) {
										Entity* currEnt = *(dummy[k]->begin());
										if (staticInf->verbose) {
											cout << "Parameter " << k << " forced to " << *currEnt << endl;
										}				
										argAssignments[k] = currEnt;
										argPreset[k] = true; // decided domain
										toJPDCfrom.push_back(k);
									}
								}
								
							}
						}
					
					
					}
					
					if (doSearch) {
						domains.push_front(dummy);
						int firstUnAss = 0;
						
						while (firstUnAss < a->parameterCount && argPreset[firstUnAss]) firstUnAss++;
						if (firstUnAss == a->parameterCount) {
							if (staticInf->verbose) {
								cout << "Decidable action - no unbound parameters" << endl;
								cout << a->name();
								for (int printEnts = 0; printEnts < a->parameterCount; printEnts++) {
									cout << " " << argAssignments[printEnts]->name();
								}
								cout << endl;
							}
							doSearch = false;
							bool applicable = true;
							for (int checkPre = 0; applicable && checkPre < a->preconditionCount; checkPre++) {
								const Predicate* currPred = a->preconditions[checkPre];
								int currArgC = currPred->getArgumentCount();
								Entity** ents = new Entity*[currArgC];
								for (int fillEnts = 0; fillEnts < currArgC; fillEnts++) {
									ents[fillEnts] = argAssignments[a->preconditionArguments[checkPre][fillEnts]];
								}
								if (currPred->isStatic()) {
									applicable = staticInf->StaticInitialState->contains(currPred, ents);
								} else {
									applicable = predicates->contains(currPred, ents);
								}
								delete [] ents;
								
							}
							if (applicable) {
								
								if (staticInf->verbose) {
									cout << "Action deemed applicable" << endl;
									
								}
								
								
								ActionInstance* g = new ActionInstance(a, argAssignments);
								if (staticInf->verbose) g->briefWrite(cout);
								if (false) {
									for (int j = 0; j < g->getPreconditionCount(); j++) {
										Entity** sf = new Entity*[g->getPreconditionArgumentCount(j)];
										for (int k = 0; k < g->getPreconditionArgumentCount(j); k++) {
											sf[k] = g->getPreconditionArgument(j,k)->boundTo();
										}
										cout << "Precondition " << j << " is at ";
										if (g->getPreconditionType(j)->isStatic()) {
											cout << staticInf->StaticInitialState->getPredicateInstance(g->getPreconditionType(j), sf);
										} else {
											cout << predicates->getPredicateInstance(g->getPreconditionType(j), sf);
											
										}
										cout << endl;
										delete sf;
									}
								}
								if (staticInf->verbose) {
									cout << "Created ActionInstance" << endl;
								}
								bool duplicateAction = false;
								
								ActionInstanceList::iterator checkDuplItr = toReturn.begin();
								for (;!duplicateAction && checkDuplItr != toReturn.end(); checkDuplItr++) {
									duplicateAction = (*(*checkDuplItr) == *g);
								}
								
								checkDuplItr = actions.begin();
								for (;!duplicateAction && checkDuplItr != actions.end(); checkDuplItr++) {
									duplicateAction = (*(*checkDuplItr) == *g);
								}
								
								if (!duplicateAction) {
									toReturn.push_back(g);
									if (staticInf->verbose) {
										cout << "Action placed on list to return" << endl;
									
									}
								} else {
									if (staticInf->verbose) {
										cout << "Action is duplicate" << endl;
									}
									delete g;
								}
								
							}
							
						} else {
							argNum.push_front(firstUnAss - 1);
							argVal.push_front(0);
							if (staticInf->verbose) {
								cout << "Searching for bindings for action " << endl;
								cout << a->name();
								for (int printEnts = 0; printEnts < a->parameterCount; printEnts++) {
									Entity* currEnt = argAssignments[printEnts]; 
									if (currEnt == 0) {
										cout << " <>";
									} else {
										cout << " " << currEnt->name();
									}
								}
								cout << endl; 
							}
						}
					} 
					
					if (!doSearch) {
						for (int j = 0; j < a->parameterCount; j++) {
					
							delete dummy[j];
						}
						delete [] dummy;
					}
					
							
					while (!argNum.empty()) {
					
						if (staticInf->verbose) cout << "Recursing..." << endl;
						
						int currArg = argNum.front();
						Entity* currVal = argVal.front();
						EntitySet** currDomainArray = domains.front();
						
						
						argNum.pop_front();
						argVal.pop_front();
						domains.pop_front();
				
						
						if (currArg != -1 && !argPreset[currArg]) {
							
							argAssignments[currArg] = currVal;
						}
						
						
						
						if (currArg == loopLimit) {  // bottom of DFS tree: action entirely ground
						
							
							
							if (staticInf->verbose) {
								cout << "Action deemed applicable" << endl;
								cout << "Woo" << endl;
							}
							
							
							ActionInstance* g = new ActionInstance(a, argAssignments);
							if (false) {
								for (int j = 0; j < g->getPreconditionCount(); j++) {
									Entity** sf = new Entity*[g->getPreconditionArgumentCount(j)];
									for (int k = 0; k < g->getPreconditionArgumentCount(j); k++) {
										sf[k] = g->getPreconditionArgument(j,k)->boundTo();
									}
									cout << "Precondition " << j << " is at ";
									if (g->getPreconditionType(j)->isStatic()) {
										cout << staticInf->StaticInitialState->getPredicateInstance(g->getPreconditionType(j), sf);
									} else {
										cout << predicates->getPredicateInstance(g->getPreconditionType(j), sf);
										
									}
									cout << endl;
									delete sf;
								}
							}
							bool duplicateAction = false;
							
							ActionInstanceList::iterator checkDuplItr = toReturn.begin();
							for (;!duplicateAction && checkDuplItr != toReturn.end(); checkDuplItr++) {
								duplicateAction = (*(*checkDuplItr) == *g);
							}
							checkDuplItr = actions.begin();
							for (;!duplicateAction && checkDuplItr != actions.end(); checkDuplItr++) {
								duplicateAction = (*(*checkDuplItr) == *g);
							}
							
							if (!duplicateAction) {
							
								toReturn.push_back(g);
								if (staticInf->verbose) {
									cout << "Action placed on list to return" << endl;
								
								}
							} else {
								if (staticInf->verbose) {
									cout << "Action is duplicate" << endl;
								}
								delete g;
							}
							
							
							
							if (!argPreset[currArg]) argAssignments[currArg] = 0;
							
						
						} else {
						
							bool domainFallenEmpty = false;
							int nextArg = currArg + 1;
							if (staticInf->verbose) {
								cout << "Expanding and pruning" << endl;
							}
							
							if (currArg != -1 && !argPreset[currArg]) {
							
								if (staticInf->verbose) cout << "Applying JPDCs" << endl;
								
								EntitySet** currJPDC = 0;
								
								
								a->getDomainConstraints(currArg, currVal, currJPDC);
								
								for (int k = nextArg; k < a->parameterCount; k++) {
						
									EntitySet* newES = Action::intersection(currJPDC[k], currDomainArray[k]);
									delete currDomainArray[k];
									currDomainArray[k] = newES;
									
									
								}
								
							}
							
							
							
							EntitySet** newDomainArray = new EntitySet*[a->parameterCount];
							
							for (int k = 0; k < nextArg; k++) {
								//newDomainArray[k] = currDomainArray[k];
								newDomainArray[k] = 0;
							}
							for (int k = nextArg; k < a->parameterCount; k++) {
								newDomainArray[k] = 0;
								if (!argPreset[k]) argAssignments[k] = 0;
							}
							
							
							
											
							for (int tmpArg = nextArg; !domainFallenEmpty && tmpArg < a->parameterCount; tmpArg++) {
							
								
								EntitySet* domainAsList = new EntitySet();
							
								EntitySet::iterator oldDomainItr = currDomainArray[tmpArg]->begin();
												
								for (; oldDomainItr != currDomainArray[tmpArg]->end(); oldDomainItr++) {
								
									Entity* tempVal = (*oldDomainItr);
									
									
									
									argAssignments[tmpArg] = tempVal;
									
									
									
									bool applicable = true;
												
									if (false && staticInf->verbose) {
									
										cout << "   Attempting to apply " << a->name();
										for (int k = 0; k < a->parameterCount; k++) {
											Entity* tmp = argAssignments[k];
											
											if (tmp == 0) {
												cout << " <>";
											} else {
												cout << " " << tmp->name();
												
											}	
										
										}
										
										cout << endl;
								
									}
									
									for (int k = 0; applicable && k < a->preconditionCount; k++) {
					
										bool notFoundYet = true;
								
										const Predicate* p = a->preconditions[k];
							
										
										//State::const_iterator pitr;
										//State::const_iterator pitrEnd;
										
										bool pStatic = p->isStatic();
										if (!pStatic || p->getArgumentCount() > 2  || needToCheckStatic) {
											Entity** parameters = new Entity*[p->getArgumentCount()];
											for (int m = 0; m < p->getArgumentCount(); m++) {
												Entity* tmp = argAssignments[a->preconditionArguments[k][m]];
												parameters[m] = tmp;
											}
								
																			
											if (false && staticInf->verbose) {
												cout << "    Searching for match for precondition " << k << ": ";
												cout << p->name();
												for (int m = 0; m < p->getArgumentCount(); m++) {
													
													Entity* tmp = parameters[m];
													if (tmp == 0) {
														cout << " <>";
													} else {
														
														cout << " " << tmp->name();
													}
													
												}
												cout << endl;
											}
													
											State * toProbe = predicates;
											
											if (pStatic) {
												toProbe = staticInf->StaticInitialState;
											}
											
											
											applicable = toProbe->contains(p, parameters);
			
											delete [] parameters;	
											
											
								
												
										}
										
										
									}
									
									if (applicable) {
										
										domainAsList->insert(tempVal);
										
									} else {
										
									}
								
								
								}
							
								if (!argPreset[tmpArg]) argAssignments[tmpArg] = 0;
							
								domainFallenEmpty = domainAsList->empty();
								newDomainArray[tmpArg] = domainAsList;
								
								
								
								
								
							
							}
							
					
							
							if (!domainFallenEmpty) {
								EntitySet::iterator dvalItr = newDomainArray[nextArg]->begin();
								
								
								for (; dvalItr != newDomainArray[nextArg]->end(); dvalItr++) {
								
									argNum.push_front(nextArg);
									argVal.push_front((*dvalItr));
									
									EntitySet** tmpDomainArray = new EntitySet*[a->parameterCount];
									
									
									
									for (int k = 0; k < a->parameterCount; k++) {
										if (newDomainArray[k] != 0) {
											tmpDomainArray[k] = new EntitySet(*(newDomainArray[k]));
											
										} else {
											tmpDomainArray[k] = 0;
										}
									}
									
									domains.push_front(tmpDomainArray);
									
									
									
								
								}
								
								
								
							} 
							
							for (int k = nextArg; k < a->parameterCount; k++) {
								
								if (newDomainArray[k] != 0) delete newDomainArray[k];
							}
							
							delete [] newDomainArray;
								
							
						
						}
						
						
						
						for (int k = 0; k < a->parameterCount; k++) {
							
							if (currDomainArray[k] != 0) delete currDomainArray[k];
							
						}
			
						
						
						delete [] currDomainArray;
						
								
						
					
					}
				
					
				
				
					delete [] argAssignments;
					
					delete [] argPreset;
					
				}
		
			}
		}
		
		
	};
	
	
	return toReturn;


}
*/

void RelaxedPlanningGraph::applyPositiveAndNegative(ActionInstance* & a, State* & posState,  State* & negState, State* & allPredicates, PredicateInstanceList* & newPositive, PredicateInstanceList* & newNegative) {

	
	PredicateInstance** addEffects = a->internalAddEffects();
	
	for (int i = 0; i < a->getAddEffectCount(); i++) {
		//cout << "8888888 Processing add effect " << *(addEffects[i]) << endl;
		PredicateInstance* lookup = 0;
		lookup = allPredicates->getPredicateInstance(addEffects[i]);
		if (lookup == 0) {
			//cout << "8888888    Is a brand-new predicate" << endl;
			allPredicates->addPredicateInstance(addEffects[i]);
			lookup = addEffects[i];
		} else {
			//cout << "8888888    Has been seen before" << endl;
			a->replaceCachedAddEffect(i, lookup);
		}
		
		PredicateInstance* lookupTwo = 0;
		lookupTwo = posState->getPredicateInstance(lookup);
		
		if (lookupTwo == 0) {
			posState->addPredicateInstance(lookup);
			//cout << "8888888    On the to-process list" << endl;
			newPositive->push_back(lookup);
		}
		
		
	}
	
	
	
	PredicateInstance** delEffects = a->internalDeleteEffects();
	
	for (int i = 0; i < a->getDeleteEffectCount(); i++) {
		//cout << "8888888 Processing del effect " << *(delEffects[i]) << endl;
		PredicateInstance* lookup = 0;
		lookup = allPredicates->getPredicateInstance(delEffects[i]);
		if (lookup == 0) {
			//cout << "8888888    Is a brand-new predicate" << endl;
			allPredicates->addPredicateInstance(delEffects[i]);
			lookup = delEffects[i];
		} else {
			//cout << "8888888    Has been seen before" << endl;
			a->replaceCachedDeleteEffect(i, lookup);
		}
		
		PredicateInstance* lookupTwo = 0;
		lookupTwo = negState->getPredicateInstance(lookup);
		
		if (lookupTwo != 0) {
			
			negState->removePredicateInstance(lookupTwo);
			newNegative->push_back(lookupTwo);
			
			//cout << "8888888    On the negative-to-process list" << endl;
		}
		
		
	}
	
	
	
	

};

void RelaxedPlanningGraph::calculateRPG(StaticInformation & problemInformation, State * initialState, const bool & stopWhenGoalsAppear){
	//cout << "Initial state: " << *(initialState) << endl;
	
//	problemInformation.verbose = false;
	
	ActionApplicabilityTester aaTester(&problemInformation);
	
	dependencyMap = aaTester.getADs();

	predicates = initialState->copy();
	
	allPredicates = predicates->copy();
	
	if (predicates->hasQueuedEffects()) predicates->advanceTime(); //quick hack
		
	State* negState = predicates->copy(); // gets delete effects applied

	if (true) {
		State::iterator nextItr;
		State::iterator itr = problemInformation.GoalPredicates->begin();
		State::iterator itrEnd = problemInformation.GoalPredicates->end();
		
		while (itr != itrEnd) {
		
			nextItr = itr;
			++nextItr;
			
			PredicateInstance* lookup = 0;
			lookup = allPredicates->getPredicateInstance(*itr);
			if (lookup == 0) {
				//cout << "8888888    Is a brand-new predicate" << endl;
				allPredicates->addPredicateInstance(*itr);
			} else {
				//cout << "8888888    Has been seen before" << endl;
				problemInformation.GoalPredicates->switchInPredicateInstance(itr, lookup);
			}
			
			itr = nextItr;
		}
		
		
		itr = problemInformation.NegativeGoalPredicates->begin();
		itrEnd = problemInformation.NegativeGoalPredicates->end();
		
		while (itr != itrEnd) {
		
			nextItr = itr;
			++itr;
			
			PredicateInstance* lookup = 0;
			lookup = allPredicates->getPredicateInstance(*itr);
			if (lookup == 0) {
				//cout << "8888888    Is a brand-new predicate" << endl;
				allPredicates->addPredicateInstance(*itr);
			} else {
				//cout << "8888888    Has been seen before" << endl;
				problemInformation.NegativeGoalPredicates->switchInPredicateInstance(itr, lookup);
			}
		
			itr = nextItr;
		}
	}
	
	PredicateInstanceList* toProcess = predicates->copyPredicates();
	PredicateInstanceList* negativeToProcess = new PredicateInstanceList();
	
	list<State::iterator> tempLevelBoundaries;

	

	levelCount = 1;

	list<State::iterator> tempLevelEnds;

	list< ActionInstanceList::iterator > tempActionLevelEnds;

	State::iterator predLyrEnd = predicates->end();
	predLyrEnd--;

	tempLevelEnds.push_back(predLyrEnd);


	bool actionsAppearing = true;
	bool didloop = false;
	if (stopWhenGoalsAppear) cout << "Stopping when goals appear" << endl;
	while(   (stopWhenGoalsAppear && (!predicates->subsumes(problemInformation.GoalPredicates, &problemInformation) || negState->intersectsWith(problemInformation.NegativeGoalPredicates, &problemInformation)))
              || (!stopWhenGoalsAppear && actionsAppearing) )  {
		didloop = true;
		cout << "+";
		cout.flush();

		//cout << "Next Layer" << endl;
		
		aaTester.makeNextActions(predicates, negState);
		
		PredicateInstanceList* newToProcess = new PredicateInstanceList();
		PredicateInstanceList* newNegToProcess = new PredicateInstanceList();
		
		actionsAppearing = false;
		
		ActionInstanceList::iterator itrEnd = aaTester.getNewActions().end();
		
		//cout << aaTester.getNewActions().size() << " actions in layer" << endl;
		
		for (ActionInstanceList::iterator itr = aaTester.getNewActions().begin();
			itr != itrEnd;
			++itr) {
			
			//cout << "8888888  Applying action ";
			//(*itr)->briefWrite(cout);
			
			//(*itr)->briefWrite(cout);
			applyPositiveAndNegative(*itr, predicates, negState, allPredicates, newToProcess, newNegToProcess);
			
			actions.push_back(*itr);
			actionsAppearing = true;
			
		}
		
		
		delete toProcess;
		delete negativeToProcess;
		
		toProcess = newToProcess;
		negativeToProcess = newNegToProcess;
				
		if (!actionsAppearing && stopWhenGoalsAppear) {
			cout << "No actions did anything" << endl;
			levelEnds = 0;
			actionLevelEnds = 0;
			delete toProcess;
			delete negativeToProcess;
			goalsAppeared = false;
			return;
		}
		
		

		if (stopWhenGoalsAppear || (!stopWhenGoalsAppear && actionsAppearing)) { // ignore empty layers
			predLyrEnd = predicates->end();
			predLyrEnd--;
		
			tempLevelEnds.push_back(predLyrEnd);
		
			ActionInstanceList::iterator actEnd = actions.end();
			actEnd--;
			
			tempActionLevelEnds.push_back(actEnd);

			levelCount++;
		}

	}

	cout << endl;
	
	delete toProcess;
	delete negativeToProcess;
	
	
	if (didloop) {
		cout << "Goal predicates not in initial fact layer" << endl;
		goalsAppeared = predicates->subsumes(problemInformation.GoalPredicates, &problemInformation) && !negState->intersectsWith(problemInformation.NegativeGoalPredicates, &problemInformation);
	} else {
		cout << "Goal predicates in initial fact layer" << endl;
		goalsAppeared = true;
	}
	
//	cout << "GoalsAppeared = ";
	if (goalsAppeared) {
//		cout << "yes";
	} else {
//		cout << "no";
//		problemInformation.verbose = true;
		goalsAppeared = predicates->subsumes(problemInformation.GoalPredicates, &problemInformation) && !negState->intersectsWith(problemInformation.NegativeGoalPredicates, &problemInformation);
	}
	cout << endl;
	//move the items from the temporary list into the arrays for faster access
	delete negState;
	
	list<State::iterator>::iterator itr = tempLevelEnds.begin();

	levelEnds = new State::iterator[tempLevelEnds.size()];

	for(int count = 0; itr != tempLevelEnds.end(); itr++, count++){

		 State::iterator levelEnd = (*itr);
		 levelEnd++; 
		 levelEnds[count] = levelEnd;
	}


	list< ActionInstanceList::iterator >::iterator eitr = tempActionLevelEnds.begin();

	actionLevelEnds = new ActionInstanceList::iterator[tempActionLevelEnds.size()];

	for(int count = 0; eitr != tempActionLevelEnds.end(); eitr++, count++){
		ActionInstanceList::iterator actionLevelEnd = (*eitr);
		actionLevelEnd++;
		actionLevelEnds[count] = actionLevelEnd;
	}


	
};

Plan RelaxedPlanningGraph::extractRelaxedPlanQuickly(RelaxedPlanningGraph & oldRPG, StaticInformation & problemInformation, int & heuristic){

//	bool noRealActions = true;
	if (problemInformation.verbose) {
		cout << "Extracting plan...";
		cout.flush();
	}
	
	//cout << levelCount << " layers in RPG" << endl;
	Plan toReturn;
	
	map<unsigned int, pair<set<unsigned int>, set<unsigned int> > > layerGoals;
	
	
	heuristic = 0;
	
	for (int i = 0; i < oldRPG.goalCount; i++) {
		//cout << "Want to achieve PI with ID:" << endl;
		//cout << oldRPG.goalIDs[i] << "; it appears first at layer " << oldRPG.achievedAt[oldRPG.goalIDs[i]] << endl;
		unsigned int currGoalID = oldRPG.goalIDs[i];
		//cout << "currgoalID = " << currGoalID << endl;
		bool achievedAtAll = oldRPG.achieved[currGoalID];
		//cout << "achieved at all: ";
		if (achievedAtAll) {
			//cout << "yes" << endl;
		} else {
			//cout << "no" << endl;
		}
		unsigned int aAt = oldRPG.achievedAt[currGoalID];
		//cout << "achieved at = " << aAt << endl;
		layerGoals[aAt].first.insert(currGoalID);
		//cout << "Noted" << endl;
	}
	
	for (int i = 0; i < oldRPG.negativeGoalCount; i++) {
		//cout << "Want to achieve PI with ID:" << endl;
		//cout << oldRPG.goalIDs[i] << "; it appears first at layer " << oldRPG.achievedAt[oldRPG.goalIDs[i]] << endl;
		unsigned int currGoalID = oldRPG.negativeGoalIDs[i];
		//cout << "currgoalID = " << currGoalID << endl;
		bool achievedAtAll = oldRPG.negativeAchieved[currGoalID];
		//cout << "achieved at all: ";
		if (achievedAtAll) {
			//cout << "yes" << endl;
		} else {
			//cout << "no" << endl;
		}
		
		if (!oldRPG.negativeInitially[currGoalID]) {
			unsigned int aAt = oldRPG.negativeAchievedAt[currGoalID];
			//cout << "achieved at = " << aAt << endl;
			layerGoals[aAt].second.insert(currGoalID);
			//cout << "Noted" << endl;
		}
	}		
	//cout << "Planning..." << endl;
	
	while (!layerGoals.empty()) {
	
		map<unsigned int, pair<set<unsigned int>, set<unsigned int> > >::iterator masterItr = layerGoals.end();
		masterItr--;
		
		unsigned int currLayer = (*masterItr).first;
		set<unsigned int>* currPositiveList = &((*masterItr).second.first);
		set<unsigned int>* currNegativeList = &((*masterItr).second.second);
		
		
		
		
		//cout << "Processing layer " << currLayer << endl;
		
		if (currLayer >= 1) {
			//cout << currLayer << ": " << currPositiveList->size() << " positive goals to achieve" << endl;
		
			while (	!currPositiveList->empty()) {
				
				
				unsigned int currGoal = *(currPositiveList->begin());
				currPositiveList->erase(currPositiveList->begin());
				
				//cout << "Finding achiever for PI " << currGoal << " at layer " << currLayer << endl;
				if (oldRPG.achieved[currGoal]) {
					//cout << "Has really been achieved" << endl;
				} else {
					//cout << "Has not been achieved" << endl;
				}
				unsigned int achieverToUse = oldRPG.firstAchiever[currGoal];

				/*cout << "Using first achiever ";
				oldRPG.applicableReal[achieverToUse]->briefWrite(cout);*/
				
				//cout << "Found" << endl;
				unsigned int masterID = achieverToUse;
				//cout << masterID;
				//cout.flush();
				while (!oldRPG.applicableReal[masterID]->getParentAction()->rootAction()) {
					masterID = oldRPG.subActionOf[masterID];
					//cout << " -> " << masterID;
					//cout.flush();
				}
				//cout << endl;
				toReturn.addStepAtBeginning(oldRPG.applicableReal[masterID], currLayer - 1.0);
				//heuristic++;
				
				const Action* toLookFor = oldRPG.applicableReal[masterID]->getParentAction();
				
				if (toLookFor->costs) {
					++heuristic;/*
					if (noRealActions) {
						noRealActions = false;
						heuristic = 1;
					}
				} else if (noRealActions) {
					++heuristic;*/
				}
				
				//cout << "Heuristic now " << heuristic << endl;
				//cout << "Propagating preconditions" << endl;
				
				bool stillLoop = true;
				
				while (stillLoop) {
					pair<set<unsigned int>, set<unsigned int> > needed = oldRPG.bareMinimumPreconditions(achieverToUse, currLayer);
					
					set<unsigned int>::const_iterator nsItr = needed.first.begin();
					set<unsigned int>::const_iterator nsEnd = needed.first.end();
					
					for (;nsItr != nsEnd; ++nsItr) {
						if (oldRPG.achieved[*nsItr]) {
							unsigned int ial = oldRPG.achievedAt[*nsItr];
							if (ial < currLayer) {
								layerGoals[ial].first.insert(*nsItr);
								//cout << "Need to have achieved " << *(oldRPG.achievedReal[*nsItr]) << " by layer " << ial << endl;
							}
						}
					}
					
					nsItr = needed.second.begin();
					nsEnd = needed.second.end();
					
					for (;nsItr != nsEnd; ++nsItr) {
						unsigned int ial = oldRPG.negativeAchievedAt[*nsItr];
						if (ial < currLayer) layerGoals[ial].second.insert(*nsItr);
					}
					
					/*for (unsigned int i = 0; i < oldRPG.actionNeeds[achieverToUse][0]; i++) {
						unsigned int tti = oldRPG.actionNeeds[achieverToUse][i+1];
						if (oldRPG.achieved[tti]) {
							//cout << "Precondition " << i << " needs dealing with" << endl;
							unsigned int ial = oldRPG.achievedAt[tti];
							
							
							if (ial < currLayer) {
								//cout << "- Action needs " << tti << " which appears first at layer " << ial << endl;
								//cout << "Achieve it in layer " << ial << endl;
								layerGoals[ial].first.insert(tti);
							} else {
								
							}
						}
					}
					
					for (unsigned int i = 0; i < oldRPG.actionNegativeNeeds[achieverToUse][0]; i++) {
						unsigned int tti = oldRPG.actionNegativeNeeds[achieverToUse][i+1];
						if (oldRPG.negativeAchieved[tti]) {
							//cout << "Negative precondition " << i << " needs dealing with" << endl;
							if (oldRPG.negativeInitially[tti]) {
								//cout << "Negative precondition is initially true" << endl;
							} else {
								unsigned int ial = oldRPG.negativeAchievedAt[tti];
							
								if (ial < currLayer) {
									//cout << "- Action needs " << tti << " which appears first at layer " << ial << endl;
									layerGoals[ial].second.insert(tti);
								}
							}
						}
					}
					*/
					for (unsigned int i = 0; i < oldRPG.achieveArray[achieverToUse][0]; i++) {
						currPositiveList->erase(oldRPG.achieveArray[achieverToUse][i+1]);
					}
					
					for (unsigned int i = 0; i < oldRPG.negativeAchieveArray[achieverToUse][0]; i++) {
						currNegativeList->erase(oldRPG.negativeAchieveArray[achieverToUse][i+1]);
					}
					if (!oldRPG.applicableReal[achieverToUse]->getParentAction()->rootAction()) {
						achieverToUse = oldRPG.subActionOf[achieverToUse];
					} else {
						stillLoop = false;
					}
				}
				
			
			}
			
			//cout << currLayer << ": " << currNegativeList->size() << " negative goals to achieve" << endl;
			
			while (	!currNegativeList->empty()) {
				
				
				unsigned int currGoal = *(currNegativeList->begin());
				currNegativeList->erase(currNegativeList->begin());
				
				//cout << "Finding achiever for PI " << currGoal << " at layer " << currLayer << endl;
				
				unsigned int achieverToUse = oldRPG.firstNegativeAchiever[currGoal];

				/*cout << "Using first achiever ";
				oldRPG.applicableReal[achieverToUse]->briefWrite(cout);*/
				
				//cout << "Found" << endl;
				
				unsigned int masterID = achieverToUse;
				
				while (!oldRPG.applicableReal[masterID]->getParentAction()->rootAction()) {
					
					unsigned int oldMaster = masterID;
					masterID = oldRPG.subActionOf[masterID];
										
					if (masterID == oldMaster) {
					
						cout << "Infinite loop!" << endl;
						exit(0);
					}
				}
				
				toReturn.addStepAtBeginning(oldRPG.applicableReal[masterID], currLayer - 1.0);
				
				//heuristic++;
				
				const Action* toLookFor = oldRPG.applicableReal[masterID]->getParentAction();
				
				if (toLookFor->costs) {
					++heuristic;
					/*if (noRealActions) {
						noRealActions = false;
						heuristic = 1;
					}
				} else if (noRealActions) {
					++heuristic;*/
				}
				
				//cout << "Heuristic now " << heuristic << endl;
				//cout << "Propagating preconditions" << endl;
				bool stillLoop = true;
				
				while (stillLoop) {
					pair<set<unsigned int>, set<unsigned int> > needed = oldRPG.bareMinimumPreconditions(achieverToUse, currLayer);
					
					set<unsigned int>::const_iterator nsItr = needed.first.begin();
					set<unsigned int>::const_iterator nsEnd = needed.first.end();
					
					for (;nsItr != nsEnd; ++nsItr) {
						unsigned int ial = oldRPG.achievedAt[*nsItr];
						if (ial < currLayer) layerGoals[ial].first.insert(*nsItr);
					}
					
					nsItr = needed.second.begin();
					nsEnd = needed.second.end();
					
					for (;nsItr != nsEnd; ++nsItr) {
						unsigned int ial = oldRPG.negativeAchievedAt[*nsItr];
						if (ial < currLayer) layerGoals[ial].second.insert(*nsItr);
					}
					
					/*for (unsigned int i = 0; i < oldRPG.actionNeeds[achieverToUse][0]; i++) {
						unsigned int tti = oldRPG.actionNeeds[achieverToUse][i+1];
						if (oldRPG.achieved[tti]) {
						
							unsigned int ial = oldRPG.achievedAt[tti];
							
							if (ial < currLayer) {
								//cout << "- Action needs " << tti << " which appears first at layer " << ial << endl;
								layerGoals[ial].first.insert(tti);
							}
						}
					}
					
					for (unsigned int i = 0; i < oldRPG.actionNegativeNeeds[achieverToUse][0]; i++) {
						unsigned int tti = oldRPG.actionNegativeNeeds[achieverToUse][i+1];
						if (oldRPG.negativeAchieved[tti]) {
							if (oldRPG.negativeInitially[tti]) {
								//cout << "Negative precondition is initially true" << endl;
							} else {
								unsigned int ial = oldRPG.negativeAchievedAt[tti];
							
								if (ial < currLayer) {
									//cout << "- Action needs " << tti << " which appears first at layer " << ial << endl;
									layerGoals[ial].second.insert(tti);
								}
							}
						}
					}*/
					
					
					for (unsigned int i = 0; i < oldRPG.negativeAchieveArray[achieverToUse][0]; i++) {
						currNegativeList->erase(oldRPG.negativeAchieveArray[achieverToUse][i+1]);
					}
					
					if (!oldRPG.applicableReal[achieverToUse]->getParentAction()->rootAction()) {
						achieverToUse = oldRPG.subActionOf[achieverToUse];
					} else {
						stillLoop = false;
					}
				}
				
			
			}
		
		}
		masterItr = layerGoals.end();
		masterItr--;
		layerGoals.erase(masterItr);
	
	}
	
	
	return toReturn;
};



Plan RelaxedPlanningGraph::extractRelaxedPlan(StaticInformation & problemInformation, int & heuristic){

	
	if (problemInformation.verbose) {
		cout << "Extracting plan...";
		cout.flush();
	}
	PredicateInstanceList * layerGoals = new PredicateInstanceList[levelCount];

	State * finalGoals = problemInformation.GoalPredicates;

	State::iterator goalItr = finalGoals->begin();

	for(; goalItr != finalGoals->end(); goalItr++) {
		PredicateInstance* curr = (*goalItr);
		layerGoals[levelCount - 1].push_back(curr);
	}

	Plan toReturn;

	for(int i = levelCount - 1; i > 0; i--){

		//find predicates that are in the previous layer
		
		//cout << "Partitioning goals at level " << i << " of up to " << levelCount - 1 << endl;
		PredicateInstanceList nonNoop;
		PredicateInstanceList noop;
		isInLayer(layerGoals[i], i - 1, noop, nonNoop);
		
		layerGoals[i] = nonNoop;
		//cout << "...goals partitioned" << endl;

		//find an achieving action for goals remaining in layer i

		//bool failedToFindAction;

		PredicateInstanceList::iterator pitr = layerGoals[i].begin();
		
		
		while (pitr != layerGoals[i].end()) {
			bool foundAction = false;
			//cout << "Looking for achiever for " << *(*pitr) << " in action level " << (i - 1) << endl;

			ActionInstanceList::iterator itr = actions.begin();

			for(; !foundAction && itr != endOfActionsApplicableAtLevel(i-1); itr++){

				if((*itr)->achieves(*pitr)) {
					
					ActionInstance* currAct = (*itr);
					foundAction = true;
					
					PredicateInstance** precons = (*itr)->getPreconditionsAsPredicateInstances();
					for (int preconIndex = 0; preconIndex < (*itr)->getPreconditionCount(); preconIndex++) {
						if (!precons[preconIndex]->getParentPredicate()->isStatic()) {
							bool alreadyIn = false;
							PredicateInstanceList::iterator prevpitr = layerGoals[i - 1].begin();
							for (; !alreadyIn && prevpitr != layerGoals[i - 1].end(); prevpitr++) {
								alreadyIn = (*(*prevpitr) == *precons[preconIndex]);
							}
							if (!alreadyIn) {
								layerGoals[i-1].push_back(precons[preconIndex]);
							}
						}
						if (precons[preconIndex]->decrUsage()) delete precons[preconIndex];
					}
					
					
					
					delete [] precons;
					
					for (int addEffectIndex = 0; addEffectIndex < currAct->getAddEffectCount(); addEffectIndex++) {
						
						Parameter** params = new Parameter*[currAct->getAddEffectArgumentCount(addEffectIndex)];
						for (int j = 0; j < currAct->getAddEffectArgumentCount(addEffectIndex); j++) {
							params[j] = currAct->getAddEffectArgument(addEffectIndex, j);
						}
						
						PredicateInstance* toMatch = new PredicateInstance(currAct->getAddEffectType(addEffectIndex), params);
						
						for (int pass = 0; pass < 2; pass++) {
						
							PredicateInstanceList* currList;
							
							if (pass == 0) {
								currList = &(layerGoals[i]);	
							} else {
								currList = &(noop);
							}
							
							PredicateInstanceList::iterator ritr = currList->begin();
							PredicateInstanceList::iterator rprevitr = currList->begin();
							
							while (ritr != currList->end()) {
								if (*(*ritr) == *toMatch) {
								
									if (ritr == currList->begin()) {
										currList->pop_front();
										ritr = currList->begin();
										rprevitr = ritr;
									} else {
										currList->erase(ritr);
										ritr = rprevitr;
										ritr++;
									}
								} else {
									rprevitr = ritr;
									ritr++;
								}
							}
						}
						
						delete toMatch;
						
					}
					
					
					double tempDouble = i - 1;
					
					//cout << "Adding action at " << tmpAI << " to plan at " << &toReturn << endl;
					toReturn.addStepAtBeginning(*itr, tempDouble);

					pitr = layerGoals[i].begin();
					
				}
				
				
			}

			if (foundAction) {
				//cout << "...Action Found" << endl;
			} else {
				//cout << "...Action Not Found" << endl;
			}
			
			

		}
		
		
		for (pitr = noop.begin();pitr != noop.end(); pitr++) {
						
			PredicateInstance* toMatch = *pitr;
								
				
					
			PredicateInstanceList::iterator ritr = layerGoals[i-1].begin();
			PredicateInstanceList::iterator rprevitr = layerGoals[i-1].begin();
			
			bool duplicate = false;			
			for (;!duplicate && ritr != layerGoals[i-1].end(); ritr++) {
				duplicate = (*(*ritr) == *toMatch);
			}
			
			if (!duplicate) {
				layerGoals[i-1].push_back(toMatch);
			}
						
		}
		

	}
	
	delete [] layerGoals;
	
	heuristic = toReturn.length();
	
	if (problemInformation.verbose) {
		cout << "done." << endl;
		
	}
	return toReturn;
}

void RelaxedPlanningGraph::isInLayer(const PredicateInstanceList & toCheck, int layer, PredicateInstanceList & found, PredicateInstanceList & notFound){

	PredicateInstanceList::const_iterator itr = toCheck.begin();

	for (; itr != toCheck.end(); itr++) {


		const PredicateInstance* curr = (*itr);


//		const Predicate* p = curr->getParentPredicate();

		bool notFoundYet = true;

		State::const_iterator sitr = beginningOfLevel(0);
		State::const_iterator eitr = endOfLevel(layer);

		for (; notFoundYet && sitr != eitr; sitr++) {
			const PredicateInstance * currPred = (*sitr);
			notFoundYet = !(*currPred == *curr);


		}

		if (notFoundYet) {
			//cout << *curr << " not found in layer" << layer << endl;
			notFound.push_back(*itr);
		}else{
			
			found.push_back(*itr);
		}
	}

};


void RelaxedPlanningGraph::write(ostream & o) {


	o << "Relaxed planning graph with " << levelCount << " layers." << endl;
	for(int i = 0; i < levelCount; i++){

		State::iterator predItr = beginningOfLevel(i);
		
		o << "---------------------------------------------" << endl;
		o << "Predicates present in layer " << i << endl << endl;

		for(; predItr != endOfLevel(i); predItr++) {
		
			o << *(*predItr) << endl;
			
		}

		if (i != levelCount - 1) {
			o << endl << "+++++++++++++++++++++++++++++++++++++++++++++" << endl;
			o << "Actions applicable in layer " << i << endl << endl;
	
			ActionInstanceList::const_iterator actItr = beginningOfActionsApplicableAtLevel(i);
			
			for(;actItr != endOfActionsApplicableAtLevel(i); actItr++) {
				o << *(*actItr);
				//(*actItr)->briefWrite(o);
				
			}
		}


	}

}

list<PredicateInstance*> RelaxedPlanningGraph::extractLandmarks(StaticInformation & problemInformation){

//	problemInformation.verbose = true;
	State::iterator itr = predicates->begin();

	list<PredicateInstance*> preconditionIntersection;
	list<PredicateInstance*> preconditionUnion;
	list<PredicateInstance*> landmarks;
	list<PredicateInstance*> * layerGoals = new list<PredicateInstance*>[levelCount];
	list< list<PredicateInstance*> > * setGoals = new list< list<PredicateInstance*> >[levelCount];

	State * finalGoals = problemInformation.GoalPredicates;

	State::iterator goalItr = finalGoals->begin();

	for(; goalItr != finalGoals->end(); goalItr++) {
		//remember to put the stuff here
		layerGoals[levelCount - 1].push_back(*goalItr);
	}

	if (problemInformation.verbose) {
		cout << "Regressing through planning graph..." << endl;
		cout << *this << endl;
	}

	for(int i = levelCount - 1; i > 0; i--){ //for each level of the planning graph

		if (problemInformation.verbose) {
			cout << "Layer " << i << endl;
		}
		list<PredicateInstance*>::iterator itr = layerGoals[i].begin();

		for(; itr != layerGoals[i].end(); itr++){	//for each goal


			list<ActionInstance *> achievingActions;

			list<ActionInstance*>::iterator applicItr = actions.begin();

			for(; applicItr != endOfActionsApplicableAtLevel(i-1); applicItr++){
				if((*applicItr)->achieves(*itr)){
					if (problemInformation.verbose) {
						(*applicItr)->briefWrite(cout);
						cout << "... achieves " << *(*itr) << endl;
					}
					achievingActions.push_back(*applicItr);
				}
			}

			preconditionIntersection.clear();
			preconditionUnion.clear();

			intersectionAndUnionOfPreconditions(achievingActions, preconditionIntersection, preconditionUnion);

			if (problemInformation.verbose) {
				cout << ">>Intersection of preconditions:" << endl;
				for (list<PredicateInstance *>::iterator acItr = preconditionIntersection.begin(); acItr != preconditionIntersection.end(); acItr++) {
					cout << ">>\t" << *(*acItr) << endl;
				}
				cout << ">>Union \\ intersection of preconditions:" << endl;
				for (list<PredicateInstance *>::iterator acItr = preconditionUnion.begin(); acItr != preconditionUnion.end(); acItr++) {
					cout << ">>\t" << *(*acItr) << endl;
				}
			}

			list<PredicateInstance*>::iterator intItr = preconditionIntersection.begin();

			for(; intItr != preconditionIntersection.end(); intItr++){
				int addAt = earliestAppearance((*intItr));
				if((addAt) != -1) {

					list<PredicateInstance*>::iterator lgoalItr = layerGoals[addAt].begin();
					bool found = false;
					for (;!found && lgoalItr != layerGoals[addAt].end(); lgoalItr++) {
						found = *(*lgoalItr) == *(*intItr);
					}

					if (!found) {
						layerGoals[addAt].push_back((*intItr));
						if(problemInformation.verbose){
							cout << "adding " << *(*intItr) << " as a high-potential landmark" << endl;
						}
						landmarks.push_back(*intItr);
					}


				}
			}

			//group by type and add to set goals
			//list< list<PredicateInstance*> > sortedTypes;

			list< list<PredicateInstance*> >::iterator sortItr;
			list<PredicateInstance*>::iterator unionItr = preconditionUnion.begin();
			bool found = false;

			for(;unionItr != preconditionUnion.end(); unionItr++){

				for(sortItr = setGoals[i].begin(); sortItr != setGoals[i].end() && !found; sortItr++){
					if((*sortItr).front()->getParentPredicate() == (*unionItr)->getParentPredicate()){
						(*sortItr).push_back(*unionItr);
						found = true;
					}
				}

				if(!found) {
					list<PredicateInstance*> newType;
					newType.push_front(*unionItr);
					setGoals[i].push_back(newType);
				}
				found = false;
			}




		}

		//list< list<PredicateInstance*> >::iterator setGoalItr = setGoals[i].begin();

		//for(; setGoalItr != setGoals[i].end(); setGoalItr++){

		list< list<PredicateInstance*> >::iterator bigItr = setGoals[i].begin();


		for(; bigItr != setGoals[i].end(); bigItr++){

			list<ActionInstance*> requiredActions;
			list<PredicateInstance*>::iterator setIterator = (*bigItr).begin();

			if (problemInformation.verbose) cout << "requiredActions are:" << endl;

			for(; setIterator != (*bigItr).end(); setIterator++){

				list<ActionInstance *> achievingActions;

				list<ActionInstance*>::iterator applicItr = actions.begin();

				for(; applicItr != endOfActionsApplicableAtLevel(i-1); applicItr++){
					if((*applicItr)->achieves(*setIterator)){
						achievingActions.push_back(*applicItr);
					}
				}

				//find out if the action is already in the actions required for this set and if not put it in

				list<ActionInstance*>::iterator requiredItr = requiredActions.begin();

				list<ActionInstance*>::iterator toAddItr = achievingActions.begin();


				for(; toAddItr != achievingActions.end(); toAddItr++){

					bool found = false;
					for(requiredItr = requiredActions.begin(); requiredItr != requiredActions.end() && !found; requiredItr++){

						if((*(*toAddItr)) == (*(*requiredItr))){
							found = true;
						}


					}

					if(!found) {
						if (problemInformation.verbose) {
							cout << "\t";
							(*toAddItr)->briefWrite(cout);
						}
						requiredActions.push_back(*toAddItr);
					}

				}

			}

			//compute intersections of preconditions in the required action list (line 11)
			if (problemInformation.verbose) cout << "End of required actions" << endl;

			list<PredicateInstance*> newIntersection;
			list<PredicateInstance*> newUnion;

			intersectionAndUnionOfPreconditions(requiredActions, newIntersection, newUnion);

			list<PredicateInstance*>::iterator intersectionItr = newIntersection.begin();
			for(; intersectionItr != newIntersection.end(); intersectionItr++){
				int addAt = earliestAppearance((*intersectionItr));
				if((addAt) != -1) {

					list<PredicateInstance*>::iterator lgoalItr = layerGoals[addAt].begin();
					bool found = false;
					for (;!found && lgoalItr != layerGoals[addAt].end(); lgoalItr++) {
						found = *(*lgoalItr) == *(*intersectionItr);
					}

					if (!found) {
						layerGoals[addAt].push_back((*intersectionItr));
						if(problemInformation.verbose){
							cout << "adding " << *(*intersectionItr) << " as a potential landmark" << endl;
						}
						landmarks.push_back(*intersectionItr);
					}


				}
				
			}
			list<PredicateInstance*> setGoalsItems;
			list<PredicateInstance*>::iterator newUnionItr = newUnion.begin();
			for(; newUnionItr != newUnion.end(); newUnionItr++){
				setGoalsItems.push_back(*newUnionItr);
			}

			//put the new things on set goals
			list<PredicateInstance*>::iterator sgItr = setGoalsItems.begin();

			//group by type and add to set goals

			list< list<PredicateInstance*> >::iterator typeItr = setGoals[i-1].begin();
			list<PredicateInstance*>::iterator unionItr = newUnion.begin();
			bool found = false;
			bool foundType = false;
			list<PredicateInstance*>::iterator sortItr;


			for(;unionItr != newUnion.end(); unionItr++){

				for(typeItr = setGoals[i-1].begin(); typeItr != setGoals[i-1].end(); typeItr++){

					if((*(*typeItr).begin())->getParentPredicate() == (*unionItr)->getParentPredicate()){
						foundType = true;
						for(sortItr = (*typeItr).begin(); sortItr != (*typeItr).end() && !found; sortItr++){
							if(*((*sortItr)) == *(*unionItr)){
								found = true;
							}
						}

						if(!found) (*typeItr).push_back(*unionItr);
					}
				}

				if(!foundType) {
					list<PredicateInstance*> newType;
					newType.push_front(*unionItr);
					setGoals[i-1].push_back(newType);
				}
				found = false;
				foundType = false;
			}

		}

		//}

	}
	list<PredicateInstance*> toReturn = verifyLandmarks(landmarks, problemInformation);
	delete[] layerGoals;
	delete[] setGoals;
	return toReturn;
}

list<PredicateInstance*> RelaxedPlanningGraph::verifyLandmarks(list<PredicateInstance*> & potentialLandmarks, StaticInformation & problemInformation){

	list<PredicateInstance*>::iterator landmarkItr = potentialLandmarks.begin();
	list<PredicateInstance*> landmarks;
	//build a relaxed planning graph without each literal
	for(; landmarkItr != potentialLandmarks.end(); landmarkItr ++){

		cout << "-----------------------------------------------------------" << endl;
		cout << "Assessing whether " << *(*landmarkItr) << " is a landmark" << endl;
		bool addedNew = true;
		

		State * tempPredicates = problemInformation.InitialState->copy();
		list<ActionInstance*> tempActions;

//		problemInformation.verbose = false;
		
		
		
		
		while((!tempPredicates->subsumes(problemInformation.GoalPredicates, &problemInformation)) && addedNew){
			
			ActionInstanceList applicable = tempPredicates->applicableRelaxedActions(&problemInformation);

			ActionInstanceList reallyApplicable;
			
			ActionInstanceList::iterator removeItr = applicable.begin();

			for(; removeItr != applicable.end(); removeItr++){
				ActionInstance* currAct = (*removeItr);
				
				PredicateInstance ** check = currAct->getPreconditionsAsPredicateInstances();
				
				int checkCount = currAct->getPreconditionCount();
				bool found = false;
				for (int cui = 0; !found && cui < checkCount; cui++) {
					found = (*(check[cui]) == *(*landmarkItr));
					
				}
				
				
				if (found) {
					
					
					delete currAct;
				} else {
					
					reallyApplicable.push_back(currAct);
				}
				for (int cui = 0; cui < checkCount; cui++) delete check[cui];
				delete [] check;
				
				
			}


			tempPredicates->applyRelaxedActionsLocally(reallyApplicable, &problemInformation, true, true);

			//tempPredicates->removeForeignPredicateInstance(*landmarkItr);

			ActionInstanceList::iterator itr = reallyApplicable.begin();

			addedNew = false;
			
			for(; itr != reallyApplicable.end(); itr++) {
				/*list<ActionInstance*>::iterator checkDuplicateItr = tempActions.begin();
				bool found = false;
				for (; !found && checkDuplicateItr != tempActions.end(); checkDuplicateItr++) {
					found = (*(*checkDuplicateItr) == *(*itr));
				}*/
				bool found = false;
				if (!found) {
					addedNew = true;
					tempActions.push_back(*itr);
				
				}
			}
			
			
			
			

			
		}
		//if the goals aren't there the literal is a landmark
		
		if(!tempPredicates->subsumes(problemInformation.GoalPredicates, &problemInformation)){
//			problemInformation.verbose = true;
			if(problemInformation.verbose){
				cout << *(*landmarkItr) << " is a landmark" << endl;
			}
			landmarks.push_back(*landmarkItr);
		} else {
//			problemInformation.verbose = true;
			if(problemInformation.verbose){
				cout << "discounting " << *(*landmarkItr) << "; it is not a landmark" << endl;
			}
		}
		
		delete tempPredicates;

	}

	return landmarks;
	
}

void RelaxedPlanningGraph::intersectionAndUnionOfPreconditions(list<ActionInstance*> & achievingActions, list<PredicateInstance*> & intersection, list<PredicateInstance*> & preconditionUnion){

	if (achievingActions.empty()) return;

	list<ActionInstance*>::iterator actItr = achievingActions.begin();

	PredicateInstance ** preconditions = (*actItr)->getPreconditionsAsPredicateInstances();
	int limit = (*actItr)->getPreconditionCount();

	for(int j = 0; j < limit; j++){
		if (preconditions[j]->getParentPredicate()->isStatic()) {
			delete preconditions[j];
		} else {
			intersection.push_back(preconditions[j]);
		}

	}

	delete [] preconditions;
	//if there is more than one achieving action we need to calculate the intersection

	if(achievingActions.size() > 1){

		//ActionInstance* comp = (*actItr);
		//PredicateInstance ** cPreconditions = (*actItr)->getPreconditionsAsPredicateInstances();

		actItr ++;

		for(; actItr!= achievingActions.end(); actItr++){
			PredicateInstance ** jPreconditions = (*actItr)->getPreconditionsAsPredicateInstances();
			int limit = (*actItr)->getPreconditionCount();

			//for each item in the intersection
			list<PredicateInstance*>::iterator citr = intersection.begin();
			list<PredicateInstance*>::iterator previtr = intersection.begin();
			while(citr != intersection.end()){

				bool found = false;
				for(int j = 0; j < limit && !found; j++){
					found = (*(*citr) == *jPreconditions[j]);
				}

				if (!found) {
					if (citr == intersection.begin()) {
						delete (*citr);
						intersection.pop_front();
						citr = intersection.begin();
					} else {
						delete (*citr);
						intersection.erase(citr);
						citr = previtr;
						citr++;
					}
				} else {
					previtr = citr;
					citr++;
				}


			}
			for (int j = 0; j < limit; j++) delete jPreconditions[j];
			delete [] jPreconditions;
		}



		list<PredicateInstance*>::iterator unionItr = preconditionUnion.begin();

		actItr = achievingActions.begin();

		for(; actItr!= achievingActions.end(); actItr++){

			PredicateInstance ** jPreconditions = (*actItr)->getPreconditionsAsPredicateInstances();
			int limit = (*actItr)->getPreconditionCount();


			for(int j = 0; j < limit; j++){
				bool found = false;
				//if the predicate is in the intersection we don't want it (this is really union\intersection)
				for(list<PredicateInstance*>::iterator citr = intersection.begin(); citr != intersection.end() && !found; citr++){
					found = (*(*citr) == *jPreconditions[j]);
				}

				if(!found && !(jPreconditions[j]->getParentPredicate()->isStatic())) {

					list<PredicateInstance*>::iterator tempItr = preconditionUnion.begin();
					bool alreadyIn = false;

					for(; tempItr != preconditionUnion.end(); tempItr++){
						alreadyIn = (*(*tempItr) == *jPreconditions[j]);
					}

					if(!alreadyIn) {
						preconditionUnion.push_back(jPreconditions[j]);
					} else {
						delete jPreconditions[j];
					}


				} else {
					delete jPreconditions[j];
				}


			}

			delete [] jPreconditions;
		}
	}
}

int RelaxedPlanningGraph::earliestAppearance(PredicateInstance* toFind){

	State::iterator itr = predicates->begin();

	int toReturn = 0;

	for(; itr != predicates->end(); itr++){

		if(itr == endOfLevel(toReturn)) toReturn++;

		if((*toFind) == (*(*itr))){
			return toReturn;
		}
	}

	return -1;
}

RelaxedPlanningGraph::~RelaxedPlanningGraph() {

	delete [] levelEnds;
	delete [] actionLevelEnds;
	
	//cout << "Clearing spike" << endl;
	delete predicates;
	//cout << "Clearing actions" << endl;
	
	if (supportArray != 0) {
		delete allPredicates;
		
		for (unsigned int i = 0; i < predicateCount; i++) {
			delete [] supportArray[i];
			delete [] negativeSupportArray[i];
			if (achievedReal[i]->decrUsage()) delete achievedReal[i];
		}


		for (unsigned int i = 0; i < actionCount; i++) {
			if (applicableReal[i]->decrUsage()) delete applicableReal[i];
			delete [] achieveArray[i];
			delete [] negativeAchieveArray[i];
			delete [] actionNeeds[i];
			delete [] actionNegativeNeeds[i];
			delete [] subActions[i];
		}
		
		delete [] supportArray;
		delete [] negativeSupportArray;
		delete [] achievedReal;
		
		delete [] applicableReal;
		delete [] negativeAchieveArray;
		delete [] achieveArray;
		delete [] actionNeeds;
		delete [] actionNegativeNeeds;
		delete [] achieved;
		delete [] negativeAchieved;
		delete [] achievedAt;
		delete [] negativeAchievedAt;
		delete [] resetAchieved;
		delete [] resetNegativeAchieved;
		delete [] firstAchiever;
		delete [] firstNegativeAchiever;

		delete [] containersActually;		
		
		
		delete [] goalIDs;
		delete [] negativeGoalIDs;
		
		delete [] predicateInstanceIsGoal;
		delete [] predicateInstanceIsNegativeGoal;
		
		delete [] parentAchieved;
		
		delete [] resetPA;
		
		
		delete [] resetContainers;
		
		delete [] containers;
		delete [] negativeInitially;
	
		delete [] defaultlyApplicable;
		
//		delete [] keepAction;
		
	
		delete [] subActionOf;
		delete [] subActions;
			
		delete dependencyMap;
	
		MasterAIHash::iterator delItr = masterAI->begin();
		const MasterAIHash::iterator delEnd = masterAI->end();
		
		for (; delItr != delEnd; ++delItr) {
		
			if ((*delItr).second->decrUsage()) delete (*delItr).second;
		}
		
		delete masterAI;
	


	}
	
}	


ostream & operator <<(ostream & o, RelaxedPlanningGraph & p) {
	p.write(o);
	return o;
}

bool RelaxedPlanningGraph::operator ==(RelaxedPlanningGraph & r) {
 
	cout << "Comparing RPG at " << this << " with one at " << &r << endl;
	StaticInformation * staticInf = 0;
	bool pmatch = predicates->subsumes(r.predicates, staticInf);
	if (!pmatch) cout << "This does not subsume the other" << endl;
	bool omatch = r.predicates->subsumes(predicates, staticInf);
	if (!omatch) cout << "The other does not subsume this" << endl;
	
	bool firstPrint = true;
	
	for (int pass = 0; pass < 2; pass++) {
	
		RelaxedPlanningGraph * first;
		RelaxedPlanningGraph * second;
		
		string fname;
		string sname;
		
		if (pass == 0) {
			first = this;
			fname = "this";
			second = &r;
			sname = "the other";
		} else {
			first = &r;
			fname = "the other";
			second = this;
			sname = "this";
		}
		for (int i = 0; i < levelCount; i++) {
			State::iterator tsol = first->beginningOfLevel(i);
			State::iterator teol = first->endOfLevel(i);
			
			for (; tsol != teol; tsol++) {
				State::iterator osol = second->beginningOfLevel(i);
				State::iterator oeol = second->endOfLevel(i);
				
				bool found = false;
				
				for (;!found && osol != oeol; osol++) {
					found = (*(*tsol) == *(*osol));
				}
				
				if (!found) {
					if (firstPrint) {
					
						cout << "*** Synopsis ***" << endl;
					
						firstPrint = false;
					}					
					cout << "Predicate " << *(*tsol) << " in level " << i << " of " << fname << " is not in level " << i << " of " << sname << endl;
				}
			}
			
			if (i < (levelCount - 1)) {
				ActionInstanceList::iterator tsoal = first->beginningOfActionsApplicableAtLevel(i);
				ActionInstanceList::iterator teoal = first->endOfActionsApplicableAtLevel(i);
				
				for (; tsoal != teoal; tsoal++) {
					ActionInstanceList::iterator osoal = second->beginningOfActionsApplicableAtLevel(i);
					ActionInstanceList::iterator oeoal = second->endOfActionsApplicableAtLevel(i);
					
					bool found = false;
					
					for (;!found && osoal != oeoal; osoal++) {
						found = (*(*tsoal) == *(*osoal));
					}
					
					if (!found) {
						if (firstPrint) {
						
							cout << "*** Synopsis ***" << endl;
						
							firstPrint = false;
						}
						cout << "Action ";
						(*tsoal)->briefWrite(cout);
						cout << "\tin level " << i << " of " << fname << " is not in level " << i << " of " << sname << endl;
					}
				}
			}
		
				
		}
		
		
	}
	if (!firstPrint || !omatch || !pmatch) return false;
	return true;
	
	
}

void RelaxedPlanningGraph::getApplicableSubActions(ActionInstance* const & a, ActionInstanceList & c) const {

	c.push_back(a);
	slist<unsigned int> toGo;
	toGo.push_front(a->getID());
	
	unsigned int curr;
	unsigned int i;
	unsigned int limit;
	unsigned int child;
	
	while (!toGo.empty()) {
		curr = toGo.front();
		toGo.pop_front();
		
		limit = subActions[curr][0];
		
		for (i = 1; i <= limit; i++) {
			child = subActions[curr][i];
			if (/*keepAction[child] && */containers[child] <= 0) {
				c.push_back(applicableReal[child]);
				toGo.push_front(child);
			}
		}
	}

}

ActionInstance* RelaxedPlanningGraph::getMasterAI(const ActionInstance* const & f) const {
	
	MasterAIHash::const_iterator itr = masterAI->find(f);
	if (itr == masterAI->end()) {
		cout << "Asked for a non-existant action: " << *f << endl;
		ActionInstanceList::const_iterator actStart = actions.begin();
		ActionInstanceList::const_iterator actEnd = actions.end();
		
		for (; actStart != actEnd; ++actStart) {
			if (*(*actStart) == *f) {
				cout << "Actually it does exist: " << endl;
				cout << *(*actStart) << endl;
			}
		}
		cout << "Checked the action list (size " << actions.size() << ") - it's not there" << endl;
		
		exit(0);
	}
	return (*itr).second;
	

}

pair<set<unsigned int>, set<unsigned int> > RelaxedPlanningGraph::bareMinimumPreconditions(const unsigned int & actionID, const unsigned int & layer) const {
	//cout << "New method" << endl;
	const ActionInstance* realAct = applicableReal[actionID];
	const Action* parentAction = realAct->getParentAction();
	
	pair<set<unsigned int>, set<unsigned int> > toReturn;
	slist<unsigned int> toProcess;
	toProcess.push_front(0);
	
	unsigned int containerID;
	
	while (!toProcess.empty()) {
		containerID = toProcess.front();
		toProcess.pop_front();
		if (parentAction->isDisjunctiveContainer(containerID)) {
			bool found = false;

			PredicateInstance* predItself;
			list<int>::const_iterator itr = parentAction->getSubPreconditions(containerID).begin();
			list<int>::const_iterator itrEnd = parentAction->getSubPreconditions(containerID).end();
			for (; !found && itr != itrEnd; ++itr) {
				predItself = realAct->internalPreconditions()[*itr];
				if (predItself != 0 && !predItself->getParentPredicate()->isStatic()) {
				
					if (achieved[predItself->getID()] && achievedAt[predItself->getID()] < layer) {
						found = true;
						toReturn.first.insert(predItself->getID());
					}
				}
			
			}
			if (!found) {
				itr = parentAction->getSubNegativePreconditions(containerID).begin();
				itrEnd = parentAction->getSubNegativePreconditions(containerID).end();
				for (; !found && itr != itrEnd; ++itr) {
					predItself = realAct->internalNegativePreconditions()[*itr];
					if (predItself != 0 && !predItself->getParentPredicate()->isStatic()) {
						if (negativeInitially[predItself->getID()]) {
							found = true;
						} else {
							if (negativeAchieved[predItself->getID()] && negativeAchievedAt[predItself->getID()] < layer) {
								found = true;
								toReturn.second.insert(predItself->getID());
							}
						}
					}
				
				}
			
			}
			if (!found) {
				itr = parentAction->getSubContainer(containerID).begin();
				itrEnd = parentAction->getSubContainer(containerID).end();
				for (; !found && itr != itrEnd; ++itr) {
					if (containers[actionID][*itr] <= 0) {
						found = true;
						toProcess.push_front(*itr);
					}
				}
			}			
		} else {
			

			PredicateInstance* predItself;
			
			list<int>::const_iterator itr = parentAction->getSubPreconditions(containerID).begin();
			list<int>::const_iterator itrEnd = parentAction->getSubPreconditions(containerID).end();
			
			for (; itr != itrEnd; ++itr) {
				predItself = realAct->internalPreconditions()[*itr];
				if (predItself != 0 && !predItself->getParentPredicate()->isStatic() && achieved[predItself->getID()]) {
					toReturn.first.insert(predItself->getID());
				}
			
			}
			
			itr = parentAction->getSubNegativePreconditions(containerID).begin();
			itrEnd = parentAction->getSubNegativePreconditions(containerID).end();
			for (; itr != itrEnd; ++itr) {
				predItself = realAct->internalNegativePreconditions()[*itr];
				if (predItself != 0 && !predItself->getParentPredicate()->isStatic()  && negativeAchieved[predItself->getID()]) {
					if (!negativeInitially[predItself->getID()]) toReturn.second.insert(predItself->getID());
				}
			
			}
		
			
			
			itr = parentAction->getSubContainer(containerID).begin();
			itrEnd = parentAction->getSubContainer(containerID).end();
			for (; itr != itrEnd; ++itr) {
				toProcess.push_front(*itr);
			}
		
		}
	}
	
	//cout << "End of new method" << endl;
	return toReturn;

}


/*set<unsigned int> RelaxedPlanningGraph::extractEarliestPreconditions(const Action const*& parentAction, const ActionInstance const *& toApply, const unsigned int & containerID, const unsigned int & layer) const {

	return extractEarliestPreconditionsInternal(parentAction, toApply, containerID, layer).first;
	

}

pair<set<unsigned int>, unsigned int> RelaxedPlanningGraph::extractEarliestPreconditionsInternal(const Action const*& parentAction, const ActionInstance const *& toApply, const unsigned int containerID, const unsigned int & layer) const {

	if (parentAction->isDisjunctiveContainer(containerID)) {
		pair<set<unsigned int>, unsigned int> toReturn;
		list<int>::const_iterator itr = parentAction->getSubPreconditions(containerID).begin();
		list<int>::const_iterator itrEnd = parentAction->getSubPreconditions(containerID).end();
		if (!itr == itrEnd) {
		
			toReturn.second = 0;
			for (;itr != itrEnd; ++itr) {
				
				toReturn.first.insert(
			}
			
		} else {
			itr = parentAction->getSubNegativePreconditions(containerID).begin();
			itrEnd parentAction->getSubNegativePreconditions(containerID).end();
			
			if (!itr == itrEnd) {
				toReturn.second = 0;
				
				
			} else {
				toReturn.second = INT_MAX;
			}
		}
		toReturn.second = INT_MAX;
		
		return;
	}
	// implicit 'else' case
	pair<set<unsigned int>, unsigned int> toReturn;
	toReturn.second = 0;
	
	
	
	
};*/
