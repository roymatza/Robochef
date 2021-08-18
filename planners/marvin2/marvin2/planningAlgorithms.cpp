#include "planningAlgorithms.h"
#include <values.h>
#include "relaxedPlanningGraph.h"
#include "SymmetryExtractor.h"
#include "SymmetryGroup.h"
#include "MacroAction.h"

#include "RelativeStateRecord.h"

#include "ActionInstanceWithSubactions.h"

#include <cassert>
#include <utility>
#include <cmath>

using std::make_pair;



//State initialState, State goalState, list<ActionInstance> ActionInstances, list<Entity> entities
struct SpareData {
	State * spareState;
	ActionInstanceList * spareHA;
	int spareHeuristic;
	double spareTime;
	double childTime;
	ActionInstance * spareAction;
	bool discard;
	
	SpareData(State * s, ActionInstanceList * r, int h, double t, double c, ActionInstance * a)
	 : spareState(s), spareHA(r), spareHeuristic(h), spareTime(t), childTime(c), spareAction(a), discard(false) {

	 	a->incrUsage();
	 };

};

struct BFSSpareData {
	State * spareState;
	int spareHeuristic;
	double spareTime;
	double childTime;
	ActionInstanceList spareAction;
	
	BFSSpareData(State * s, const int & h, const double & t, const double & c, ActionInstanceList & a)
	 : spareState(s), spareHeuristic(h), spareTime(t), childTime(c), spareAction(a) {

	 	//a->incrUsage();
	 };

};

class Spare {

private:
	SpareData sd;
	bool used;
public:
	Spare(State * s, ActionInstanceList * r, int h, double t, double c, ActionInstance * a)
	 : sd(s,r,h,t,c, a), used(false) {};
	 
	~Spare() {
		if (!used) {
			if (sd.spareState != 0) delete sd.spareState;
			delete sd.spareHA;			
		}
		if (sd.spareAction->decrUsage()) delete sd.spareAction;
	}
	
	inline SpareData & getData() {
		used = true;
		return sd;
	}
	
};
class BFSSpare {

private:
	BFSSpareData sd;
	bool used;
public:
	BFSSpare(State * s, const int & h, const double & t, const double & c, ActionInstanceList & a)
	 : sd(s,h,t,c, a), used(false) {};
	 
	~BFSSpare() {
		if (!used) {
			if (sd.spareState != 0) delete sd.spareState;
			
		}
		//if (sd.spareAction->decrUsage()) delete sd.spareAction;
	}
	
	inline const BFSSpareData & getData() {
		used = true;
		return sd;
	}
	
};
struct SearchData {
	State * stateToUse;
	ActionInstanceList * helpful;
	Plan* planToUse;
	int currHeuristic;
	double currTime;
	Plan* cyclicalList;
	bool isMacro;
	
	SearchData(State * s, ActionInstanceList * r, Plan* p, int h, double t, Plan * c, const bool & m)
	 : stateToUse(s), helpful(r), planToUse(p), currHeuristic(h), currTime(t), cyclicalList(c), isMacro(m) {};

	SearchData(const SearchData & s)
	: stateToUse(s.stateToUse->copy()),
	  helpful(new ActionInstanceList(*(s.helpful))),
	  planToUse(new Plan(*(s.planToUse))),
	  currHeuristic(s.currHeuristic),
	  currTime(s.currTime),
	  cyclicalList(new Plan(*(s.cyclicalList))),
	  isMacro(s.isMacro) {};

};

class SearchQueueItem {

private:
	SearchData sd;
	bool used;
public:
	SearchQueueItem(State * s, ActionInstanceList * r, Plan* p, int h, double t, Plan * c, const bool & m)
	 : sd(s,r,p,h,t,c,m), used(false) {};
	
	SearchQueueItem(State * s, ActionInstanceList * r, Plan* p, int h, double t,const bool & m)
	 : sd(s,r,p,h,t,new Plan(),m), used(false) {};
	 
	SearchQueueItem(const SearchQueueItem & s) : sd(s.sd), used(s.used) {};

	~SearchQueueItem() {
		if (!used) {
			delete sd.stateToUse;
			delete sd.planToUse;
			delete sd.cyclicalList;
			delete sd.helpful;
		}
	}
	const SearchData & getData() {
		used = true;
		return sd;
	}
	
	int getHeuristic() const {
		return sd.currHeuristic;
	}
	
	bool isMacro() const {
		return sd.isMacro;
	}
};

struct BFSSearchData {
	State * stateToUse;
	Plan* planToUse;
	int currHeuristic;
	double currTime;
	int upTo;
	ActionInstanceWithSubActionsList* applicableActions;
	
	BFSSearchData(State * s, Plan* p, const int & h, const double & t)
	 : stateToUse(s), planToUse(p), currHeuristic(h), currTime(t), upTo(0), applicableActions(0) {};

	
};

class BFSSearchQueueItem {

private:
	BFSSearchData sd;
	bool used;
public:
	BFSSearchQueueItem(State * s, Plan* p, const int & h, const double & t)
	 : sd(s,p,h,t), used(false) {};
	
	
	~BFSSearchQueueItem() {
		if (!used) {
			delete sd.stateToUse;
			delete sd.planToUse;
		}
		if (sd.applicableActions != 0) delete sd.applicableActions;
	}
	inline BFSSearchData & getData() {
		used = true;
		return sd;
	}
	
	inline void deleteIfFinish() {
		used = false;
	}
	
	inline const int & getHeuristic() const {
		return sd.currHeuristic;
	}
	
	
};

void breakNextDot() {
};


Plan * planningAlgorithms::planEhc(StaticInformation * staticInf){

	if (staticInf->verbose) {
		cout << "Planning using EHC" << endl;
	}
	
	State * currState = staticInf->InitialState->copy();
	
	Plan * planActions = new Plan();


	if(currState->subsumes(staticInf->GoalPredicates, staticInf)) return planActions;

	
	ActionInstanceList::iterator actItr;

	
	//int time = 0;

//	staticInf->verbose = false;

	RelaxedPlanningGraph * rpg = new RelaxedPlanningGraph(*staticInf, currState);

	int bestHeuristic;

	Plan startingPlan = rpg->extractRelaxedPlan(*staticInf, bestHeuristic);

	delete rpg;

//	RelaxedPlanningGraph * masterRPG = new RelaxedPlanningGraph(*staticInf, currState, false);

	State * bestState = currState;

	list<State *> stateQueue;
	list<Plan* > planQueue;
	list<double> timeQueue;
	list<int> heuristicQueue;

	stateQueue.push_back(currState);
	planQueue.push_back(planActions);
	timeQueue.push_back(0);
	heuristicQueue.push_back(bestHeuristic);


	ActionInstanceList::iterator bestAction;
	ActionInstanceList toDelete;
	
	cout << "Search beginning" << endl;
	while(true){

		State* stateToUse = stateQueue.front();
		stateQueue.pop_front();

		Plan * planToUse = planQueue.front();
		planQueue.pop_front();
		
		double currTime = timeQueue.front();
		timeQueue.pop_front();

//		int currHeuristic = heuristicQueue.front();
		heuristicQueue.pop_front();

		if (staticInf->verbose) {
			cout << "Expanding node; plan so far:" << endl;
			cout << *planToUse << endl;
			cout << "Creating list of actions" << endl;
		}
		//staticInf->verbose = false;
		
		ActionInstanceList applicableActions = stateToUse->applicableActions(staticInf);
		
		State * tempCopy = stateToUse->copy();
		tempCopy->applyRelaxedActionsLocally(applicableActions, staticInf, true, true);
		
		
		
		delete tempCopy;
		//staticInf->verbose = true;

		if (staticInf->verbose) {
			cout << "Pruned list of actions" << endl;
		}
		
		
		
		bool foundBetter = false;
		
		actItr = applicableActions.begin();
		
		list<State *> spareStates;
		list<int> spareHeuristics;
		
		if (staticInf->verbose) {
			cout << "Iterating over list of actions" << endl;
		}
		//staticInf->verbose = false;	
		for(; actItr != applicableActions.end(); actItr++){

			//staticInf->verbose = true;	
			if (staticInf->verbose) {
				cout << "Applying action ";
				(*actItr)->briefWrite(cout);
				cout << " to produce new state" << endl;
			}
			//staticInf->verbose = false;
			State * nextState = stateToUse->applyAction(*actItr, staticInf);
			nextState->advanceTime();
			
			//int h = nextState->getMarginallyLessRubbishHeuristic(staticInf);
	
			// staticInf->verbose = true;	
			
			if (staticInf->verbose) {
				cout << "Generating relaxed planning graph" << endl;
			}
			
			// staticInf->verbose = false;
				
			RelaxedPlanningGraph * rpg = new RelaxedPlanningGraph(*staticInf, nextState);
			int heuristicToReturn;
			// staticInf->verbose = true;	
			if (staticInf->verbose) {
			
				cout << "Extracting relaxed planning graph heuristic" << endl;
			}				
			Plan relaxedPlan = rpg->extractRelaxedPlan(*staticInf, heuristicToReturn);
			if (staticInf->verbose) {
			
				cout << "Extracted relaxed planning graph heuristic" << endl;
				cout << "  = " << heuristicToReturn << endl;
			}
			
			int h = heuristicToReturn;
			
			
			delete rpg;
			
			if(h == 0){
				if (staticInf->verbose) {
					cout << "Heuristic 0 returned - solution found!" << endl;
					cout << *nextState << endl;
				}
				ActionInstance * a = (*actItr);
				
				planToUse->addStepAtEnd(a, currTime);
				delete nextState;
				if (foundBetter) {
					delete bestState;
					delete *bestAction;
				}
				while (!stateQueue.empty()) {
					delete stateQueue.front();
					stateQueue.pop_front();
				}
				if (staticInf->verbose) {
					cout << "Clearing spare states" << endl;
				}
				while (!spareStates.empty()) {
					delete spareStates.front();
					spareStates.pop_front();
				}
				if (staticInf->verbose) {
					cout << "Clearing spare action instances" << endl;
				}
				while (!toDelete.empty()) {
					delete toDelete.front();
					toDelete.pop_front();
				}
				if (staticInf->verbose) {
					cout << "BFS queue cleared" << endl;
				}
				while (!planQueue.empty()) {
					delete planQueue.front();
					planQueue.pop_front();
				}
				timeQueue.clear();
				heuristicQueue.clear();
				
				spareHeuristics.clear();
				return planToUse;
			}
			

						
			if(h < bestHeuristic) {
				if (staticInf->verbose) {
					cout << "Heuristic returned - " << h << " - is better than the best so far, " << bestHeuristic << endl;
				}
				if (foundBetter) {
					spareStates.push_back(bestState);
					spareHeuristics.push_back(bestHeuristic);
					toDelete.push_back(*bestAction);
					
				}
				bestHeuristic = h;
				bestState = nextState;
				bestAction = actItr;
				foundBetter = true;
			} else {
				if (staticInf->verbose) {
					cout << "Heuristic returned - " << h << " - is no better than the best so far, " << bestHeuristic << endl;
				}
				spareStates.push_back(nextState);
				spareHeuristics.push_back(h);
				toDelete.push_back(*actItr);
				
			}
			
		}
	
		if (foundBetter) {
			if (staticInf->verbose) {
				cout << "Better state found; clearing BFS queue" << endl;
			}
			while (!stateQueue.empty()) {
				delete stateQueue.front();
				stateQueue.pop_front();
			}
			while (!planQueue.empty()) {
				delete planQueue.front();
				planQueue.pop_front();
			}
			timeQueue.clear();
			heuristicQueue.clear();
			
			
			if (staticInf->verbose) {
				cout << "Clearing spare states" << endl;
			}
			while (!spareStates.empty()) {
				delete spareStates.front();
				spareStates.pop_front();
			}
			if (staticInf->verbose) {
				cout << "Clearing spare action instances" << endl;
			}
			while (!toDelete.empty()) {
				delete toDelete.front();
				toDelete.pop_front();
			}
			
			spareHeuristics.clear();
			
			if (staticInf->verbose) {
				cout << "BFS queue cleared" << endl;
			}
	
			
			
			planToUse->addStepAtEnd(*bestAction, currTime);
			if (staticInf->verbose) {
				cout << "Best plan is now:" << endl;
				cout << *planToUse;
			}
			currTime++;
			
			stateQueue.push_back(bestState);
			planQueue.push_back(planToUse);
			timeQueue.push_back(currTime);
			heuristicQueue.push_back(bestHeuristic);

		
		} else {
			if (staticInf->verbose) {
				cout << "No better state found: doing BFS" << endl;
			}
			
				
			while (!spareStates.empty()) {
				// staticInf->verbose = false;
				ActionInstance* toApply = toDelete.front();
				State * nextState = spareStates.front();
				
				Plan * nextPlan = new Plan(*planToUse);
				nextPlan->addStepAtEnd(toApply, currTime);
				
				// staticInf->verbose = true;
				if (staticInf->verbose) {
					cout << "Added child for applying";
					toApply->briefWrite(cout);
					cout << " at time " << currTime << endl;
				}
				// staticInf->verbose = false;
				int h = spareHeuristics.front();
				//h += currTime + 1; // do A* instead of BFS
				
				if (heuristicQueue.empty()) {
					stateQueue.push_back(nextState);
					planQueue.push_back(nextPlan);
					heuristicQueue.push_back(h);
					timeQueue.push_back(currTime + 1);
				} else {
					list<State*>::iterator sqItr = stateQueue.begin();
					list<Plan*>::iterator pqItr = planQueue.begin();
					list<int>::iterator hqItr = heuristicQueue.begin();
					list<double>::iterator tqItr = timeQueue.begin();
					
										
					while (hqItr != heuristicQueue.end() && (*hqItr) <= h) {
						sqItr++;
						pqItr++;
						hqItr++;
						tqItr++;
					}
					
					stateQueue.insert(sqItr, nextState);
					planQueue.insert(pqItr, nextPlan);
					heuristicQueue.insert(hqItr, h);
					timeQueue.insert(tqItr, currTime + 1);
					
						
				}

				
				
				spareStates.pop_front();
				spareHeuristics.pop_front();
				toDelete.pop_front();
				// staticInf->verbose = true;
			}
			
			cout << "Clearing up parent plan" << endl;
					
			delete planToUse;
			
		}
		
		cout << "Clearing up parent state" << endl;
		delete stateToUse;
		
	}
	
	return planActions;
}

void breakHere() {

}

void partitionActions(auto_ptr<ActionInstanceWithSubActionsList> & applicable, ActionInstanceList * helpful, ActionInstanceWithSubActionsList & helpfulActs, ActionInstanceWithSubActionsList & spareActs, list<SymmetryGroup*> * exemplars);

void localSearchSample(StaticInformation* staticInf, Plan* planToUse, ActionInstanceWithSubActionsList & helpfulApplicableActions, ActionInstanceWithSubActionsList & spareApplicableActions, const int method=0);

void sampleSpares(StaticInformation* staticInf, Plan* planToUse, list<Spare*> & spares, const int method=0);

void rouletteSelect(StaticInformation* staticInf, Plan* planToUse, list<Spare*> & spares, const double bias);

