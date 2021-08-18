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
#include "staticinformation.h"

#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>

using std::ifstream;

#include "FlexLexer.h"

#include <sys/times.h>
#include <unistd.h>

#include "Type.h"
#include "Entity.h"
#include "Parameter.h"
#include "Predicate.h"
#include "PredicateInstance.h"
#include "Action.h"
#include "ADLAction.h"
#include "MacroAction.h"
#include "ActionInstance.h"
#include "State.h"
#include "ActionSequenceValidator.h"

#include "SymmetryExtractor.h"
#include "SymmetryGroup.h"

extern int yyparse();
extern int yydebug;

namespace VAL {
parse_category* top_thing = NULL;

analysis an_analysis;
analysis* current_analysis;

yyFlexLexer* yfl;

};

char * current_filename;



class FlattenProblemGoals : public VisitController {

private:

	bool currentPolarity;
	PredicateInstanceList* predicates;
	int * goalCount;
	int * negativeGoalCount;
	list<bool> * polarities;
	
	hash_map<pddl_type*, Type* >* pddl_typeToType;
	
	hash_map<parameter_symbol*, Entity* > * parameter_symbolToEntity;
	hash_map<const pred_symbol*, Predicate* > * pred_symbolToPredicate;

public:

	FlattenProblemGoals() : currentPolarity(true) {};

	
	void parseGoal(goal* topGoal,
			PredicateInstanceList * destPred,
			list<bool> * destPolarities,
			int * destGoalCount,
			int * destNegativeGoalCount,
			hash_map<pddl_type*, Type* >* pddl_typeToTypeIn,
			hash_map<parameter_symbol*, Entity* > * conv,
			hash_map<const pred_symbol*, Predicate* > * predConv) {

		predicates = destPred;
		polarities = destPolarities;
		goalCount = destGoalCount;
		negativeGoalCount = destNegativeGoalCount;
		pddl_typeToType = pddl_typeToTypeIn;
		parameter_symbolToEntity = conv;
		pred_symbolToPredicate = predConv;
		topGoal->visit(this);

	}

	void visit_simple_goal(simple_goal * s) {

			
		
			
		PredicateInstance* p
		  = new PredicateInstance(s,
					  *pred_symbolToPredicate,
					  *parameter_symbolToEntity);
		
		predicates->push_back(p); 			
		
		bool pol = (s->getPolarity() == E_POS);
		if (!currentPolarity) pol = !pol;
		
		if (pol) {
			(*goalCount)++;
		} else {
			(*negativeGoalCount)++;
		}
		
		polarities->push_back(pol);
		

	};
	
	void visit_conj_goal(conj_goal * c) {
	
		if (!currentPolarity) {
		
			cerr << "Disjunctive goals not supported, sorry" << endl;
			exit(0);
		}
		c->getGoals()->visit(this);
	
	};
	
	void visit_neg_goal(neg_goal * n) {
	
		currentPolarity = !currentPolarity;
		
		n->getGoal()->visit(this);
		
		currentPolarity = !currentPolarity;
	};

	void visit_qfied_goal(qfied_goal * q) {
	
		
		
		if (q->getQuantifier() != E_FORALL || !currentPolarity) {
		
			cerr << "Disjunctive goals not supported, sorry" << endl;
			exit(0);
		}
			
		
		hash_map<parameter_symbol*, Entity*> * old_parameter_symbolToEntity = parameter_symbolToEntity;
		
		parameter_symbolToEntity = new hash_map<parameter_symbol*, Entity*>(*old_parameter_symbolToEntity);
				
		
		all_combinations(q, q->getVars()->begin());

		
		delete parameter_symbolToEntity;
		
		parameter_symbolToEntity = old_parameter_symbolToEntity;
		
	
	}
	
		
	void all_combinations(qfied_goal *& l, var_symbol_list::const_iterator itr) {
		if (itr == l->getVars()->end()) {
			l->getGoal()->visit(this);
			return;
		}
		
		const Type* currType = (*pddl_typeToType)[(*itr)->type];
		var_symbol_list::const_iterator nextItr = itr;
		nextItr++;
		for (int i = 0; i < currType->getInstanceCount(); i++) {
		
			(*parameter_symbolToEntity)[(*itr)] = currType->getInstance(i);
			all_combinations(l, nextItr);
		}
	
	}
	
	

};

