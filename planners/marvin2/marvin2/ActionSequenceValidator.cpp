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
#include "ActionSequenceValidator.h"
#include "State.h"
    
#include <climits>
    
ActionSequenceValidator::ActionSequenceValidator(const Predicate* predcicateConcerned, int & param1, int & param2, StaticInformation & problemInformation, Action* actionToUse): staticTransPred(predcicateConcerned), actionConcerned(actionToUse), loc1(param1), loc2(param2), variablesNeeded(false){

	//list<Entity*> EntitiesConcerned;
	
	entitiesConcerned = hash_map<Entity*, int>(problemInformation.numberOfEntities);
	
	
	typeConcerned = staticTransPred->getArgumentType(0);
	entityCount = 0;
	list<Entity*> mapping;
	
	for(int i = 0; i < problemInformation.numberOfEntities; i++){
	
		if(problemInformation.Entities[i]->getEntityType() == typeConcerned){
			entitiesConcerned[problemInformation.Entities[i]] = entityCount;
			mapping.push_back(problemInformation.Entities[i]);		
			entityCount++;
		}
	}
	
	if(problemInformation.verbose) cout << "Created Entity Table" << endl;
	dereference = new Entity*[entityCount];
	
	list<Entity*>::iterator itr = mapping.begin();
	
	for(int i = 0; itr != mapping.end(); itr++, i++){
	
		dereference[i] = (*itr);
	}
	
	if(problemInformation.verbose) cout << "created dereferencing array" << endl;
	digraph = new int*[entityCount];
	routeGraph = new int*[entityCount];
	
	for(int i = 0; i < entityCount; i++){
	
		digraph[i] = new int[entityCount];
		routeGraph[i] = new int[entityCount];	
		for(int j = 0; j < entityCount; j++){

			if(i != j) {
				routeGraph[i][j] = -1;
				digraph[i][j] = INT_MAX;
			} else {
				routeGraph[i][j] = j;
				digraph[i][j] = 0;
			}
		}
	
	}
	
	if(problemInformation.verbose){
		for(int i = 0; i < entityCount; i++){
			for(int j = 0; j < entityCount; j++){
				if(digraph[i][j] != INT_MAX) {
					cout << digraph[i][j];
					
				} else {
					cout << "inf";
				}
			}
			
			cout << endl;
		}
		
		for(int i = 0; i < entityCount; i++){
			for(int j = 0; j < entityCount; j++){
				cout << routeGraph[i][j];
				
			}
			
			cout << endl;
		}
		cout << "initialised digraph and route map" << endl;
	}
	
	
	//cout << "Static Initial State = ";
	//cout << *(problemInformation.StaticInitialState) << endl;
	
	State::const_iterator sitr = problemInformation.StaticInitialState->begin();
	
	for(; sitr != problemInformation.StaticInitialState->end(); sitr++){
		//if we've found this predicate we need to add an edge between the two things
		if((*sitr)->getParentPredicate() == staticTransPred){
			
			int index1 = entitiesConcerned[(*sitr)->getParameter(0)->boundTo()];
			int index2 = entitiesConcerned[(*sitr)->getParameter(1)->boundTo()];
			
			digraph[index1][index2] = 1;
			routeGraph[index1][index2] = index2;
			
		}
	}
	
	/*cout << "digraph Floyd {" << endl;
	cout << "\tnode [shape=circle, fontsize=10, color=black, fillcolor=white, fontcolor=black];" << endl;
	cout << "\tedge [style=solid, color=black];" << endl;
	
	for (int i = 0; i < entityCount; i++) {
		cout << "\t node" << i << " [label=\"" << dereference[i]->name() << "\"];" << endl;
	}
	
	for (int i = 0; i < entityCount; i++) {
		for (int j = 0; j < entityCount; j++) {
			if ((i != j) && (digraph[i][j] == 1)) {
				cout << "\t node" << i << " -> node" << j << ";" << endl;
			}
		}
	}
	cout << "}" << endl;
	*/
	if(problemInformation.verbose){
		for(int i = 0; i < entityCount; i++){
			for(int j = 0; j < entityCount; j++){
				if(digraph[i][j] != INT_MAX) {
					cout << digraph[i][j] << " ";
				} else {
					cout << "inf ";
				}
			}
			
			cout << endl;
		}
		
		cout << "put all data into route map and digraph" << endl;
	}
	//now do floyd-warshall to find out reachable states and their associated costs
	
	for(int k = 0; k < entityCount; k++){
	
		for(int i = 0; i < entityCount; i++){
			for(int j = 0; j < entityCount; j++){
				if(digraph[i][k] != INT_MAX && digraph[k][j] != INT_MAX){
					int routeThroughK = digraph[i][k] + digraph[k][j];
					if(problemInformation.verbose) cout << "cost of going through " << dereference[k]->name() << " = " << routeThroughK << endl;
					if(routeThroughK < digraph[i][j]){
						digraph[i][j] = routeThroughK;
						routeGraph[i][j] = k; //update the map to show which route to take from i to j
						if(problemInformation.verbose) cout << "found a better way to get from " << *dereference [i] << " to " << *dereference[j] << ": going via " << *dereference[k] << endl;
					}else {
						if(problemInformation.verbose) cout << "not better to go from " << dereference [i]->name() << " to " << dereference[j]->name() << "via " << dereference[k]->name() << endl;
					}
				} 
				
			}	
		
		}
	}
	
	
	
	//cout << "done Floyd now..." << endl;
	if(problemInformation.verbose){
		for(int i = 0; i < entityCount; i++){
			for(int j = 0; j < entityCount; j++){
				
				
				if(digraph[i][j] != INT_MAX) {
					cout << "this is validator for " << actionConcerned->name() << " distance from " << dereference[i]->name() << " to " << dereference[j]->name() << " is now " << digraph[i][j] << " route taken is through " << *dereference[routeGraph[i][j]] << endl;
				} else {
					cout << "this is validator for " << actionConcerned->name() << " distance from " << dereference[i]->name() << " to " << dereference[j]->name() << " is infinite" << endl;
				}
				
				
			}
		}
	}
	 if(problemInformation.verbose) cout << "completed construction" << endl;
	 
};	

