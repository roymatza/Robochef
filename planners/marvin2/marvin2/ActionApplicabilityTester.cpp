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
#include "ActionApplicabilityTester.h"

#include "State.h"
#include "staticinformation.h"

#define APPLICABILITYTEST_DEBUG 0

struct SearchPosition {

	const atsChildType nodeType;
	const int predID;
	
	const list<int> * subContainers;
	const list<int> * subPreconditions;
	const list<int> * subNegativePreconditions;
	
	
	list<int>::const_iterator subPreconditionProgress;
	list<int>::const_iterator subNegativePreconditionProgress;
	list<int>::const_iterator subContainerProgress;
	
	SearchPosition(atsChildType ntIn, const int & pIDin, const list<int> * scIn, const list<int> * spIn, const list<int> * snpIn)
	 :  nodeType(ntIn), predID(pIDin), subContainers(scIn), subPreconditions(spIn), subNegativePreconditions(snpIn) {
	
		if (subContainers != 0) {
			subContainerProgress = subContainers->begin();
		}
		
		if (subPreconditions != 0) {
			subPreconditionProgress = subPreconditions->begin();
		}
		if (subNegativePreconditions != 0) {
			subNegativePreconditionProgress = subNegativePreconditions->begin();
		}
	}
	SearchPosition(const SearchPosition & s)
	 : nodeType(s.nodeType), predID(s.predID),
	   subContainers(s.subContainers), subPreconditions(s.subPreconditions), subNegativePreconditions(s.subNegativePreconditions),
	   subPreconditionProgress(s.subPreconditionProgress), subNegativePreconditionProgress(s.subNegativePreconditionProgress), subContainerProgress(s.subContainerProgress)
	   {
	   
	   };

};


class ApplicabilityTestingState {


private:
	int dummyInt;
	int dummyUpdate;
	bool dummyBool;
	list<int> dummySC;
	list<int> dummySP;
	list<int> dummySNP;
	
	const int & getCUInternal(const StripsAction * a, const int & i) const {
		return dummyInt;
	}
	
	const int & getCUInternal(const ADLAction * a, const int & i) const {
		return a->getContainerUpdates(i);
	}
	
	const int & getPUInternal(const StripsAction * a, const int & i) const {
		return dummyUpdate;
	}
	
	const int & getPUInternal(const ADLAction * a, const int & i) const {
		return a->getPreconditionUpdates(i);
	}

	const int & getNPUInternal(const StripsAction * a, const int & i) const {
		return dummyUpdate;
	}
	
	const int & getNPUInternal(const ADLAction * a, const int & i) const {
		return a->getNegativePreconditionUpdates(i);
	}

	const list<int> & getSCInternal(const ADLAction * a, const int & i) const {
		return a->getSubContainer(i);
	}
	
	const list<int> & getSCInternal(const StripsAction * a, const int & i) const {
		return dummySC;
	}
	

	const list<int> & getSPInternal(const ADLAction * a, const int & i) const {
		return a->getSubPreconditions(i);
	}
	
	const list<int> & getSPInternal(const StripsAction * a, const int & i) const {
		return dummySP;
	}

	const list<int> & getSNPInternal(const ADLAction * a, const int & i) const {
		return a->getSubNegativePreconditions(i);
	}
	
	const list<int> & getSNPInternal(const StripsAction * a, const int & i) const {
		return dummySNP;
	}

	const bool & isEssCInternal(const StripsAction * a, const int & i) const {
		return dummyBool;
	}

	const bool & isEssPInternal(const StripsAction * a, const int & i) const {
		return dummyBool;
	}

	const bool & isEssNPInternal(const StripsAction * a, const int & i) const {
		return dummyBool;
	}

	const bool & isEssCInternal(const ADLAction * a, const int & i) const {
		return a->isEssentialContainer(i);
	}

	const bool & isEssPInternal(const ADLAction * a, const int & i) const {
		return a->isEssentialPrecondition(i);
	}

	const bool & isEssNPInternal(const ADLAction * a, const int & i) const {
		return a->isEssentialNegativePrecondition(i);
	}
			
public:
	const Action* actionConcerned;
	ActionType actType;
	
	Entity** argAssignments;

	
	int* containers;

	bool* happyPreconditions;
	bool* happyNegativePreconditions;
	
	int assignmentsMade;
	slist<SearchPosition*> testTracer;
	State* lookupPIsIn;
	StaticInformation* staticInf;
	
	list<AbstractPredicate> waitingFor;
	list<AbstractPredicate> negativeWaitingFor;

	bool abort;
	bool waiting;
	ActionInstance* dependent;
	
