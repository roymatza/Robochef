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
#include "PredicateInstance.h"
#include "ActionInstance.h"

ostream & operator <<(ostream & o, const PredicateInstance & a) {
	a.write(o);
	return o;
};

PredicateInstance::PredicateInstance(PredicateInstance* p)
	: 	WithRefCounter(), 
		parentPredicate(p->getParentPredicate()),
		parameterCount(parentPredicate->argumentCount),
		parameters(new Parameter*[parameterCount]),
		masterID(0),
		haveID(false) {

	for (int i = 0; i < parameterCount; ++i) {
		parameters[i] = p->getParameter(i); 
		parameters[i]->incrUsage();
	}
		
}

bool PredicateInstance::operator==(const PredicateInstance & p) const {
	
	if (p.parentPredicate != parentPredicate) return false;
	
	for (int i = 0; i < parameterCount; ++i) {
	
		if (!(*(parameters[i]) == *(p.parameters[i]))) return false;
	
	}
	
	return true;

}

bool PredicateInstance::equalsWithout(const PredicateInstance & p, Entity* exclude, Entity* matchExclude) const {
	//if the predicates aren't the same type they're not the same
	if (p.parentPredicate != parentPredicate) return false;

	for (int i = 0; i < parameterCount; ++i) {
		//if the predicate we're looking for is the one to be excluded
		
		if(parameters[i]->boundTo() == exclude) {
		
			if (!(p.parameters[i]->boundTo() == matchExclude) ) return false;
			
		} else {
		
			if(!(*(parameters[i]) == *(p.parameters[i])) ) return false;
		
		}


	}

	return true;

}

int PredicateInstance::equalsWithoutPlusOne(const PredicateInstance & p, Entity* exclude, Entity* matchExclude) const {
	//if the predicates aren't the same type they're not the same
	if (p.parentPredicate != parentPredicate) return -1;

	int toReturn = 0;

	for (int i = 0; i < parameterCount; ++i) {
		//if the predicate we're looking for is the one to be excluded
		if( (parameters[i]->boundTo() == exclude)) {
		 
			if (!(p.parameters[i]->boundTo() == matchExclude) ) return -1;
			
		} else {
		
			if(!(*(parameters[i]) == *(p.parameters[i])) ) ++toReturn;
			
		}


	}

	return toReturn;

}

bool PredicateInstance::operator<(const PredicateInstance & p) const {

	const Predicate* const opp = p.parentPredicate;
	
	if (parentPredicate != opp) {
		return (parentPredicate < opp);
	}

	for (int i = 0; i < parameterCount; ++i) {
	
		const Parameter* const a = parameters[i];
		const Parameter* const b = p.parameters[i];
		
		if (a < b) return true;
		if (a > b) return false;
		
	}
	
	return false;
	
}