bool ActionSequenceValidator::canApplyActionInstance(const ActionInstance * toCheck, int & heuristic){

	//need to find the relevant predicate
	
	PredicateInstance** preconds = toCheck->getPreconditionsAsPredicateInstances();
	
	Entity* loc1;
	Entity* loc2;
	
	for(int i = 0; i < toCheck->getPreconditionCount(); i++){
		if(preconds[i]->getParentPredicate() == staticTransPred){
			loc1 = preconds[i]->getParameter(0)->boundTo();
			loc2 = preconds[i]->getParameter(1)->boundTo();
		}
		if(preconds[i]->decrUsage()) delete preconds[i];
	}
	
	delete[] preconds;
	
	if(toCheck->getParentAction() != actionConcerned){
	
		cout << "asking for applicability of unrelated predicate" << endl;
		return -1;
	}

	int i = entitiesConcerned[loc1];
	int j = entitiesConcerned[loc2];
	
	heuristic = digraph[i][j];
	return digraph[i][j] != -1;


};


Plan* ActionSequenceValidator::GetActionsToApply(Entity* param1, Entity* param2, double startingTime, ActionInstance* toApply){
	
	
	//cout << "Asking for action " << actionConcerned->name() << " linking " << *param1 << " and " << *param2 << endl;
	int i = entitiesConcerned[param1];
	int j = entitiesConcerned[param2];
	//cout << "i = " << i << "; j = " << j << endl;
	//cout << "digraph is:" << endl;
	
	/*for(int i = 0; i < entityCount; i++){
		cout << i << "  ";
		if(i < 10) cout << " ";
	}
	cout << endl;
	for(int k = 0; k < entityCount; k++){
		cout << k << " ";
		if(k < 10) cout << " ";
		for(int m = 0; m < entityCount; m++){
			if(digraph[k][m] == INT_MAX) {
				
				cout << "inf ";
				
			} else if (digraph[k][m] < 10){
				cout << digraph[k][m] << "   ";
				
			}else {
				cout << digraph[k][m] << "  ";
			}
		}
		cout << endl;
	}
	for(int i = 0; i < entityCount; i++){
		cout << i << "  ";
		if(i < 10) cout << " ";
	}
	cout << endl;
	for(int k = 0; k < entityCount; k++){
		cout << k << " ";
		for(int m = 0; m < entityCount; m++){
			if(routeGraph[k][m] == -1) {
				
				cout << "-1  ";
				
			} else if (routeGraph[k][m] < 10){
				cout << routeGraph[k][m] << "   ";
				
			}else {
				cout << routeGraph[k][m] << "  ";
			}
		}
		cout << endl;
	}*/
	
	Plan* toReturn = new Plan();
	
	if(routeGraph[i][j] == j) {
		toReturn->addStepAtBeginning(toApply, startingTime);
		return toReturn;
	}
	int prev = i;
	int count = 0;
	while(i != j){
	
		//cout << "i = " << i << "; j = " << j << endl;
		if(i == -1 || digraph[i][j] == INT_MAX) {
			//cout << "following a path that doesn't exist from " << (*dereference[prev]) << " to " << (*dereference[j]) << endl;
			/*if(digraph[prev][j] == INT_MAX) {
				cout << "connectivity for this is inf" << endl;
			} else {
				cout << "connectivity for this is " << digraph[prev][j] << endl;
			}*/
		}
		if(digraph[i][j] == 1){
		
		
			ActionInstance* toAdd = actionConcerned->getInstance();
			toAdd->getParameter(loc1)->bindTo(dereference[i]);
			//cout << "routeGraph[" << i << "] [" << j << "] = " << routeGraph[i][j] << endl;
			toAdd->getParameter(loc2)->bindTo(dereference[routeGraph[i][j]]);
			
			for(int k = 0; k < toAdd->getParentAction()->parameterCount; k++){
				if(k != loc1 && k != loc2){
					toAdd->getParameter(k)->bindTo(toApply->getParameter(k)->boundTo());
				}
			}
			
			
			//toAdd->getParameter(1)->bindTo(dereference[i]);
			//toAdd->getParameter(2)->bindTo(dereference[routeGraph[i][j]]);
			toReturn->push_back(pair<ActionInstance*,double>(toAdd,(startingTime + count)));
			++count;
			prev = i;
			//cout << "Route to follow from " << i << ":" << *dereference[i] << " to " << j << ":" << *dereference[j] << " is" << routeGraph[i][j] << endl;

			return toReturn;
			
		} else{
		
			ActionInstance* toAdd = actionConcerned->getInstance();
			toAdd->getParameter(loc1)->bindTo(dereference[i]);
			//cout << "routeGraph[" << i << "] [" << j << "] = " << routeGraph[i][j] << endl;
			toAdd->getParameter(loc2)->bindTo(dereference[routeGraph[i][j]]);
			
			for(int k = 0; k < toAdd->getParentAction()->parameterCount; k++){
				if(k != loc1 && k != loc2){
					toAdd->getParameter(k)->bindTo(toApply->getParameter(k)->boundTo());
				}
			}
			
			
			//toAdd->getParameter(1)->bindTo(dereference[i]);
			//toAdd->getParameter(2)->bindTo(dereference[routeGraph[i][j]]);
			toReturn->push_back(pair<ActionInstance*,double>(toAdd,(startingTime + count)));
			
			prev = i;
			//cout << "Route to follow from " << i << ":" << *dereference[i] << " to " << j << ":" << *dereference[j] << " is" << routeGraph[i][j] << endl;

			++count;
			i = routeGraph[i][j];
			//cout << "Now i = " << i << " and j = " << j << endl;
		}
		
		
	} 
	
	//cout << "action sequence validator asked for plan; returning plan: " << endl;
	cout << (*toReturn) << endl;
	return toReturn;
	
	
}

