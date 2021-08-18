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

#ifndef HASHDEFINITIONS_H
#define HASHDEFINITIONS_H

#include <ext/hash_map>
#include <set>
#include <list>

using std::set;
using std::list;
using __gnu_cxx::hash_map;

#include <ptree.h>

using namespace VAL;

#define HASHFOR(NAME) namespace __gnu_cxx { using std::size_t; template<> struct hash<NAME*> { size_t operator()(NAME* const & __x) const { return reinterpret_cast<size_t>(__x); } }; template<> struct hash<const NAME*> { size_t operator()(const NAME* const & __x) const { return reinterpret_cast<size_t>(__x); } }; }

#define SETFOR(NAME, SETNAME) class NAME; typedef set<NAME*, lt<NAME> > SETNAME;

template<class T> struct lt {

	bool operator()(const T* const & a, const T* const & b) const {
		return (reinterpret_cast<unsigned int>(a) < reinterpret_cast<unsigned int>(b));
	}
	
};

HASHFOR(pred_decl);

HASHFOR(pred_symbol);

HASHFOR(parameter_symbol);

HASHFOR(const_symbol);

HASHFOR(pddl_type);

class Entity;
class Predicate;
class Action;
class PredicateInstance;
class ActionInstance;
class PredicateInstanceTrieNode;
class Type;
class ApplicabilityTestingState;
class PONode;
class ADLPONode;
class ActionInstance;


HASHFOR(Entity);
SETFOR(Entity, EntitySet);

HASHFOR(Predicate);

HASHFOR(Action);


HASHFOR(PredicateInstance);
HASHFOR(ActionInstance);

HASHFOR(PredicateInstanceTrieNode);

HASHFOR(Type);

HASHFOR(ApplicabilityTestingState);
SETFOR(ApplicabilityTestingState, ATSSet);

SETFOR(PONode, PONodeSet);
SETFOR(ADLPONode, ADLPONodeSet);

SETFOR(ActionInstance, ActionInstanceSet);



/*class EntitySet : public list<Entity*> {

public:

	typedef list<Entity*>::iterator iterator;
	typedef list<Entity*>::const_iterator const_iterator;
	
	
	pair<iterator, bool> insert(Entity* p) {
		
		iterator checkIn = find(p);
		if (checkIn == end()) {
			push_front(p);
	 		return pair<iterator, bool>(begin(), true);
		}
		return pair<iterator, bool>(checkIn, false);
		
	}
	
	const_iterator find(const Entity* p) const {
	
		for (const_iterator itr = begin(); itr != end(); itr++) {
			if (p == (*itr)) return itr;
		}
		return end();
	
	}
	
	iterator find(const Entity* p) {
	
		for (iterator itr = begin(); itr != end(); itr++) {
			if (p == (*itr)) return itr;
		}
		return end();
	
	}
	

};*/

#endif
