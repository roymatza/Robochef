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
#ifndef ACTIONSEQUENCEVALIDATOR_H
#define ACTIONSEQUENCEVALIDATOR_H

#include "plan.h"
#include "ActionInstance.h"
#include "staticinformation.h"

/**
@author Andrew Ian Coles, Amanda Smith
*/
class ActionSequenceValidator{

private:

	hash_map<Entity*, int> entitiesConcerned;
	Entity** dereference;
	Type* typeConcerned;
	const Predicate* staticTransPred;
	Action* actionConcerned;
	int entityCount;
	int** digraph;
	int** routeGraph;
	int loc1;
	int loc2;
	bool variablesNeeded;
public:
    
    ActionSequenceValidator(const Predicate* predcicateConcerned, int & param1, int & param2, StaticInformation & problemInformation, Action* actionToUse);
    bool isTransformationalOperator(const Predicate* & staticTransPred, int & loc1, int & loc2) const;
    bool canApplyActionInstance(const ActionInstance * toCheck, int & heuristic);
    
    Plan* GetActionsToApply(Entity* param1, Entity* param2, double startingTime, ActionInstance* toApply);
    
    void addRequiredPredicates(State* toModify);
    
    ~ActionSequenceValidator();
    
    int getParameterOne() const { return loc1;}
    
    int getParameterTwo() const { return loc2;}
    
    const Predicate* getPredicateConcerned() const { return staticTransPred;}
    
    void write(ostream & o) const;
    
    void updateEntities(StaticInformation & problemInformation);
    
    void needVariables(const bool & need){
    	variablesNeeded = need;
    };

};

ostream & operator <<(ostream & o, const ActionSequenceValidator & a);

#endif