Plan * planningAlgorithms::planLookAheadEhc(StaticInformation * staticInf, const bool & inferMacro, const bool & concurrent, const bool & derivedPredicates, const bool & symmPrune, const bool onlyBFS, const bool noBFS, const bool bestFSPlateau){

	bool onlyHelpful = true;
	bool firstBestAction = true;
	
	if (staticInf->verbose) {
		cout << "Planning using look-ahead EHC" << endl;
	}
	
	State * currState = staticInf->InitialState->copy();
	
	
	
	Plan * planActions = new Plan();
//	cout << "New plan at " << planActions << endl;
	// staticInf->verbose = false;
	if(currState->subsumes(staticInf->GoalPredicates, staticInf) && !currState->intersectsWith(staticInf->NegativeGoalPredicates, staticInf)) {
		cout << "Goals satisfied in initial state" << endl;
		delete currState;
		return planActions;
	}

	ActionInstanceWithSubActionsList::iterator actItr;
	ActionInstanceWithSubActionsList::iterator actEnd;
	if(staticInf->verbose) {
		cout << "creating relaxed planning graph" << endl;
	}
	

	

	
	
	
	
	
	/*if(staticInf->verbose) {
		cout << "extracting landmarks..." << endl;
	}

	list<PredicateInstance*> landmarks = rpg->extractLandmarks(*staticInf);

	if(staticInf->verbose) {

		list<PredicateInstance*>::iterator itr = landmarks.begin();
		cout << "extracted landmarks; they are:" << endl;

		for(; itr != landmarks.end(); itr++){

			cout << *(*itr) << endl;
		}
	}
*/
	
	
	masterRPG = new RelaxedPlanningGraph(*staticInf, staticInf->InitialState, false);
	if (!masterRPG->didGoalsAppear()) {
		cout << "Goals did not appear in the relaxed planning graph from the initial state" << endl;
		
		
		delete currState;
		return planActions;
	
	}
	
	masterRPG->makeSupportStructure(staticInf);

	RelativeStateRecord alreadyVisited;
	HeuristicCache alreadyCalculated;

	int bestHeuristic;
	
	auto_ptr<ActionInstanceList> freeActions;
	
	if (!onlyBFS) {	
	
		SearchQueueItem * restartTo = 0;
		int totalRestarts = 0;
		int restartsAtDepth = 0;
		int depthCutOff = 20;
		int depth = 0;
		int rootCount = 0;
		if (derivedPredicates) {
			
			freeActions = masterRPG->freeActions(*staticInf, currState);
			
			while (!freeActions->empty()) {
				//cout << freeActions->size() << " free actions at the start" << endl;
				currState->applyActionsLocally(*(freeActions.get()), staticInf);
				currState->advanceTime();
				currState->timeBackOne();
				freeActions = masterRPG->freeActions(*staticInf, currState);
			} 
			
			//cout << "No more free actions at the start" << endl;
		}
		RelaxedPlanningGraph * rpg = new RelaxedPlanningGraph(*masterRPG, *staticInf, currState);
	
		if (!rpg->didGoalsAppear()) {
		
			cout << "This should not happen: goals did not appear in the incremental relaxed planning graph from the initial state" << endl;
			
			delete rpg;
			return planActions;
		
		}
		
		
		
		if(staticInf->verbose) {
			cout << "extracting initial heuristic" << endl;
		}
		//cout << "Derived initial state: " << endl;
		//cout << *currState << endl;
		
		Plan startingPlan = rpg->extractRelaxedPlanQuickly(*masterRPG, *staticInf, bestHeuristic);
		
		
		
		State * bestState = currState;
		double bestEATime;
		State * gotStuckAt = 0;
		double stuckAtTime = 0;
		double timeAtStart;
		
		ActionInstanceList * bestHA = new ActionInstanceList(startingPlan.helpfulActions());
	
		
		
		
		
		list<SearchQueueItem* > searchQueue;
		searchQueue.push_back(new SearchQueueItem(currState, bestHA, planActions, bestHeuristic, 0, false));
		
		
		ActionInstance* bestAction;
			
		bool firstExpansion = true;
		
		//cout << "Initial heuristic = " << bestHeuristic << endl;
		//cout << "From relaxed plan:" << endl;
		//cout << startingPlan << endl;
		
		delete rpg;
		
		cout << "Search beginning" << endl;
		// staticInf->verbose = false;
		
		bool doingPlateauLocalSearch = false;		
		
		while(!searchQueue.empty()){
			
			cout << " ";
			cout.flush();
			
			
			
			list<State*> statesToDelete;
			list<ActionInstanceWithSubActionsList> allSpareActions;
			list<ActionInstanceWithSubActionsList> allHelpfulActions;
			list<double> correspondingTimes;
			
			
			State * sState = searchQueue.front()->getData().stateToUse;	
			ActionInstanceList * haToUse = searchQueue.front()->getData().helpful;
			Plan * planToUse = searchQueue.front()->getData().planToUse;
			double currTime = searchQueue.front()->getData().currTime;
			int currHeuristic = searchQueue.front()->getData().currHeuristic;
			Plan* cyclicalList = searchQueue.front()->getData().cyclicalList;
			timeAtStart = currTime;
			delete searchQueue.front();
			searchQueue.pop_front();
	
			
			if (staticInf->verbose) {
				cout << "Expanding node; plan so far:" << endl;
				cout << *planToUse << endl;
				//cout << "State to use : " << endl;
				//cout << *sState << endl;
				
				cout << "Creating list of actions" << endl;
			}
			
			bool stillLoop = true;
			
			statesToDelete.push_back(sState);
			correspondingTimes.push_back(currTime);
			bool foundBetter = false;
			
			double bestTime = currTime;
		
			list<Spare*> spares;
			
			while (stillLoop) {
			
				State* stateToUse = statesToDelete.back();
				
				auto_ptr<ActionInstanceWithSubActionsList> applicableActions;

				ActionInstanceWithSubActionsList helpfulApplicableActions;
				ActionInstanceWithSubActionsList spareApplicableActions;
				
				
				if (symmPrune) {
					SymmetryExtractor symm;
				
					list<SymmetryGroup*> * symGroups = symm.extractSymmetryGroups(*staticInf, true, stateToUse);
					const EntitySet* const exemplars = (EntitySet*)0;
					
				
				
					auto_ptr<ActionInstanceWithSubActionsList> rawApplicableActions = masterRPG->applicableRelaxedActions(*staticInf, stateToUse, exemplars);
					applicableActions = stateToUse->filterApplicableActions(staticInf, *(rawApplicableActions.get()));
					
					partitionActions(applicableActions, haToUse, helpfulApplicableActions, spareApplicableActions, symGroups);

					while (!symGroups->empty()) {
						delete symGroups->front();
						symGroups->pop_front();
					}
				
					delete exemplars;

					
				
					
				} else {
					const EntitySet* exemplars = (EntitySet*)0;
					auto_ptr<ActionInstanceWithSubActionsList> rawApplicableActions = masterRPG->applicableRelaxedActions(*staticInf, stateToUse, exemplars);
					applicableActions = stateToUse->filterApplicableActions(staticInf, *(rawApplicableActions.get()));

					ActionInstanceWithSubActionsList::iterator moveSpareItr = applicableActions->begin();
					ActionInstanceWithSubActionsList::iterator moveSpareEnd = applicableActions->end();
					
					for(; moveSpareItr != moveSpareEnd; ++moveSpareItr) {
					
						
						/*cout << "Applicable action: ";
						(*moveSpareItr)->getFirst()->briefWrite(cout);*/
						bool isHelpful = false;
						
						if ((*moveSpareItr)->getFirst()->isMacroAction()) {
						
							isHelpful = (((MacroAction*) ((*moveSpareItr)->getFirst()->getParentAction())))->isMarvinAction();
							
						
						} else {
							for (ActionInstanceList::iterator findHItr = haToUse->begin(); !isHelpful && findHItr != haToUse->end(); ++findHItr) {
								isHelpful = (*((*moveSpareItr)->getFirst()) == *(*findHItr));
							}
						}
						
						if (isHelpful) {
							helpfulApplicableActions.push_back(*moveSpareItr);
							
							
						} else {
							spareApplicableActions.push_back(*moveSpareItr);
							
						}
						
						
					
						
					}
				}
				
				if (doingPlateauLocalSearch) {
					
					if (restartsAtDepth < 50) spareApplicableActions.clear();
					const int sizeBefore = helpfulApplicableActions.size();
					localSearchSample(staticInf, planToUse, helpfulApplicableActions, spareApplicableActions);
					const int sizeAfter = helpfulApplicableActions.size();
					const int diff = sizeBefore - sizeAfter;
					for (int printUs = 0; printUs < diff; ++printUs) {
						cout << "_";
					}
					cout.flush();
				}
				
				if (onlyHelpful) spareApplicableActions.clear();
				//cout << rawApplicableActions.size() << " actions applicable relaxedly" << endl;
				
				
				
				//cout << applicableActions.size() << " actions applicable normally" << endl;
				
				
				//ActionInstanceList applicableActions = stateToUse->applicableActions(staticInf);
				
				
				//State * tempCopy = stateToUse->copy();
				//tempCopy->applyRelaxedActionsLocally(applicableActions, staticInf, true, true);
				
				
				//delete tempCopy;
				
				
				
				if (staticInf->verbose) {
					cout << "Pruned list of actions" << endl;
				}
				
				
		
				
				
				
				
				
				
				
				
				
				
				/*cout << haToUse->size() << " helpful actions, of which" << endl;
				cout << helpfulApplicableActions.size() << " are applicable helpful actions" << endl;
				*/
				allSpareActions.push_back(spareApplicableActions);
				allHelpfulActions.push_back(helpfulApplicableActions);		
				
				for (int i = 0; i < staticInf->numberOfActions; i++) {
				 
					if (staticInf->Actions[i]->isMacroAction) {
						MacroAction* currMA = static_cast<MacroAction*>(staticInf->Actions[i]);
						//cout << "*** Applicability-testing " << currMA->name() << endl;
						if (currMA->isMarvinAction()) {
							auto_ptr<ActionInstanceList> na;
							if (onlyHelpful) {
								na = currMA->derivedActions(helpfulApplicableActions, stateToUse, masterRPG, staticInf);
							} else {
								na = currMA->derivedActions(*(applicableActions.get()), stateToUse, masterRPG, staticInf);
							}
							ActionInstanceList::iterator naItr = na->begin();
							ActionInstanceList::iterator naEnd = na->end();
							//cout << "**** Done, applicable:" << endl;
							for (;naItr != naEnd; ++naItr) {
								//(*naItr)->briefWrite(cout);
								
								
								helpfulApplicableActions.push_front(new ActionInstanceWithSubActions(*naItr));
								
								
							}
						}
						//cout << "**** (end)" << endl;
					}
								 
				}
				
				
				
				
				
				
				
				actItr = helpfulApplicableActions.begin();
				actEnd = helpfulApplicableActions.end();
				
				
				if (staticInf->verbose) {
					cout << "Iterating over list of actions" << endl;
				}
					
				for(; (!foundBetter || !firstBestAction) && actItr != actEnd; ++actItr){
		
					
					
					if (staticInf->verbose) {
						cout << endl << "Applying action ";
						(*actItr)->getFirst()->briefWrite(cout);
						cout << " at time " << currTime << " to produce new state";
					}
					//cout << "State was:" << endl;
					//cout << *stateToUse << endl;
					ActionInstanceList actionsToApply;
					
					actionsToApply.push_back(const_cast<ActionInstance*>((*actItr)->getFirst()));
					
					ActionInstanceList::iterator rseItr = (*actItr)->getSecond().begin();
					ActionInstanceList::iterator rseEnd = (*actItr)->getSecond().end();
							
					for (; rseItr != rseEnd; ++rseItr) {
					
						actionsToApply.push_back(*rseItr);
						if (staticInf->verbose) {
							cout << " + sub-action ";
							(*rseItr)->briefWrite(cout);
						}
					}
					
					//cout << "Passing " << actionsToApply.size() << " actions to apply" << endl;
					
					State * nextState = stateToUse->applyActions(actionsToApply, masterRPG, staticInf);
					
					double eaTime = currTime;
					
					if (nextState->checkEventQueueConsistency()) {
						if (gotStuckAt != 0 || !concurrent) {
							while (nextState->hasQueuedEffects()) {
								nextState->advanceTime();
								eaTime++;
							}
							if (derivedPredicates) {
								freeActions = masterRPG->freeActions(*staticInf, nextState);
								while (!freeActions->empty()) {
									nextState->applyActionsLocally(*(freeActions.get()), staticInf);
									nextState->advanceTime();
									nextState->timeBackOne();
									freeActions = masterRPG->freeActions(*staticInf, nextState);
								}
							} 
						}
						if (gotStuckAt == 0 || alreadyVisited.needToVisit(nextState)) {
							cout << ".";
							cout.flush();
							breakNextDot();
							//int h = nextState->getMarginallyLessRubbishHeuristic(staticInf);
					
								
							//staticInf->verbose = true;
							if (staticInf->verbose) {
								cout << "Generating relaxed planning graph" << endl;
							}
							
							
							/*cout << "Applying action ";
							(*actItr)->briefWrite(cout);*/
							
							CachedHeuristic * lookupHeuristic = 0;
							auto_ptr<StateDiff> theDiff(0);
							bool nogood = false;
							if (doingPlateauLocalSearch) {
								theDiff = alreadyCalculated.getDiff(nextState);
								lookupHeuristic = alreadyCalculated.isCached(theDiff);
								if (lookupHeuristic) nogood = lookupHeuristic->deadend;
							}
							RelaxedPlanningGraph * nextRPG = (lookupHeuristic ? 0 : new RelaxedPlanningGraph(*masterRPG, *staticInf, nextState));
							
							/*
							RelaxedPlanningGraph * testRPG = new RelaxedPlanningGraph(*staticInf, nextState);
							
							if (!(*testRPG == *nextRPG)) {
								cout << "Duff incremental RPG constructor" << endl;
								exit(0);
							} else {
								cout << endl;
							}
							delete testRPG;
							
							*/
							
							if (nogood || (!lookupHeuristic && !nextRPG->didGoalsAppear())) {
								if (staticInf->verbose) {
									cout << " Dead end" << endl;
								} else {
									cout << "d";
									//cout << *nextState << endl;
									breakHere();
									cout.flush();
								}
								if (doingPlateauLocalSearch && !lookupHeuristic) {
									lookupHeuristic = new CachedHeuristic();
									alreadyCalculated.cache(theDiff, lookupHeuristic);
									delete lookupHeuristic;
								}
								delete nextState;
								delete nextRPG;
								
							} else {
								int heuristicToReturn;
								//cout << "_";
								//cout.flush();
								if (staticInf->verbose) {
								
									cout << "Extracting relaxed planning graph heuristic" << endl;
								}				
								//Plan relaxedPlan = nextRPG->extractRelaxedPlan(*staticInf, heuristicToReturn);
								//int heuristicToIgnore;
								Plan relaxedPlan = nextRPG->extractRelaxedPlanQuickly(*masterRPG, *staticInf, heuristicToReturn);
								
							
								//cout << " heuristic   = " << heuristicToReturn << endl;
								
								if (staticInf->verbose) {
								
									cout << " heuristic   = " << heuristicToReturn << endl;
									//cout << " heuristic 2 = " << heuristicToIgnore << endl;
									cout << "From relaxed plan:" << endl;
									cout << relaxedPlan;
									//cout << " or " << endl;
									//cout << relaxedPlanTwo;
								}
								
								ActionInstanceList* nextHA = new ActionInstanceList(relaxedPlan.helpfulActions());
								int h = heuristicToReturn;
								
								
				
								
								if(h == 0){
									if (staticInf->verbose) {
										cout << "Heuristic 0 returned - solution found!" << endl;
										cout << *nextState << endl;
									}
									const ActionInstance * a = (*actItr)->getFirst();
									
									planToUse->addStepAtEnd(const_cast<ActionInstance*>(a), currTime);
									
									delete nextRPG;
									
									delete gotStuckAt;
									gotStuckAt = 0;
								
									delete cyclicalList;
									
									delete nextHA;
									delete nextState;
									if (foundBetter) {
										spares.push_back(new Spare(bestState, bestHA, bestHeuristic, bestTime, bestEATime, bestAction));
										bestAction->decrUsage();
									}
									while (!searchQueue.empty()) {
										delete searchQueue.front();
										searchQueue.pop_front();
									}
									
									if (staticInf->verbose) {
										cout << "Clearing spare states" << endl;
									}
									while (!spares.empty()) {
										delete spares.front();
										spares.pop_front();
									}
									if (staticInf->verbose) {
										cout << "BFS queue cleared" << endl;
									}
									while (!statesToDelete.empty()) {
										delete statesToDelete.front();
										statesToDelete.pop_front();
									}
									delete haToUse;
									return planToUse;
								}
								
								
											
								if(h < bestHeuristic) {
									
									if (staticInf->verbose) {
										cout << "Heuristic returned - " << h << " - is better than the best so far, " << bestHeuristic << endl;
									}
									if (foundBetter) {
										spares.push_back(new Spare(bestState, bestHA, bestHeuristic, bestTime, bestEATime, bestAction));
										bestAction->decrUsage();
									}
									bestHeuristic = h;
									bestState = nextState;
									bestHA = nextHA;
									bestTime = currTime;
									bestEATime = eaTime;
									bestAction = const_cast<ActionInstance*>((*actItr)->getFirst());
									bestAction->incrUsage();
									foundBetter = true;
								} else {
									if (staticInf->verbose) {
										cout << "Heuristic returned - " << h << " - is no better than the best so far, " << bestHeuristic << endl;
									}
									spares.push_back(new Spare(nextState, nextHA, h, currTime, eaTime, const_cast<ActionInstance*>((*actItr)->getFirst())));
									
									
								}
								delete nextRPG;
							}
						} else {
							cout << "p";
							cout.flush();
							
							delete nextState;
						}
					} else {
						cout << "i";
						cout.flush();
						delete nextState;
					}
				}
			
				if (stateToUse->hasQueuedEffects()) {
					stillLoop = true;
					State* newState = stateToUse->copy();
					newState->advanceTime();
					currTime++;
					correspondingTimes.push_back(currTime);
					statesToDelete.push_back(newState);
				} else {
					stillLoop = false;
				}
				
				
			}
			
			list<State*>::iterator pastStateItr = statesToDelete.begin();
			list<ActionInstanceWithSubActionsList>::iterator pastActItr = allSpareActions.begin();
			
			list<double>::iterator pastTime = correspondingTimes.begin();
			
			for (; !foundBetter  && pastStateItr != statesToDelete.end(); ++pastStateItr, ++pastActItr, ++pastTime) {
			
				
				State* stateToUse = (*pastStateItr);
				ActionInstanceWithSubActionsList toApply = (*pastActItr);
				
				actItr = (*pastActItr).begin();
				actEnd = (*pastActItr).end();
				
				currTime = (*pastTime);
				
				if (staticInf->verbose) {
					cout << "Iterating over list of actions" << endl;
				}
					
				for(; (!foundBetter || !firstBestAction) && actItr != actEnd; ++actItr){
		
					double eaTime = currTime;
					
					if (staticInf->verbose) {
						cout << "Applying action ";
						(*actItr)->getFirst()->briefWrite(cout);
						cout << " at time " << currTime << " to produce new state";
					}
					
					
					//State * nextState = stateToUse->applyAction(*actItr, staticInf);
					
					ActionInstanceList actionsToApply;
					
					
					
					actionsToApply.push_back( const_cast<ActionInstance*>((*actItr)->getFirst()) );
					
					ActionInstanceList::iterator rseItr = (*actItr)->getSecond().begin();
					ActionInstanceList::iterator rseEnd = (*actItr)->getSecond().end();
							
					for (; rseItr != rseEnd; ++rseItr) {
					
						actionsToApply.push_back(*rseItr);
						
						
						if (staticInf->verbose) {
							cout << " + sub-action ";
							(*rseItr)->briefWrite(cout);
						}
					
					}
					
					//cout << "State was:" << endl;
					//cout << *stateToUse << endl;
					
					State * nextState = stateToUse->applyActions(actionsToApply, masterRPG, staticInf);
					
					if (nextState->checkEventQueueConsistency()) {
						if (gotStuckAt != 0 || !concurrent) {
							while (nextState->hasQueuedEffects()) {
								nextState->advanceTime();
								eaTime++;
							}
							if (derivedPredicates) {
								freeActions = masterRPG->freeActions(*staticInf, nextState);
								while (!freeActions->empty()) {
									nextState->applyActionsLocally(*(freeActions.get()), staticInf);
									nextState->advanceTime();
									nextState->timeBackOne();
									freeActions = masterRPG->freeActions(*staticInf, nextState);
								}
							} 
							
						}
						if (gotStuckAt == 0 || alreadyVisited.needToVisit(nextState)) {
							cout << ".";
							cout.flush();
							//int h = nextState->getMarginallyLessRubbishHeuristic(staticInf);
							//staticInf->verbose = true;
								
							
							if (staticInf->verbose) {
								cout << "Generating relaxed planning graph" << endl;
							}
							
							
							/*cout << "Applying action ";
							(*actItr)->briefWrite(cout);*/
							
							RelaxedPlanningGraph * nextRPG = new RelaxedPlanningGraph(*masterRPG, *staticInf, nextState);
							
							/*
							RelaxedPlanningGraph * testRPG = new RelaxedPlanningGraph(*staticInf, nextState);
							
							if (!(*testRPG == *nextRPG)) {
								cout << "Duff incremental RPG constructor" << endl;
								exit(0);
							} else {
								cout << endl;
							}
							delete testRPG;
							
							*/
							
							if (!nextRPG->didGoalsAppear()) {
								if (staticInf->verbose) {
									cout << " Dead end" << endl;
								} else {
									cout << "d";
									//cout << *nextState << endl;
									breakHere();
									cout.flush();
								}
								delete nextState;
								delete nextRPG;
								
							} else {
								int heuristicToReturn;
								
								if (staticInf->verbose) {
								
									cout << "Extracting relaxed planning graph heuristic" << endl;
								}				
								//Plan relaxedPlan = nextRPG->extractRelaxedPlan(*staticInf, heuristicToReturn);
								Plan relaxedPlan = nextRPG->extractRelaxedPlanQuickly(*masterRPG, *staticInf, heuristicToReturn);
								//cout << " heuristic   = " << heuristicToReturn << endl;
								if (staticInf->verbose) {
								
									cout << " heuristic   = " << heuristicToReturn << endl;
									
									cout << "From relaxed plan:" << endl;
									cout << relaxedPlan;
								}
								
								ActionInstanceList* nextHA = new ActionInstanceList(relaxedPlan.helpfulActions());
								int h = heuristicToReturn;
								
								
				
								
								if(h == 0){
									if (staticInf->verbose) {
										cout << "Heuristic 0 returned - solution found!" << endl;
										cout << *nextState << endl;
									}
									const ActionInstance * a = (*actItr)->getFirst();
									
									planToUse->addStepAtEnd( const_cast<ActionInstance*>(a), currTime);
									
									delete nextRPG;
									
									delete gotStuckAt;
									gotStuckAt = 0;
								
									delete cyclicalList;
									
									delete nextHA;
									delete nextState;
									if (foundBetter) {
										spares.push_back(new Spare(bestState, bestHA, bestHeuristic, bestTime, bestEATime, bestAction));
										bestAction->decrUsage();
									}
									while (!searchQueue.empty()) {
										delete searchQueue.front();
										searchQueue.pop_front();
									}
									
									if (staticInf->verbose) {
										cout << "Clearing spare states" << endl;
									}
									while (!spares.empty()) {
										delete spares.front();
										spares.pop_front();
									}
									if (staticInf->verbose) {
										cout << "BFS queue cleared" << endl;
									}
									
									while (!statesToDelete.empty()) {
										delete statesToDelete.front();
										statesToDelete.pop_front();
									}
									delete haToUse;
									return planToUse;
								}
								
								
											
								if(h < bestHeuristic) {
									
									if (staticInf->verbose) {
										cout << "Heuristic returned - " << h << " - is better than the best so far, " << bestHeuristic << endl;
									}
									if (foundBetter) {
										spares.push_back(new Spare(bestState, bestHA, bestHeuristic, bestTime, bestEATime, bestAction));
										bestAction->decrUsage();
									}
									bestHeuristic = h;
									bestState = nextState;
									bestHA = nextHA;
									bestTime = currTime;
									bestEATime = eaTime;
									bestAction =  const_cast<ActionInstance*>((*actItr)->getFirst());
									bestAction->incrUsage();
									foundBetter = true;
								} else {
									if (staticInf->verbose) {
										cout << "Heuristic returned - " << h << " - is no better than the best so far, " << bestHeuristic << endl;
									}
									spares.push_back(new Spare(nextState, nextHA, h, currTime, eaTime,  const_cast<ActionInstance*>((*actItr)->getFirst())));
									
									
								}
								delete nextRPG;
							}
						} else {
							//cout << "p";
							//cout.flush();
							delete nextState;
						}
					} else {
						delete nextState;
					}
					
				}
			
			
			}
			
			list<ActionInstanceWithSubActionsList>::iterator pastHelpfulActItr = allHelpfulActions.begin();
			pastStateItr = statesToDelete.begin();
			pastTime = correspondingTimes.begin();
			for (; !foundBetter && pastStateItr != statesToDelete.end(); pastStateItr++, pastHelpfulActItr++, pastTime++) {
				
			
				State* stateToUse = *pastStateItr;
				double currTime = *pastTime;
				
				ActionInstanceList mToApply;
				
				for (int i = 0; (!foundBetter || !firstBestAction) && i < staticInf->numberOfActions; i++) {
				 
					if (staticInf->Actions[i]->isMacroAction) {
						MacroAction* currMA = static_cast<MacroAction*>(staticInf->Actions[i]);
						//cout << "*** Applicability-testing " << currMA->name() << endl;
						if (!currMA->isMarvinAction()) {
							DASearchStatus searchStatus = currMA->beginDerivedActions(*pastHelpfulActItr, stateToUse);
							
							ActionInstance* nextAct = 0;
							do {
								nextAct = currMA->derivedActions(searchStatus, masterRPG, staticInf);

								if (nextAct) {
									if (staticInf->verbose) {
										cout << "Applying ";
										nextAct->briefWrite(cout);
									}
									//cout << "State was:" << endl;
									//cout << *stateToUse << endl;
									ActionInstanceList actionsToApply;
									
									actionsToApply.push_back(nextAct);
									
														
									State * nextState = stateToUse->applyActions(actionsToApply, masterRPG, staticInf);
									
									if (!nextState->checkEventQueueConsistency()) {
										delete nextState;
									} else {
									
										double eaTime = currTime;
										
										if (gotStuckAt != 0 || !concurrent) {
											while (nextState->hasQueuedEffects()) {
												nextState->advanceTime();
												eaTime++;
											}
											if (derivedPredicates) {
												freeActions = masterRPG->freeActions(*staticInf, nextState);
												while (!freeActions->empty()) {
													nextState->applyActionsLocally(*(freeActions.get()), staticInf);
													nextState->advanceTime();
													nextState->timeBackOne();
													freeActions = masterRPG->freeActions(*staticInf, nextState);
												}
											} 
										}
										if (gotStuckAt == 0 || alreadyVisited.needToVisit(nextState)) {
											cout << ".";
											cout.flush();
											breakNextDot();
											//int h = nextState->getMarginallyLessRubbishHeuristic(staticInf);
									
												
											//staticInf->verbose = true;
											if (staticInf->verbose) {
												cout << "Generating relaxed planning graph" << endl;
											}
											
											
											/*cout << "Applying action ";
											(*actItr)->briefWrite(cout);*/
											
											
											RelaxedPlanningGraph * nextRPG = new RelaxedPlanningGraph(*masterRPG, *staticInf, nextState);
											
											/*
											RelaxedPlanningGraph * testRPG = new RelaxedPlanningGraph(*staticInf, nextState);
											
											if (!(*testRPG == *nextRPG)) {
												cout << "Duff incremental RPG constructor" << endl;
												exit(0);
											} else {
												cout << endl;
											}
											delete testRPG;
											
											*/
											
											if (!nextRPG->didGoalsAppear()) {
												if (staticInf->verbose) {
													cout << " Dead end" << endl;
												} else {
													cout << "d";
													//cout << *nextState << endl;
													breakHere();
													cout.flush();
												}
												delete nextState;
												delete nextRPG;
												
											} else {
												int heuristicToReturn;
												//cout << "_";
												//cout.flush();
												if (staticInf->verbose) {
												
													cout << "Extracting relaxed planning graph heuristic" << endl;
												}				
												//Plan relaxedPlan = nextRPG->extractRelaxedPlan(*staticInf, heuristicToReturn);
												//int heuristicToIgnore;
												Plan relaxedPlan = nextRPG->extractRelaxedPlanQuickly(*masterRPG, *staticInf, heuristicToReturn);
												
											
												//cout << " heuristic   = " << heuristicToReturn << endl;
												
												if ( staticInf->verbose) {
												
													cout << " heuristic   = " << heuristicToReturn << endl;
													//cout << " heuristic 2 = " << heuristicToIgnore << endl;
													cout << "From relaxed plan:" << endl;
													cout << relaxedPlan;
													//cout << " or " << endl;
													//cout << relaxedPlanTwo;
												}
												
												ActionInstanceList* nextHA = new ActionInstanceList(relaxedPlan.helpfulActions());
												int h = heuristicToReturn;
												
												
								
												
												if(h == 0){
													if (staticInf->verbose) {
														cout << "Heuristic 0 returned - solution found!" << endl;
														cout << *nextState << endl;
													}
													const ActionInstance * a = nextAct;
													
													planToUse->addStepAtEnd( const_cast<ActionInstance*>(a), currTime);
													
													delete nextRPG;
													
													delete cyclicalList;
													delete gotStuckAt;
													gotStuckAt = 0;
													delete nextHA;
													delete nextState;
													if (foundBetter) {
														spares.push_back(new Spare(bestState, bestHA, bestHeuristic, bestTime, bestEATime, bestAction));
														bestAction->decrUsage();
													}
													while (!searchQueue.empty()) {
														delete searchQueue.front();
														searchQueue.pop_front();
													}
													
													if (staticInf->verbose) {
														cout << "Clearing spare states" << endl;
													}
													while (!spares.empty()) {
														delete spares.front();
														spares.pop_front();
													}
													if (staticInf->verbose) {
														cout << "BFS queue cleared" << endl;
													}
													while (!statesToDelete.empty()) {
														delete statesToDelete.front();
														statesToDelete.pop_front();
													}
													delete haToUse;
													return planToUse;
												}
												
												
															
												if(h < bestHeuristic) {
													
													if (staticInf->verbose) {
														cout << "Heuristic returned - " << h << " - is better than the best so far, " << bestHeuristic << endl;
													}
													if (foundBetter) {
														//cout << "Marking current 'better' as spare" << endl;
														spares.push_back(new Spare(bestState, bestHA, bestHeuristic, bestTime, bestEATime, bestAction));
														bestAction->decrUsage();
													}
													//cout << "Marking new thing as better" << endl;
													bestHeuristic = h;
													bestState = nextState;
													bestHA = nextHA;
													bestTime = currTime;
													bestEATime = eaTime;
													bestAction =  nextAct;
													//cout << "Have new best action; increasing its usage" << endl;
													bestAction->incrUsage();
													//cout << "Done that bit" << endl;
													foundBetter = true;
													//cout << "Happy with that" << endl;
												} else {
													if (staticInf->verbose) {
														cout << "Heuristic returned - " << h << " - is no better than the best so far, " << bestHeuristic << endl;
													}
													spares.push_back(new Spare(nextState, nextHA, h, currTime, eaTime, nextAct));
													
													
												}
												delete nextRPG;
											}
										} else {
											//cout << "p";
											//cout.flush();
											delete nextState;
										}
									}
									mToApply.push_back(nextAct);
								}
							} while (nextAct && (!foundBetter || !firstBestAction));

							
						}
						//cout << "**** (end)" << endl;
					}
								 
				}
				
				
				
			}
			
			
			
			
			
			if (foundBetter) {
				if (staticInf->verbose) {
					cout << "Better state found; clearing BFS queue" << endl;
				}
				while (!searchQueue.empty()) {
					delete searchQueue.front();
					searchQueue.pop_front();
				}
				while (!spares.empty()) {
					delete spares.front();
					spares.pop_front();
				}
				if (!cyclicalList->empty()) {
				
					if (inferMacro) {
						//cout << "\nNon-empty cyclical list" << endl;
						cyclicalList->addStepAtEnd(bestAction, bestTime);
						//cout << "Unmacrifying..." << endl;
						Plan* unmacrified = cyclicalList->unmacrify(staticInf);
						//cout << "...done" << endl;
						
						State* startAt = gotStuckAt->copy();
						
						double gaTime = stuckAtTime;
						
						list<pair<ActionInstanceList, double> > toPass;
						
						Plan::iterator clItr = unmacrified->begin();
						Plan::iterator clEnd = unmacrified->end();
						
						auto_ptr<ActionInstanceWithSubActionsList> maApplicableActions;
						
						if (symmPrune) {
							SymmetryExtractor symmTwo;
				
							list<SymmetryGroup*> * symGroups = symmTwo.extractSymmetryGroups(*staticInf, true, startAt);
				
							EntitySet* exemplars = new EntitySet();
				
							list<SymmetryGroup*>::const_iterator itr = symGroups->begin();
							const list<SymmetryGroup*>::const_iterator itrEnd = symGroups->end();
							
							for ( ; itr != itrEnd; ++itr) { 
							
								exemplars->insert((*itr)->getExemplar());
									
							}
							maApplicableActions = masterRPG->applicableRelaxedActions(*staticInf, startAt, exemplars);	
						
							while (!symGroups->empty()) {
								delete symGroups->front();
								symGroups->pop_front();
							}
						
							delete exemplars;
						} else {
							const EntitySet* exemplars = (EntitySet*)0;
							maApplicableActions = masterRPG->applicableRelaxedActions(*staticInf, startAt, exemplars);	
						}
							
						//ActionInstanceList applicableActions = stateToUse->applicableActions(staticInf, symGroups);
						
						
												
						
						
						
						for (; clItr != clEnd; ++clItr) {
							while (gaTime < (*clItr).second) {
								startAt->advanceTime();
								gaTime++;
								if (derivedPredicates) {
									freeActions = masterRPG->freeActions(*staticInf, startAt);
									while (!freeActions->empty()) {
										startAt->applyActionsLocally(*(freeActions.get()), staticInf);
										startAt->advanceTime();
										startAt->timeBackOne();
										freeActions = masterRPG->freeActions(*staticInf, startAt);
									}
								}
								if (symmPrune) {
									SymmetryExtractor symmTwo;
				
									list<SymmetryGroup*> * symGroups = symmTwo.extractSymmetryGroups(*staticInf, true, startAt);
							
									EntitySet* exemplars = new EntitySet();
							
									list<SymmetryGroup*>::const_iterator itr = symGroups->begin();
									const list<SymmetryGroup*>::const_iterator itrEnd = symGroups->end();
									for (; itr != itrEnd; ++itr) {
									
										exemplars->insert((*itr)->getExemplar());
											
									}
									
									//ActionInstanceList applicableActions = stateToUse->applicableActions(staticInf, symGroups);
									
									maApplicableActions = masterRPG->applicableRelaxedActions(*staticInf, startAt, exemplars);	
														
									while (!symGroups->empty()) {
										delete symGroups->front();
										symGroups->pop_front();
									}
								
									delete exemplars;
									
								} else {
									const EntitySet* exemplars = (EntitySet*)0;
									maApplicableActions = masterRPG->applicableRelaxedActions(*staticInf, startAt, exemplars);	
								}
								
							}
							ActionInstanceList actionsToApply;
					
							ActionInstanceWithSubActionsList::iterator actItr = maApplicableActions->begin();
							ActionInstanceWithSubActionsList::iterator actEnd = maApplicableActions->end();
							bool foundIt = false;
							for (; !foundIt && actItr != actEnd; ++actItr) {
								if ((*actItr)->getFirst() == (*clItr).first) {
									foundIt = true;
									actionsToApply.push_back( const_cast<ActionInstance*>((*actItr)->getFirst()));
									
									ActionInstanceList::iterator rseItr = (*actItr)->getSecond().begin();
									ActionInstanceList::iterator rseEnd = (*actItr)->getSecond().end();
											
									for (; rseItr != rseEnd; ++rseItr) {
									
										actionsToApply.push_back(*rseItr);
										
									}
									
									startAt->applyActionsLocally(actionsToApply, staticInf, true);
									toPass.push_back(pair<ActionInstanceList, double>(actionsToApply, gaTime));
								}
							}
							
							
							
							
							
							
						}
						
						
						delete startAt;
						
						
						
						//cout << "Plan segment to get out of local minimum:" << endl;
						//cout << *cyclicalList << endl;
						
						//cout << "When unmacrified:" << endl;
						//cout << *unmacrified << endl;
						//list<Plan*> streams = unmacrified->untangle();
						
						list<Plan*> streams = Plan::untangle(toPass);
						//cout << streams.size() << " stream(s) extracted" << endl;
		
						delete unmacrified;
						
						
						for (	list<Plan*>::iterator strItr = streams.begin();
							strItr != streams.end();
							strItr++) {
						
							Plan* currMacroPlan = *strItr;
							//cout << "Stream:" << endl;
							//cout << *currMacroPlan << endl;
							
							if (inferMacro && currMacroPlan->length() > 1) {
								/*cout << endl;
								cout << "-------------------------"<< endl;
								cout << "Macro-op for:" << endl;
								cout << *currMacroPlan;
								cout << "-------------------------"<< endl;*/
								MacroAction* hierachicalAction = new MacroAction(currMacroPlan, *staticInf);
								hierachicalAction->setEscapeAction();
		
								//cout << "new Action is " << (*hierachicalAction) << endl;
								bool found = false;
								for (int i = 0; !found && i < staticInf->numberOfActions; i++) {
									if (staticInf->Actions[i]->isMacroAction) {
										found = ((*hierachicalAction) == *(static_cast<MacroAction*>(staticInf->Actions[i])));
									}
								}
								if (!found) {
									staticInf->addReserveAction(hierachicalAction);
								} else {
									delete hierachicalAction;
								}
							}
							
							delete currMacroPlan;
						}
					}	
					cyclicalList->clear();				
					
					
				};
				
				if (staticInf->verbose) {
					cout << "BFS queue cleared" << endl;
				}
		
				if (staticInf->verbose) {
					cout << endl << "New best action: " << *bestAction << endl;
				} else {
					cout << "b (" << bestHeuristic << ")";
					cout.flush();
				}
				
				
				planToUse->addStepAtEnd(bestAction, bestTime);
				if (staticInf->verbose) {
					cout << "Best plan is now:" << endl;
					cout << *planToUse;
				}
				bestAction->decrUsage();
				
	
				delete gotStuckAt;
				gotStuckAt = 0;
				
				alreadyVisited.clear(0);
				if (doingPlateauLocalSearch) alreadyCalculated.clear(0);
				doingPlateauLocalSearch = false;
				delete restartTo;
				restartTo = 0;
				totalRestarts = 0;
				searchQueue.push_back(new SearchQueueItem(bestState, bestHA, planToUse, bestHeuristic, bestEATime, false));
				
			
			} else {
				if (staticInf->verbose) {
					cout << "No better state found: doing BFS" << endl;
				} else {
					cout << "e";
					cout.flush();
				}
				
				list<Spare*>::iterator spareItr = spares.begin();
				
				bool visitChildren = false;
				if (gotStuckAt == 0) {
					gotStuckAt = sState->copy();
					stuckAtTime = timeAtStart;
					alreadyVisited.clear(gotStuckAt);
					
					visitChildren = true;
					if (concurrent) {
					
						bool yet = false;
						list<Spare*>::iterator spareItrTwo = spares.begin();
						while (!yet && spareItrTwo != spares.end()) { 
						
							State * nextState = (*spareItrTwo)->getData().spareState;
							ActionInstanceList * nextHA = (*spareItrTwo)->getData().spareHA;
							double timeToUse = (*spareItrTwo)->getData().spareTime;
						
							if (!yet && timeToUse < currTime) {
								
								
								
								delete nextState;
								delete nextHA;
								delete *spareItrTwo;
								spareItr++;
								spareItrTwo++;
							} else {
								while (nextState->hasQueuedEffects()) {
									nextState->advanceTime();
									(*spareItrTwo)->getData().childTime++;
								}
								yet = true;
								spareItrTwo++;
							}
				
						};
					
					}
					
				}
				
				
				
				const bool discardAll = doingPlateauLocalSearch && (depth == depthCutOff);
				
				for (; spareItr != spares.end(); spareItr++) {	
					
					//SpareData sd = (*spareItr)->getData();
					
					ActionInstance* toApply = (*spareItr)->getData().spareAction;
					State * nextState = (*spareItr)->getData().spareState;
					ActionInstanceList * nextHA = (*spareItr)->getData().spareHA;
					double timeToUse = (*spareItr)->getData().spareTime;
					double eaTime = (*spareItr)->getData().childTime;
					int h = (*spareItr)->getData().spareHeuristic;
					

					bool discard = discardAll || (*spareItr)->getData().discard;
					
					
					if (!doingPlateauLocalSearch && !discard && visitChildren) {
						discard = !alreadyVisited.needToVisit(nextState);
					}
					
					if (discard) {
						delete nextState;
						delete nextHA;
					} else {
					
						bool macroToUse = toApply->isMacroAction();
						
						Plan * nextPlan = new Plan(*planToUse);
						nextPlan->addStepAtEnd(toApply, timeToUse);
						
						Plan * nextCyc = new Plan(*cyclicalList);
						nextCyc->addStepAtEnd(toApply, timeToUse);
						
						
						if (staticInf->verbose) {
							cout << "Added child for applying ";
							toApply->briefWrite(cout);
							cout << " at time " << timeToUse << endl;
							
						}
						
						
						
						if (staticInf->verbose) {
							cout << "Heuristic value of state = " << h << endl;		
						}
						
						//h += (h + timeToUse); // do WA* with W = 2
						//h += ((int) timeToUse); // do A*
		
						
						
						if (searchQueue.empty() || !bestFSPlateau) {
							searchQueue.push_back(new SearchQueueItem(nextState, nextHA, nextPlan, h, eaTime, nextCyc, macroToUse));
		
						} else {
							list<SearchQueueItem *>::iterator sqItr = searchQueue.begin();
							
							
							while (       sqItr != searchQueue.end()
								&& (     ((*sqItr)->getHeuristic() <= h)
								|| (    ((*sqItr)->getHeuristic() == h)
									&& !(macroToUse && !((*sqItr)->isMacro()))))) {
								sqItr++;
								
							}
							
							
							searchQueue.insert(sqItr, new SearchQueueItem(nextState, nextHA, nextPlan, h, eaTime, nextCyc, macroToUse));
		
								
						}
					}
					
					delete *spareItr;
						
					
						
						
						
					
				}

				if (doingPlateauLocalSearch && searchQueue.empty()) {
					++totalRestarts;
					++restartsAtDepth;
					if (totalRestarts > 3000) {
						cout << endl << "Abandoning plateau-escaping local search" << endl;
					} else {
						if (restartsAtDepth == 500) {
							depthCutOff *= 4;
							restartsAtDepth = 0;
							cout << endl << "Restarting from start of plateau, extending depth cut-off to " << depthCutOff << endl;
						} else if (restartsAtDepth == 250) {
							cout << endl << "Restarting from start of plateau, including non-helpful actions" << endl;
						} else {
							cout << endl << "Restarting from start of plateau" << endl;
							
						}
						alreadyVisited.clear(0);
						alreadyVisited.clear(gotStuckAt);
						searchQueue.push_back(new SearchQueueItem(*restartTo));
						depth = 0;
					}
				}		
				//cout << "Clearing up parent plan" << endl;		
				delete planToUse;
				
			}
			
			while (!statesToDelete.empty()) {
				delete statesToDelete.front();
				statesToDelete.pop_front();
			}
			//cout << "Clearing up parent RPG" << endl;
			delete haToUse;
		
			delete cyclicalList;
		
			
			
			
			firstExpansion = false;	
			
			
			
		}
		
		delete gotStuckAt;
		gotStuckAt = 0;
		
		if (noBFS) {
			cout << "Could not solve reduced instance using EHC" << endl;
			
			Plan* empty = new Plan();
			return empty;
		} else {
			cout << "No plan found via EHC, resorting to BestFS" << endl;
		}

		
		
	}
	
	
	
	//currState = staticInf->InitialState->copy();
		
	list<BFSSearchQueueItem* > BFSsearchQueue;
	Plan* dummyEmptyPlan = new Plan();
	
	BFSsearchQueue.push_back(new BFSSearchQueueItem(0, dummyEmptyPlan, 0, 0));
	
	
	alreadyVisited.clear(staticInf->InitialState);
	
	bestHeuristic = INT_MAX;
	
	while(!BFSsearchQueue.empty()){
		
		
		
		
		list<State*> statesToDelete;
		list<ActionInstanceList> allSpareActions;
		list<double> correspondingTimes;
		
		
		//State * sState = BFSsearchQueue.front()->getData().stateToUse;	
		State * sState = staticInf->InitialState->copy();
		Plan * planToUse = BFSsearchQueue.front()->getData().planToUse;
		double currTime = BFSsearchQueue.front()->getData().currTime;
		int currHeuristic = BFSsearchQueue.front()->getData().currHeuristic;
		int upTo = BFSsearchQueue.front()->getData().upTo;
		ActionInstanceWithSubActionsList * applicableActions = BFSsearchQueue.front()->getData().applicableActions;
		
		Plan::iterator prevPlanStep = planToUse->begin();
		Plan::iterator itr = planToUse->begin();
		const Plan::iterator itrEnd = planToUse->end();
		
		for ( ; itr != itrEnd; ++itr) {
		
			while ((*itr).second > sState->getTimeStamp()) {
				//cout << "Time stamp of current action = " << (*itr).second << endl;
				//cout << "Time stamp of state = " << sState->getTimeStamp() << endl;
				sState->advanceTime();
			}
			
			sState->applyActionLocally((*itr).first, staticInf);
			
				
		}
		
		while (sState->hasQueuedEffects()) {
			sState->advanceTime();
		}
			
		if (derivedPredicates) {
			
			freeActions = masterRPG->freeActions(*staticInf, sState);
			
			while (!freeActions->empty()) {
				sState->applyActionsLocally(*(freeActions.get()), staticInf);
				sState->advanceTime();
				sState->timeBackOne();
				freeActions = masterRPG->freeActions(*staticInf, sState);
			} 
			
		}
		

		//delete BFSsearchQueue.front();
		//BFSsearchQueue.pop_front();



		if (staticInf->verbose) {
			cout << "Expanding node; plan so far:" << endl;
			cout << *planToUse << endl;
			cout << "Creating list of actions" << endl;
		}


		statesToDelete.push_back(sState);
		correspondingTimes.push_back(currTime);
//		bool foundBetter = false;

//		double bestTime = currTime;

		list<BFSSpare*> spares;

		State* stateToUse = statesToDelete.back();
		
		
			
		//ActionInstanceList applicableActions = stateToUse->applicableActions(staticInf, symGroups);
		if (applicableActions == 0) {
			if (symmPrune) {
			
				SymmetryExtractor symm;
				
				list<SymmetryGroup*>* symGroups = symm.extractSymmetryGroups(*staticInf, true, stateToUse);
		
				EntitySet* exemplars = new EntitySet();
					
				list<SymmetryGroup*>::const_iterator itr = symGroups->begin();
				const list<SymmetryGroup*>::const_iterator itrEnd = symGroups->end();
				
				for ( ; itr != itrEnd; ++itr) {
					exemplars->insert((*itr)->getExemplar());
						
				}
				auto_ptr<ActionInstanceWithSubActionsList> rawApplicableActions = masterRPG->applicableRelaxedActions(*staticInf, stateToUse, exemplars);
				auto_ptr<ActionInstanceWithSubActionsList> tmpApplicableActions = stateToUse->filterApplicableActions(staticInf, *(rawApplicableActions.get()));
				applicableActions = tmpApplicableActions.release();
				
				delete exemplars;
				
				while (!symGroups->empty()) {
					delete symGroups->front();
					symGroups->pop_front();
				}
				
			} else {
				const EntitySet* exemplars = (EntitySet*)0;
				auto_ptr<ActionInstanceWithSubActionsList> rawApplicableActions = masterRPG->applicableRelaxedActions(*staticInf, stateToUse, exemplars);
				auto_ptr<ActionInstanceWithSubActionsList> tmpApplicableActions = stateToUse->filterApplicableActions(staticInf, *(rawApplicableActions.get()));
				applicableActions = tmpApplicableActions.release();
				
			
			}
			BFSsearchQueue.front()->getData().applicableActions = applicableActions;
			
		}
		
		
		
		
		
		
		//ActionInstanceList applicableActions = stateToUse->applicableActions(staticInf);
		
		
		//State * tempCopy = stateToUse->copy();
		//tempCopy->applyRelaxedActionsLocally(applicableActions, staticInf, true, true);
		
		
		//delete tempCopy;
		
		
		
		if (staticInf->verbose) {
			cout << "Pruned list of actions" << endl;
		}
		
		

		
		
		actItr = applicableActions->begin();
		
		
		
		if (staticInf->verbose) {
			cout << "Iterating over list of actions" << endl;
		}
		
		int i = 0;
		
		bool breakOut = false;
				
		for(; !breakOut && actItr != applicableActions->end(); i++, actItr++){

			if (i >= upTo) {
			
			
				if (staticInf->verbose) {
					cout << "Applying action ";
					(*actItr)->getFirst()->briefWrite(cout);
					cout << " at time " << currTime << " to produce new state";
				}
				
				ActionInstanceList actionsToApply;
					
					
					
				actionsToApply.push_back( const_cast<ActionInstance*>((*actItr)->getFirst()) );
				
				ActionInstanceList::iterator rseItr = (*actItr)->getSecond().begin();
				ActionInstanceList::iterator rseEnd = (*actItr)->getSecond().end();
						
				for (; rseItr != rseEnd; ++rseItr) {
				
					actionsToApply.push_back(*rseItr);
				
				}
				
				
				State * nextState = stateToUse->applyActions(actionsToApply, masterRPG, staticInf);
				
				//State * nextState = stateToUse->applyAction(*actItr, staticInf);
				
				double eaTime = currTime;
				
				if (nextState->checkEventQueueConsistency()) {
					while (nextState->hasQueuedEffects()) {
						nextState->advanceTime();
						eaTime++;
					}
					
					if (derivedPredicates) {
			
						freeActions = masterRPG->freeActions(*staticInf, nextState);
						
						while (!freeActions->empty()) {
							nextState->applyActionsLocally(*(freeActions.get()), staticInf);
							nextState->advanceTime();
							nextState->timeBackOne();
							freeActions = masterRPG->freeActions(*staticInf, nextState);
						} 
						
					}
					
					if (alreadyVisited.needToVisit(nextState)) {
						cout << ".";
						cout.flush();
						//int h = nextState->getMarginallyLessRubbishHeuristic(staticInf);
				
							
						
						if (staticInf->verbose) {
							cout << "Generating relaxed planning graph" << endl;
						}
						
						
							
						
						RelaxedPlanningGraph * nextRPG = new RelaxedPlanningGraph(*masterRPG, *staticInf, nextState);
						
						/*
						RelaxedPlanningGraph * testRPG = new RelaxedPlanningGraph(*staticInf, nextState);
						
						if (!(*testRPG == *nextRPG)) {
							cout << "Duff incremental RPG constructor" << endl;
							exit(0);
						} else {
							cout << endl;
						}
						delete testRPG;
						
						*/
						
						if (!nextRPG->didGoalsAppear()) {
							if (staticInf->verbose) {
								cout << " Dead end" << endl;
							}
							delete nextState;
							delete nextRPG;
							
						} else {
							int heuristicToReturn;
							//cout << "_";
							//cout.flush();
							if (staticInf->verbose) {
							
								cout << "Extracting relaxed planning graph heuristic" << endl;
							}				
							//Plan relaxedPlan = nextRPG->extractRelaxedPlan(*staticInf, heuristicToReturn);
							//int heuristicToIgnore;
							Plan relaxedPlan = nextRPG->extractRelaxedPlanQuickly(*masterRPG, *staticInf, heuristicToReturn);
							
						
							
							if ( staticInf->verbose) {
							
								cout << " heuristic   = " << heuristicToReturn << endl;
								//cout << " heuristic 2 = " << heuristicToIgnore << endl;
								cout << "From relaxed plan:" << endl;
								cout << relaxedPlan;
								//cout << " or " << endl;
								//cout << relaxedPlanTwo;
							}
							
							
							int h = heuristicToReturn;
							
							delete nextRPG;
			
							
							if(h == 0){
								if (staticInf->verbose) {
									cout << "Heuristic 0 returned - solution found!" << endl;
									cout << *nextState << endl;
								}
								const ActionInstance * a = (*actItr)->getFirst();
								
								planToUse->addStepAtEnd( const_cast<ActionInstance*>(a), currTime);
								
								
								delete nextState;
								
								while (!BFSsearchQueue.empty()) {
									delete BFSsearchQueue.front();
									BFSsearchQueue.pop_front();
								}
								
								if (staticInf->verbose) {
									cout << "Clearing spare states" << endl;
								}
								while (!spares.empty()) {
									delete spares.front();
									spares.pop_front();
								}
								if (staticInf->verbose) {
									cout << "BFS queue cleared" << endl;
								}
								
								delete stateToUse;
								return planToUse;
							}
							
							
							if (h < bestHeuristic) {
								bestHeuristic = h;
								cout << endl << "h = " << h << endl;
							}		
							
							delete nextState;
							nextState = 0;
							
							spares.push_back(new BFSSpare(nextState, h, currTime, eaTime, actionsToApply));
								
								
							if (h < currHeuristic) breakOut = true;
							
						}
					} else {
						
						delete nextState;
					}
				} else {
					delete nextState;
				}
				
			}
		}
		
		BFSSearchQueueItem* holdIt = 0;
		if (actItr == applicableActions->end()) {
			holdIt = BFSsearchQueue.front();
			BFSsearchQueue.pop_front();
						
		
		} else {
			BFSsearchQueue.front()->getData().upTo = i;
			BFSsearchQueue.front()->deleteIfFinish();
		}
		
		
		for (list<BFSSpare*>::iterator spareItr = spares.begin(); spareItr != spares.end(); spareItr++) {	
			
			
						
			ActionInstanceList toApply = (*spareItr)->getData().spareAction;
			State * nextState = (*spareItr)->getData().spareState;
			double timeToUse = (*spareItr)->getData().spareTime;
			double eaTime = (*spareItr)->getData().childTime;
			
			
			
			Plan * nextPlan = new Plan(*planToUse);
			
			nextPlan->addStepsAtEnd(toApply, timeToUse);
			
			
			if (staticInf->verbose) {
				cout << "Added child for applying ";
				toApply.front()->briefWrite(cout);
				cout << " at time " << timeToUse << endl;
				
			}
			
			int h = (*spareItr)->getData().spareHeuristic;
			
			
			if (staticInf->verbose) {
				cout << "Heuristic value of state = " << h << endl;		
			}
			
			//h += (h + timeToUse); // do WA* with W = 2
			//h += timeToUse; // do A*
			
			if (BFSsearchQueue.empty()) {
				BFSsearchQueue.push_back(new BFSSearchQueueItem(nextState, nextPlan, h, eaTime));
				
			} else {
				list<BFSSearchQueueItem *>::iterator sqItr = BFSsearchQueue.begin();
				
				
				while (       sqItr != BFSsearchQueue.end()
					&& ((*sqItr)->getHeuristic() <= h)
						) {
					sqItr++;
					
				}
				
				
				BFSsearchQueue.insert(sqItr, new BFSSearchQueueItem(nextState, nextPlan, h, eaTime));

					
			}
			
			delete *spareItr;
				
			
			
			
			
		}
		

		
		//cout << "Clearing up parent plan" << endl;		
		if (actItr == applicableActions->end()) {
			delete planToUse;
		}
		
		
		while (!statesToDelete.empty()) {
			delete statesToDelete.front();
			statesToDelete.pop_front();
		}
		//cout << "Clearing up parent RPG" << endl;
		delete holdIt;
	
		
	}
	
	cout << "Could not find plan by exhaustive search" << endl;
	Plan* empty = new Plan();
	return empty;
}

