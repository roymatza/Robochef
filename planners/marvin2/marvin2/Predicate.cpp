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
#include "Predicate.h"
#include "PredicateInstance.h"

Predicate::Predicate(pred_decl* & p,
		  hash_map<pddl_type*, Type* > & pddl_typeToType,
		  const int & predID)
		  	: originalPredPointer(p), argumentCount(originalPredPointer->getArgs()->size()),
			argumentTypes(new Type*[argumentCount]),
			predicateID(predID),
			staticPredicate(false), eq(false), derived(false) {
	
		
		var_symbol_list::const_iterator itr = originalPredPointer->getArgs()->begin();
		const var_symbol_list::const_iterator itrEnd = originalPredPointer->getArgs()->end();
		
		for (int i = 0; itr != itrEnd; ++itr, ++i) {
			argumentTypes[i] = pddl_typeToType[(*itr)->type];
		}
		
	
};

Predicate::Predicate(Type* rootType, const int & predID)
	: originalPredPointer(0), argumentCount(2), argumentTypes(new Type*[2]), predicateID(predID), staticPredicate(true), eq(true), derived(false) {
	
		argumentTypes[0] = rootType;
		argumentTypes[1] = rootType;
	

}
	
PredicateInstance* Predicate::getInstance() {

	return new PredicateInstance(this);

}
