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
#ifndef PREDICATE_H
#define PREDICATE_H

#include "ptree.h"

#include "Type.h"

#include <ext/hash_map>

#include "hashdefinitions.h"

/**
@author Andrew Ian Coles, Amanda Smith
*/
class PredicateInstance;

class Predicate{

	friend class PredicateInstance;

private:

	pred_decl* originalPredPointer;

	const int argumentCount;
	Type** argumentTypes;
	
	int predicateID;
	bool staticPredicate;
	bool eq;
	bool derived;

public:
	Predicate(Type* rootType, const int & predID);
	
	Predicate(pred_decl* & p,
		  hash_map<pddl_type*, Type* > & pddl_typeToType,
		  const int & predID);
		  
	~Predicate() {
		delete [] argumentTypes;
	};

	const string name() const {
		static const string eqName("=");
		if (!originalPredPointer) {
			return eqName;
		} else {
			return (originalPredPointer->getPred()->getName());
		}
	};
	
	//int getArgumentCount() const { return argumentCount; };
	inline const int & getArgumentCount() const { return argumentCount; };
	int getID() const { return predicateID; };
	Type* getArgumentType(const int & i) const { return argumentTypes[i]; };

	PredicateInstance* getInstance();
	
	void setStatic(const bool & b) { staticPredicate = b; };
	bool isStatic() const { return staticPredicate; };
	
	inline const bool & isEqualityPredicate() const { return eq; };
	
	const bool & isDerived() const { return derived; };
	void setDerived() { derived = true; };
};

#endif
