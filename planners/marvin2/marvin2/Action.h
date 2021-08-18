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
#ifndef ACTION_H
#define ACTION_H


#include <ext/slist>

using __gnu_cxx::slist;

#include "ptree.h"

#include "VisitController.h"

#include "Parameter.h"
#include "Type.h"

#include "Predicate.h"
#include "PredicateInstance.h"

#include "AbstractStuff.h"

class ActionInstance;
class StaticInformation;

#include "hashdefinitions.h"


/**
@author Andrew Ian Coles, Amanda Smith
*/
class Entity;
class MacroAction;
class StripsAction;
class ADLAction;
class Action;
class ActionSequenceValidator;

class ActionVisitor {

public:

	virtual void visitAction(const Action * c) {};
	virtual void visitStripsAction(const StripsAction * c) {};
	virtual void visitADLAction(const ADLAction * c) {};

};

enum ActionType { ACT_STRIPS, ACT_ADL };

class Action{

	friend class ActionInstance;
	friend class MacroAction;
	friend class StripsAction;

protected:

	static const int intIsOne;
	static const int intIsMinusOne;
	static const int intIsZero;
	static const bool boolIsFalse;
	
	bool borrowedParameterDomains;
	
	list<int> emptyList;
	
	list<int> preconditionIndices;
	list<int> negativePreconditionIndices;
	
	operator_ * originalOperatorPointer;

	EntitySet** parameterDomains;
	

	
	bool* ownParameterDomains;
	
	hash_map<Entity*, EntitySet** >** jointParameterDomainConstraints;
	//hash_map<Entity*, int*>** parameterDomainConstraintSizes;
	
	
	string definedName;
	bool ownName;

	const Action** subActionTypes;
	int subActionCount;
	int** subActionArguments;

	Action(operator_ * & o,
		hash_map<pddl_type*, Type* > & pddl_typeToType,
		hash_map<const pred_symbol*, Predicate* > & pred_symbolToPredicate,
		hash_map<const const_symbol*, int > & const_symbolToConstantIndex,
		Entity** constantsIn,
		EntitySet** constantDomainsIn,
		bool friendOnly,
		const int & pc)
		: borrowedParameterDomains(false),
		  originalOperatorPointer(o),
		  ownName(false),
		  subActionTypes(0),
		  constants(constantsIn),
		  constantDomains(constantDomainsIn),
		  isMacroAction(false),
		  parameterCount(pc),
		  parameterTypes(new Type*[pc]),
		  parameterDependentCounts(0),
		  parameterDependents(0),
		  parameterNegativeDependentCounts(0),
		  parameterNegativeDependents(0),
		  preconditionCount(0),
		  preconditions(0),
		  constOnly(0),
		  negativePreconditionCount(0),
		  negativePreconditions(0),
		  negativeConstOnly(0),
		  addEffectCount(0),
		  addEffects(0),
		  deleteEffectCount(0),
		  deleteEffects(0),
		  dummySatCount(new int[1]),
		  transformationalOperator(false),
		  sequenceValidator(0),
		  costs(true),
		  isDPAction(false)
		   {dummySatCount[0] = 0;};
	
	Action(derivation_rule * & o,
		hash_map<pddl_type*, Type* > & pddl_typeToType,
		hash_map<const pred_symbol*, Predicate* > & pred_symbolToPredicate,
		hash_map<const const_symbol*, int > & const_symbolToConstantIndex,
		Entity** constantsIn,
		EntitySet** constantDomainsIn,
		bool friendOnly,
		const int & pc)
		: borrowedParameterDomains(false),
		  originalOperatorPointer(0),
		  ownName(true),
		  subActionTypes(0),
		  constants(constantsIn),
		  constantDomains(constantDomainsIn),
		  isMacroAction(false),
		  parameterCount(pc),
		  parameterTypes(new Type*[pc]),
		  parameterDependentCounts(0),
		  parameterDependents(0),
		  parameterNegativeDependentCounts(0),
		  parameterNegativeDependents(0),
		  preconditionCount(0),
		  preconditions(0),
		  constOnly(0),
		  negativePreconditionCount(0),
		  negativePreconditions(0),
		  negativeConstOnly(0),
		  addEffectCount(0),
		  addEffects(0),
		  deleteEffectCount(0),
		  deleteEffects(0),
		  dummySatCount(new int[1]),
		  transformationalOperator(false),
		  sequenceValidator(0),
		  costs(true),
		  isDPAction(false)
		   {dummySatCount[0] = 0;
		   	definedName = "derive_" + pred_symbolToPredicate[o->get_head()->head]->name();
		   };

		   
	Action(operator_ * & o,
		hash_map<pddl_type*, Type* > & pddl_typeToType,
		hash_map<const pred_symbol*, Predicate* > & pred_symbolToPredicate,
		hash_map<const const_symbol*, int > & const_symbolToConstantIndex,
		Entity** constantsIn,
		EntitySet** constantDomainsIn);
		