StaticInformation::StaticInformation(char* domainFilename, char* problemFilename, char * macroFilename, double & parsetime) : verbose(false), needToCheckStatic(true), clearUpTables(true), clearUpActions(true) {

		current_analysis= &an_analysis;
	
	ifstream domainFile(domainFilename);

	if(!domainFile) {
    		cerr << "Bad domain file!\n";
    		exit(1);
    	};
    
    	yfl= new yyFlexLexer(&domainFile,&cout);
    	yydebug=0;
    	yyparse();
    	delete yfl;

    	if(!an_analysis.the_domain) {
    		cerr << "Problem parsing the domain definition!\n";
    		exit(1);
    	};
    

	ifstream problemFile(problemFilename);
    
    	if(!problemFile) {
    		cerr << "Bad problem file!\n";
    		exit(1);
    	};
    
    	yfl = new yyFlexLexer(&problemFile,&cout);
    	yyparse();
    	delete yfl;
	
	tms ptms;
	times(&ptms);
	
	parsetime = ((double)ptms.tms_utime + (double)ptms.tms_stime) / ((double) sysconf(_SC_CLK_TCK));

	
	
	createTypesTable();
	createPredicatesTable();
	createConstantsTable();
	createEntitiesTable();
	
	numberOfPossiblySymmetricEntities = numberOfEntities;
	possiblySymmetric = 0;
	
	numberOfAsymmetricEntities = 0;
	Asymmetric = 0;
	
	createDerivedPredicatesTable();
	
	createActionsTable();
	
	
	recogniseStaticPredicates();

	createSubActions();
			
	createInitialState();
	createGoalState();
	
	calculateParameterDomainConstraints();
	calculateJointParameterDomainConstraints();
	needToCheckStatic = false;
	
	reorderActionPreconditions();
	calculatePotentialSymmetries();

	loadMacroActions(macroFilename);
};


StaticInformation::StaticInformation(char* domainFilename, char* problemFilename, char * macroFilename, bool verbosity, double & parsetime) : verbose(verbosity), needToCheckStatic(true), clearUpTables(true), clearUpActions(true) 
{

	current_analysis= &an_analysis;
	
	ifstream domainFile(domainFilename);

	if(!domainFile) {
    		cerr << "Bad domain file!\n";
    		exit(1);
    	};
    
    	yfl= new yyFlexLexer(&domainFile,&cout);
    	yydebug=0;
    	yyparse();
    	delete yfl;

    	if(!an_analysis.the_domain) {
    		cerr << "Problem parsing the domain definition!\n";
    		exit(1);
    	};
    

	ifstream problemFile(problemFilename);
    
    	if(!problemFile) {
    		cerr << "Bad problem file!\n";
    		exit(1);
    	};
    
    	yfl = new yyFlexLexer(&problemFile,&cout);
    	yyparse();
    	delete yfl;
	
	tms ptms;
	times(&ptms);
	
	parsetime = ((double)ptms.tms_utime + (double)ptms.tms_stime) / ((double)sysconf(_SC_CLK_TCK));
	
	
	
	createTypesTable();
	createPredicatesTable();
	createConstantsTable();
	createEntitiesTable();
	
	numberOfPossiblySymmetricEntities = numberOfEntities;
	possiblySymmetric = 0;
	
	numberOfAsymmetricEntities = 0;
	Asymmetric = 0;
	
	createDerivedPredicatesTable();
	createActionsTable();
	
	
	recogniseStaticPredicates();
	
	createSubActions();
	
	createInitialState();
	createGoalState();
	
	calculateParameterDomainConstraints();
	calculateJointParameterDomainConstraints();
	needToCheckStatic = false;
	reorderActionPreconditions();
	calculatePotentialSymmetries();

	loadMacroActions(macroFilename);
	

}

void StaticInformation::createTypesTable() {

	isTypedDomain = (an_analysis.the_domain->req & E_TYPING);
	
	if (!isTypedDomain) {
		numberOfTypes = 1;
		Types = new Type*[1];
		pddl_type* tmp = 0;
		Types[0] = new Type(tmp);
		pddl_typeToType[0] = Types[0];
		return;
	}
	
	pddl_type_list* types = an_analysis.the_domain->types;
	pddl_type_list::const_iterator itr = types->begin();
    
	hash_map<pddl_type*, Type* >::iterator hashItr;
	
	numberOfTypes = 0;
    
	for (; itr != types->end(); itr++) {
    	
    		pddl_type* curr = (*itr);
		Type* currAsType;
	
		hashItr = pddl_typeToType.find(curr);
		
		if (hashItr == pddl_typeToType.end()) { // not previously defined
			currAsType = new Type(curr);
			pddl_typeToType[curr] = currAsType;	
			numberOfTypes++;
		} else {
			currAsType = (*hashItr).second;
		}
		
		pddl_type* parentType = curr->type;
		
		if (parentType != 0) {
		
			Type* parentAsType;
			
			hashItr = pddl_typeToType.find(parentType);
		
			if (hashItr == pddl_typeToType.end()) { // not previously defined
				parentAsType = new Type(parentType);
				pddl_typeToType[parentType] = parentAsType;	
				numberOfTypes++;
			} else {
				parentAsType = (*hashItr).second;
			}
			
			currAsType->setParent(parentAsType);
			parentAsType->incrSubTypeCounter();
			
		}	
	}
    
	Types = new Type*[numberOfTypes];
   
	hashItr = pddl_typeToType.begin();
   
	for (int i = 0; hashItr != pddl_typeToType.end(); i++, hashItr++) {	
		Types[i] = (*hashItr).second;
		Types[i]->allocSubTypeArray();
	}
	
	for (int i = 0; i < numberOfTypes; i++) {
		Types[i]->registerWithParent();
		Types[i]->setID(i);
	}
   
	if (verbose) {
		for (int i = 0; i < numberOfTypes; i++) cout << *Types[i];
	}
  
}