/*
Plan planningAlgorithms::planLookAheadEhc(StaticInformation & problemInformation){

	State currState = *(problemInformation.InitialState);
	Plan planActions;

	if(currState.subsumes(problemInformation.GoalPredicates, &problemInformation)) return planActions;
*/

/*
	list<ActionInstance *> applicableActions = currState.applicableActions(&problemInformation);

	list<ActionInstance*>::iterator actItr = applicableActions.begin();
	double time = 0;
	int bestHeuristic = INT_MAX;
	State bestState;
	bool advanceTime = false;

	list<ActionInstance*>::iterator bestAction;

	bool firstAction = true;
	list<ActionInstance*> selectedActions;


	//find the best action to apply
	for(; actItr != applicableActions.end() && firstAction; actItr++){

		State nextState = (*currState.applyAction((*actItr), &problemInformation));
		int h = currState.getHeuristic(&problemInformation);

		if(h < bestHeuristic) {
			bestHeuristic = h;
			bestState = nextState;
			bestAction = actItr;
		}

		if(nextState.subsumes(problemInformation.GoalPredicates, &problemInformation)){	//if the next state contains the goal return the plan
			planActions.addStepAtEnd(*actItr, time);
			return planActions;
		}
	}

	ActionInstance * a = (*bestAction);
	selectedActions.push_back(a);

	while(true){

		int bestNowHeuristic = INT_MAX;
		State bestNowState;
		ActionInstance* bestNowAction;

		int bestFutureHeuristic = INT_MAX;
		State bestFutureState;
		ActionInstance* bestFutureAction;


		//try applying more actions at this time
		for(; actItr != applicableActions.end() && firstAction; actItr++){

			//if the action is mutex with those already applied skip it
			list<ActionInstance*>::iterator appliedItr = selectedActions.begin();
			bool actionsRemaining = true;
			//find an applicable action
			for(; actionsRemaining && appliedItr != selectedActions.end(); appliedItr++){
				actionsRemaining = !((*appliedItr)->isMutexWith(*actItr));
				}

			if(actionsRemaining){
				State nextState = (*currState.applyAction(*actItr, &problemInformation));
				int h = currState.getMarginallyLessRubbishHeuristic(&problemInformation);
				if(h < bestNowHeuristic) {
					bestNowHeuristic = h;
					bestNowState = nextState;
					bestNowAction = (*actItr);
				}

				if(nextState.subsumes(problemInformation.GoalPredicates, &problemInformation)){	//if the next state contains the goal return the plan
					planActions.addStepAtEnd(*actItr, time);
					return planActions;
				}
			} else{
				firstAction = true;
				time++;
			}
		}


		//try advancing time and applying an action

		State nextState = (*currState.applyActions(selectedActions, &problemInformation));

		for(; actItr != applicableActions.end() && firstAction; actItr++){

			State newState = (*nextState.applyAction((*actItr), &problemInformation));
			int h = nextState.getHeuristic(&problemInformation);

			if(h < bestFutureHeuristic) {
				bestFutureHeuristic = h;
				bestFutureState = nextState;
				bestFutureAction = (*actItr);
			}

			if(nextState.subsumes(problemInformation.GoalPredicates, &problemInformation)){	//if the next state contains the goal return the plan
				planActions.addStepAtEnd(*actItr, time);
				return planActions;
			}
		}

		if(bestFutureHeuristic < bestNowHeuristic){	//if it's better to advance time

			State * newState = currState.applyActions(selectedActions, &problemInformation);
			list<double> timeStamps;
			for(int i = 0; i < selectedActions.size(); i++) timeStamps.push_back(time);
			planActions.addStepsAtEnd(selectedActions, timeStamps);
			newState->advanceTime();
			currState = (*newState);
			time++;
			selectedActions.clear();
			selectedActions.push_front(bestFutureAction);

		}else{
			ActionInstance * a = bestNowAction;
			selectedActions.push_back(a);
		}
	}
};
*/

