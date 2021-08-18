#ifndef RELAXEDPLANNINGGRAPH_H
#define RELAXEDPLANNINGGRAPH_H

#include "State.h"
#include "staticinformation.h"
#include "plan.h"
#include "hashdefinitions.h"

#include "ActionInstanceWithSubactions.h"

class RelaxedPlanningGraph{

private:
	typedef hash_map<const ActionInstance*, ActionInstance*, ActionInstanceHash, ActionInstanceEq> MasterAIHash;
	State::iterator * levelEnds;
	int levelCount;
	State* predicates;
	State* allPredicates;
	
	ActionInstanceList actions;
	ActionInstanceList::iterator * actionLevelEnds;
	bool goalsAppeared;

	void isInLayer(const PredicateInstanceList & toCheck, int layer, PredicateInstanceList & found, PredicateInstanceList & notFound);

	void intersectionAndUnionOfPreconditions(list<ActionInstance*> & achievingActions, list<PredicateInstance*> & intersection, list<PredicateInstance*> & unionMinusIntersection);

	list<PredicateInstance*> verifyLandmarks(list<PredicateInstance*> & potentialLandmarks, StaticInformation & problemInformation);

	void calculateRPG(StaticInformation & problemInformation, State * initialState, const bool & stopWhenGoalsAppear);
	
	//ActionInstanceList extraActions(StaticInformation * staticInf, const PredicateInstanceList & extras);
	
	
	int goalCount;
	int negativeGoalCount;
	unsigned int* goalIDs;
	unsigned int* negativeGoalIDs;
	
	unsigned int ** supportArray;
	unsigned int ** negativeSupportArray;
	ActionInstance** applicableReal;
	
	//bool* keepAction;
	bool* parentAchieved;
	
	bool* resetPA;
	
	unsigned int ** actionNeeds;
	unsigned int ** actionNegativeNeeds;
	
	unsigned int * subActionOf;
	unsigned int ** subActions;
	
	map<ActionInstance*, ActionInstanceList, lt<ActionInstance> > * dependencyMap;
	MasterAIHash* masterAI;
	
	
	unsigned int ** achieveArray;
	unsigned int ** negativeAchieveArray;
	
	bool* achieved;
	bool* negativeAchieved;
	
	unsigned int * achievedAt;
	unsigned int * firstAchiever;
	
	unsigned int * negativeAchievedAt;
	unsigned int * firstNegativeAchiever;
	
	bool* resetAchieved;
	bool* resetNegativeAchieved;
	bool* negativeInitially;
	
	PredicateInstance** achievedReal;
	bool* predicateInstanceIsGoal;
	bool* predicateInstanceIsNegativeGoal;

	int** containers;
	
	int* containersActually;
	int* resetContainers;
	
	unsigned int* defaultlyApplicable;
	
	unsigned int actionCount;
	unsigned int predicateCount;
	
	unsigned int resetActionSize;
	unsigned int resetPredicateSize;
	
	unsigned int resetPASize;
	
	inline void resetRemaining() {
		memcpy(containersActually, resetContainers, resetActionSize);
		memcpy(achieved, resetAchieved, resetPredicateSize);
		memcpy(negativeAchieved, resetNegativeAchieved, resetPredicateSize);
		memcpy(negativeInitially, resetNegativeAchieved, resetPredicateSize);
		memcpy(parentAchieved, resetPA, resetPASize);
	}
	
	inline void miniResetRemaining() {
		memcpy(containersActually, resetContainers, resetActionSize);
		memcpy(achieved, resetAchieved, resetPredicateSize);
		memcpy(negativeAchieved, resetNegativeAchieved, resetPredicateSize);
		memcpy(parentAchieved, resetPA, resetPASize);
	}
	
	void applyPositiveAndNegative(ActionInstance* & a,
					State* & posState, State* & negState,State* & allPredicates,
					PredicateInstanceList* & newPositive, PredicateInstanceList* & newNegative);
	
public:

	RelaxedPlanningGraph(StaticInformation & problemInformation, State * initialState);
	
	RelaxedPlanningGraph(RelaxedPlanningGraph & oldRPG, StaticInformation & problemInformation, State * initialState);

	RelaxedPlanningGraph(StaticInformation & problemInformation, State * initialState, const bool & stopWhenGoalsAppear);

	~RelaxedPlanningGraph();
	
	State::iterator beginningOfLevel(const int & index) {
		if (index == 0) return predicates->begin();
		return levelEnds[index-1] ;
	};

	State::iterator endOfLevel(const int & index) {
		if(index < levelCount) {
			return levelEnds[index];
		} else {
			return levelEnds[levelCount - 1];
		}
	};

	ActionInstanceList::iterator beginningOfActionsApplicableAtLevel(const int & index) {
		if (index == 0) return actions.begin();
		return actionLevelEnds[index - 1];
	};

	ActionInstanceList::iterator endOfActionsApplicableAtLevel(const int & index) {
		if(index < levelCount - 1) {
			return actionLevelEnds[index];
		} else {
			return actionLevelEnds[levelCount - 2];
		}
	};

	void write(ostream & o);

	list<PredicateInstance*> extractLandmarks(StaticInformation & problemInformation);

	int earliestAppearance(PredicateInstance* toFind);

	Plan extractRelaxedPlan(StaticInformation & problemInformation, int & heuristic);
	Plan extractRelaxedPlanQuickly(RelaxedPlanningGraph & oldRPG, StaticInformation & problemInformation, int & heuristic);

	bool didGoalsAppear() const {return goalsAppeared; };
	
	bool operator==(RelaxedPlanningGraph & r);
	
	void makeSupportStructure(StaticInformation * staticInf);
	
	
	ActionInstance* getMasterAI(const ActionInstance* const & f) const;
	
	auto_ptr<ActionInstanceList> freeActions(StaticInformation & problemInformation, State * initialState); 

	auto_ptr<ActionInstanceWithSubActionsList> applicableRelaxedActions(StaticInformation & problemInformation, State * initialState, const EntitySet* const & exemplars);
	
	void getApplicableSubActions(ActionInstance* const & a, ActionInstanceList & c) const;
	
	pair<set<unsigned int>, set<unsigned int> > bareMinimumPreconditions(const unsigned int & actionID, const unsigned int & layer) const;


};

ostream & operator <<(ostream & o, RelaxedPlanningGraph & p);

#endif