void StaticInformation::createPredicatesTable() {

	pred_decl_list* predicates = an_analysis.the_domain->predicates;

	numberOfPredicates = predicates->size();
  	Predicates = new Predicate*[numberOfPredicates + 1];
	
	numberOfStaticPredicates = 1;
	numberOfDynamicPredicates = numberOfPredicates;
  
  	pred_decl_list::const_iterator predItr = predicates->begin();
  	  
  	for (int i = 0; predItr != predicates->end(); predItr++, i++) {
  
  		pred_decl* curr = (*predItr);
	  	Predicates[i] = new Predicate(curr, pddl_typeToType, i);
	
		const pred_symbol* currSymb = curr->getPred();
		pred_symbolToPredicate[currSymb] = Predicates[i];
  	}
  
	Type* rootType = 0;
	
	for (int i = 0; rootType == 0 && i < numberOfTypes; i++) {
		if (Types[i]->getParent() == 0) {
			rootType = Types[i];
		}
	}
	Predicates[numberOfPredicates] = new Predicate(rootType, numberOfPredicates);
	
	pred_symbol * getEqualsSymbol = an_analysis.pred_tab.symbol_ref("=");
	
	pred_symbolToPredicate[getEqualsSymbol] = Predicates[numberOfPredicates];
	
	numberOfPredicates++;
	if (verbose) {
		for (int i = 0; i < numberOfPredicates; i++) {
	
			cout << "Predicate " << Predicates[i]->name();
		
			for (int j = 0; j < Predicates[i]->getArgumentCount(); j++) {
		
				cout << " <" << Predicates[i]->getArgumentType(j)->name() << ">";
		
			}
		
			cout << endl;
		
		}
	}

}

void StaticInformation::createConstantsTable() {

	const_symbol_list* constants = an_analysis.the_domain->constants;
  
	if (constants == 0) {
		Constants = new Entity*[0];
		ConstantDomains = new EntitySet*[0];
		numberOfConstants = 0;
		return;
	}
	
	numberOfConstants = constants->size();
	
	
	Constants = new Entity*[numberOfConstants];
	ConstantDomains = new EntitySet*[numberOfConstants];
	
	const_symbol_list::const_iterator objItr = constants->begin();
  
  	for (int i = 0; objItr != constants->end(); objItr++, i++) {	
	
		const_symbol* curr = (*objItr);
		
		Constants[i] = new Entity(curr, pddl_typeToType[curr->type], true);
		//cout << "Constant with name " << Constants[i]->name() << endl;
		
		ConstantDomains[i] = new EntitySet();
		ConstantDomains[i]->insert(Constants[i]);
		
		const_symbolToConstantIndex[curr] = i;
		parameter_symbolToEntity[curr] = Constants[i];
  
  	}

}

void StaticInformation::createActionsTable() {

	
	bool adlActions = hasDerivedPredicates;
	
	if (	   an_analysis.req & E_EQUALITY
		|| an_analysis.req & E_DISJUNCTIVE_PRECONDS
		|| an_analysis.req & E_EXT_PRECS
		|| an_analysis.req & E_UNIV_PRECS
		|| an_analysis.req & E_COND_EFFS ) {
		adlActions = true;	
		cout << "Using ADL actions" << endl;
	};
	
	
	
	operator_list* ops = an_analysis.the_domain->ops;
  
	numberOfActions = ops->size();
	numberOfSimpleActions = numberOfActions;
  
	Actions = new Action*[numberOfActions];
  
	operator_list::const_iterator opItr = ops->begin();
  
	for (int i = 0; opItr != ops->end(); opItr++, i++) {
  
		operator_* curr = (*opItr);
		if (adlActions) {
			Actions[i] = new ADLAction(curr, pddl_typeToType, pred_symbolToPredicate, const_symbolToConstantIndex, Constants, ConstantDomains , this); 
		} else {
			Actions[i] = new StripsAction(curr, pddl_typeToType, pred_symbolToPredicate, const_symbolToConstantIndex, Constants, ConstantDomains );  
		}
	
		/*if (i < 4) {
			cout << "Action " << Actions[i]->name() << " has no cost" << endl;
			Actions[i]->costs = false;
		}*/
	}
	
	if (verbose) {
	
		for (int i = 0; i < numberOfActions; i++) {
			cout << *Actions[i];
		}
	
	}
	
}