	void unsatisfyPrecondition(const int & i) {
	
		happyPreconditions[i] = false;
		int toUpdate = getPreconditionUpdates(i);
		while (toUpdate != -1) {
			const int before = containers[toUpdate];
			const int after = ++containers[toUpdate];
			if (before <= 0 && after > 0) {
				toUpdate = getContainerUpdates(toUpdate);
			} else {
				toUpdate = -1;
			}
		}

	}
	
	void unsatisfyNegativePrecondition(const int & i) {
	
		happyNegativePreconditions[i] = false;
		int toUpdate = getNegativePreconditionUpdates(i);
		while (toUpdate != -1) {
			const int before = containers[toUpdate];
			const int after = ++containers[toUpdate];
			if (before <= 0 && after > 0) {
				toUpdate = getContainerUpdates(toUpdate);
			} else {
				toUpdate = -1;
			}
		}

	}
	
	void satisfyPrecondition(const int & i) {
	
		if (APPLICABILITYTEST_DEBUG) {
			cout << "> Satisfying precondition " << i << endl;
		}
		if (happyPreconditions[i]) {
			if (APPLICABILITYTEST_DEBUG) {
				cout << "\tPrecondition already satisfied" << endl;
			}	
			return;
		}
		
		happyPreconditions[i] = true;
		int toUpdate = getPreconditionUpdates(i);
		if (APPLICABILITYTEST_DEBUG) {
			cout << "> Precondition updates container " << toUpdate << endl;
		}
		while (toUpdate != -1) {
			const int before = containers[toUpdate];
			const int after = --containers[toUpdate];
			if (before > 0 && after <= 0) {
				toUpdate = getContainerUpdates(toUpdate);
				if (APPLICABILITYTEST_DEBUG) cout << "> Container updates container " << toUpdate << endl;
			} else {
				toUpdate = -1;
			}
		}

	}
	
	void satisfyNegativePrecondition(const int & i) {
	
		if (APPLICABILITYTEST_DEBUG) {
			cout << "> Satisfying negative precondition " << i << endl;
		}
		
		if (happyNegativePreconditions[i]) {
			if (APPLICABILITYTEST_DEBUG) {
				cout << "\tNegative precondition already satisfied" << endl;
			}	
			return;
		}
		
		happyNegativePreconditions[i] = true;
		int toUpdate = getNegativePreconditionUpdates(i);
		if (APPLICABILITYTEST_DEBUG) {
			cout << "> Negative precondition updates container " << toUpdate << endl;
		}
		while (toUpdate != -1) {
			const int before = containers[toUpdate];
			const int after = --containers[toUpdate];
			if (before > 0 && after <= 0) {
			
				toUpdate = getContainerUpdates(toUpdate);
				if (APPLICABILITYTEST_DEBUG) cout << "> Container updates container " << toUpdate << endl;
			} else {
				toUpdate = -1;
			}
		}

	}

	
	ApplicabilityTestingState(const StripsAction * a, State * predicates, StaticInformation * staticInfIn)
	 : dummyInt(-1),
	   dummyUpdate(0),
	   dummyBool(true),
	   actionConcerned(a),
	   actType(ACT_STRIPS),
	   argAssignments(new Entity*[a->parameterCount]),
	   containers(new int[1]),
	   happyPreconditions(new bool[a->preconditionCount]),
	   happyNegativePreconditions(new bool[a->negativePreconditionCount]),
	   assignmentsMade(0),
	   lookupPIsIn(predicates),
	   staticInf(staticInfIn),
	   abort(false),
	   waiting(false),
	   dependent(0) {
	   
	   for (int i = 0; i < a->parameterCount; i++) argAssignments[i] = 0;
	   
	   containers[0] = a->preconditionCount + a->negativePreconditionCount;
	   
	   for (int i = 0; i < a->preconditionCount; i++) {
	   	happyPreconditions[i] = false;
		dummySP.push_back(i);
	   }
	   
	   for (int i = 0; i < a->negativePreconditionCount; i++) {
	   
	   	happyNegativePreconditions[i] = false;
		dummySNP.push_back(i);
	   }
	   
	      
	};
	
	ApplicabilityTestingState(const ADLAction * a, State * predicates, StaticInformation * staticInfIn)
	 : dummyInt(-1),
	   dummyUpdate(0),
	   dummyBool(true),
	   actionConcerned(a),
	   actType(ACT_ADL),
	   argAssignments(new Entity*[a->parameterCount]),
	   containers(new int[a->getContainerCount()]),
	   happyPreconditions(new bool[a->preconditionCount]),
	   happyNegativePreconditions(new bool[a->negativePreconditionCount]),
	   assignmentsMade(0),
	   lookupPIsIn(predicates),
	   staticInf(staticInfIn),
	   abort(false),
	   waiting(false),
	   dependent(0) {
	   
	   for (int i = 0; i < a->parameterCount; i++) argAssignments[i] = 0;
	   
	   for (int i = 0; i < a->getContainerCount(); i++) {
	   	containers[i] = a->defaultSatisfactionCounters[i];
	   }
	   
	   for (int i = 0; i < a->preconditionCount; i++) {
	   	happyPreconditions[i] = false;
		
	   }
	   
	   for (int i = 0; i < a->negativePreconditionCount; i++) {
	   
	   	happyNegativePreconditions[i] = true;
		unsatisfyNegativePrecondition(i);
		
	   }
	   
	      
	};
	
