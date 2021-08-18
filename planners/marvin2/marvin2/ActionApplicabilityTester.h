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
#ifndef ACTIONAPPLICABILITYTESTER_H
#define ACTIONAPPLICABILITYTESTER_H

/**
@author Andrew Ian Coles, Amanda Smith
*/
#include <ext/slist>

using __gnu_cxx::slist;

#include "Action.h"
#include "ADLAction.h"
#include "ActionInstance.h"
#include "AbstractStuff.h"
#include <set>

using std::set;

class State;
class StaticInformation;

enum atsChildType { ATS_CONTAINER, ATS_PRECONDITION, ATS_NEGATIVEPRECONDITION };


class ApplicabilityTestingState;




struct NewLT {

	bool operator()(const pair<AbstractPredicate, ApplicabilityTestingState*> & a,
			const pair<AbstractPredicate, ApplicabilityTestingState*> & b) const {
	
		if (a.first < b.first) return true;
		if (b.first < a.first) return false;
		return (a.second < b.second);
	
	}
	
	bool verboseCompare(const pair<AbstractPredicate, ApplicabilityTestingState*> & a,
			    const pair<AbstractPredicate, ApplicabilityTestingState*> & b) const {
	
		if (a.first.verboseLT(b.first)) {
			cout << "Comparison = true: first AbstractPredicate is less than second AbstractPredicate" << endl;
			return true;
		} else {
			cout << "First AbstractPredicate not is less than second AbstractPredicate" << endl;
		}
		if (b.first.verboseLT(a.first)) {
			cout << "Comparison = false: second AbstractPredicate is less than first AbstractPredicate" << endl;
			return false;
		} else {
			cout << "Second AbstractPredicate not is less than first AbstractPredicate" << endl;
		}
		return (a.second < b.second);
	
	}
};
typedef set<pair<AbstractPredicate, ApplicabilityTestingState*>, NewLT > WaitingSet;
typedef hash_map<const Predicate*, WaitingSet > WaitingHash;

struct RemoveQueueLT {
	
	bool operator()(const pair<WaitingSet*, WaitingSet::iterator> & a,
			const pair<WaitingSet*, WaitingSet::iterator> & b) {
	
		if ((a.first) < (b.first)) return true;
		if ((b.first) < (a.first)) return false;
		
		const WaitingSet::iterator ione = a.second;
		const WaitingSet::iterator itwo = b.second;
		
		NewLT toUse;
		return toUse(*ione, *itwo);
					
	}
};


class ActionApplicabilityTester : public ActionVisitor {

private:
	
	
	
	
	map<ActionInstance*, ActionInstanceList, lt<ActionInstance> > * actionDependencies;
	
	StaticInformation * staticInf;
	
	list<ApplicabilityTestingState*>* waitingQueue;
	list<ApplicabilityTestingState*>* processQueue;
	
	
	ActionInstanceList newActions;
	PredicateInstanceList newPredicates;

	void process(ApplicabilityTestingState * s, State * p, State * n) ;
	
	
	
public:
    
    	ActionApplicabilityTester(StaticInformation * staticInfIn);

    	virtual ~ActionApplicabilityTester();
	
	void makeNextActions(State * positiveState, State * negativeState);

	
	virtual void visitStripsAction(const StripsAction * c);
	
	virtual void visitADLAction(const ADLAction * c);
	
	ActionInstanceList & getNewActions() { return newActions; };
	
	map<ActionInstance*, ActionInstanceList, lt<ActionInstance> > * getADs() {
		return actionDependencies;
	}

};

#endif