void ActionSequenceValidator::addRequiredPredicates(State* toModify){

	for(int i = 0; i < entityCount; i++){
		for(int j = 0; j < entityCount; j++){
			if(digraph[i][j] != INT_MAX && digraph[i][j] != 1 && i != j){
				//cout << "validator for " << actionConcerned->name() << "distance from " << *dereference[i] << " to " << *dereference[j] << "is not infinite; nor is it 1" << endl;
				PredicateInstance* toAdd = new PredicateInstance(staticTransPred);
				toAdd->setParameter(0,dereference[i]->getEntityAsParameter());
				toAdd->setParameter(1,dereference[j]->getEntityAsParameter());
				
				toModify->addPredicateInstance(toAdd);
				//cout << " adding new predicate " << *toAdd << " to state at" << toModify << endl;
			}
		}
	
	}

}

ActionSequenceValidator::~ActionSequenceValidator(){

	if(!variablesNeeded) {
		for(int i = 0; i < entityCount; i++){
			delete[] digraph[i];
			delete[] routeGraph[i];
		}
		
		delete[] digraph;
		delete[] routeGraph;
		delete[] dereference;
	}
};

void ActionSequenceValidator::write(ostream & o) const{

	o << "Entities concerned are: " << endl;
	for(int i = 0; i < entityCount; i++){
		o << i << " " << (*dereference[i]) << endl;
	}
	o << "all entities are of type " << (*typeConcerned) << endl;
	o << "Common Predicate is " << staticTransPred->name() << endl;
	o << "Action concerned " << actionConcerned << endl;
	o << "Connectivity Graph" << endl;
	
	for(int i = 0; i < entityCount; i++){
		for(int j = 0; j < entityCount; j++){
			if(digraph[i][j] != INT_MAX) {
				o << digraph[i][j] << " ";
			} else {
				o << "inf ";
			}
		}
		
		o << endl;
	}
	
	o << "Route Graph" << endl;
	
	for(int i = 0; i < entityCount; i++){
		for(int j = 0; j < entityCount; j++){
			o << routeGraph[i][j] << " ";
		}
		
		o << endl;
	}
	
};

void ActionSequenceValidator::updateEntities(StaticInformation & problemInformation){

	ActionSequenceValidator* tmp = new ActionSequenceValidator(staticTransPred, loc1, loc2, problemInformation, actionConcerned);
	//delete this's variables
	
	
	for(int i = 0; i < entityCount; i++){
		delete[] digraph[i];
		delete[] routeGraph[i];
	}
		
	delete[] digraph;
	delete[] routeGraph;
	delete[] dereference;
		
	entitiesConcerned = tmp->entitiesConcerned;
	dereference = tmp->dereference;
	entityCount = tmp->entityCount;
	digraph = tmp->digraph;
	routeGraph = tmp->routeGraph;
	tmp->needVariables(true);
	delete tmp;
	
};



ostream & operator <<(ostream & o, const ActionSequenceValidator & a) {
	a.write(o);
	return o;
};