	ApplicabilityTestingState(const StripsAction * a, ApplicabilityTestingState & toCopy, Entity* assignment)
	:  dummyInt(toCopy.dummyInt),
	   dummyUpdate(toCopy.dummyUpdate),
	   dummyBool(toCopy.dummyBool),
	   actionConcerned(toCopy.actionConcerned),
	   actType(toCopy.actType),
	   argAssignments(new Entity*[a->parameterCount]),
	   containers(new int[1]),
	   happyPreconditions(new bool[a->preconditionCount]),
	   happyNegativePreconditions(new bool[a->negativePreconditionCount]),
	   assignmentsMade(toCopy.assignmentsMade),
	   lookupPIsIn(toCopy.lookupPIsIn),
	   staticInf(toCopy.staticInf),
	   abort(false),
	   waiting(false),
	   dependent(0) {
	 
	   
	   	int i;
		
		int looplimit = a->parameterCount;
		
		for (i = 0; i < looplimit; ++i) {
			argAssignments[i] = toCopy.argAssignments[i];
		}
		argAssignments[assignmentsMade] = assignment;
		
		
		containers[0] = toCopy.containers[0];
		
		looplimit = a->preconditionCount;
		for (i = 0; i < looplimit; ++i) {
		   	happyPreconditions[i] = toCopy.happyPreconditions[i];
			dummySP.push_back(i);
		}
	   
		looplimit = a->negativePreconditionCount;
		for (i = 0; i < looplimit; ++i) {
	   
			happyNegativePreconditions[i] = toCopy.happyNegativePreconditions[i];
			dummySNP.push_back(i);
		}
		resetContainers();
		++assignmentsMade;
	     
	}
	
	ApplicabilityTestingState(const ADLAction * a, ApplicabilityTestingState & toCopy, Entity* assignment)
	:  dummyInt(toCopy.dummyInt),
	   dummyUpdate(toCopy.dummyUpdate),
	   dummyBool(toCopy.dummyBool),
	   actionConcerned(toCopy.actionConcerned),
	   actType(toCopy.actType),
	   argAssignments(new Entity*[a->parameterCount]),
	   containers(new int[a->getContainerCount()]),
	   happyPreconditions(new bool[a->preconditionCount]),
	   happyNegativePreconditions(new bool[a->negativePreconditionCount]),
	   assignmentsMade(toCopy.assignmentsMade),
	   lookupPIsIn(toCopy.lookupPIsIn),
	   staticInf(toCopy.staticInf),
	   abort(false),
	   waiting(false),
	   dependent(0) {
	 
	   
	   	//int i;
	   	int looplimit = a->parameterCount;
		
		memcpy(argAssignments, toCopy.argAssignments, looplimit * sizeof(Entity*));
		
		
		argAssignments[assignmentsMade] = assignment;
		
		
		looplimit = a->getContainerCount();
		
		memcpy(containers, toCopy.containers, looplimit * sizeof(int));
		
		
		looplimit = a->preconditionCount;
		
		memcpy(happyPreconditions, toCopy.happyPreconditions, looplimit * sizeof(bool));
		
		looplimit = a->negativePreconditionCount;
		
		memcpy(happyNegativePreconditions, toCopy.happyNegativePreconditions, looplimit * sizeof(bool));
		
		resetContainers();
		++assignmentsMade;
	     
	}
	
	~ApplicabilityTestingState() {
	
		while (!testTracer.empty()) {
			delete testTracer.front();
			testTracer.pop_front();
		}
		
		delete [] argAssignments;
		delete [] containers;
		
		delete [] happyPreconditions;
		delete [] happyNegativePreconditions;
		
	}   
	
	
	const int & getContainerUpdates(const int & i) {
		if (actType == ACT_STRIPS) {
			return getCUInternal(static_cast<const StripsAction*>(actionConcerned),i);
		} else {
			return getCUInternal(static_cast<const ADLAction*>(actionConcerned),i);
		}
	}
	
	const int & getPreconditionUpdates(const int & i) {
		if (actType == ACT_STRIPS) {
			return getPUInternal(static_cast<const StripsAction*>(actionConcerned),i);
		} else {
			return getPUInternal(static_cast<const ADLAction*>(actionConcerned),i);
		}
	}

