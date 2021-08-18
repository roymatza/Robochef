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
#ifndef PREDICATEINSTANCEANDSTATE_H
#define PREDICATEINSTANCEANDSTATE_H

#include <PredicateInstance.h>

/**
@author Andrew Ian Coles, Amanda Smith
*/
class PredicateInstanceAndState
{
private:

	bool unsafe;
	bool safe;
	PredicateInstance* currPred;
	
	int usageCount;
	
public:
	PredicateInstanceAndState(Predicate* p)
	   : 	currPred(new PredicateInstance(p)), unsafe(false), safe(false), usageCount(0) {currPred->incrUsage();};
	
	PredicateInstanceAndState(simple_effect* s,
			  hash_map<const pred_symbol*, Predicate* > & pred_symbolToPredicate,
			  hash_map<parameter_symbol*, Entity* > parameter_symbolToEntity)
	   :    currPred(new PredicateInstance(s, pred_symbolToPredicate, parameter_symbolToEntity)), unsafe(false), safe(false), usageCount(0) {currPred->incrUsage();};

	PredicateInstanceAndState(simple_goal* s,
			  hash_map<const pred_symbol*, Predicate* > & pred_symbolToPredicate,
			  hash_map<parameter_symbol*, Entity* > & parameter_symbolToEntity)
			  
			  
	   :	currPred(new PredicateInstance(s, pred_symbolToPredicate, parameter_symbolToEntity)), unsafe(false), safe(false), usageCount(0) {currPred->incrUsage();};
	  
	PredicateInstanceAndState(PredicateInstance* p)
	   :   currPred(p),
	       unsafe(false),
	       safe(false), usageCount(0) {
	 
	       currPred->incrUsage();

	}
	
	PredicateInstanceAndState(const PredicateInstanceAndState & p)
	   :   currPred(p.currPred),
	       unsafe(p.unsafe),
	       safe(p.safe), usageCount(0) {
	 
	       currPred->incrUsage();

	}
	
	PredicateInstanceAndState(PredicateInstanceAndState * p)
	   :   currPred(p->currPred),
	       unsafe(p->unsafe),
	       safe(p->safe), usageCount(0) {
	 
	       currPred->incrUsage();

	}
	
	~PredicateInstanceAndState() {
	
		if (currPred->decrUsage()) delete currPred;
	}	     
	
	PredicateInstance * getPredicateInstance() {
		return currPred;
	}
	
	void incrUsage() {
		usageCount++;
	}
	
	bool decrUsage() {
		return (--usageCount <= 0);
	}
	bool isSafe() const { return safe; };
	bool isUnsafe() const { return unsafe; };
	
	void setSafe(const bool & b) { safe = b; };
	void setUnsafe(const bool & b) { unsafe = b; };
	
	void write(ostream & o) const {
		o << *currPred;
		if (safe) o << " (safe)";
		if (unsafe) o << " (unsafe)";
	};

	void setParameter(int index, Parameter* p) {
		currPred->setParameter(index, p);
	}
	
	Parameter* getParameter(int index) {
		return currPred->getParameter(index);
			}
	
	const Parameter* getParameter(int index) const {
		return currPred->getParameter(index);
	}

	bool isOfType(const Predicate * p) const {
		return currPred->isOfType(p);
	};
	
	Predicate* getParentPredicate() const {
		return currPred->getParentPredicate();
	}
};
ostream & operator <<(ostream & o, const PredicateInstanceAndState & a);

#endif
