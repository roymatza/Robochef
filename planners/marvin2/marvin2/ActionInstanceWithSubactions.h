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
#ifndef ACTIONINSTANCEWITHSUBACTIONS_H
#define ACTIONINSTANCEWITHSUBACTIONS_H

/**
@author Andrew Ian Coles, Amanda Smith
*/
#include "withrefcounter.h"
#include "withrefcounterlist.h"

#include "ActionInstance.h"

/**
@author Andrew Ian Coles, Amanda Smith
*/
class ActionInstanceWithSubActions;

typedef WithRefCounterList<ActionInstanceWithSubActions> ActionInstanceWithSubActionsList;


class ActionInstanceWithSubActions : public WithRefCounter {

private:

	const ActionInstance* first;
	ActionInstanceList second;

public:

	const ActionInstance* const & getFirst() const { return first; };
	ActionInstanceList & getSecond() { return second; };

	ActionInstanceWithSubActions(const ActionInstanceWithSubActions & toCopy) : first(toCopy.first), second(toCopy.second) {
		const_cast<ActionInstance*>(first)->incrUsage();
	};
	
	ActionInstanceWithSubActions(const ActionInstance* const & aiIn) : first(aiIn) {
		const_cast<ActionInstance*>(first)->incrUsage();
	};
	
    	ActionInstanceWithSubActions(const ActionInstance* const & aiIn, const ActionInstanceList & saIn) : first(aiIn), second(saIn) {
		const_cast<ActionInstance*>(first)->incrUsage();
	};

    	~ActionInstanceWithSubActions() {
		if (const_cast<ActionInstance*>(first)->decrUsage()) delete const_cast<ActionInstance*>(first);	
	};

	ActionInstanceWithSubActions & operator=(const ActionInstanceWithSubActions & toCopy) {
		
		if (const_cast<ActionInstance*>(first)->decrUsage()) delete const_cast<ActionInstance*>(first);	
		second.clear();
		
		first = toCopy.first;
		const_cast<ActionInstance*>(first)->incrUsage();
		
		second = toCopy.second;
		
		return *this;
	};
	
	
};

#endif
