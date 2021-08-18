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
#ifndef STATICINFORMATION_H
#define STATICINFORMATION_H

#include "ptree.h"
#include "hashdefinitions.h"

class Type;
class Entity;
class Parameter;
class Predicate;
class PredicateInstance;
class Action;
class ActionInstance;
class State;


/**
@author Andrew Ian Coles, Amanda Smith
*/
class StaticInformation{

private:

	void createTypesTable();
	void createPredicatesTable();
	void createActionsTable();
	void createDerivedPredicatesTable();
	void createConstantsTable();
	void createEntitiesTable();
	
	void recogniseStaticPredicates();
	void reorderActionPreconditions();
	void createSubActions();
	void createInitialState();
	void createGoalState();
	
	void calculateParameterDomainConstraints();
	void calculateJointParameterDomainConstraints();
	
	void loadMacroActions(char* macroFilename);

	bool isTypedDomain;	
	
	
public:

	const bool verbose;
	
	int numberOfTypes;
	Type** Types;
	
	int numberOfPredicates;
	int numberOfStaticPredicates;
	int numberOfDynamicPredicates;
	Predicate** Predicates;
	
	int numberOfSimpleActions;
	int numberOfActions;
	Action** Actions;
	
	int numberOfDerivedPredicateActions;
	Action** DerivedPredicateActions;
	
	int numberOfEntities;
	Entity** Entities;
	
	int numberOfPossiblySymmetricEntities;
	Entity** possiblySymmetric;
	
	int numberOfAsymmetricEntities;
	Entity** Asymmetric;
	
	int numberOfConstants;
	Entity** Constants;
	
	EntitySet** ConstantDomains;
	
	int numberOfStaticPredicatesInInitialState;
	State* StaticInitialState;
	
	int numberOfPredicatesInInitialState;
	State* InitialState;
	
	int goalCount;
	State* GoalPredicates;
	
	int negativeGoalCount;
	State* NegativeGoalPredicates;
  
	bool needToCheckStatic;
	
	bool hasDerivedPredicates;
	
	hash_map<pddl_type*, Type* > pddl_typeToType;
	hash_map<const pred_symbol*, Predicate* > pred_symbolToPredicate;
	hash_map<const const_symbol*, int > const_symbolToConstantIndex;
	
	hash_map<parameter_symbol*, Entity* > parameter_symbolToEntity;
	
	bool clearUpTables;
	bool clearUpActions;

	StaticInformation(char* domainFilename, char* problemFilename, char * macroFilename, double & parsetime);
	StaticInformation(char* domainFilename, char* problemFilename, char * macroFilename, bool verbosity, double & parsetime);

	StaticInformation(const StaticInformation & s, list<Entity*> replacementEntities, State * newStaticInitialState, State * newInitialState, State * newGoalState, State * newNegativeGoalState);
	
	~StaticInformation();
	
	void trimEntities(const EntitySet* trimTo);
	void resetEntities();
	
	void addAction(Action* toAdd);
	void addReserveAction(Action* toAdd);
	void incrSimple();
	void recalculateJPDCs();
	void recogniseTransformationalOperators();
	void refreshEntitiesInActions();
	void calculatePotentialSymmetries();
};

#endif
