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
#ifndef ADLACTION_H
#define ADLACTION_H

#include "Action.h"
#include "AbstractStuff.h"



/**
@author Andrew Ian Coles, Amanda Smith
*/

class EffectNode;
class SatisfactionNode;
class StaticInformation;

class MakeSubActionVisitor;

class ADLAction : public Action {

	friend class MakeSubActionVisitor;
	
private:

	bool* essentialPrecondition;
	int* preconditionsUpdate;
	
	bool* essentialNegativePrecondition;
	int* negativePreconditionsUpdate;
	
	int containerCount;
	
	bool* essentialContainer;
	bool* disjunctiveContainer;
	
	int* containersUpdate;
	list<int>* subContainers;
	list<int>* subPreconditions;
	list<int>* subNegativePreconditions;
	
	int subActionCount;
	ADLAction** subActions;	
	bool* applicableSubActions;
	
	bool isRootAction;

		
public:
	EffectNode* rootEffectNode;
	
	StaticInformation* staticInf;

		int* defaultSatisfactionCounters;
	
	ADLAction(operator_ * & o,
		hash_map<pddl_type*, Type* > & pddl_typeToType,
		hash_map<const pred_symbol*, Predicate* > & pred_symbolToPredicate,
		hash_map<const const_symbol*, int > & const_symbolToConstantIndex,
		Entity** constantsIn,
		EntitySet** constantDomainsIn,
		StaticInformation* staticInfIn);

	ADLAction(derivation_rule * & o,
		hash_map<pddl_type*, Type* > & pddl_typeToType,
		hash_map<const pred_symbol*, Predicate* > & pred_symbolToPredicate,
		hash_map<const const_symbol*, int > & const_symbolToConstantIndex,
		Entity** constantsIn,
		EntitySet** constantDomainsIn,
		StaticInformation* staticInfIn);		
		
	ADLAction(const ADLAction & toCopy,
		  SatisfactionNode * satNode,
		  EffectNode * effNode);
		
		
	~ADLAction();


	void createEffectsArrays(const Entity** & argAssignments,
				 PredicateInstance** & destAddEffectArray,
				 int & destAddEffectCount,
				 PredicateInstance** & destDeleteEffectArray,
				 int & destDeleteEffectCount) const;
	
	const int & getContainerUpdates(const int & i) const { 
		return containersUpdate[i];
	}
	/*const int & getContainerDependsOn(const int & i) const { 
		return containersDependsOn[i];
	}*/
	const int & getPreconditionUpdates(const int & i) const {
		return preconditionsUpdate[i];
	}
	
	const int & getNegativePreconditionUpdates(const int & i) const {
		return negativePreconditionsUpdate[i];
	}
	
	const list<int> & getSubContainer(const int & i) const {
		return subContainers[i];
	}

	const list<int> & getSubPreconditions(const int & i) const {
		return subPreconditions[i];
	}
	
	const list<int> & getSubNegativePreconditions(const int & i) const {
		return subNegativePreconditions[i];
	}
	
	virtual void visit(ActionVisitor * v) const{
		v->visitADLAction(this);
	}
	
	const bool & isEssentialContainer(const int & i) const {
		return essentialContainer[i];
	}
	
	const bool & isDisjunctiveContainer(const int & i) const {
		return disjunctiveContainer[i];
	}
	
	const bool & isEssentialPrecondition(const int & i) const {
		return essentialPrecondition[i];
	}
	
	const bool & isEssentialNegativePrecondition(const int & i) const {
		return essentialNegativePrecondition[i];
	}
	
	virtual const int & getContainerCount() const { return containerCount; };
	virtual const int * getDefaultSatisfactionCounters() const {return defaultSatisfactionCounters; };
	virtual void calculateParameterDomains(StaticInformation * staticInf);
	virtual void calculateStaticJointParameterDomainConstraints(StaticInformation * staticInf);
	virtual bool addsOrDeletes(const Predicate * p) const;
	virtual list<AbstractPredicate> addCases(const Predicate* p) const;
	virtual list<AbstractPredicate> deleteCases(const Predicate* p) const;
	virtual void reorderPreconditions(StaticInformation * staticInf);
	virtual void createSubActions();
	virtual bool hasSubActions() const;
	virtual int getSubActionCount() const;
	virtual Action* getSubAction(const int & i) const;
	
	virtual bool rootAction() const { return isRootAction; };
	
	
};

#endif