void StaticInformation::createDerivedPredicatesTable() {

	derivations_list* drvs = an_analysis.the_domain->drvs;
	
	if (drvs == 0) {
		hasDerivedPredicates = false;
		numberOfDerivedPredicateActions = 0;
		DerivedPredicateActions = new Action*[numberOfDerivedPredicateActions];
		return;
	}
	
	
	
	numberOfDerivedPredicateActions = drvs->size();
	
	DerivedPredicateActions = new Action*[numberOfDerivedPredicateActions];
	
	hasDerivedPredicates = (numberOfDerivedPredicateActions != 0);
	  
	derivations_list::const_iterator drvsItr = drvs->begin();
  
	for (int i = 0; drvsItr != drvs->end(); drvsItr++, i++) {
  
		derivation_rule* curr = (*drvsItr);
		DerivedPredicateActions[i] = new ADLAction(curr,  pddl_typeToType, pred_symbolToPredicate, const_symbolToConstantIndex, Constants, ConstantDomains , this); 
		
		DerivedPredicateActions[i]->costs = false;
		DerivedPredicateActions[i]->isDPAction = true;
	
	}
	
	if (verbose) {
		cout << "Derived-Predicate-Production Actions:" << endl;
		for (int i = 0; i < numberOfDerivedPredicateActions; i++) {
			cout << *DerivedPredicateActions[i];
		}
	
	}

}

void StaticInformation::createEntitiesTable() {

	
	const_symbol_list* objects = an_analysis.the_problem->objects;
  
	if (objects != 0) {
		numberOfEntities = objects->size();
		Entities = new Entity*[numberOfEntities];
		const_symbol_list::const_iterator objItr = objects->begin();
	
	
		for (int i = 0; objItr != objects->end(); objItr++, i++) {
		
			const_symbol* curr = (*objItr);
		
			Entities[i] = new Entity(curr, pddl_typeToType[curr->type], false);
			parameter_symbolToEntity[curr] = Entities[i];
	
		}
	} else {
		numberOfEntities = 0;
		Entities = new Entity*[0];
	}
  
	list<Entity*> entityList;
	const list<Entity*> emptyList;
	
  	for (int i = 0; i < numberOfEntities; i++) {
   		Entities[i]->getEntityType()->incrInstanceCount();
		entityList.push_back(Entities[i]);
  	}
	for (int i = 0; i < numberOfEntities; i++) {
		Entities[i]->setSymmetricWith(entityList, emptyList);
	}
	
	for (int i = 0; i < numberOfConstants; i++) {
   		Constants[i]->getEntityType()->incrInstanceCount();
  	}
  
  
  	for (int i = 0; i < numberOfTypes; i++) {
  		Types[i]->allocateInstanceArray();
  	}
  
  	for (int i = 0; i < numberOfEntities; i++) {
   		Entities[i]->registerInstanceWithType();
  	}
	
	for (int i = 0; i < numberOfConstants; i++) {
   		Constants[i]->registerInstanceWithType();
  	}
	
	for (int i = 0; i < numberOfTypes; i++) {
		
  		Types[i]->makeEntitySet();
		Type* currType = Types[i];
/*		cout << "Entities of type " << currType->name() << ":";
		for (int j = 0; j < currType->getInstanceCount(); j++) {
			cout << " " << currType->getInstance(j)->name();
		}
		cout << endl;*/
  	}
  
	
	if (verbose) {
		for (int i = 0; i < numberOfEntities; i++) {
			cout << *Entities[i] << endl;
		}
	}

}

void StaticInformation::recogniseStaticPredicates() {

	for (int i = 0; i < numberOfPredicates; i++) {
	
		bool currStatic = true;
		
		Predicate* currPred = Predicates[i];
		if (!currPred->isStatic()) {
		
			for (int j = 0; currStatic && j < numberOfActions; j++) {
			
				Action* currAction = Actions[j];
				
				currStatic = !currAction->addsOrDeletes(currPred);
					
			}
			
			for (int j = 0; currStatic && j < numberOfDerivedPredicateActions; j++) {
			
				Action* currAction = DerivedPredicateActions[j];
				
				currStatic = !currAction->addsOrDeletes(currPred);
					
			}
			
			if (currStatic) {
				numberOfStaticPredicates++;
				numberOfDynamicPredicates--;
			}
			
			currPred->setStatic(currStatic);
			
			if (verbose) {
			
				cout << "Predicate " << currPred->name() << " is";
				if (!currStatic) {
					cout << " not";
				}
				cout << " static" << endl;
			}
		}
	
	}

}

