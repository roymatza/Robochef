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
#ifndef WITHREFCOUNTER_H
#define WITHREFCOUNTER_H

#include <iostream>

using std::cout;
using std::endl;

/**
@author Andrew Ian Coles, Amanda Smith
*/




class WithRefCounter {

protected:

	int usageCount;

public:

	WithRefCounter() : usageCount(0) {};

    	virtual ~WithRefCounter() {
		#ifdef REFCOUNTDEBUG
		if (usageCount) {
		
			cout << "Ref-counted item at " << this << " being deleted has non-zero usage!" << endl;
			exit(0);
		}
		#endif
	};
#ifdef REFCOUNTDEBUG
	virtual void incrUsage() { ++usageCount; };
#else
	inline void incrUsage() { ++usageCount; };
#endif
	
#ifdef REFCOUNTDEBUG
	virtual bool decrUsage() {
		--usageCount;
		if (usageCount < 0) {
		
			cout << "Ref-counted item at " << this << " has negative usage!" << endl;
			exit(0);
		}
		return (usageCount == 0);
	};

#else
	inline bool decrUsage() { --usageCount; return (usageCount == 0); };

#endif


};


#endif
