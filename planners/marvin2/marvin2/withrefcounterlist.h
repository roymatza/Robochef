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
#ifndef WITHREFCOUNTERLIST_H
#define WITHREFCOUNTERLIST_H

#include "withrefcounter.h"
#include "hashdefinitions.h"
#include <list>
#include <set>
#include <utility>

using std::list;
using std::set;
using std::pair;

/**
@author Andrew Ian Coles, Amanda Smith
*/


template<class T> class WithRefCounterList : list<T*> {

public:

	
	typedef typename list<T*>::iterator iterator;
	typedef typename list<T*>::const_iterator const_iterator;
	
	typedef typename list<T*>::reverse_iterator reverse_iterator;
	typedef typename list<T*>::const_reverse_iterator const_reverse_iterator;
	
	WithRefCounterList() : list<T*>() { };
	
	WithRefCounterList(const WithRefCounterList<T> & p) : list<T*>(p) {
		iterator itr = begin();
		const iterator itrEnd = end();

		for (; itr != itrEnd; ++itr) {
			if ((*itr) != 0) (*itr)->incrUsage();
		}

	}

	WithRefCounterList<T> & operator = (const WithRefCounterList<T> & p) {

		clear();

		for (const_iterator itr = p.begin(); itr != p.end(); itr++) {
			push_back(*itr);
		}

		return *this;
	}

	~WithRefCounterList() {

		clear();
	}

	void push_back(T * p) {

		((list<T*>*)this)->push_back(p);
		if (p != 0) p->incrUsage();

	}

	void push_front(T * p) {

		((list<T*>*)this)->push_front(p);
		if (p != 0) p->incrUsage();

	}

	void pop_back() {
		if (back() != 0 && back()->decrUsage()) {
			delete back();
		}
		((list<T*>*)this)->pop_back();

	}

	void pop_front() {
		if (front() != 0 && front()->decrUsage()) {
			delete front();
		}
		((list<T*>*)this)->pop_front();

	}


	void clear() {

		for (iterator itr = begin(); itr != end(); itr++) {
			if ((*itr) != 0 && (*itr)->decrUsage()) delete (*itr);
		}

		((list<T*>*)this)->clear();
	}


	inline iterator begin() 		{ return (((list<T*>*)this)->begin());	};
	inline const_iterator begin() const 	{ return (((list<T*>*)this)->begin());	};
	inline iterator end() 			{ return (((list<T*>*)this)->end());	};
	inline const_iterator end() const 	{ return (((list<T*>*)this)->end());	};

	inline reverse_iterator rbegin() 		{ return (((list<T*>*)this)->rbegin());	};
	inline const_reverse_iterator rbegin() const 	{ return (((list<T*>*)this)->rbegin());	};
	inline reverse_iterator rend() 			{ return (((list<T*>*)this)->rend());	};
	inline const_reverse_iterator rend() const 	{ return (((list<T*>*)this)->rend());	};

	inline bool empty() const 		{ return (((list<T*>*)this)->empty());	};

	inline T* front() const 		{ return ((list<T*>*)this)->front(); 	};
	inline T* back() const 			{ return ((list<T*>*)this)->back(); 	};

	iterator erase(iterator pos) {
		if ((*pos) != 0 && (*pos)->decrUsage()) delete *pos;
		return (((list<T*>*)this)->erase(pos));
	}

	inline int size() const { return (((list<T*>*)this)->size()); };

};

template<class T> class WithRefCounterSet : set<T*, lt<T> > {

public:

	typedef typename set<T*, lt<T> >::iterator iterator;
	typedef typename set<T*, lt<T> >::const_iterator const_iterator;
	
	WithRefCounterSet() : set<T*, lt<T> >() { };
	
	WithRefCounterSet(const WithRefCounterSet<T> & p) : set<T*, lt<T> >(p) {
		
		for (iterator itr = begin(); itr != end(); itr++) {
			if ((*itr) != 0) (*itr)->incrUsage();
		}
	
	}
	
	WithRefCounterSet<T> & operator = (const WithRefCounterSet<T> & p) {
		
		clear();
		
		for (const_iterator itr = p.begin(); itr != p.end(); itr++) {
			insert(*itr);
		}
		
		return *this;
	}
	
	~WithRefCounterSet() {
		
		clear();	
	}
	

	pair<iterator, bool> insert(T* p) {
		
	 	pair<iterator, bool> toReturn = ((set<T*, lt<T> >*)this)->insert(p);
		
		if (toReturn.second) {
			if (p != 0) p->incrUsage();
		}
		return toReturn;		
		
	}
	
	iterator insert(iterator pos, T* p) {
		
	 	if (p != 0) p->incrUsage();
		return ((set<T*, lt<T> >*)this)->insert(p);
		
	
	}
		
	
	void clear() {
		
		for (iterator itr = begin(); itr != end(); itr++) {
			if ((*itr) != 0) {
				if ((*itr)->decrUsage()) delete (*itr);
			}
		}
		
		((set<T*, lt<T> >*)this)->clear();
	}

	
	iterator begin() {
		return (((set<T*, lt<T> >*)this)->begin());
	};
	const_iterator begin() const {
		return (((set<T*, lt<T> >*)this)->begin());
	};

	iterator end() {
		return (((set<T*, lt<T> >*)this)->end());
	};
	const_iterator end() const {
		return (((set<T*, lt<T> >*)this)->end());
	};
	
	bool empty() const {
		return (((set<T*, lt<T> >*)this)->empty());
	};
	
	iterator find(T* k) {
		return (((set<T*, lt<T> >*)this)->find(k));
	}
	
	const_iterator find(T* k) const {
		return (((set<T*, lt<T> >*)this)->find(k));
	}
	
	iterator erase(iterator pos) {
		if ((*pos) != 0 && (*pos)->decrUsage()) delete *pos;
		return (((set<T*, lt<T> >*)this)->erase(pos));
	}
	
	int size() const { return (((set<T*, lt<T> >*)this)->size()); };
	
};






#endif