bool planningAlgorithms::isCyclical(const ActionInstanceList * pruneCheck, const ActionInstance* toApply) {

	ActionInstanceList::const_reverse_iterator itr = pruneCheck->rbegin();
	ActionInstanceList::const_reverse_iterator itrEnd = pruneCheck->rend();

	for (; itr != itrEnd; itr++) {
		const ActionInstance* curr = *itr;
		if (curr->isMutexWith(*toApply)) return (curr->isInverseOf(*toApply));
	}

	return false;

};


Plan * planningAlgorithms::StrictSymmetryHierachyPlan(StaticInformation & problemInformation){

	State* currState = problemInformation.InitialState->copy();

	SymmetryExtractor symm;

	list<SymmetryGroup*> symmetryGroups(*symm.extractSymmetryGroups(problemInformation, true, currState));

	//if there is only one entity in every group we get no benifits from the symmetry
//	if(((int) symmetryGroups.size()) == problemInformation.numberOfEntities) cout << "We probably shouldn't be bothering with symmetry in this problem" << endl;

	Plan * planActions = new Plan();

	if(currState->subsumes(problemInformation.GoalPredicates, &problemInformation) && !currState->intersectsWith(problemInformation.NegativeGoalPredicates, &problemInformation)) {

		delete currState;
		return planActions;
	}

	State * newStaticInitialState = new State();

	list<Entity*> entitiesToKeep;

	list<Entity*> goalEntitiesToKeep;

	State * newInitialState = pruneState(symmetryGroups, currState, problemInformation, true, entitiesToKeep, newStaticInitialState);

	EntitySet trimTo;

	for (list<Entity*>::iterator ttsItr = entitiesToKeep.begin(); ttsItr != entitiesToKeep.end(); ttsItr++) {
		trimTo.insert(*ttsItr);
	}

	for (list<Entity*>::iterator ttsItr = goalEntitiesToKeep.begin(); ttsItr != goalEntitiesToKeep.end(); ttsItr++) {
		trimTo.insert(*ttsItr);
	}


	State * newGoalState = pruneState(symmetryGroups, problemInformation.GoalPredicates, problemInformation, false, goalEntitiesToKeep, 0);

	State * newNegativeGoalState = pruneState(symmetryGroups, problemInformation.NegativeGoalPredicates, problemInformation, false, goalEntitiesToKeep, 0);
	
	//if(goalEntitiesToKeep.size() != entitiesToKeep.size()) cout << "WARNING: different number of entites requred by goal and initial state" << endl;

	// problemInformation.verbose = true;

	if(problemInformation.verbose) cout << "new initial state is " << (*newInitialState) << endl;

	if(problemInformation.verbose) cout << "new goal state is " << (*newGoalState) << endl;

	StaticInformation newProblem(problemInformation, entitiesToKeep, newStaticInitialState, newInitialState, newGoalState, newNegativeGoalState);
	newProblem.trimEntities(&trimTo);

	if(problemInformation.verbose) cout << "Completed creation of new static information" << endl;

	Plan* smallPlanActions = planLookAheadEhc(&newProblem, false, false, false, false, false, true);

	problemInformation.resetEntities();
	if(problemInformation.verbose) cout << "Got a plan to achieve the new goal state " << endl;

	if(problemInformation.verbose) cout << *(smallPlanActions) << endl;

	//state::const_iterator achievedItr = newGoalState->begin();

	//if(newGoalState->size() > 1) cout << "Error in symmetric planning support for more than one goal predicate required" << endl;

	//PredicateInstance* goalAchieved = newGoalState->begin();

	//list<SymmetryGroup*>::iterator groupItr = symmetryGroups.begin();

	MacroAction* hierachicalAction = new MacroAction(smallPlanActions, problemInformation);
	hierachicalAction->setMarvinAction();

//	cout << "new Action is " << (*hierachicalAction) << endl;

	problemInformation.addAction(hierachicalAction);

	problemInformation.incrSimple();

	Plan* toReturn = planLookAheadEhc(&problemInformation, false, false, false, false, false, false);

	return toReturn;

	/*first see if inverse operators will return us to the initial state minus the objects we got to the goal


	for(; groupItr != symmetryGroups.end(); groupItr++){

		//if this goal is required for each object in the symmetry group
		if(goalAchieved->isADefiningGoalPredicate()){
			//iterate over the plan's actions replacing 'ball1' with 'ball2'
		}

	}*/


	return planActions;
};