	const int & getNegativePreconditionUpdates(const int & i) {
		if (actType == ACT_STRIPS) {
			return getNPUInternal(static_cast<const StripsAction*>(actionConcerned),i);
		} else {
			return getNPUInternal(static_cast<const ADLAction*>(actionConcerned),i);
		}
	}
	
	const list<int> & getSubContainer(const int & i) const {
		if (actType == ACT_STRIPS) {
			return getSCInternal(static_cast<const StripsAction*>(actionConcerned),i);
		} else {
			return getSCInternal(static_cast<const ADLAction*>(actionConcerned),i);
		}
		
	}
	
	const list<int> & getSubPreconditions(const int & i) const {
		if (actType == ACT_STRIPS) {
			return getSPInternal(static_cast<const StripsAction*>(actionConcerned),i);
		} else {
			return getSPInternal(static_cast<const ADLAction*>(actionConcerned),i);
		}
		
	}
	
	const list<int> & getSubNegativePreconditions(const int & i) const {
		if (actType == ACT_STRIPS) {
			return getSNPInternal(static_cast<const StripsAction*>(actionConcerned),i);
		} else {
			return getSNPInternal(static_cast<const ADLAction*>(actionConcerned),i);
		}
	}

	const bool & isEssentialContainer(const int & i) const {
		if (actType == ACT_STRIPS) {
			return isEssCInternal(static_cast<const StripsAction*>(actionConcerned),i);
		} else {
			return isEssCInternal(static_cast<const ADLAction*>(actionConcerned),i);
		}
	
	}

	const bool & isEssentialPrecondition(const int & i) const {
		if (actType == ACT_STRIPS) {
			return isEssPInternal(static_cast<const StripsAction*>(actionConcerned),i);
		} else {
			return isEssPInternal(static_cast<const ADLAction*>(actionConcerned),i);
		}
	
	}
		
	const bool & isEssentialNegativePrecondition(const int & i) const {
		if (actType == ACT_STRIPS) {
			return isEssNPInternal(static_cast<const StripsAction*>(actionConcerned),i);
		} else {
			return isEssNPInternal(static_cast<const ADLAction*>(actionConcerned),i);
		}
	
	}

	
	
	
	void resetContainers() {
		/*int pc = actionConcerned->preconditionCount;
		
		for (int i = 0; i < pc; ++i) {
			if (actionConcerned->preconditions[i].interactsWith(assignmentsMade)) {
				if (happyPreconditions[i]) {
					unsatisfyPrecondition(i);
				}
			}
		}
		
		pc = actionConcerned->negativePreconditionCount;
		
		for (int i = 0; i < pc ; ++i) {
			if (actionConcerned->negativePreconditions[i].interactsWith(assignmentsMade)) {
				if (happyNegativePreconditions[i]) {
					unsatisfyNegativePrecondition(i);
				}
			}
		}*/
		
		int depCount = actionConcerned->parameterDependentCounts[assignmentsMade];
		int* depArray = actionConcerned->parameterDependents[assignmentsMade];
		
		for (int i = 0; i < depCount; ++i) {
		
			const int p = depArray[i];
		
			//cout << "Parameter interacts with precondition " << p << " of " << actionConcerned->preconditionCount << endl;
			
			if (happyPreconditions[p]) {
				unsatisfyPrecondition(p);
			}
		}
		
		depCount = actionConcerned->parameterNegativeDependentCounts[assignmentsMade];
		depArray = actionConcerned->parameterNegativeDependents[assignmentsMade];
		
		for (int i = 0; i < depCount; ++i) {
			const int p = depArray[i];
			//cout << "Parameter interacts with negative precondition " << p << " of " << actionConcerned->negativePreconditionCount << endl;
			if (happyNegativePreconditions[p]) {
				unsatisfyNegativePrecondition(p);
			}
		}
		
		
	}

