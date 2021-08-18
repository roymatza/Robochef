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
#ifndef STATE_H
#define STATE_H

#include <list>
#include <ext/hash_map>

using std::list;
using __gnu_cxx::hash_map;

#include "ActionInstance.h"

#include "ActionInstanceWithSubactions.h"


/**
@author Andrew Ian Coles, Amanda Smith
*/

#include "PredicateInstanceTrie.h"

class StaticInformation;
class SymmetryGroup;
class Type;
class ActionSearchData;
class StateDiff;
class RelaxedPlanningGraph;

struct EventQueueItem {

	PredicateInstanceSet deleteEffects;
	PredicateInstanceSet addEffects;
	PredicateInstanceSet invariants;
	PredicateInstanceSet negativeInvariants;

	EventQueueItem() {};
	
	EventQueueItem(const EventQueueItem & e)
	 : deleteEffects(e.deleteEffects),
	   addEffects(e.addEffects),
	   invariants(e.invariants),
	   negativeInvariants(e.negativeInvariants) {};
	   
	EventQueueItem & operator =(const EventQueueItem & e) {
	
		deleteEffects.clear();
		addEffects.clear();
		invariants.clear();
		negativeInvariants.clear();
		
		deleteEffects = e.deleteEffects;
		addEffects = e.addEffects;
		invariants = e.invariants;
		negativeInvariants = e.negativeInvariants;
		
		return *this;
	
	}
	
};

class State {

	friend class ActionSearchData;
	friend class StateDiff;
private:

	State* parentState;
#ifdef CHECK_STATE_LEAKS
	int* leakCheck;
#endif
	
	//PredicateInstanceList currState;
	PredicateInstanceTrie currStateTrie;

	int timeStamp;
	
	ActionInstanceList internalApplicableActions( StaticInformation * staticInf, hash_map<Type*, EntitySet* > * exemplarMap, const bool & relaxedActions, const bool & tryMacroActions);
	
	State* internalApplyActions(const ActionInstanceList & toApply, const bool & relaxedActions, RelaxedPlanningGraph*& rpg, StaticInformation *& staticInf, const bool & local, const bool & supressTOs);
	
	PredicateInstanceSet predicatesToAdd;
	PredicateInstanceSet predicatesToDelete;
	PredicateInstanceSet predicatesInvariant;
	PredicateInstanceSet predicatesNegativeInvariant;
	
	
	list<EventQueueItem> eventQueue;
	
	void setToAdd(PredicateInstance* p);
	void setToDelete(PredicateInstance* p);
	
	void setInvariant(PredicateInstance* p);
	void setNegativeInvariant(PredicateInstance* p);
public:
	
	const PredicateInstanceSet & getPredicatesToAdd() const { return predicatesToAdd; };
	bool isToAdd(PredicateInstance* p) const;
	bool isToDelete(PredicateInstance* p) const;

	bool isInvariant(PredicateInstance* p) const;
	bool isNegativeInvariant(PredicateInstance* p) const;

	const list<EventQueueItem> & getEventQueue() const {
		return eventQueue;
	};
	
	ActionInstanceList achievingActions;
	typedef PredicateInstanceTrie::iterator iterator;
	typedef PredicateInstanceTrie::const_iterator const_iterator;
	
	State();
	
	~State();
	
	PredicateInstanceList* copyPredicates() const {
		return new PredicateInstanceList(currStateTrie.flatten());
	}
	
	State(StaticInformation * s, const PredicateInstanceList & p);
	
	State(StaticInformation * s);
	
	State(const State & s);
	
	State & operator=(const State & s);
	
	//void addPredicateInstance(PredicateInstance * p);
	void addPredicateInstance(PredicateInstance * p);
	void removePredicateInstance(PredicateInstance * p);
    	void removeForeignPredicateInstance(PredicateInstance * p);	
	
	void removePredicateInstance(State::iterator p);
	
	iterator begin() { return currStateTrie.begin(); };
	const_iterator begin() const { return currStateTrie.begin(); };
	
	iterator end() { return currStateTrie.end(); };
	const_iterator end() const { return currStateTrie.end(); };
	
	bool empty() const { return currStateTrie.empty(); };

	bool checkEventQueueConsistency() const;	
	ActionInstanceList applicableActions( StaticInformation * staticInf);
	ActionInstanceList applicableActions( StaticInformation * staticInf, list<SymmetryGroup*> symGroups);
	
	ActionInstanceList applicableRelaxedActions( StaticInformation * staticInf);
	ActionInstanceList applicableRelaxedUnitActions( StaticInformation * staticInf);
	
	auto_ptr<ActionInstanceWithSubActionsList> filterApplicableActions(StaticInformation * staticInf, ActionInstanceWithSubActionsList & toFilter);
	
	State* applyActions(const ActionInstanceList & toApply, RelaxedPlanningGraph*& rpg, StaticInformation *& staticInf);
	State* applyActions(const ActionInstanceList & toApply, RelaxedPlanningGraph*& rpg, StaticInformation *& staticInf, const bool & supressTOs);
	
	State* applyAction(ActionInstance * toApply, StaticInformation * staticInf);
	
	State* applyRelaxedActions(const ActionInstanceList & toApply, StaticInformation * staticInf);
    
	PredicateInstance* getPredicateInstance(const Predicate * p, Entity** parameters) const;
	PredicateInstance* getPredicateInstance(PredicateInstance* p) const;

	void switchInPredicateInstance(State::iterator itr, PredicateInstance* p);
	void applyRelaxedActionsLocally(	ActionInstanceList & toApply,
					StaticInformation * staticInf,
					bool popNonApplied,
					bool deleteNonApplied);

	void applyActionLocally(ActionInstance * toApply, StaticInformation * staticInf);
	void applyActionsLocally(ActionInstanceList & toApply, StaticInformation * staticInf);
	void applyActionsLocally(ActionInstanceList & toApply, RelaxedPlanningGraph*& rpg, StaticInformation * staticInf);
	
	void applyActionsLocally(ActionInstanceList & toApply, StaticInformation * staticInf, const bool & supressTOs);
	
	bool hasQueuedEffects() const;
	void clearPredicateFlags();
	void advanceTime();
	void advanceTimeRelaxedly();

	bool contains(const Predicate * p, Entity** parameters) const;
	bool containsNegative(const Predicate * p, Entity** parameters) const;
	bool subsumes(const State* s, StaticInformation * staticInf) const;
	bool intersectsWith(const State* s, StaticInformation * staticInf) const;
	bool containsPredicateInstance(PredicateInstance* p) const;
	
	
	
	State* copy();
	
	int getHeuristic(StaticInformation * staticInf);
	int getMarginallyLessRubbishHeuristic(StaticInformation * staticInf);
	void write(ostream & o) const;
	
	int size() const {return currStateTrie.size(); };
	const int & getTimeStamp() const { return timeStamp; };
	void timeBackOne() { timeStamp--;};
	
	
	
};

ostream & operator <<(ostream & o, const State & a);

#endif