Plan * planningAlgorithms::AlmostSymmetryHierachyPlan(StaticInformation & problemInformation, const bool & inferMacro, const bool & concurrent, const bool & derivedPredicates, const bool & symmPrune, const bool & onlyBFS, const bool & noBFS, const bool & transOps){

//	problemInformation.verbose = false;

	State* currState = problemInformation.InitialState->copy();

	SymmetryExtractor symm;

	list<SymmetryGroup*> symmetryGroups = symm.almostSymmetryDifferenceOnePredicate(problemInformation);
//	problemInformation.verbose = false;

	if(problemInformation.verbose){
		cout << "Almost-Symmetry groups:" << endl;
		list<SymmetryGroup*>::iterator itr = symmetryGroups.begin();

		for(; itr != symmetryGroups.end(); itr++){

			cout << (*(*itr)) << endl;

		}
	}

	list<SymmetryGroup*> trimmedSymmetryGroups;

	list<SymmetryGroup*>::iterator itr = symmetryGroups.begin();
	const list<SymmetryGroup*>::iterator itrEnd = symmetryGroups.end();
	
	list<SymmetryGroup*> ignore;
	//here we create fake symmetry groups pretending that almost symmetric objects are symmetric

	for(;itr != itrEnd; ++itr){



		bool ignoreThis = false;
		//check whether this related group has been added as a related group elsewhere

		for(list<SymmetryGroup*>::iterator ig = ignore.begin(); ig != ignore.end() && !ignoreThis; ig++){

			if((*ig) == (*itr)) {
				ignoreThis = true;
				if(problemInformation.verbose) cout << "ignoring current group" << endl;
			}

		}

		if(!ignoreThis) {

			if(problemInformation.verbose) cout << "+++ adding {" << (*(*itr)) << " to the symmetry groups to use" << endl << "}" << endl;
			trimmedSymmetryGroups.push_back(new SymmetryGroup(*(*itr)));
			list<SymmetryGroup*>::iterator tabooItr = (*itr)->getRelatedGroups().begin();

			list<Entity*> ent = (*itr)->getEntities();
			bool addedNewEntity = false;

			for(; tabooItr != (*itr)->getRelatedGroups().end(); tabooItr++){
				bool ignoreGroup = false;

				for(list<SymmetryGroup*>::iterator ig = ignore.begin(); ig != ignore.end() && !ignoreGroup; ig++){

					if((*ig) == (*itr)) {
						ignoreGroup = true;
						if(problemInformation.verbose) cout << "    ignoring current related group" << endl;
					}

				}
				if (!ignoreGroup) {
					ignore.push_back(*tabooItr);
					if(problemInformation.verbose) cout << "--- adding {" << (*(*tabooItr)) << " to the symmetry groups to ignore " << endl << "}" << endl;
					//pretend that the entities in this group are in the same group as the other entities
					list<Entity*>::iterator fakeItr = (*tabooItr)->getEntities().begin();




					for(;fakeItr != (*tabooItr)->getEntities().end(); fakeItr++){
						//put the entity into the list of things in this symmetry group

						ent.push_back(*fakeItr);
						addedNewEntity = true;


					}


				}


			}


			if(addedNewEntity){
				delete trimmedSymmetryGroups.back();
				trimmedSymmetryGroups.pop_back();

				SymmetryGroup * fakeGroup = new SymmetryGroup((*itr)->getDefiningPredicates(), (*itr)->getDefiningGoalPredicates(), ent, (*itr)->getType());
				trimmedSymmetryGroups.push_back(fakeGroup);
			}



		}

	}
	
	
//	bool wasVerbose = problemInformation.verbose;
//	problemInformation.verbose = false;

	if(problemInformation.verbose){
		cout << "Trimmed symmetry groups:" << endl;
		list<SymmetryGroup*>::iterator itr = trimmedSymmetryGroups.begin();

		for(; itr != trimmedSymmetryGroups.end(); itr++){

			cout << (*(*itr)) << endl;

		}
	}
	
	itr = symmetryGroups.begin();
	
	for (; itr != itrEnd; ++itr) delete (*itr);
	
	
//	problemInformation.verbose = wasVerbose;

	//if there is only one entity in every group we get no benifits from the symmetry
	

	

	if(currState->subsumes(problemInformation.GoalPredicates, &problemInformation) && !currState->intersectsWith(problemInformation.NegativeGoalPredicates, &problemInformation)) {

		delete currState;
		Plan * planActions = new Plan();
		return planActions;
	}

		
	State * newStaticInitialState = new State();

	list<Entity*> entitiesToKeep;
	list<Entity*> goalEntitiesToKeep;

	State * newInitialState = pruneState(trimmedSymmetryGroups, currState, problemInformation, true, entitiesToKeep, newStaticInitialState);

	delete currState;
	
	State * newGoalState = pruneState(trimmedSymmetryGroups, problemInformation.GoalPredicates, problemInformation, false, goalEntitiesToKeep, 0);
	State * newNegativeGoalState = pruneState(trimmedSymmetryGroups, problemInformation.NegativeGoalPredicates, problemInformation, false, goalEntitiesToKeep, 0);
	
	
	
//	if(goalEntitiesToKeep.size() != entitiesToKeep.size()) cout << "WARNING: different number of entites requred by goal and initial state" << endl;

	EntitySet trimTo;

	for (list<Entity*>::iterator ttsItr = entitiesToKeep.begin(); ttsItr != entitiesToKeep.end(); ttsItr++) {
		trimTo.insert(*ttsItr);
	}

	for (list<Entity*>::iterator ttsItr = goalEntitiesToKeep.begin(); ttsItr != goalEntitiesToKeep.end(); ttsItr++) {
		trimTo.insert(*ttsItr);
	}

	if(((int) trimTo.size()) == problemInformation.numberOfEntities) {
//		cout << "We won't be bothering with almost symmetry in this problem" << endl;
		return planLookAheadEhc(&problemInformation, inferMacro, concurrent, derivedPredicates, symmPrune, onlyBFS, noBFS);
	} else {
		cout << "Found at least some symmetry in the problem" << endl;
	}
	
//	problemInformation.verbose = true;

	if(problemInformation.verbose) cout << "new initial state is " << (*newInitialState) << endl;

	if(problemInformation.verbose) cout << "new goal state is " << (*newGoalState) << endl;

//	problemInformation.verbose = false;
	StaticInformation newProblem(problemInformation, entitiesToKeep, newStaticInitialState, newInitialState, newGoalState, newNegativeGoalState);

	for (int i = 0; i < problemInformation.numberOfConstants; ++i) {
		trimTo.insert(problemInformation.Constants[i]);
	}
	
	newProblem.trimEntities(&trimTo);
	if(problemInformation.verbose) cout << "Completed creation of new static information" << endl;

	//newProblem.needToCheckStatic = true;

	if (transOps) newProblem.recogniseTransformationalOperators();
	
	Plan* smallPlanActions = planLookAheadEhc(&newProblem, inferMacro, concurrent, derivedPredicates, symmPrune, false, true);
	

	if (true) {
		if (problemInformation.clearUpActions) {
			for (int i = 0; i < problemInformation.numberOfActions; ++i) {
				delete problemInformation.Actions[i];
			}
		}
		
		delete [] problemInformation.Actions;
		
		problemInformation.Actions = new Action*[newProblem.numberOfActions];
		
		memcpy(problemInformation.Actions, newProblem.Actions, newProblem.numberOfActions * sizeof(Action*));
		
		newProblem.clearUpActions = false;
		
	}
	
	if(!smallPlanActions->empty()){
		if(problemInformation.verbose) cout << "Got a plan to achieve the new goal state " << endl;

		Plan* unmacrified = smallPlanActions->unmacrify(&newProblem);
		
		if(problemInformation.verbose) cout << *(smallPlanActions) << endl;

		delete smallPlanActions;
		
		list<pair<ActionInstanceList, double> > toUntangle;
		
		Plan::const_iterator umItr = unmacrified->begin();
		const Plan::const_iterator umEnd = unmacrified->end();
		
		State* currTmpState = newProblem.InitialState;
		StaticInformation* tmpPointer = &newProblem;
		slist<State*> statesToDelete;
		
		static const EntitySet* nullExemplars = 0;
		
		masterRPG->applicableRelaxedActions(newProblem, currTmpState, nullExemplars);
		
		ActionInstanceList toApply;
		
		double prevTime = 0.0;
		
		bool finished = false;
		
		while (!finished) {
		
			if (umItr == umEnd) {
				finished = true;
			}
			
			if (finished || (*umItr).second > prevTime) {
				currTmpState = currTmpState->applyActions(toApply, masterRPG, tmpPointer);
				currTmpState->advanceTime();
				statesToDelete.push_front(currTmpState);
				masterRPG->applicableRelaxedActions(newProblem, currTmpState, nullExemplars);
				toApply.clear();
				prevTime++;
			}		

			if (!finished) {
				pair<ActionInstanceList, double> nextEntry;
				
				masterRPG->getApplicableSubActions((*umItr).first, nextEntry.first);
				nextEntry.second = (*umItr).second;
				
				toUntangle.push_back(nextEntry);
				
				
				masterRPG->getApplicableSubActions((*umItr).first, toApply);
				++umItr;
			}
			
					
		}
		
		
		
		while (!statesToDelete.empty()) {
			delete statesToDelete.front();
			statesToDelete.pop_front();
		}
		
		list<Plan*> independentPlans = Plan::untangle(toUntangle);

		list<Plan*>::iterator planItr = independentPlans.begin();

		for (;planItr != independentPlans.end(); planItr++) {
			
			Plan* repeatedRemoved = (*planItr)->removeSimilarAdjacentSteps();
			
			
			if (repeatedRemoved->makeSpan() > 1.0 && repeatedRemoved->makeSpan() < 5.0) {
				MacroAction* hierachicalAction = new MacroAction(repeatedRemoved, problemInformation);
				hierachicalAction->setMarvinAction();
				//cout << "new Action is " << (*hierachicalAction) << endl;
	
				problemInformation.addAction(hierachicalAction);
				//problemInformation.addPreferredAction(hierachicalAction);
			} else {
				MacroAction* hierachicalAction = new MacroAction(repeatedRemoved, problemInformation);
				hierachicalAction->setMarvinAction();
				//cout << "not using macro " << (*hierachicalAction) << endl;
				delete hierachicalAction;
			}
			delete *planItr;
			delete repeatedRemoved;
		}
		
		delete unmacrified;
		
	} else {
		cout << "Unable to infer macro-action new problem unsolvable" << endl;
		delete smallPlanActions;
	}

	delete masterRPG;
	masterRPG = 0;
		
	problemInformation.resetEntities();
	if (transOps) problemInformation.recogniseTransformationalOperators();
	
	Plan* toReturn = planLookAheadEhc(&problemInformation, inferMacro, concurrent, derivedPredicates, symmPrune, onlyBFS, noBFS);
	
	list<SymmetryGroup*>::iterator sgItr = trimmedSymmetryGroups.begin();
	const list<SymmetryGroup*>::iterator sgEnd = trimmedSymmetryGroups.end();
	
	for (;sgItr != sgEnd; ++sgItr) {

		delete (*sgItr);
		
	}
	
	return toReturn;

	/*first see if inverse operators will return us to the initial state minus the objects we got to the goal


	for(; groupItr != symmetryGroups.end(); groupItr++){

		//if this goal is required for each object in the symmetry group
		if(goalAchieved->isADefiningGoalPredicate()){
			//iterate over the plan's actions replacing 'ball1' with 'ball2'
		}

	}*/



};