void StaticInformation::reorderActionPreconditions() {

	for (int i = 0; i < numberOfActions; i++) {

		Action* currAction = Actions[i];
			
		if (verbose) {
		
			cout << "Re-ordering preconditions of action " << currAction->name() << endl;
			
			cout << "Were in the order: ";
			
			for (int j = 0; j < currAction->preconditionCount; j++) {
		
				cout << currAction->preconditions[j].predType->name() << " ";
			}
			cout << endl;
		}
		
		currAction->reorderPreconditions(this);
		
		if (verbose) {
			cout << "Now in the order: ";
			
			for (int j = 0; j < currAction->preconditionCount; j++) {
		
				cout << currAction->preconditions[j].predType->name() << " ";
			}
			cout << endl;
		}
	
	}

}


void StaticInformation::createInitialState() {

	pc_list<simple_effect*> * currEffects = &(an_analysis.the_problem->initial_state->add_effects);
  
	numberOfStaticPredicatesInInitialState = 0;
	numberOfPredicatesInInitialState = 0;
	
	//numberOfPredicatesInInitialState = currEffects->size();
  
	StaticInitialState = new State();
	InitialState = new State();
  
	pc_list<simple_effect*>::const_iterator effItr = currEffects->begin();
  
	for (int i = 0; effItr != currEffects->end(); effItr++, i++) {
	
		simple_effect* curr = (*effItr);
	
		PredicateInstance* p
		 = new PredicateInstance(curr, pred_symbolToPredicate,
							parameter_symbolToEntity);
		
		if (p->getParentPredicate()->isStatic()) {
			StaticInitialState->addPredicateInstance(p);
			numberOfStaticPredicatesInInitialState++; 
			
		} else {
			InitialState->addPredicateInstance(p);
			numberOfPredicatesInInitialState++;
		}
	
	
  	}

  	if (verbose) {

  		cout << "Dynamic initial state:" << endl;  
  	
		State::iterator itr = InitialState->begin();
		for (; itr != InitialState->end(); itr++) {
	
			cout << "\t" << *(*itr) << endl;
			
		}

  		cout << "Static initial state:" << endl;  
  	
		itr = StaticInitialState->begin();
		for (; itr != StaticInitialState->end(); itr++) {
	
			cout << "\t" << *(*itr) << endl;
			
		}
	
		cout << "Types now:" << endl;	
  		for (int i = 0; i < numberOfTypes; i++) {
   			
			Types[i]->detailedWrite(cout);
		}
		
	}
}

void StaticInformation::createGoalState() {

//	cout << "Creating goal state" << endl;
	PredicateInstanceList goalStateAsList;
  
  	goalCount = 0;
  	negativeGoalCount = 0;
	
  	FlattenProblemGoals flattener;
  
	list<bool> polarities;
	
  	flattener.parseGoal(	an_analysis.the_problem->the_goal,
				&goalStateAsList,
				&polarities,
				&goalCount,
				&negativeGoalCount,
				&pddl_typeToType,
				&parameter_symbolToEntity,
				&pred_symbolToPredicate);

	GoalPredicates = new State();
	NegativeGoalPredicates = new State();
  
//	cout << "Goal count = " << goalCount << endl;
//	cout << "Negative goal count = " << negativeGoalCount << endl;
	
	PredicateInstanceList::iterator gstItr = goalStateAsList.begin();
	list<bool>::iterator polItr = polarities.begin();
	
	
	for (; gstItr != goalStateAsList.end(); gstItr++, polItr++) {
		
		if (*polItr) {
			//cout << "- " << *(*gstItr) << endl;
			GoalPredicates->addPredicateInstance(*gstItr);
		} else {
			NegativeGoalPredicates->addPredicateInstance(*gstItr);
			cout << "- �(" << *(*gstItr) << ")" << endl;
		}
	}
  
	if (verbose) {
		if ((goalCount + negativeGoalCount) == 1) {
	  		cout << "Goal:" << endl;
	  	} else {
	  		cout << "Goals:" << endl;
		}
		
		State::iterator itr = GoalPredicates->begin();
		
		for(; itr != GoalPredicates->end(); itr++) {
		
			cout << "\t" << *(*itr) << endl;
		}
		itr = NegativeGoalPredicates->begin();
		
		for(; itr != NegativeGoalPredicates->end(); itr++) {
		
			cout << "\t�(" << *(*itr) << ")" << endl;
		}
	}

}

void StaticInformation::calculateParameterDomainConstraints() {

	for (int i = 0; i < numberOfActions; i++) {
	
		Action* currAction = Actions[i];
		if (verbose) {
			cout << "Calculating parameter domain constraints for " << currAction->name() << endl;
		}
	
		currAction->calculateParameterDomains(this);
	
	}
	
	for (int i = 0; i < numberOfDerivedPredicateActions; i++) {
	
		Action* currAction = DerivedPredicateActions[i];
		if (verbose) {
			cout << "Calculating parameter domain constraints for " << currAction->name() << endl;
		}
	
		currAction->calculateParameterDomains(this);
	
	}
	
};