	void deleteParameterDependencies();
	
	
public:

	Entity** constants;
	EntitySet** constantDomains;
	
	
	bool isMacroAction;
    	const int parameterCount;
	Type** const parameterTypes;

	int* parameterDependentCounts;
	int** parameterDependents;
	
	int* parameterNegativeDependentCounts;
	int** parameterNegativeDependents;
	
	int preconditionCount;
	AbstractPredicate* preconditions;
	bool * constOnly;
	
	int negativePreconditionCount;
	AbstractPredicate* negativePreconditions;
	bool * negativeConstOnly;
	
	int addEffectCount;
	AbstractPredicate* addEffects;
	

	int deleteEffectCount;
	AbstractPredicate* deleteEffects;
	

	
	int* dummySatCount;
	
	bool transformationalOperator;
	ActionSequenceValidator* sequenceValidator;
	
	bool costs;
	bool isDPAction;	

	/*Action(	int actionParameterCount, Type** actionParameterTypes,
		int actionPreconditionCount, Predicate ** actionPreconditions, int** actionPreconditionArguments,
		int actionAddEffectCount, Predicate** actionAddEffects, int** actionAddEffectArguments,
		int actionDeleteEffectCount, Predicate** actionDeleteEffects, int** actionDeleteEffectArguments,
		const Action** subActionTypesIn, int subActionCountIn, int** subActionArgumentsIn)
			:	parameterCount(actionParameterCount),
				parameterTypes(actionParameterTypes),

				preconditionCount(actionPreconditionCount),
				preconditions(actionPreconditions),
				preconditionArguments(actionPreconditionArguments),

				addEffectCount(actionAddEffectCount),
				addEffects(actionAddEffects),
				addEffectArguments(actionAddEffectArguments),

				deleteEffectCount(actionDeleteEffectCount),
				deleteEffects(actionDeleteEffects),
				deleteEffectArguments(actionDeleteEffectArguments),

				subActionTypes(subActionTypesIn),
				subActionCount(subActionCountIn),
				subActionArguments(subActionArgumentsIn)  {

		parameterDomains = new EntitySet*[parameterCount];
		
		ownParameterDomains = new bool[parameterCount];

		jointParameterDomainConstraints = new hash_map<Entity*, EntitySet**>*[parameterCount];
		
		for (int i = 0; i < parameterCount; i++) {

			hash_map<Entity*, EntitySet**> * eHash = new hash_map<Entity*, EntitySet**>();
			

			jointParameterDomainConstraints[i] = eHash;
			

			parameterDomains[i] = 0;
			
			ownParameterDomains[i] = false;
		}

		definedName = "Macro-action";
		ownName = true;
	};*/


	Action(const int & pc): borrowedParameterDomains(false),
		  originalOperatorPointer(0),
		  ownName(false),
		  subActionTypes(0),
		  isMacroAction(false),
		  parameterCount(pc),
		  parameterTypes(new Type*[pc]),
		  parameterDependentCounts(0),
		  parameterDependents(0),
		  parameterNegativeDependentCounts(0),
		  parameterNegativeDependents(0),
		  preconditionCount(0),
		  preconditions(0),
		  constOnly(0),
		  negativePreconditionCount(0),
		  negativePreconditions(0),
		  negativeConstOnly(0),
		  addEffectCount(0),
		  addEffects(0),
		  deleteEffectCount(0),
		  deleteEffects(0),
		  dummySatCount(new int[1]),
		  transformationalOperator(false),
		  sequenceValidator(0),
		  costs(true),
		  isDPAction(false)
		  {
		  
		  	dummySatCount[0] = preconditionCount;
		  };

