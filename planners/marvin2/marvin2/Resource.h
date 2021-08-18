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
#ifndef RESOURCE_H
#define RESOURCE_H

/**
@author Andrew Ian Coles, Amanda Smith
*/

#include "ptree.h"

#include "Type.h"

#include <ext/hash_map>

#include "hashdefinitions.h"

class ResourceInstance;

class Resource{

	friend class ResourceInstance;
	
private:
	func_decl* originalFuncPointer;
	
	Type** argumentTypes;
	int argumentCount;
	
public:
	Resource(func_decl* & p,
		  hash_map<pddl_type*, Type* > & pddl_typeToType)
		  	: originalFuncPointer(p) {
	
		const var_symbol_list * args = originalFuncPointer->getArgs();
		
		argumentCount = args->size();
		argumentTypes = new Type*[argumentCount];
		
		var_symbol_list::const_iterator itr = args->begin();
		
		for (int i = 0; itr != args->end(); itr++, i++) {
			argumentTypes[i] = pddl_typeToType[(*itr)->type];
		}
		
	
	};
	~Resource() {
		delete [] argumentTypes;
	};
		
	const string name() const {
		return (originalFuncPointer->getFunction()->getName());
	};
	
	int getArgumentCount() const { return argumentCount; };
	Type* getArgumentType(const int & i) const { return argumentTypes[i]; };

	ResourceInstance* getInstance();
	
};

#endif