State * planningAlgorithms::pruneState(list <SymmetryGroup*> symmGroups, const State* toPrune, StaticInformation & problemInformation, bool includeStaticInitialState, list<Entity*> & entitiesToKeep, State* staticInitialState){
	//this code needs to remove predicate instances instead of adding them
//	bool was = problemInformation.verbose;

//	problemInformation.verbose = false;

	if(problemInformation.verbose) cout << "***********************************************************" << endl;
	list<SymmetryGroup*>::iterator groupItr = symmGroups.begin();
	State* toReturn = new State();
	PredicateInstanceList toAdd;
	PredicateInstanceList staticToAdd;
	list<Entity*> taboo;


	Action* actionConcerned;
	for(; groupItr != symmGroups.end(); groupItr++){
		Entity* currEntity = (*groupItr)->getEntities().front();
		int instances = 0;
		if(problemInformation.verbose) cout << "----------------------------------------------------------" << endl;
		//make sure there is no action which requires more than one thing from the same group
		for(int i = 0; i < problemInformation.numberOfActions; i++){
			Action* currAction = problemInformation.Actions[i];
			int tempInstances = 0;
			for(int j = 0; j < currAction->parameterCount; j++){

				if(currAction->parameterTypes[j] == currEntity->getEntityType()){
					tempInstances++;
					if(problemInformation.verbose) cout << "parameter " << j << "of action ";
					if(problemInformation.verbose) currAction->briefWrite(cout);
					if(problemInformation.verbose) cout << " of type " << (currAction->parameterTypes[j])->name() << " is of the same type as " << (currEntity->getEntityType())->name() << endl;
					actionConcerned = currAction;
				}
			}

			if(tempInstances > instances) instances = tempInstances;
		}


		list<Entity*> groupEntities = (*groupItr)->getEntities();
		list<Entity*>::iterator throughGroupItr = groupEntities.begin();

		if(problemInformation.verbose) cout << "found " << instances << " instances of predicate in parameters for action ";
		if(problemInformation.verbose) actionConcerned->briefWrite(cout);
//		bool needAnotherGroup = false;

		bool tabooInThisGroup = true;
		int p = 0;
		while(p < instances && tabooInThisGroup){
			p++;
			entitiesToKeep.push_back(*throughGroupItr);
			throughGroupItr++;
			if(throughGroupItr == groupEntities.end()) tabooInThisGroup = false;
		}

		if(problemInformation.verbose) cout << p << " instances of object symmetric with " << (*(*groupEntities.begin())) << " required" << endl;

		for(; tabooInThisGroup && throughGroupItr != groupEntities.end(); throughGroupItr++){
			if(problemInformation.verbose) cout << "Removing predicates for " << (*(*throughGroupItr)) << "; it is symmetric with " << (*groupEntities.front()) << endl;
			taboo.push_back((*throughGroupItr));
		}

	}


	
	list<Entity*>::iterator tabooItr = taboo.begin();

	if(problemInformation.verbose){
		cout << "Taboo entities are:" << endl;
		for(; tabooItr != taboo.end(); tabooItr++){
			cout << (*(*tabooItr)) << endl;
		}

		tabooItr = taboo.begin();
	}


	int passLimit = 1;
	if (includeStaticInitialState) passLimit++;
	
	for (int pass = 0; pass < passLimit; pass++) {
		State::const_iterator itr;
		State::const_iterator itrEnd;
		
		if (pass == 0) {
			itr = toPrune->begin();
			itrEnd = toPrune->end();
		} else {
			itr = problemInformation.StaticInitialState->begin();
			itrEnd = problemInformation.StaticInitialState->end();
		}
	
		for(; itr != itrEnd; itr++){

			bool add = true;
	
	
			for(int paramIndex = 0; add && paramIndex < (*itr)->getParentPredicate()->getArgumentCount(); paramIndex++){
	
				for(tabooItr = taboo.begin(); add && tabooItr != taboo.end(); tabooItr++){
	
					if((*tabooItr) == (((*itr)->getParameter(paramIndex))->boundTo())){
						if(problemInformation.verbose) cout << (*(*tabooItr)) << " is the same as " << (*((*itr)->getParameter(paramIndex))->boundTo()) << endl;
						add = false;
					} else {
						if(problemInformation.verbose) cout << (*(*tabooItr)) << " is not the same as " << (*((*itr)->getParameter(paramIndex))->boundTo()) << endl;
					}
				}
	
			}
	
			if(add) {
				if(problemInformation.verbose) cout << "adding predicate " << (*(*itr)) << "; none of its parameters are taboo" << endl;
				if(pass == 1 && staticInitialState != 0){
	
					staticInitialState->addPredicateInstance(*itr);
	
				}else{
					toReturn->addPredicateInstance(*itr);
				}
			}
	
			if(problemInformation.verbose) cout << "------------------------------------------" << endl;
	
		}
	}
//	problemInformation.verbose = was;
	return toReturn;
}