	ActionInstance* getActionInstance() {
	
		ActionInstance* toReturn = new ActionInstance(actionConcerned, argAssignments);
		return toReturn;
	}
	void writeStateName() {
		cout << actionConcerned->name();
		for (int i = 0; i < actionConcerned->parameterCount; i++) {
			if (argAssignments[i] != 0) {
				cout << " " << argAssignments[i]->name(); 
			} else {
				cout << " <>";
			}
		}
		
	}
	bool isGrounded() const {
		//cout << "Is action " << actionConcerned->name() << " grounded?" << endl;
		//cout << "Parameter count = " << actionConcerned->parameterCount << "; assignments made = " << assignmentsMade << endl;
		return (assignmentsMade == actionConcerned->parameterCount);
	}
	void checkApplicability(State * currState, State * negState) {
		waiting = false;
		if (testTracer.empty() && containers[0] > 0) {
			if (APPLICABILITYTEST_DEBUG) {
				cout << "Still need to do effort to get ";
				writeStateName();
				cout << " to be applicable" << endl;
			}
			testTracer.push_front(new SearchPosition(ATS_CONTAINER, 0, &getSubContainer(0), &getSubPreconditions(0), &getSubNegativePreconditions(0)));
		}
		if (testTracer.empty()) {
			return;
		}
		waiting = false;
		while(!testTracer.empty()) {
			auto_ptr<SearchPosition> oldFront(testTracer.front());
			testTracer.pop_front();
			
			const int oldFront_predID = oldFront->predID;
			
			
			if (oldFront->nodeType == ATS_CONTAINER) {
				if (containers[oldFront_predID] <= 0) {
					//delete oldFront;
				} else  {
								
					if (APPLICABILITYTEST_DEBUG) {
						cout << "Satisfaction count of container " << oldFront_predID << " is " << containers[oldFront->predID] << endl;
					}
					if (oldFront->subPreconditionProgress != oldFront->subPreconditions->end()) {
	
						testTracer.push_front(oldFront.get());
						testTracer.push_front(new SearchPosition(ATS_PRECONDITION, *(oldFront->subPreconditionProgress), 0,0,0));
						++(oldFront->subPreconditionProgress);
						oldFront.release();
						
					} else if (oldFront->subNegativePreconditionProgress != oldFront->subNegativePreconditions->end()){
						testTracer.push_front(oldFront.get());
						testTracer.push_front(new SearchPosition(ATS_NEGATIVEPRECONDITION, *(oldFront->subNegativePreconditionProgress), 0,0,0));
						++(oldFront->subNegativePreconditionProgress);
						oldFront.release();
					
					} else if (oldFront->subContainerProgress != oldFront->subContainers->end()) {
						testTracer.push_front(oldFront.get());
						testTracer.push_front(new SearchPosition(ATS_CONTAINER, *(oldFront->subContainerProgress), &getSubContainer(*(oldFront->subContainerProgress)), &getSubPreconditions(*(oldFront->subContainerProgress)), &getSubNegativePreconditions(*(oldFront->subContainerProgress))));
						++(oldFront->subContainerProgress);
						oldFront.release();
						
					} else {
						if (isEssentialContainer(oldFront_predID)) {
//							bool was = staticInf->verbose;
//							staticInf->verbose = false;
							if (APPLICABILITYTEST_DEBUG) {
								
								cout << "Container " << oldFront_predID << " is essential and non-satisfied" << endl;
								cout << "Currently contains '" << containers[oldFront_predID] << "'" << endl;
								cout << "Default would be " << actionConcerned->getDefaultSatisfactionCounters()[oldFront_predID] << endl;
							}
							slist<int> contToProcess;
							contToProcess.push_front(oldFront_predID);
							
							waitingFor.clear();
							negativeWaitingFor.clear();
							
							//cout << "********" << endl;
							
							if (APPLICABILITYTEST_DEBUG) cout << "Waiting on one of:" << endl;
							
							
							
							while (!contToProcess.empty()) {
							
								int cont = contToProcess.front();
								contToProcess.pop_front();
								
								if (true) {
								
								list<int>::const_iterator itrS = getSubPreconditions(cont).begin();
								const list<int>::const_iterator itrE = getSubPreconditions(cont).end();
								
								for (;itrS != itrE; ++itrS) {
									
									if (!happyPreconditions[*itrS]) {
										AbstractPredicate wf(actionConcerned->preconditions[*itrS]);
										
										const int limit = wf.predType->getArgumentCount();
										for (int i = 0; i < limit; ++i) {
										
											if (!wf.hardParameters[i]) {
										
												const int tP = wf.parameters[i];
												
												if (tP >= 0) {
													wf.hardParameters[i] = argAssignments[tP];
												} else {
													wf.hardParameters[i] = actionConcerned->constants[-1 - tP];
												}
										
												
										
											}
										
											
										}
										
										if (APPLICABILITYTEST_DEBUG) {
											cout << wf.predType->name();
											for (int i = 0; i < limit; ++i) {
												if (wf.hardParameters[i] != 0) {
													cout << " " << wf.hardParameters[i]->name();
												} else {
													cout << " <>";
												}
											}
											cout << endl;
										
										}
										
										waitingFor.push_back(wf);
										
										
									} else {
										if (APPLICABILITYTEST_DEBUG) {
											cout << "Can't wait on ";
											AbstractPredicate wf(actionConcerned->preconditions[*itrS]);
											
											cout << wf.predType->name();
										
											for (int i = 0; i < wf.predType->getArgumentCount(); i++) {
											
												if (wf.hardParameters[i] == 0) {
											
													int tP = wf.parameters[i];
													
													if (tP >= 0) {
														wf.hardParameters[i] = argAssignments[tP];
													} else {
														wf.hardParameters[i] = actionConcerned->constants[-1 - tP];
													}
											
													
											
												}
											
												
												if (wf.hardParameters[i] != 0) {
													cout << " " << wf.hardParameters[i]->name();
												} else {
													cout << " <>";
												}
												
											
											}
											
											cout << " - it is already satisfied" << endl;
										}
									}
								
								}
								
								}
								
								if (true) {
								
								list<int>::const_iterator itrS = getSubNegativePreconditions(cont).begin();
								const list<int>::const_iterator itrE = getSubNegativePreconditions(cont).end();
								
								for (;itrS != itrE; ++itrS) {
									if (!happyNegativePreconditions[*itrS]) {
										AbstractPredicate wf(actionConcerned->negativePreconditions[*itrS]);
										
										const int limit = wf.predType->getArgumentCount();
										for (int i = 0; i < limit; ++i) {
										
											if (!wf.hardParameters[i]) {
										
												const int tP = wf.parameters[i];
												
												if (tP >= 0) {
													wf.hardParameters[i] = argAssignments[tP];
												} else {
													wf.hardParameters[i] = actionConcerned->constants[-1 - tP];
												}
										
												
										
											}
										
										}
										
										negativeWaitingFor.push_back(wf);
										if (APPLICABILITYTEST_DEBUG) {
											cout << "¬(" << wf.predType->name();
											for (int i = 0; i < limit; ++i) {
										
												if (wf.hardParameters[i]) {
													cout << " " << wf.hardParameters[i]->name();
												} else {
													cout << " <>";
												}
												
											}
											cout << ")" << endl;
										
										}
									} else {
										if (APPLICABILITYTEST_DEBUG) {
											cout << "Can't wait on ¬(";
											AbstractPredicate wf(actionConcerned->negativePreconditions[*itrS]);
											
											cout << wf.predType->name();
											for (int i = 0; i < wf.predType->getArgumentCount(); i++) {
											
												if (wf.hardParameters[i] == 0) {
											
													int tP = wf.parameters[i];
													
													if (tP >= 0) {
														wf.hardParameters[i] = argAssignments[tP];
													} else {
														wf.hardParameters[i] = actionConcerned->constants[-1 - tP];
													}
											
													
											
												}
											
												if (wf.hardParameters[i] != 0) {
													cout << " " << wf.hardParameters[i]->name();
												} else {
													cout << " <>";
												}
												
											
											}
											
											cout << ") - it is already satisfied" << endl;
										}
									}
									
									
								}
								
								}
								
								if (true) {
								
								list<int>::const_iterator itrS = getSubContainer(cont).begin();
								const list<int>::const_iterator itrE = getSubContainer(cont).end();
								
								for (; itrS != itrE; ++itrS) {
								
									contToProcess.push_front(*itrS);
							
								}
								
								}
								
								
							}
							testTracer.push_front(oldFront.get());
							oldFront->subPreconditionProgress = oldFront->subPreconditions->begin();
							oldFront->subNegativePreconditionProgress = oldFront->subNegativePreconditions->begin();
							oldFront->subContainerProgress = oldFront->subContainers->begin();
							oldFront.release();
							
							waiting = true;
//							staticInf->verbose = was;
							return;
						} else {
							//delete oldFront;
						}
						
					}
				}
			} else if (oldFront->nodeType == ATS_PRECONDITION) {
			
				if (!happyPreconditions[oldFront_predID]) {
					AbstractPredicate * cap = &(actionConcerned->preconditions[oldFront_predID]);
					const Predicate* cp = cap->predType;
					const int limit = cp->getArgumentCount();
					
					Entity** localHardParameters = cap->hardParameters;
					int* localParameters = cap->parameters;
					
					
					
					Entity** tEntities = new Entity*[limit];
					
					
					
					for (int i = 0; i < limit; ++i) {
						if (localHardParameters[i]) {
							tEntities[i] = localHardParameters[i];
						} else {
							const int tP = localParameters[i];
							if (tP >= 0) {
								tEntities[i] = argAssignments[tP];
							} else {
								tEntities[i] = actionConcerned->constants[-1 - tP];
							}
						}
					}
					bool foundIt;
					if (cp->isStatic()) {
						foundIt = staticInf->StaticInitialState->contains(cp, tEntities);
					} else {
						foundIt = currState->contains(cp, tEntities);
					}
					
					if (foundIt) {
						if (APPLICABILITYTEST_DEBUG) {
							cout << "Precondition " << oldFront->predID << " satisfied" << endl;
						}
						satisfyPrecondition(oldFront_predID);
						
						//delete oldFront;
					} else if (isEssentialPrecondition(oldFront_predID)) {
						if (APPLICABILITYTEST_DEBUG) {
							cout << "Essential precondition " << oldFront_predID << " not satisfied:  ";
							cout << cp->name();
							for (int i = 0; i < limit; ++i) {
								if (tEntities[i]) {
									cout << " " << tEntities[i]->name();
								} else {
									cout << " <>";
								}
							}
							cout << endl;
						}
						if (cp->isStatic()) {
						
							if (APPLICABILITYTEST_DEBUG) cout << "Precondition is static, no chance of it appearing later" << endl;
							abort = true;
							delete [] tEntities;
							//delete oldFront;
							return;
						} else {
							AbstractPredicate wf(cp, limit, tEntities);
							/*for (int i = 0; i < limit; ++i) {
								wf.parameters[i] = -1;
								wf.hardParameters[i] = tEntities[i];
							}*/
							waitingFor.clear();
							negativeWaitingFor.clear();
							waitingFor.push_back(wf);
							waiting = true;
							//delete [] tEntities;
							testTracer.push_front(oldFront.release());
							return;
						}
					} else {
						//delete oldFront;
					}
					delete [] tEntities;
				} else {
					//delete oldFront;
				}
			
			} else if (oldFront->nodeType == ATS_NEGATIVEPRECONDITION) {
				if (!happyNegativePreconditions[oldFront_predID]) {
					AbstractPredicate * cap = &(actionConcerned->negativePreconditions[oldFront_predID]);
					
					const Predicate* cp = cap->predType;
					const int limit = cp->getArgumentCount();
					
					Entity** localHardParameters = cap->hardParameters;
					int* localParameters = cap->parameters;
					
					Entity** tEntities = new Entity*[limit];
					
					
					for (int i = 0; i < limit; ++i) {
						if (localHardParameters[i]) {
							tEntities[i] = localHardParameters[i];
						} else {
							const int tP = localParameters[i];
							if (tP >= 0) {
								tEntities[i] = argAssignments[tP];
							} else {
								tEntities[i] = actionConcerned->constants[-1 - tP];
							}
						}
						
					}
					bool foundIt;
					if (cp->isStatic()) {
						foundIt = staticInf->StaticInitialState->containsNegative(cp, tEntities);
					} else {
						foundIt = negState->containsNegative(cp, tEntities);
					}
					if (foundIt) {
						if (APPLICABILITYTEST_DEBUG) {
							cout << "Negative precondition " << oldFront_predID << " satisfied" << endl;
						}
						satisfyNegativePrecondition(oldFront_predID);
						//delete oldFront;
					} else if (isEssentialNegativePrecondition(oldFront_predID)) {
						if (APPLICABILITYTEST_DEBUG) {
							cout << "Essential negative precondition " << oldFront_predID << " not satisfied:  ";
							cout << cp->name();
							for (int i = 0; i < limit; ++i) {
								if (tEntities[i]) {
									cout << " " << tEntities[i]->name();
								} else {
									cout << " <>";
								}
							}
							cout << endl;
						}
						if (cp->isStatic()) {
							if (APPLICABILITYTEST_DEBUG) cout << "Negative precondition is static, no chance of it appearing later" << endl;
							abort = true;
							delete [] tEntities;
							//delete oldFront;
							return;
						
						} else {
							AbstractPredicate wf(cp, limit, tEntities);
							
							/*for (int i = 0; i < limit; ++i) {
								wf.parameters[i] = -1;
								wf.hardParameters[i] = tEntities[i];
							}*/
							waitingFor.clear();
							negativeWaitingFor.clear();
							negativeWaitingFor.push_back(wf);
							//delete [] tEntities;
							waiting = true;
							testTracer.push_front(oldFront.release());
							return;
						}
					} else {
						//delete oldFront;
					}
					
					delete [] tEntities;
				}
			} else {
				//delete oldFront;
			}
		}
	}
	
