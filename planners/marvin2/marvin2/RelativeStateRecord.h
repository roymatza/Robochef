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
#ifndef RELATIVESTATERECORD_H
#define RELATIVESTATERECORD_H

#include "State.h"
#include "PredicateInstanceTrie.h"
#include <ext/hash_set>

using __gnu_cxx::hash_set;
/**
@author Andrew Ian Coles, Amanda Smith
*/

class StateDiff {

private:

	
	PredicateInstanceSet added;
	PredicateInstanceSet deleted;
	
	PredicateInstanceSet nowToAdd;
	PredicateInstanceSet nowToDelete;
	PredicateInstanceSet nowInvariant;
	PredicateInstanceSet nowNegativeInvariant;
	
	void copy(PredicateInstanceList & into, const PredicateInstanceList & from) {
		into = from;
	}
	
	void copy(PredicateInstanceSet & into, const PredicateInstanceSet & from) {
		into = from;
	}
	
	void copy(PredicateInstanceList & into, const PredicateInstanceSet & from) {
		for (	PredicateInstanceSet::const_iterator itr = from.begin();
			itr != from.end();
			itr++) {
		
			into.push_back(*itr);
			
		}
	}
	
	void insert(PredicateInstanceList & into, PredicateInstance* p) {
		if (!contains(into, p)) {
			into.push_back(p);
		}
	}
	
	void insert(PredicateInstanceSet & into, PredicateInstance* p) {
		
		if (!contains(into, p)) {
			into.insert(p);
		}
	}
	
	void clear(PredicateInstanceList & toClear) {
		toClear.clear();
		
	}
	
	void clear(PredicateInstanceSet & toClear) {
		toClear.clear();
		
	}
	
	bool contains(const PredicateInstanceList & toSearch, const PredicateInstance* p) const {
	
		for (	PredicateInstanceList::const_iterator itr = toSearch.begin();
			itr != toSearch.end();
			itr++) {
		
			if (*(*itr) == *p) return true;
			
		}
		
		return false;
	
	}
	
	bool contains(const PredicateInstanceSet & toSearch, const PredicateInstance* p) const {
		for (	PredicateInstanceSet::const_iterator itr = toSearch.begin();
			itr != toSearch.end();
			itr++) {
		
			if (*(*itr) == *p) return true;
			
		}
		
		return false;
	
	}
	
	
	list<EventQueueItem> queue;
	
	unsigned int hashValue;

public:
	StateDiff() : hashValue(0) {
	
	}
	
	~StateDiff() {
		
		clear(added);
		clear(deleted);
		clear(nowToAdd);
		clear(nowToDelete);
		clear(nowInvariant);
		clear(nowNegativeInvariant);
	}
	
	StateDiff(const StateDiff & d)
	 : queue(d.queue),
	   hashValue(d.hashValue) {
	   
		copy(added, d.added);
		copy(deleted, d.deleted);
		
		copy(nowToAdd, d.nowToAdd);
		copy(nowToDelete, d.nowToDelete);
		
		copy(nowInvariant, d.nowInvariant);
		copy(nowNegativeInvariant, d.nowNegativeInvariant);
	      
	};
	
	void insertSorted(list<unsigned int> & sortedList, const unsigned int & n) const {
		if (sortedList.empty()) {
			sortedList.push_back(n);
			return;
		}
		list<unsigned int>::iterator itr = sortedList.begin();
		
		for (;itr != sortedList.end() && (*itr <= n); itr++) {}

			
		sortedList.insert(itr, n);
				
	}
	
	StateDiff(State * oldSet, State * newSet)
	 : queue(newSet->eventQueue),
	   hashValue(0) {

	   	copy(nowToAdd, newSet->predicatesToAdd);
		copy(nowToDelete, newSet->predicatesToDelete);

		copy(nowInvariant, newSet->predicatesInvariant);
		copy(nowNegativeInvariant, newSet->predicatesNegativeInvariant);
		
		list<unsigned int> sortedAddList;
		list<unsigned int> sortedDelList;
		
		for (State::iterator itr = newSet->begin(); itr != newSet->end(); itr++) {
		
			bool inOld = oldSet->containsPredicateInstance(*itr);
			
			if (!inOld) {
				
				insert(added, *itr); 
				insertSorted(sortedAddList, (*itr)->getHashValue());
				
			
			}
		
		}
		for (State::iterator itr = oldSet->begin(); itr != oldSet->end(); itr++) {
		
			bool inNew = newSet->containsPredicateInstance(*itr);
			
			if (!inNew) {
				
				insert(deleted, *itr);
				insertSorted(sortedDelList, (*itr)->getHashValue());
			
			}
		
		}
		
		for (	list<unsigned int>::iterator itr = sortedAddList.begin();
			itr != sortedAddList.end();
			itr++) {
		
			hashValue += (*itr);	
		}
		
		for (	list<unsigned int>::iterator itr = sortedDelList.begin();
			itr != sortedDelList.end();
			itr++) {
		
			hashValue -= (*itr);	
		}
	
	}