class Signature {

public:

	const Action* const actType;
	const unsigned int parameterCount;
	Entity** const params;
	SymmetryGroup** const groups;
	unsigned int* exID;
	size_t hashVal;

	void calculateHash() {
		hashVal = reinterpret_cast<size_t>(actType);
		for (unsigned int i = 0; i < parameterCount; ++i) {
			for (unsigned int j = 0; j <= exID[i]; ++j) {
				hashVal += reinterpret_cast<size_t>(groups[i]);
			}
		}
		
	}		
	
	Signature(ActionInstance * pa, map<Entity*, SymmetryGroup*, lt<Entity> > & entGroupMap) : actType(pa->getParentAction()), parameterCount(actType->parameterCount), params(new Entity*[parameterCount]), groups(new SymmetryGroup*[parameterCount]), exID(new unsigned int[parameterCount]) {

		for (unsigned int i = 0; i < parameterCount; ++i) {
		
			Entity* const nextEnt = pa->getParameter(i)->boundTo();
			
			params[i] = nextEnt;
			if (nextEnt->isConstant()) {
				groups[i] = 0;
			} else {
				groups[i] = entGroupMap[nextEnt];
			}
		
		}
		
		for (unsigned int i = 0; i < parameterCount; ++i) {
			
			unsigned int nv = 0;
			
			if (groups[i]) {
				for (unsigned int j = 0; j < i; ++j) {
					if (groups[j]) {
						if (groups[i] == groups[j]) {
							if (params[i] == params[j]) {
								nv = exID[j];	
								j = i;
							} else {
								++nv;
							}
						}
					}
				}
			}
			
			exID[i] = nv;
			
		}
		
		calculateHash();
		
	};

	Signature(const Signature & s) : actType(s.actType), parameterCount(s.parameterCount), params(new Entity*[parameterCount]), groups(new SymmetryGroup*[parameterCount]), exID(new unsigned int[parameterCount]), hashVal(s.hashVal) {

		memcpy(params, s.params, parameterCount * sizeof(Entity*));
		memcpy(groups, s.groups, parameterCount * sizeof(SymmetryGroup*));
		memcpy(exID, s.exID, parameterCount * sizeof(unsigned int));
		
	};
	
	
	~Signature() {
		delete [] params;
		delete [] groups;
		delete [] exID;
	}
	
	bool operator==(const Signature & s) const {
	
		if (actType != s.actType) return false;
		
		for (unsigned int i = 0; i < parameterCount; ++i) {
		
			Entity* const thisEnt = params[i];
			Entity* const otherEnt = s.params[i];
			
			const bool thisConst = thisEnt->isConstant();
			const bool otherConst = otherEnt->isConstant();
			if (thisConst) {
				if (!otherConst) return false; // case one - this const, other not
				if (thisEnt != otherEnt) return false; // case two - both const, not equal
			} else {
				if (otherConst) return false; // case three - this not const, other is, cannot be equal
				
				if (groups[i] != s.groups[i]) return false; // different groups - not the same
				if (exID[i] != s.exID[i]) return false;  // different relative exemplar indices - not the same
			}
		
		};
		
		return true;
	
	};


};

struct SigHash {

	size_t operator() (const pair<Signature, ActionInstanceWithSubActions*> & s) const  {
	
		return (s.first.hashVal);
	
	};

};

struct SigEq {

	size_t operator() (const pair<Signature, ActionInstanceWithSubActions*> & s, const pair<Signature, ActionInstanceWithSubActions*> & t) const  {
	
		return (s.first == t.first);
	
	};

};

void partitionActions(auto_ptr<ActionInstanceWithSubActionsList> & applicable, ActionInstanceList * helpful, ActionInstanceWithSubActionsList & helpfulActs, ActionInstanceWithSubActionsList & spareActs, list<SymmetryGroup*> * exemplars) {

	map<Entity*, SymmetryGroup*, lt<Entity> > entGroupMap;
	{
		list<SymmetryGroup*>::iterator itr = exemplars->begin();
		const list<SymmetryGroup*>::iterator itrEnd = exemplars->end();
		
		for (; itr != itrEnd; ++itr) {
		
			const list<Entity*> & tmp = (*itr)->getEntities();
			
			list<Entity*>::const_iterator inItr = tmp.begin();
			const list<Entity*>::const_iterator inEnd = tmp.end();
			
			for (; inItr != inEnd; ++inItr) {
			
				entGroupMap[*inItr] = *itr;
			}
		
		}
	}
	typedef hash_set<pair<Signature, ActionInstanceWithSubActions*>, SigHash, SigEq> SigSet;
	SigSet seenAsHelpful;
	SigSet seenAsNormal;
	
	ActionInstanceWithSubActionsList::iterator itr = applicable->begin();
	const ActionInstanceWithSubActionsList::iterator itrEnd = applicable->end();
	
	for (; itr != itrEnd; ++itr) {
	
		ActionInstance* const currAI = const_cast<ActionInstance*>((*itr)->getFirst());
		
		bool isHelpful = false;
		if (currAI->isMacroAction()) {
					
			isHelpful = (((MacroAction*) (currAI->getParentAction())))->isMarvinAction();
		
			if (isHelpful) {
				helpfulActs.push_back(*itr);
			
			} else {
				spareActs.push_back(*itr);
				
			}	
		
		} else {
		
			for (ActionInstanceList::iterator findHItr = helpful->begin(); !isHelpful && findHItr != helpful->end(); ++findHItr) {
				isHelpful = (*(currAI) == *(*findHItr));
			}

			if (isHelpful) {
				Signature lookFor(currAI, entGroupMap);
				
				SigSet::iterator helpFind = seenAsHelpful.find(make_pair(lookFor, *itr));
				
				if (helpFind == seenAsHelpful.end()) {
					helpfulActs.push_back(*itr);
					seenAsHelpful.insert(make_pair(lookFor, *itr));
				} else {
					spareActs.push_back(*itr);
					cout << ",";
					cout.flush();
				}
				
			} else {
				spareActs.push_back(*itr);
				
			}
		}
	
		
	
	}
	
};

static const int sampleSize = 3;
static map<const Action*, int, lt<Action> >* actionIDmap = 0;


map<const Action*, int, lt<Action> >* getActionIDs(StaticInformation* staticInf) {
	if (!actionIDmap) {
		actionIDmap = new map<const Action*, int, lt<Action> >();
		const int lim = staticInf->numberOfActions;
		for (int i = 0; i < lim; ++i) {
			(*actionIDmap)[staticInf->Actions[i]] = i;
			//cout << "ID for action " << staticInf->Actions[i]->name() << " = " << i << endl;
		}
	}
	return actionIDmap;
};

int getPeakParameterCount(StaticInformation* staticInf) {

	static int peakParameterCount = -1;

	if (peakParameterCount == -1) {
		peakParameterCount = 0;
		const int lim = staticInf->numberOfActions;
		for (int i = 0; i < lim; ++i) {
			const int thisLim = staticInf->Actions[i]->parameterCount;
			if (peakParameterCount < thisLim) peakParameterCount = thisLim;
		}
		//cout << "Peak parameter count = " << peakParameterCount << endl;
	}
	return peakParameterCount;
}

