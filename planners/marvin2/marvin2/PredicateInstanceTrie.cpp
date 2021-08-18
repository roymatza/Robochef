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
#include "PredicateInstanceTrie.h"

#include "staticinformation.h"


PredicateInstanceTrie::PredicateInstanceTrie(const StaticInformation * s) {}

PredicateInstanceTrie::PredicateInstanceTrie() {};

PredicateInstanceTrie::PredicateInstanceTrie(const PredicateInstanceTrie & p)
 : children(p.children) {

	childSet::iterator itr = children.begin();
	childSet::iterator itrEnd = children.end();
	
	for (; itr != itrEnd; ++itr) {
		(*itr)->incrUsage();
	}


 };

PredicateInstanceTrie & PredicateInstanceTrie::operator =(const PredicateInstanceTrie & p) {

	childSet::iterator itr = children.begin();
	childSet::iterator itrEnd = children.end();
	
	for (; itr != itrEnd; ++itr) {
		if ((*itr)->decrUsage()) delete (*itr);
	}

	children = p.children;
	
	itr = children.begin();
	itrEnd = children.end();
	
	for (; itr != itrEnd; ++itr) {
		(*itr)->incrUsage();
	}

	return *this;
}

PredicateInstanceTrie::~PredicateInstanceTrie()
{

	childSet::iterator itr = children.begin();
	const childSet::iterator itrEnd = children.end();
	
	for (; itr != itrEnd; ++itr) {
		if ((*itr)->decrUsage()) delete (*itr);
	}

}


PredicateInstanceTrie::PredicateInstanceTrie(StaticInformation * staticInf, const PredicateInstanceList & p)
{

	PredicateInstanceList::const_iterator itr = p.begin();
	const PredicateInstanceList::const_iterator itrEnd = p.end();
	
	for (; itr != itrEnd; ++itr) {
		addPredicateInstance(*itr);
	}

};

bool PredicateInstanceTrie::contains(PredicateInstance * p) const {

	if (p->getParentPredicate()->isEqualityPredicate()) {
		return (*(p->getParameter(0)) == *(p->getParameter(1)));
	}
	
	childSet::const_iterator itr = children.find(p);
	
	return (itr != children.end());
		
}


bool PredicateInstanceTrie::containsNegative(const Predicate * p, Entity** parameters) const {

	if (p->isEqualityPredicate()) {
		if (parameters[0] == 0 || parameters[1] == 0) return true;
		return (parameters[0] != parameters[1]);
	}

	const int lim = p->getArgumentCount();
	
	for (int i = 0; i < lim; ++i) {
		if (parameters[i] == 0) return true;
	}
	
	PredicateInstance* tmpPI = new PredicateInstance(p, parameters);
	childSet::const_iterator itr = children.find(tmpPI);
	delete tmpPI;
	
	return (itr == children.end());

}

bool PredicateInstanceTrie::contains(const Predicate * p, Entity** parameters) const {
	if (p->isEqualityPredicate()) {
		if (parameters[0] == 0 || parameters[1] == 0) return true;
		return (parameters[0] == parameters[1]);
	}
	
	const int lim = p->getArgumentCount();
	for (int i = 0; i < lim; ++i) {
		if (parameters[i] == 0) return true;
	}
	
	PredicateInstance* tmpPI = new PredicateInstance(p, parameters);
	childSet::const_iterator itr = children.find(tmpPI);
	delete tmpPI;
	
	return (!(itr == children.end()));

	
}

PredicateInstance* PredicateInstanceTrie::getPredicateInstance(const Predicate * p, Entity** parameters) const {


	PredicateInstance* tmpPI = new PredicateInstance(p, parameters);
	childSet::const_iterator itr = children.find(tmpPI);
	delete tmpPI;
	
	if (itr == children.end()) {
		return 0;
	} else {
		return (*itr);
	}

	
}

PredicateInstance* PredicateInstanceTrie::getPredicateInstance(PredicateInstance * p) const {

	
	childSet::const_iterator itr = children.find(p);
	
	
	if (itr == children.end()) {
		return 0;
	} else {
		return (*itr);
	}
}


bool PredicateInstanceTrie::contains(const Predicate * p, Entity** parameters, const PredicateInstanceSet & exclusions) const {

	std::cerr << "Deprecated" << endl;
	exit(0);
	return false;
	
}


void PredicateInstanceTrie::addPredicateInstance(PredicateInstance * p) {

	if (children.insert(p).second) {
		p->incrUsage();
	};
	
	
}




void PredicateInstanceTrie::removePredicateInstance(PredicateInstance * p) {

	childSet::iterator itr = children.find(p);
	
	if (itr != children.end()) {
	
		if ((*itr)->decrUsage()) delete (*itr);
		
		children.erase(itr);
	
	}
	
	
}

void PredicateInstanceTrie::remove(const Predicate * p, Entity** parameters) {

	
	PredicateInstance* tmpPI = new PredicateInstance(p, parameters);
	childSet::iterator itr = children.find(tmpPI);
	delete tmpPI;
	
	if (itr != children.end()) {
	
		if ((*itr)->decrUsage()) delete (*itr);
		
		children.erase(itr);
	
	}
	
}




PredicateInstanceList PredicateInstanceTrie::flatten() const {

	
	PredicateInstanceList toReturn;
	
	childSet::iterator itr = children.begin();
	const childSet::iterator itrEnd = children.end();
	
	for (; itr != itrEnd; ++itr) {
		
		toReturn.push_back(*itr);
	
	}
	
	return toReturn;
	
}


void PredicateInstanceTrie::write(ostream & o) const {

	o << "Predicate Instance Trie" << endl;
	
	childSet::const_iterator itr = children.begin();
	
	for (; itr != children.end(); itr++) {
		o << *(*itr) << endl;
	}

}

bool PredicateInstanceTrie::contains(const PredicateInstanceTrie & p) const {

	childSet::const_iterator itr = p.children.begin();
	const childSet::const_iterator itrEnd = p.children.end();
	
	for (; itr != itrEnd; ++itr) {
	
		if (!contains(*itr)) return false;
		
	
	}

	return true;

};

bool PredicateInstanceTrie::contains(const PredicateInstanceTrie & p, const PredicateInstanceSet & exceptions) const {

	childSet::const_iterator itr = p.children.begin();
	const childSet::const_iterator itrEnd = p.children.end();
	
	for (; itr != itrEnd; ++itr) {
	
		if (contains(*itr)) {
			if (!(exceptions.find(*itr) == exceptions.end())) return false;
		} else {
			return false;
		}
		
	
	}
	return true;
	
};




bool PredicateInstanceTrie::operator ==(const PredicateInstanceTrie & p) const {

	return (children == p.children);	
	

};


void PredicateInstanceTrie::clear() {
	childSet::iterator itr = children.begin();
	const childSet::iterator itrEnd = children.end();
	
	for (; itr != itrEnd; ++itr) {
		
		if ((*itr)->decrUsage()) delete (*itr);
	
	}
	
	children.clear();
	
}

void PredicateInstanceTrie::switchIn(iterator where, PredicateInstance* what) {

	if ((*where)->decrUsage()) delete (*where);
	children.erase(where);
	
	addPredicateInstance(what);
	
	

}

ostream & operator <<(ostream & o, const PredicateInstanceTrie & p) {
	p.write(o);
	return o;
};
