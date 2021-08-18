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
#ifndef PREDICATEINSTANCE_H
#define PREDICATEINSTANCE_H

#include <cstring>
#include <list>

using std::list;

#include "Predicate.h"
#include "Parameter.h"

#include "withrefcounter.h"
#include "withrefcounterlist.h"

/**
@author Andrew Ian Coles, Amanda Smith
*/

class ActionInstance;

class PredicateInstance : public WithRefCounter {


private:

	const Predicate* const parentPredicate;

	const int parameterCount;
	
	Parameter** const parameters;
	
	unsigned int masterID;
	bool haveID;
	

public:

    	PredicateInstance(const Predicate* p)
     	   : 	WithRefCounter(),
	   	parentPredicate(p),
		parameterCount(parentPredicate->argumentCount),
		parameters(new Parameter*[parameterCount]),
		//masterID(0),
		haveID(false) {
		
		memset(parameters, 0, sizeof(Parameter*) * parameterCount);
		

	};
	
	PredicateInstance(const Predicate* p, Parameter** params)
     	   : 	WithRefCounter(),
	        parentPredicate(p),
		parameterCount(parentPredicate->argumentCount),
		parameters(params),
		//masterID(0),
		haveID(false) {
		
		for (int i = 0; i < parameterCount; ++i) parameters[i]->incrUsage();

	};
	
	PredicateInstance(const Predicate* p, Entity** params)
     	   : 	WithRefCounter(),
	   	parentPredicate(p),
		parameterCount(parentPredicate->argumentCount),
		parameters(new Parameter*[parameterCount]),
		//masterID(0),
		haveID(false) {

		
		for (int i = 0; i < parameterCount; ++i) {
			parameters[i] = params[i]->getEntityAsParameter();
			parameters[i]->incrUsage();
		}

	};
	PredicateInstance(PredicateInstance* p);
	
	PredicateInstance(simple_effect* s,
			  hash_map<const pred_symbol*, Predicate* > & pred_symbolToPredicate,
			  hash_map<parameter_symbol*, Entity* > parameter_symbolToEntity)
		: WithRefCounter(),
		  parentPredicate(pred_symbolToPredicate[s->prop->head]),
		  parameterCount(parentPredicate->argumentCount),
		  parameters(new Parameter*[parameterCount]),
		  //masterID(0),
		  haveID(false) {
	
		parameter_symbol_list::const_iterator itr = s->prop->args->begin();
		  
		for (int i = 0; itr != s->prop->args->end(); ++itr, i++) {
			Entity* curr = parameter_symbolToEntity[(*itr)];
			parameters[i] = curr->getEntityAsParameter();
			parameters[i]->incrUsage();
		}
				  
	}
	
	PredicateInstance(simple_goal* s,
			  hash_map<const pred_symbol*, Predicate* > & pred_symbolToPredicate,
			  hash_map<parameter_symbol*, Entity* > & parameter_symbolToEntity)
		: WithRefCounter(),
		  parentPredicate(pred_symbolToPredicate[s->getProp()->head]),
		  parameterCount(parentPredicate->argumentCount),
		  parameters(new Parameter*[parameterCount]),
		  //masterID(0),
		  haveID(false) {
	
		const proposition* prop = s->getProp();
		parameter_symbol_list::const_iterator itr = prop->args->begin();
		
		
		for (int i = 0; itr != prop->args->end(); ++itr, ++i) {
			
			Entity* curr = parameter_symbolToEntity[(*itr)];
			parameters[i] = curr->getEntityAsParameter();
			parameters[i]->incrUsage();
			
		}
		
		
				  
	}


    	~PredicateInstance() {
		for (int i = 0; i < parameterCount; ++i) {
			if (parameters[i]) {
				if (parameters[i]->decrUsage()) delete parameters[i];
			}
		}
		delete [] parameters;
	};
	
	void setParameter(int index, Parameter* p) {
		if (parameters[index]) {
			if (parameters[index]->decrUsage()) delete parameters[index];
		}
		parameters[index] = p;
		p->incrUsage();
	}
	
	Parameter* const getParameter(int index) {
		return parameters[index];
	}
	
	const Parameter* const getParameter(int index) const {
		return parameters[index];
	}

	void write(ostream & o) const {
		o << parentPredicate->name();
		for (int i = 0; i < parameterCount; ++i) {
			o << " " << *(parameters[i]);
		}
	};
	
	bool isOfType(const Predicate * p) const {
		return (parentPredicate == p);	
	};
	
	const Predicate* const & getParentPredicate() const {
		return parentPredicate;
	}
	
	const int & getParameterCount() const { return parameterCount; };
	
	bool operator==(const PredicateInstance & p) const ;
	
	bool operator<(const PredicateInstance & p) const;
	
	bool equalsWithout(const PredicateInstance & p, Entity* exclude, Entity* matchExclude) const;

	int equalsWithoutPlusOne(const PredicateInstance & p, Entity* exclude, Entity* matchExclude) const;
	
	inline const bool & assignedID() const { return haveID; };
	inline const unsigned int & getID() const { return masterID; };
	inline void setID(const unsigned int & i) { masterID = i; haveID = true;};
	
	unsigned int getHashValue() const {
	
		unsigned int toReturn = 0;
		
		toReturn += reinterpret_cast<unsigned int>(parentPredicate);
		for (int i = 0; i < parameterCount; i++){
			toReturn += reinterpret_cast<unsigned int>(parameters[i]->boundTo());
		}
		
		return toReturn;
		
	}
	
};

ostream & operator <<(ostream & o, const PredicateInstance & a);

typedef WithRefCounterList<PredicateInstance> PredicateInstanceList;
typedef WithRefCounterSet<PredicateInstance> PredicateInstanceSet;


#endif