void StaticInformation::calculateJointParameterDomainConstraints() {

	for (int i = 0; i < numberOfActions; i++) {
	
		Action* currAction = Actions[i];
		if (verbose) {
			cout << "Calculating joint parameter domain constraints for " << currAction->name() << endl;
		}
	
		//bool oldv = verbose;
		//verbose = false;
		currAction->calculateStaticJointParameterDomainConstraints(this);
		//verbose = oldv;
	
	}
	
	for (int i = 0; i < numberOfDerivedPredicateActions; i++) {
	
		Action* currAction = DerivedPredicateActions[i];
		if (verbose) {
			cout << "Calculating joint parameter domain constraints for " << currAction->name() << endl;
		}
	
		//bool oldv = verbose;
		//verbose = false;
		currAction->calculateStaticJointParameterDomainConstraints(this);
		//verbose = oldv;
	
	}
};
StaticInformation::~StaticInformation()
{
	
	if (verbose) {

		cout << "~StaticInformation:" << endl;

	}
	
	if (verbose) cout << "Deleting Goal State" << endl;
	delete GoalPredicates;
	delete NegativeGoalPredicates;

	if (verbose) cout << "Deleting Initial State" << endl;
	delete InitialState;
	delete StaticInitialState;

	if (clearUpTables) {
		if (verbose) cout << "Deleting Entities..." << endl;
		for (int i = 0; i < numberOfEntities; i++) {
			if (verbose) cout << "\t" << *Entities[i] << endl;
			delete Entities[i];
		}

		
		
		for (int i = 0; i < numberOfConstants; i++) {
			if (verbose) cout << "\t" << *Constants[i] << endl;
			delete Constants[i];
			delete ConstantDomains[i];
		}
		
		
		delete [] ConstantDomains;
	}
	
	if (clearUpActions) {
		if (verbose) cout << "Deleting Actions..." << endl;

		for (int i = 0; i < numberOfActions; i++) {
			if (verbose) {
				cout << "\t";
				Actions[i]->briefWrite(cout);
			}

			//if (Actions[i]->isMacroAction) {
				delete Actions[i];
			//} else {
			//	delete ((MacroAction*) Actions[i]);
			//}
		}
		
		for (int i = 0; i < numberOfDerivedPredicateActions; i++) {
			delete DerivedPredicateActions[i];
		}
	}
	
	if (clearUpTables) {
		for (int i = 0; i < numberOfPredicates; i++) delete Predicates[i];
		

		for (int i = 0; i < numberOfTypes; i++) delete Types[i];
		
	}
	
	delete [] Actions;
	delete [] DerivedPredicateActions;
	delete [] Constants;
	delete [] Entities;
	delete [] possiblySymmetric;
	delete [] Asymmetric;
	delete [] Predicates;
	delete [] Types;
}

StaticInformation::StaticInformation(const StaticInformation & s, list<Entity*> replacementEntities, State * newStaticInitialState, State * newInitialState, State * newGoalState, State * newNegativeGoalState)
   :	verbose(s.verbose),
	numberOfTypes(s.numberOfTypes),
	Types(new Type*[numberOfTypes]),
	numberOfPredicates(s.numberOfPredicates),
	numberOfStaticPredicates(s.numberOfStaticPredicates),
	numberOfDynamicPredicates(s.numberOfDynamicPredicates),
	Predicates(new Predicate*[numberOfPredicates]),
	numberOfActions(s.numberOfActions),
	Actions(new Action*[numberOfActions]),
	numberOfDerivedPredicateActions(s.numberOfDerivedPredicateActions),
	DerivedPredicateActions(new Action*[numberOfDerivedPredicateActions]),
	numberOfEntities(replacementEntities.size()),
	Entities(new Entity*[numberOfEntities]),
	numberOfPossiblySymmetricEntities(numberOfEntities),
	possiblySymmetric(new Entity*[numberOfPossiblySymmetricEntities]),
	numberOfAsymmetricEntities(0),
	Asymmetric(0),
	numberOfConstants(s.numberOfConstants),
	Constants(new Entity*[numberOfConstants]),
	numberOfStaticPredicatesInInitialState(newStaticInitialState->size()),
	StaticInitialState(newStaticInitialState),
	numberOfPredicatesInInitialState(newInitialState->size()),
	InitialState(newInitialState),
	goalCount(newGoalState->size()),
	GoalPredicates(newGoalState),
	NegativeGoalPredicates(newNegativeGoalState),
	needToCheckStatic(s.needToCheckStatic),
	pddl_typeToType(s.pddl_typeToType),
	pred_symbolToPredicate(s.pred_symbolToPredicate),
	parameter_symbolToEntity(s.parameter_symbolToEntity),
	clearUpTables(false),
	clearUpActions(false) {

	
	for (int i = 0; i < numberOfTypes; i++) Types[i] = s.Types[i];
	for (int i = 0; i < numberOfPredicates; i++) Predicates[i] = s.Predicates[i];
	for (int i = 0; i < numberOfActions; i++) Actions[i] = s.Actions[i];
	for (int i = 0; i < numberOfDerivedPredicateActions; i++) DerivedPredicateActions[i] = s.DerivedPredicateActions[i];
	
	list<Entity*>::iterator itr = replacementEntities.begin();
	for (int i = 0; i < numberOfEntities; i++, itr++) {
		Entities[i] = *itr;
		possiblySymmetric[i] = *itr;
	}
	for (int i = 0; i < numberOfConstants; i++) Constants[i] = s.Constants[i];

}

