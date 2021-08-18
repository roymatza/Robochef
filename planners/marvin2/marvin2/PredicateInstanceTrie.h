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
#ifndef PREDICATEINSTANCETRIE_H
#define PREDICATEINSTANCETRIE_H

#include "hashdefinitions.h"

#include "PredicateInstance.h"

/**
@author Andrew Ian Coles, Amanda Smith
*/


class StaticInformation;

struct PIPLT {

	bool operator()(const PredicateInstance* const & a, const PredicateInstance* const & b) const {
	
		return (*a < *b);
	
	};

};

class PredicateInstanceTrie {

private:

	typedef set<PredicateInstance*, PIPLT> childSet;
	childSet children;
		
	//void addPredicateInstanceInternal(PredicateInstance * p, PredicateInstanceTrieNode * currNode, int & level, const int & limit);
	//bool removePredicateInstanceInternal(PredicateInstance * p, PredicateInstanceTrieNode * currNode, int & level, const int & limit);
	
	//bool removeInternal(const Predicate * p, Entity** parameters, PredicateInstanceTrieNode * currNode, int & level, const int & limit);
	
	//bool containsInternal(PredicateInstance * p, const PredicateInstanceTrieNode * currNode, int & level, const int & limit) const;
	
	
	//bool containsInternal(PredicateInstanceTrieNode * currNode, PredicateInstanceTrieNode * compareNode) const;
	
	//bool equalsInternal(PredicateInstanceTrieNode * currNode, PredicateInstanceTrieNode * compareNode) const;
	
	
	//bool containsInternal(PredicateInstanceTrieNode * currNode, PredicateInstanceTrieNode * compareNode, const PredicateInstanceSet & exceptions) const;
	
	//bool containsInternalTwo(const Predicate * p, Entity** Parameters, const PredicateInstanceTrieNode * currNode, int level, const int & limit) const;

	//PredicateInstance* getPredicateInstanceInternal(const Predicate * p, Entity** Parameters, const PredicateInstanceTrieNode * currNode, int level, const int & limit) const;

	//PredicateInstance* getPredicateInstanceInternal(PredicateInstance * p, const PredicateInstanceTrieNode * currNode, int level, const int & limit) const;
	
	//bool containsInternalTwo(const Predicate * p, Entity** Parameters, const PredicateInstanceSet & exclusions, const PredicateInstanceTrieNode * currNode, int level, const int & limit) const;
	
	//void flattenInternal(PredicateInstanceTrieNode * currNode, int level, int & limit, PredicateInstanceList & toReturn) const;

public:

	typedef childSet::iterator iterator;
	typedef childSet::const_iterator const_iterator;
	
	PredicateInstanceTrie(const StaticInformation * s);
	PredicateInstanceTrie();
	PredicateInstanceTrie(const PredicateInstanceTrie & p);

	PredicateInstanceTrie(StaticInformation * staticInf, const PredicateInstanceList & p);
	void addPredicateInstance(PredicateInstance * p);
	void removePredicateInstance(PredicateInstance * p);
	bool contains(PredicateInstance * p) const;
	
	
	PredicateInstance* getPredicateInstance(const Predicate * p, Entity** parameters) const;
	
	PredicateInstance* getPredicateInstance(PredicateInstance * p) const;
	
	void remove(const Predicate * p, Entity** parameters);
	
	bool contains(const Predicate * p, Entity** parameters) const;
	bool containsNegative(const Predicate * p, Entity** parameters) const;
	bool contains(const Predicate * p, Entity** parameters, const PredicateInstanceSet & exclusions) const;

	bool contains(const PredicateInstanceTrie & p) const;
	bool contains(const PredicateInstanceTrie & p, const PredicateInstanceSet & exceptions) const;
	
	~PredicateInstanceTrie();

	PredicateInstanceTrie & operator =(const PredicateInstanceTrie & p);

	PredicateInstanceList flatten() const;
	
	void write(ostream & o) const;
	void write(ostream & o, PredicateInstanceTrieNode * currNode, list<string> & soFar, int level, int & limit) const;
	
	bool operator==(const PredicateInstanceTrie & other) const;
	
	void clear();
	
	iterator begin() { return children.begin(); };
	const_iterator begin() const { return children.begin(); };
	
	iterator end() { return children.end(); };
	const_iterator end() const { return children.end(); };
	
	bool empty() const { return children.empty(); };
	int size() const { return children.size(); };
	
	void switchIn(iterator where, PredicateInstance* what);

};

ostream & operator <<(ostream & o, const PredicateInstanceTrie & p);


#endif