	Action(Entity** constantsIn,
		EntitySet** constantDomainsIn,
		const int & pcIn,
		Type** const & tpIn)
		: borrowedParameterDomains(false),
		  originalOperatorPointer(0),
		  ownName(false),
		  subActionTypes(0),
		  constants(constantsIn),
		  constantDomains(constantDomainsIn),
		  isMacroAction(false),
		  parameterCount(pcIn),
		  parameterTypes(tpIn),
		  parameterDependentCounts(0),
		  parameterDependents(0),
		  parameterNegativeDependentCounts(0),
		  parameterNegativeDependents(0),
		  preconditionCount(0),
		  preconditions(0),
		  constOnly(0),
		  negativePreconditionCount(0),
		  negativePreconditions(0),
		  negativeConstOnly(0),
		  addEffectCount(0),
		  addEffects(0),
		  deleteEffectCount(0),
		  deleteEffects(0),
		  dummySatCount(new int[1]),
		  transformationalOperator(false),
		  sequenceValidator(0),
		  costs(true),
		  isDPAction(false)
		   {dummySatCount[0] = 0;};

    	virtual ~Action();

	ActionInstance* getInstance();

	const virtual string name() const {
		//cout << "Getting the name of an action at " << this << endl;
		if(ownName) {
			//cout << "Action owns own name" << endl;
			return (definedName);
		} else {
			//cout << "Action doesn't own own name" << endl;
			return (originalOperatorPointer->name->getName());
		}
	};
		
	virtual void write(ostream & o) const;
	virtual void briefWrite(ostream & o) const;
	
	virtual void reorderPreconditions(StaticInformation * staticInf);
	
	virtual const EntitySet* getParameterDomain(const int & i) const;
	
	virtual void calculateParameterDomains(StaticInformation * staticInf);
	virtual void calculateStaticJointParameterDomainConstraints(StaticInformation * staticInf);
	virtual void dummyStaticJointParameterDomainConstraints(StaticInformation * staticInf);
	
	static void intersection(	Entity** & firstArray, int & firstSize,
				Entity** & secondArray, int & secondSize,
				Entity** & destArray, int & destSize);
	
	static void intersection(	Entity** & firstArray, const int & firstSize,
					slist<Entity*> * secondArray);
					
	static void intersection(	slist<Entity*> & firstArray, 
					slist<Entity*> * secondArray);
	
	static EntitySet * intersection(	const EntitySet * firstArray,
						const EntitySet * secondArray); 
							
	virtual void getDomainConstraints(const int & parameter,
				  Entity* assignment,
				  EntitySet** & domains) const;
				  
	virtual inline const bool & isConstOnly(const int & i) const {
		return constOnly[i];
	};
	virtual void visit(ActionVisitor * v) const{
		v->visitAction(this);
	}
	
	virtual list<AbstractPredicate> addCases(const Predicate * p) const {
	
		list<AbstractPredicate> toReturn;
		
		for (int i = 0; i < addEffectCount; ++i) {
			if (addEffects[i].predType == p) toReturn.push_back(addEffects[i]);
		}
		
		return toReturn;
	}
	
	virtual list<AbstractPredicate> deleteCases(const Predicate * p) const {
	
		list<AbstractPredicate> toReturn;
		
		for (int i = 0; i < deleteEffectCount; ++i) {
			if (deleteEffects[i].predType == p) toReturn.push_back(deleteEffects[i]);
		}
		
		return toReturn;
	}
	
	
	
	
	virtual bool addsOrDeletes(const Predicate * p) const {
		
		for (int i = 0; i < addEffectCount; i++) {
			if (addEffects[i].predType == p) return true;
		}
		for (int i = 0; i < deleteEffectCount; i++) {
			if (deleteEffects[i].predType == p) return true;
		}
		return false;	
	}
	