	void expand(list<ApplicabilityTestingState*>* & searchQueue) {
		if (APPLICABILITYTEST_DEBUG) {
			cout << "Expanding ATS at " << this << endl;
		}
		if (abort) return;

		//cout << actionConcerned->name() << endl;
				
		EntitySet::iterator itr = actionConcerned->getParameterDomain(assignmentsMade)->begin();
		const EntitySet::iterator itrEnd = actionConcerned->getParameterDomain(assignmentsMade)->end();
		
		for (	; itr != itrEnd; ++itr) {
		
			if (actType == ACT_STRIPS) {
				searchQueue->push_front(new ApplicabilityTestingState(static_cast<const StripsAction*>(actionConcerned), *this, *itr));
			} else {
				searchQueue->push_front(new ApplicabilityTestingState(static_cast<const ADLAction*>(actionConcerned), *this, *itr));
			}
			/*cout << "Added child for ";
			searchQueue.front()->writeStateName();
			cout << endl;	*/
		}
		
	};
};






void ActionApplicabilityTester::process(ApplicabilityTestingState * s, State * p, State * n) {
	/*cout << "Processing ATS at " << s << endl;
	cout << "(";
	s->writeStateName();
	cout << ")" << endl;*/
	s->checkApplicability(p, n);
	if (!s->abort) {
		if (s->waiting) {
			waitingQueue->push_back(s);		
		} else {
			if (s->isGrounded()) {
				//cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
				//cout << "Applicable action:" << endl;
				ActionInstance* newAI = s->getActionInstance();
				newActions.push_back(newAI);
				//(newActions.back())->briefWrite(cout);
				//cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
				
				if (s->dependent != 0) {
					//cout << "Noting that ";
					//newAI->briefWrite(cout);
					//cout << " is a sub-action of ";
					//s->dependent->briefWrite(cout);
					(*actionDependencies)[s->dependent].push_back(newAI);
				
				}
				
				
				if (newAI->getParentAction()->hasSubActions()) {
					//cout << "Action has sub actions - ";
					//newAI->briefWrite(cout);
					
					int lim = newAI->getParentAction()->getSubActionCount();
					
					for (int i = 0; i < lim; ++i) {
					
						//cout << "Sub action " << i << " - ";
						
						Action* ca = newAI->getParentAction()->getSubAction(i);
						
						//cout << ca->name();
						//cout << endl;
						
						ca->visit(this);
						
						ApplicabilityTestingState* currATS = waitingQueue->back();
						
						waitingQueue->pop_back();
						
						currATS->assignmentsMade = newAI->getParentAction()->parameterCount;
						for (int j = 0; j < currATS->assignmentsMade; j++) {
							currATS->argAssignments[j] = s->argAssignments[j];
							
						}
						
						currATS->dependent = newAI;
						
						processQueue->push_back(currATS);
						
					}
				}
			} else {
				//cout << "Action is not grounded: assignments made to ATS = " << s->assignmentsMade << endl;
				s->expand(processQueue);
				
			}
			delete s;
			
			
		}
	} else {
		delete s;
	}
	
}