void StaticInformation::addAction(Action* toAdd){

	/*cout << "Actions were: " << endl;
	
	for (int i = 0; i < numberOfActions; ++i) {
		const Action* actPtr = Actions[i];
		cout << actPtr->name() << endl;
	}*/
	/*int numberOfActions;
	Action** Actions;*/
	


	Action** newActions = new Action*[numberOfActions + 1];

	//for(int i = 0; i < numberOfActions; i++){
		memcpy(&(newActions[1]), Actions, numberOfActions * sizeof(Action*));
		//newActions[i+1] = Actions[i];

	//}

	newActions[0] = toAdd;

	numberOfActions++;
	numberOfSimpleActions++;

	delete [] Actions;

	Actions = newActions;
	
	/*cout << "Actions are now: " << endl;
	
	for (int i = 0; i < numberOfActions; ++i) {
		cout << Actions[i]->name() << endl;
	}*/

};

void StaticInformation::addReserveAction(Action* toAdd){

	/*int numberOfActions;
	Action** Actions;*/

	Action** newActions = new Action*[numberOfActions + 1];

	for(int i = 0; i < numberOfActions; i++){

		newActions[i] = Actions[i];

	}

	newActions[numberOfActions] = toAdd;

	numberOfActions++;
	numberOfSimpleActions++;

	delete [] Actions;

	Actions = newActions;

};

void StaticInformation::incrSimple() {

	//numberOfSimpleActions++;

}

void StaticInformation::trimEntities(const EntitySet* trimTo) {

	//cout << "StaticInformation::trimEntities" << endl;
	for (int i = 0; i < numberOfTypes; i++) {
		Types[i]->trimEntitySet(trimTo);
	}
	
	refreshEntitiesInActions();
	calculatePotentialSymmetries();

}

void StaticInformation::resetEntities() {
	//cout << "StaticInformation::resetEntities" << endl;
	for (int i = 0; i < numberOfTypes; i++) {
		Types[i]->resetEntitySet();
	}
	refreshEntitiesInActions();



}

void StaticInformation::createSubActions() {
	for (int j = 0; j < numberOfActions; j++) {
		Actions[j]->createSubActions();
	}
	for (int j = 0; j < numberOfDerivedPredicateActions; j++) {
		DerivedPredicateActions[j]->createSubActions();
	}
}


void StaticInformation::refreshEntitiesInActions() {
	
	Action** oldActions = Actions;
	Action** oldDPActions = DerivedPredicateActions;
	
	int oldActionCount = numberOfActions;
	int oldDPCount = numberOfDerivedPredicateActions;
	
	createDerivedPredicatesTable();
	createActionsTable();
		
	createSubActions();
	
	calculateParameterDomainConstraints();
	calculateJointParameterDomainConstraints();
	
	Action** donorActions = Actions;
	Action** donorDPs = DerivedPredicateActions;
	
	Actions = oldActions;
	DerivedPredicateActions = oldDPActions;
	
	int j = 0;
	
	hash_map<Action*, Action*> convertActions;
	
	for (int i = 0; i < oldActionCount; ++i) {
		if (!Actions[i]->isMacroAction) {
//			cout << "Replacing " << Actions[i]->name() << " with " << donorActions[j]->name() << endl;
//			cout << "Action that was at " << Actions[i] << " is now at " << donorActions[j] << endl;
			convertActions[Actions[i]] = donorActions[j];
			if (clearUpActions) delete Actions[i];
			Actions[i] = donorActions[j];
			
			++j;
		} else {
			//cout << "Action " << i << " is a macro action" << endl;
		}
	}

	j = 0;
	
	for (int i = 0; i < oldActionCount; ++i) {
		if (Actions[i]->isMacroAction) {
			(static_cast<MacroAction*>(Actions[i]))->updateActions(convertActions);
			//cout << "Action " << i << " is a macro action" << endl;
		}
	}
	
	for (int i = 0; i < numberOfDerivedPredicateActions; ++i) {
		if (clearUpActions) delete DerivedPredicateActions[i];
		DerivedPredicateActions[i] = donorDPs[i];
	}
		
	delete [] donorActions;
	delete [] donorDPs;

	/*calculateParameterDomainConstraints();
	calculateJointParameterDomainConstraints();*/
	clearUpActions = true;
	numberOfActions = oldActionCount;
	numberOfDerivedPredicateActions = oldDPCount;
}