	StateDiff & operator =(const StateDiff & s) {
	
		clear(added);
		clear(deleted);
		clear(nowToAdd);
		clear(nowToDelete);
		clear(nowInvariant);
		clear(nowNegativeInvariant);
		
		queue.clear();
		
		copy(added, s.added);
		copy(deleted, s.deleted);
		copy(nowToAdd, s.nowToAdd);
		copy(nowToDelete, s.nowToDelete);
		
		
		copy(nowInvariant, s.nowInvariant);
		copy(nowNegativeInvariant, s.nowNegativeInvariant);
		
		queue = s.queue;
		
		hashValue = s.hashValue;
		 

		return *this;
	}
	
	bool compSetsManually(const PredicateInstanceSet & a, const PredicateInstanceSet & b) const {
	
		if (a.size() != b.size()) return false;
		if (a.empty() && b.empty()) return true;
		if (a.empty()) return false;
		if (b.empty()) return false;
		for (	PredicateInstanceSet::const_iterator itr = a.begin();
			itr != a.end();
			itr++ ) {
		
			bool found  = false;
			if ((*itr) == 0) {
				cout << "Zero value on set to compare 1" << endl;
			}
			
			for (	PredicateInstanceSet::const_iterator oitr = b.begin();
				!found && oitr != b.end();
				oitr++) {
				if ((*oitr) == 0) {
					cout << "Zero value on set to compare 2" << endl;
				}
				found = (*(*itr) == *(*oitr));
					
			}
			
			if (!found) return false;
				
		}
		return true;
	
	};
	
	bool compSetsManually(const PredicateInstanceList & a, const PredicateInstanceList & b) const {
	
		if (a.size() != b.size()) return false;
		
		for (	PredicateInstanceList::const_iterator itr = a.begin();
			itr != a.end();
			itr++ ) {
		
			bool found  = false;
			
			for (	PredicateInstanceList::const_iterator oitr = b.begin();
				!found && oitr != b.end();
				oitr++) {
			
				found = (*(*itr) == *(*oitr));
					
			}
			
			if (!found) return false;
				
		}
		return true;
	
	};
	
	bool equalQueue(const StateDiff & other) const {
	
		if (other.queue.size() != queue.size()) return false;
		
		list<EventQueueItem>::const_iterator itrOne = queue.begin();
		list<EventQueueItem>::const_iterator itrTwo = other.queue.begin();
		
		for (; itrOne != queue.end(); itrOne++, itrTwo++) {
		
			if (!compSetsManually(itrOne->deleteEffects, itrTwo->deleteEffects)) return false;
			if (!compSetsManually(itrOne->addEffects, itrTwo->addEffects)) return false;
			if (!compSetsManually(itrOne->invariants, itrTwo->invariants)) return false;
		
		}
		
		return true;
	
	}
	
	bool operator==(const StateDiff & other) const {
	
		
		if (!compSetsManually(added, other.added)) return false;
		if (!compSetsManually(deleted, other.deleted)) return false;
		if (!compSetsManually(nowToAdd, other.nowToAdd)) return false;
		if (!compSetsManually(nowToDelete, other.nowToDelete)) return false;
		if (!compSetsManually(nowInvariant, other.nowInvariant)) return false;
		if (!compSetsManually(nowNegativeInvariant, other.nowNegativeInvariant)) return false;
		
		if (!equalQueue(other)) return false;
		
		return true;
	
	}	
	
	int getHashValue() const { return hashValue; };
};

struct StateDiffHash {

	size_t operator()(const StateDiff & x) const {
		return (size_t) x.getHashValue();
	} 

};

struct StateEq {

	bool operator()(const StateDiff & a, const StateDiff & b ) const {
		return (a == b);
	} 

};


class RelativeStateRecord {

private:
	State* oldState;
	hash_set<StateDiff, StateDiffHash, StateEq> memoised;

public:

    	RelativeStateRecord() {};

	void clear(State* w) {
		oldState = w;
		memoised.clear();
	}
	
	bool needToVisit(State* newState) {
		//return true;
		StateDiff d(oldState, newState);
		
		return (memoised.insert(d).second);
	}
	
    	~RelativeStateRecord() {};

};

class CachedHeuristic {

public:
	const bool deadend;
	int heuristic;
	ActionInstanceList helpfulActions;

	CachedHeuristic() : deadend(true) {}

	CachedHeuristic(const int & h, const ActionInstanceList & l) : deadend(false), heuristic(h), helpfulActions(l) {};

};

class HeuristicCache {

private:
	State* oldState;
	hash_map<StateDiff, CachedHeuristic, StateDiffHash, StateEq> memoised;

public:

    	HeuristicCache() {};

	void clear(State* w) {
		oldState = w;
		memoised.clear();
	}
	
	auto_ptr<StateDiff> getDiff(State* newState) {
		return auto_ptr<StateDiff>(new StateDiff(oldState, newState));
	}

	CachedHeuristic * isCached(auto_ptr<StateDiff> & d) {

		hash_map<StateDiff, CachedHeuristic, StateDiffHash, StateEq>::iterator itr = memoised.find(*(d.get()));
		if (itr == memoised.end()) return 0;
		return &(itr->second);
	}
	
	void cache(auto_ptr<StateDiff> & d,CachedHeuristic * c) {
		memoised.insert(pair<StateDiff, CachedHeuristic>(*(d.get()), *c));
	}
    	~HeuristicCache() {};

};

#endif