void localSearchSample(StaticInformation* staticInf, Plan* planToUse, ActionInstanceWithSubActionsList & applicableActions, ActionInstanceWithSubActionsList & spareApplicableActions, const int method) {
	if (planToUse->empty()) {
		const int listSize = applicableActions.size();
		const int listTwoSize = spareApplicableActions.size();
		if (listSize + listTwoSize <= sampleSize) {
			ActionInstanceWithSubActionsList::iterator moveSpareItr = spareApplicableActions.begin();
			const ActionInstanceWithSubActionsList::iterator moveSpareEnd = spareApplicableActions.end();
					
			for(; moveSpareItr != moveSpareEnd; ++moveSpareItr) {
				applicableActions.push_back(*moveSpareItr);
			}
			return;
		}
		map<const Action*, int, lt<Action> >* const actionIDs = getActionIDs(staticInf);
		const int bucketCount = staticInf->numberOfActions;
		list<bool> keepers;
		vector<list<list<bool>::iterator> > buckets(bucketCount);
		vector<int> spareInBuckets(bucketCount);
		for (int i = 0; i < bucketCount; ++i) spareInBuckets[i] = 0;
		{
			ActionInstanceWithSubActionsList::iterator alItr = applicableActions.begin();
			const ActionInstanceWithSubActionsList::iterator alEnd = applicableActions.end();
			
			for (;alItr != alEnd; ++alItr) {
				keepers.push_back(false);
				list<bool>::iterator keepItr = keepers.end();
				--keepItr;
				const Action* const currAct = (*alItr)->getFirst()->getParentAction();
				const int bucketID = (*actionIDs)[currAct];
				buckets[bucketID].push_back(keepItr);
				++(spareInBuckets[bucketID]);
			}
		}
		if (listSize <= sampleSize) {
			ActionInstanceWithSubActionsList::iterator alItr = spareApplicableActions.begin();
			const ActionInstanceWithSubActionsList::iterator alEnd = spareApplicableActions.end();
			
			for (;alItr != alEnd; ++alItr) {
				keepers.push_back(false);
				list<bool>::iterator keepItr = keepers.end();
				--keepItr;
				const Action* const currAct = (*alItr)->getFirst()->getParentAction();
				const int bucketID = (*actionIDs)[currAct];
				buckets[bucketID].push_back(keepItr);
				++(spareInBuckets[bucketID]);
			}
		}
		vector<int> chooseFrom(bucketCount);
		for (int i = 0; i < sampleSize; ++i) {
			
			int chooseSize = 0;
			for (int j = 0; j < bucketCount; ++j) {
				if (spareInBuckets[j] > 0) {
					chooseFrom[chooseSize] = j;
					++chooseSize;
				}
			}
			const int keepFromBucket = chooseFrom[rand() % chooseSize];
			const int bucketSize = spareInBuckets[keepFromBucket];
			const int keepEntry = rand() % bucketSize;
			list<list<bool>::iterator >::iterator keepItr = buckets[keepFromBucket].begin();
			for (int j = 0; j < keepEntry; ++j, ++keepItr);
			(*(*keepItr)) = true;
			buckets[keepFromBucket].erase(keepItr);
			--(spareInBuckets[keepFromBucket]);
		}


		list<bool>::iterator keepItr = keepers.begin();

		{
			ActionInstanceWithSubActionsList::iterator alItr = applicableActions.begin();
			const ActionInstanceWithSubActionsList::iterator alEnd = applicableActions.end();
		

			while (alItr != alEnd) {
				if (*keepItr) {
					++alItr;
					++keepItr;
				} else {
					ActionInstanceWithSubActionsList::iterator alTmp = alItr;
					++alItr;
					++keepItr;
					applicableActions.erase(alTmp);
				}
			}
		}

		if (listSize <= sampleSize) {
			ActionInstanceWithSubActionsList::iterator alItr = spareApplicableActions.begin();
			const ActionInstanceWithSubActionsList::iterator alEnd = spareApplicableActions.end();
			

			while (alItr != alEnd) {
				if (*keepItr) {
					applicableActions.push_back(*alItr);
				};
				++alItr;
				++keepItr;
			}
		
		}
		
	} else {

		const int listSize = applicableActions.size();
		const int listTwoSize = spareApplicableActions.size();
		if (listSize + listTwoSize <= sampleSize) {
			ActionInstanceWithSubActionsList::iterator moveSpareItr = spareApplicableActions.begin();
			const ActionInstanceWithSubActionsList::iterator moveSpareEnd = spareApplicableActions.end();
					
			for(; moveSpareItr != moveSpareEnd; ++moveSpareItr) {
				applicableActions.push_back(*moveSpareItr);
			}
			return;
		}
		
		map<const Action*, int, lt<Action> >* const actionIDs = getActionIDs(staticInf);
		Plan::iterator lastPlanAction = planToUse->end();
		--lastPlanAction;
		ActionInstance* const lastAction = lastPlanAction->first;
		const int parCount = lastAction->getParameterCount();
		const int peakParCount = getPeakParameterCount(staticInf) + 1;
		const int bucketCount = staticInf->numberOfActions;
		list<bool> keepers;

		vector<vector<list<list<bool>::iterator> > > buckets(bucketCount);

		for (int i = 0; i < bucketCount; ++i) buckets[i] = vector<list<list<bool>::iterator> >(peakParCount);

		vector<vector<int> > spareInBuckets(bucketCount);
		vector<int> spareInBucketColumn(bucketCount);
		for (int i = 0; i < bucketCount; ++i) {
			spareInBucketColumn[i] = 0;
			spareInBuckets[i] = vector<int>(peakParCount);
			for (int j = 0; j < peakParCount; ++j) spareInBuckets[i][j] = 0;
		}

		{
			ActionInstanceWithSubActionsList::iterator alItr = applicableActions.begin();
			const ActionInstanceWithSubActionsList::iterator alEnd = applicableActions.end();
			
			for (;alItr != alEnd; ++alItr) {
				keepers.push_back(false);
				list<bool>::iterator keepItr = keepers.end();
				--keepItr;
				ActionInstance* const currAI = const_cast<ActionInstance*>((*alItr)->getFirst());
				int parametersInCommon = 0;
				const int thisActParCount = currAI->getParameterCount();
				for (int j = 0; j < thisActParCount; ++j) {
					Entity* const lookFor = currAI->getParameter(j)->boundTo();
					
					for (int k = 0; k < parCount; ++k) {
						if (lastAction->getParameter(k)->boundTo() == lookFor) {
							++parametersInCommon;
							break;
						}
					}
				}
				const Action* const currAct = (*alItr)->getFirst()->getParentAction();
				const int bucketID = (*actionIDs)[currAct];
				buckets[bucketID][parametersInCommon].push_back(keepItr);
				++(spareInBuckets[bucketID][parametersInCommon]);
				++(spareInBucketColumn[bucketID]);
			}
		}
		if (listSize <= sampleSize) {
			ActionInstanceWithSubActionsList::iterator alItr = spareApplicableActions.begin();
			const ActionInstanceWithSubActionsList::iterator alEnd = spareApplicableActions.end();
			
			for (;alItr != alEnd; ++alItr) {
				keepers.push_back(false);
				list<bool>::iterator keepItr = keepers.end();
				--keepItr;
				ActionInstance* const currAI = const_cast<ActionInstance*>((*alItr)->getFirst());
				int parametersInCommon = 0;
				const int thisActParCount = currAI->getParameterCount();
				for (int j = 0; j < thisActParCount; ++j) {
					Entity* const lookFor = currAI->getParameter(j)->boundTo();
					
					for (int k = 0; k < parCount; ++k) {
						if (lastAction->getParameter(k)->boundTo() == lookFor) {
							++parametersInCommon;
							break;
						}
					}
				}
				const Action* const currAct = (*alItr)->getFirst()->getParentAction();
				const int bucketID = (*actionIDs)[currAct];
				buckets[bucketID][parametersInCommon].push_back(keepItr);
				++(spareInBuckets[bucketID][parametersInCommon]);
				++(spareInBucketColumn[bucketID]);
			}
		}
		const bool debug = true;
		if (debug) {
			for (int i = 0; i < bucketCount; ++i) {
				int numFound = 0;
				for (int j = 0; j < peakParCount; ++j) {
					numFound += spareInBuckets[i][j];
				}
				assert(numFound == spareInBucketColumn[i]);
			}
		}
		vector<int> chooseFrom(bucketCount);
		vector<int> chooseFromTwo(peakParCount);
		for (int i = 0; i < sampleSize; ++i) {
			
			int chooseSize = 0;
			for (int j = 0; j < bucketCount; ++j) {
				if (spareInBucketColumn[j] > 0) {
					chooseFrom[chooseSize] = j;
					++chooseSize;
				}
			}
			const int keepFromBucket = chooseFrom[rand() % chooseSize];
			int chooseSizeTwo = 0;
			for (int j = 0; j < peakParCount; ++j) {
				if (spareInBuckets[keepFromBucket][j] > 0) {
					chooseFromTwo[chooseSizeTwo] = j;
					++chooseSizeTwo;
				}
			}
			const int keepFromBucketTwo = chooseFromTwo[rand() % chooseSizeTwo];
			const int bucketSize = spareInBuckets[keepFromBucket][keepFromBucketTwo];
			const int keepEntry = rand() % bucketSize;
			list<list<bool>::iterator>::iterator keepItr = buckets[keepFromBucket][keepFromBucketTwo].begin();
			for (int j = 0; j < keepEntry; ++j, ++keepItr);
			(*(*keepItr)) = true;
			buckets[keepFromBucket][keepFromBucketTwo].erase(keepItr);
			--(spareInBuckets[keepFromBucket][keepFromBucketTwo]);
			--(spareInBucketColumn[keepFromBucket]);
		}

		list<bool>::iterator keepItr = keepers.begin();

		{
			ActionInstanceWithSubActionsList::iterator alItr = applicableActions.begin();
			const ActionInstanceWithSubActionsList::iterator alEnd = applicableActions.end();
			

			while (alItr != alEnd) {
				if (*keepItr) {
					++alItr;
					++keepItr;
				} else {
					ActionInstanceWithSubActionsList::iterator alTmp = alItr;
					++alItr;
					++keepItr;
					applicableActions.erase(alTmp);
				}
			}
		}

		if (listSize <= sampleSize) {
			ActionInstanceWithSubActionsList::iterator alItr = spareApplicableActions.begin();
			const ActionInstanceWithSubActionsList::iterator alEnd = spareApplicableActions.end();
			

			while (alItr != alEnd) {
				if (*keepItr) {
					applicableActions.push_back(*alItr);
				};
				++alItr;
				++keepItr;
			}
		
		}

	}
};

void sampleSpares(StaticInformation* staticInf, Plan* planToUse, list<Spare*> & spares, const int method) {

	
	if (planToUse->empty()) {
		const int listSize = spares.size();
		if (listSize <= sampleSize) return;
		map<const Action*, int, lt<Action> >* const actionIDs = getActionIDs(staticInf);
		const int bucketCount = staticInf->numberOfActions;
		list<bool> keepers;
		vector<list<list<bool>::iterator> > buckets(bucketCount);
		vector<int> spareInBuckets(bucketCount);
		for (int i = 0; i < bucketCount; ++i) spareInBuckets[i] = 0;
		{
			list<Spare*>::iterator alItr = spares.begin();
			const list<Spare*>::iterator alEnd = spares.end();
			
			for (;alItr != alEnd; ++alItr) {
				keepers.push_back(false);
				list<bool>::iterator keepItr = keepers.end();
				--keepItr;
				const Action* const currAct = (*alItr)->getData().spareAction->getParentAction();
				const int bucketID = (*actionIDs)[currAct];
				buckets[bucketID].push_back(keepItr);
				++(spareInBuckets[bucketID]);
			}
		}
		vector<int> chooseFrom(bucketCount);
		for (int i = 0; i < sampleSize; ++i) {
			
			int chooseSize = 0;
			for (int j = 0; j < bucketCount; ++j) {
				if (spareInBuckets[j] > 0) {
					chooseFrom[chooseSize] = j;
					++chooseSize;
				}
			}
			const int keepFromBucket = chooseFrom[rand() % chooseSize];
			const int bucketSize = spareInBuckets[keepFromBucket];
			const int keepEntry = rand() % bucketSize;
			list<list<bool>::iterator >::iterator keepItr = buckets[keepFromBucket].begin();
			for (int j = 0; j < keepEntry; ++j, ++keepItr);
			(*(*keepItr)) = true;
			buckets[keepFromBucket].erase(keepItr);
			--(spareInBuckets[keepFromBucket]);
		}

		{
			list<Spare*>::iterator alItr = spares.begin();
			list<Spare*>::iterator alEnd = spares.end();
			list<bool>::iterator keepItr = keepers.begin();

			while (alItr != alEnd) {
				if (*keepItr) {
					++alItr;
					++keepItr;
				} else {
					(*alItr)->getData().discard = true;
					++alItr;
					++keepItr;
				}
			}
		}
		
	} else {

		const int listSize = spares.size();
		if (listSize <= sampleSize) return;
		map<const Action*, int, lt<Action> >* const actionIDs = getActionIDs(staticInf);
		Plan::iterator lastPlanAction = planToUse->end();
		--lastPlanAction;
		ActionInstance* const lastAction = lastPlanAction->first;
		const int parCount = lastAction->getParameterCount();
		const int peakParCount = getPeakParameterCount(staticInf) + 1;
		const int bucketCount = staticInf->numberOfActions * peakParCount;
		list<bool> keepers;
		vector<list<list<bool>::iterator> > buckets(bucketCount);
		vector<int> spareInBuckets(bucketCount);
		for (int i = 0; i < bucketCount; ++i) spareInBuckets[i] = 0;
		{
			list<Spare*>::iterator alItr = spares.begin();
			const list<Spare*>::iterator alEnd = spares.end();
			
			for (;alItr != alEnd; ++alItr) {
				keepers.push_back(false);
				list<bool>::iterator keepItr = keepers.end();
				--keepItr;
				
				ActionInstance* const currAI = (*alItr)->getData().spareAction;
				int parametersInCommon = 0;
				const int thisActParCount = currAI->getParameterCount();
				for (int j = 0; j < thisActParCount; ++j) {
					Entity* const lookFor = currAI->getParameter(j)->boundTo();
					
					for (int k = 0; k < parCount; ++k) {
						if (lastAction->getParameter(k)->boundTo() == lookFor) {
							++parametersInCommon;
							break;
						}
					}
				}
				const Action* const currAct = currAI->getParentAction();
				
				const int bucketID = (peakParCount * (*actionIDs)[currAct]) + parametersInCommon;
				buckets[bucketID].push_back(keepItr);
				++(spareInBuckets[bucketID]);
			}
		}
		vector<int> chooseFrom(bucketCount);
		for (int i = 0; i < sampleSize; ++i) {
			
			int chooseSize = 0;
			for (int j = 0; j < bucketCount; ++j) {
				if (spareInBuckets[j] > 0) {
					chooseFrom[chooseSize] = j;
					++chooseSize;
				}
			}
			const int keepFromBucket = chooseFrom[rand() % chooseSize];
			const int bucketSize = spareInBuckets[keepFromBucket];
			const int keepEntry = rand() % bucketSize;
			list<list<bool>::iterator>::iterator keepItr = buckets[keepFromBucket].begin();
			for (int j = 0; j < keepEntry; ++j, ++keepItr);
			(*(*keepItr)) = true;
			buckets[keepFromBucket].erase(keepItr);
			--(spareInBuckets[keepFromBucket]);
		}

		{
			list<Spare*>::iterator alItr = spares.begin();
			list<Spare*>::iterator alEnd = spares.end();
			list<bool>::iterator keepItr = keepers.begin();

			while (alItr != alEnd) {
				if (*keepItr) {
					++alItr;
					++keepItr;
				} else {
					(*alItr)->getData().discard = true;
					++alItr;
					++keepItr;
				}
			}
		}

	}

};

void rouletteSelect(StaticInformation* staticInf, Plan* planToUse, list<Spare*> & spares, const double bias) {

	list<pair<list<Spare*>::iterator, double> > segments;
	double sigma = 0.0;
	{
		list<Spare*>::iterator alItr = spares.begin();
		const list<Spare*>::iterator alEnd = spares.end();
		
		for (;alItr != alEnd; ++alItr) {
			if (!(*alItr)->getData().discard) {
				const double weight = pow(1.0 / (*alItr)->getData().spareHeuristic, bias);
				segments.push_back(pair<list<Spare*>::iterator, double>(alItr, weight));
				sigma += weight;
			}
		}
	}

	if (segments.empty()) return;

	const double randomChoice = sigma * (rand() / ((double) RAND_MAX));

	{
		list<pair<list<Spare*>::iterator, double> >::iterator segItr = segments.begin();
		const list<pair<list<Spare*>::iterator, double> >::iterator segEnd = segments.end();

		bool foundIt = false;
		for (double acc = 0.0; segItr != segEnd; ++segItr) {
			acc += segItr->second;
			if (foundIt) {
				(*(segItr->first))->getData().discard = true;
				cout << "r";
				cout.flush();
			} else {
				if (randomChoice <= acc) {
					foundIt = true;
				} else {
					cout << "r";
					cout.flush();
					(*(segItr->first))->getData().discard = true;
				}
			} 
			
		}
		if (!foundIt) {
			cout << "Warning - check roulette selection implementation" << endl;
			(*(segItr->first))->getData().discard = false;
		}

	}

};