ActionApplicabilityTester::ActionApplicabilityTester(StaticInformation * staticInfIn)
  : actionDependencies(new map<ActionInstance*, ActionInstanceList, lt<ActionInstance> >()),
    staticInf(staticInfIn),
    waitingQueue(new list<ApplicabilityTestingState*>()),
    processQueue(0)
     {
	
	for (int i = 0; i < staticInf->numberOfActions; i++) {
		if (!staticInf->Actions[i]->isMacroAction) {
			staticInf->Actions[i]->visit(this);
		} else {
			//cout << "Disregarding " << staticInf->Actions[i]->name() << " - it is a macro action" << endl;
		}
	}
	
	for (int i = 0; i < staticInf->numberOfDerivedPredicateActions; i++) {
		staticInf->DerivedPredicateActions[i]->visit(this);
	}
};

ActionApplicabilityTester::~ActionApplicabilityTester() {
	
	list<ApplicabilityTestingState*>::iterator rmItr = waitingQueue->begin();
	const list<ApplicabilityTestingState*>::iterator rmEnd = waitingQueue->end();
	
	for (;rmItr != rmEnd; ++rmItr) {
		delete *rmItr;
	}
	
	delete waitingQueue;
	
};

void ActionApplicabilityTester::makeNextActions(State * positiveState, State * negativeState) {

	if (APPLICABILITYTEST_DEBUG) cout << "Making new batch of actions" << endl;
	newActions.clear();
	newPredicates.clear();
	
	processQueue = waitingQueue;
	waitingQueue = new list<ApplicabilityTestingState*>();
	
	while (!processQueue->empty()) {
		//cout << "ProcessQueue of size " << processQueue.size() << endl;
		ApplicabilityTestingState* n = processQueue->front();
		processQueue->pop_front();
		process(n, positiveState, negativeState);
	}
	
	delete processQueue;
	
	
}




void ActionApplicabilityTester::visitStripsAction(const StripsAction * c) {
	(const_cast<StripsAction*>(c))->calculateParameterDependencies();
	waitingQueue->push_back(new ApplicabilityTestingState(c, staticInf->InitialState, staticInf));
	
	if (APPLICABILITYTEST_DEBUG) cout << "Created initial ATS at " << waitingQueue->back() << " for STRIPS action " << c->name() << endl;
};

void ActionApplicabilityTester::visitADLAction(const ADLAction * c) {
	(const_cast<ADLAction*>(c))->calculateParameterDependencies();
	waitingQueue->push_back(new ApplicabilityTestingState(c, staticInf->InitialState, staticInf));
	
	if (APPLICABILITYTEST_DEBUG) cout << "Created initial ATS at " << waitingQueue->back() << " for ADL action " << c->name() << endl;
};