	virtual void createEffectsArrays(const Entity** & argAssignments,
				 PredicateInstance** & destAddEffectArray,
				 int & destAddEffectCount,
				 PredicateInstance** & destDeleteEffectArray,
				 int & destDeleteEffectCount) const {
	
		destAddEffectCount = addEffectCount;
		destDeleteEffectCount = deleteEffectCount;
		
		destAddEffectArray = new PredicateInstance*[addEffectCount];
		destDeleteEffectArray = new PredicateInstance*[deleteEffectCount];
		
		for (int i = 0; i < addEffectCount; i++) {
	
			Parameter** params = new Parameter*[addEffects[i].predType->getArgumentCount()];
		
			for (int j = 0; j < addEffects[i].predType->getArgumentCount(); j++) {
				int tP = addEffects[i].parameters[j];
				if (tP >= 0) {
					params[j] = argAssignments[tP]->getEntityAsParameter();
				} else {
					params[j] = constants[-1 - tP]->getEntityAsParameter();
				}
			}
		
			destAddEffectArray[i] = new PredicateInstance(addEffects[i].predType, params);
			destAddEffectArray[i]->incrUsage();
		
		}
		for (int i = 0; i < deleteEffectCount; i++) {
	
			Parameter** params = new Parameter*[deleteEffects[i].predType->getArgumentCount()];
		
			for (int j = 0; j < deleteEffects[i].predType->getArgumentCount(); j++) {
				int tP = deleteEffects[i].parameters[j];
				if (tP >= 0) {
					params[j] = argAssignments[tP]->getEntityAsParameter();
				} else {
					params[j] = constants[-1 - tP]->getEntityAsParameter();
				}
			}
		
			destDeleteEffectArray[i] = new PredicateInstance(deleteEffects[i].predType, params);
			destDeleteEffectArray[i]->incrUsage();
		
		}
		
				 			 
	};
	
	virtual const bool & isDisjunctiveContainer(const int & i) const { return Action::boolIsFalse; };
	virtual const int & getContainerCount() const { return Action::intIsOne; };
	virtual const int * getDefaultSatisfactionCounters() const {return dummySatCount; };
	virtual const int & getContainerUpdates(const int & i) const {return Action::intIsMinusOne; };
	virtual const int & getPreconditionUpdates(const int & i) const {return Action::intIsZero; };
	virtual const int & getNegativePreconditionUpdates(const int & i) const {return Action::intIsZero; };
	
	virtual const list<int> & getSubContainer(const int & i) const { return emptyList; };
	
	virtual const list<int> & getSubPreconditions(const int & i) const { return preconditionIndices; };
	
	virtual const list<int> & getSubNegativePreconditions(const int & i) const { return negativePreconditionIndices; };
	
	virtual void createSubActions() {};
	virtual bool hasSubActions() const {
		return false;
	};
	virtual int getSubActionCount() const {
		return 0;
	};
	virtual Action* getSubAction(const int & i) const {
		return 0;
	};
	virtual bool rootAction() const { return true; };
	
	virtual void setSequenceValidator(ActionSequenceValidator * a);
	
	virtual bool isTransformationalOperator(const Predicate* & staticTransPred, int & loc1, int & loc2);
	
	virtual void transplantAction(Action & donor) {
		//nothing in simple case - the action doesn't need refreshing
	};
	
	virtual void calculateParameterDependencies();
	 
};

class StripsAction : public Action {

public:
	StripsAction(operator_ * & o,
		hash_map<pddl_type*, Type* > & pddl_typeToType,
		hash_map<const pred_symbol*, Predicate* > & pred_symbolToPredicate,
		hash_map<const const_symbol*, int > & const_symbolToConstantIndex,
		Entity** constantsIn,
		EntitySet** constantDomainsIn)
		
		: Action(o, pddl_typeToType, pred_symbolToPredicate, const_symbolToConstantIndex, constantsIn, constantDomainsIn) {
		
	}
		
	virtual ~StripsAction() {
	
	};
	
	virtual void visit(ActionVisitor * v) const{
		v->visitStripsAction(this);
	}
	
	virtual void createSubActions() {};
	
	
	
	
};


ostream & operator <<(ostream & o, const Action & a);

#endif