void StaticInformation::recalculateJPDCs() {

	calculateParameterDomainConstraints();
	calculateJointParameterDomainConstraints();

}

void StaticInformation::recogniseTransformationalOperators() {

	
	const Predicate * staticPred;
	int param1;
	int param2;
	
	list<ActionSequenceValidator*> chainingActions;
	for(int i = 0; i < numberOfActions; ++i){
	
		bool transOperator = Actions[i]->isTransformationalOperator(staticPred, param1, param2);
		
		if(transOperator){
			cout << "**** " << Actions[i]->name() << " is a transformational operator" << endl;
			cout << "static predicate is " << staticPred->name() << endl;
			
			ActionSequenceValidator* act = new ActionSequenceValidator(staticPred, param1, param2, *this, Actions[i]);
			chainingActions.push_back(act);
			Actions[i]->setSequenceValidator(act);
		}else{
			//cout << problemInformation.Actions[i]->name() << " is not a transformational opeartor" << endl;
		}
	}
	
	list<ActionSequenceValidator*>::iterator moditr = chainingActions.begin();
	const list<ActionSequenceValidator*>::iterator modEnd = chainingActions.end();
	
	for(; moditr != modEnd; ++moditr) {
		(*moditr)->addRequiredPredicates(StaticInitialState);
	}
	
	recalculateJPDCs();
	

}

void StaticInformation::calculatePotentialSymmetries() {

	delete [] possiblySymmetric;
	delete [] Asymmetric;
	
	possiblySymmetric = 0;
	Asymmetric = 0;
	
	list<Entity*> entityList;
	const list<Entity*> emptyList;
	
  	for (int i = 0; i < numberOfEntities; i++) {
   		entityList.push_back(Entities[i]);
  	}
	for (int i = 0; i < numberOfEntities; i++) {
		Entities[i]->setSymmetricWith(entityList, emptyList);
	}
	
	State dummyState;
	SymmetryExtractor symm;
	
	list<SymmetryGroup*> * symmetryGroups = symm.extractSymmetryGroups(*this, false, &dummyState);
	
	list<SymmetryGroup*>::const_iterator itr = symmetryGroups->begin();
	const list<SymmetryGroup*>::const_iterator itrEnd = symmetryGroups->end();
	
	list<Entity*> possiblySymmetricList;
	list<Entity*> asymmetricList;
	
	for (; itr != itrEnd; ++itr) {
	
		list<Entity*> tail = (*itr)->getEntities();
		list<Entity*> head;
		
		while (!tail.empty()) {
			Entity* currEntity = tail.front();
			tail.pop_front();
			
			currEntity->setSymmetricWith(head, tail);
			head.push_back(currEntity);
			
			if (currEntity->isAsymmetric()) {
				asymmetricList.push_back(currEntity);
			} else {
				possiblySymmetricList.push_back(currEntity);
			}
		}
		
		delete *itr;
	
	}
	
	numberOfPossiblySymmetricEntities = possiblySymmetricList.size();
	possiblySymmetric = new Entity*[numberOfPossiblySymmetricEntities];
	
	list<Entity*>::iterator psItr = possiblySymmetricList.begin();
	const list<Entity*>::iterator psEnd = possiblySymmetricList.end();
		
	
	
	for (int index = 0; psItr != psEnd; ++psItr, ++index) {
		possiblySymmetric[index] = (*psItr);
	}
	
	numberOfAsymmetricEntities = asymmetricList.size();
	Asymmetric = new Entity*[numberOfAsymmetricEntities];
	
	list<Entity*>::iterator asItr = asymmetricList.begin();
	const list<Entity*>::iterator asEnd = asymmetricList.end();
		
	
	
	for (int index = 0; asItr != asEnd; ++asItr, ++index) {
		Asymmetric[index] = (*asItr);
	}
	
	
}

void StaticInformation::loadMacroActions(char* macroFilename) {

	if (!macroFilename) return;
	ifstream listfile(macroFilename);
	int limit;
	listfile >> limit;
	
	cout << "Loading " << limit << " macros" << endl;
	
	for (int i = 0; i < limit; ++i) {
	
		string name;
		listfile >> name;
		unsigned int tsu;
		unsigned int uc;
		listfile >> tsu;
		listfile >> uc;
		cout << "Loading macros from " << name << endl;
		ifstream macrofile(name.c_str());
		MacroAction* hierachicalAction = MacroAction::loadMacroAction(macrofile, *this, tsu, uc);
		hierachicalAction->setEscapeAction();
		addReserveAction(hierachicalAction);
	
	};
	
};
