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
#include "plan.h"
#include "MacroAction.h"
#include <list>

#include "ActionSequenceValidator.h"

#include "relaxedPlanningGraph.h"

ActionInstanceList Plan::helpfulActions() const {

	ActionInstanceList toReturn;
	
	if (empty()) return toReturn;
	
	list<pair<ActionInstance*, double> >::const_iterator actItr = planActions.begin();
	list<pair<ActionInstance*, double> >::const_iterator actEnd = planActions.end();
	
	double firstTimestamp = planActions.front().second;
	
	for(; actItr != actEnd && (*actItr).second == firstTimestamp; ++actItr){

		toReturn.push_back((*actItr).first);	
	}
	
	return toReturn;

};


struct PONode {

	ActionInstance* act;
	list<PONode*> parents;
	list<PONode*> children;
	PONodeSet ultimateChildren;
	int fanIn;
	double timeStamp;
	
	PONode(ActionInstance* t) : act(t), fanIn(0), timeStamp(0.0) {act->incrUsage();};	
	
	~PONode() {
		if (act->decrUsage()) delete act;
	}
	
	void registerUltimate(PONode* c) {
		PONode* pushUp = c;
		bool doPush;
		if (c != 0) {
			pair<PONodeSet::iterator, bool> success;
			success = ultimateChildren.insert(c);
			doPush = success.second;
		} else {
			pushUp = this;
			doPush = true;
		}
		
		if (doPush) {
			for (list<PONode*>::iterator itr = parents.begin();
				itr != parents.end();
				itr++) {
				
				(*itr)->registerUltimate(pushUp);
			}
		}
	}
	

	void updateTime(const double & newTime) {
		if (newTime > timeStamp) timeStamp = newTime;
	}
};

struct ADLPONode {

	ActionInstanceList act;
	list<ADLPONode*> parents;
	list<ADLPONode*> children;
	ADLPONodeSet ultimateChildren;
	int fanIn;
	double timeStamp;
	
	ADLPONode(const ActionInstanceList & t) : act(t), fanIn(0), timeStamp(0.0) {};	
	
	~ADLPONode() {
	}
	
	void registerUltimate(ADLPONode* c) {
		ADLPONode* pushUp = c;
		bool doPush;
		if (c != 0) {
			pair<ADLPONodeSet::iterator, bool> success;
			success = ultimateChildren.insert(c);
			doPush = success.second;
		} else {
			pushUp = this;
			doPush = true;
		}
		
		if (doPush) {
			for (list<ADLPONode*>::iterator itr = parents.begin();
				itr != parents.end();
				itr++) {
				
				(*itr)->registerUltimate(pushUp);
			}
		}
	}
	

	void updateTime(const double & newTime) {
		if (newTime > timeStamp) timeStamp = newTime;
	}
	
	bool isMutexWith(ADLPONode * cw) {
		ActionInstanceList::iterator ocItr = act.begin();
		ActionInstanceList::iterator ocEnd = act.end();
		
		ActionInstanceList::iterator ccItr;
		ActionInstanceList::iterator ccEnd;
		
		for (; ocItr != ocEnd; ocItr++) {
			
			ccItr = cw->act.begin();
			ccEnd = cw->act.end();
			
			for (; ccItr != ccEnd; ccItr++) {
				if ((*ocItr)->isMutexWith(*(*ccItr))) {
					return true;
				}
			}
			
		}
		return false;
	}
};

Plan* Plan::unmacrify(StaticInformation * staticInf) const{
	//cout << "Unmacrifying plan" << endl;
	//cout << *this << endl;
	int numberOfMarvins = 0;
	int numberOfEscapeMacroActions = 0;
	int numberOfMacroActionSteps = 0;
	int potentialMacroSteps = 0;
	
	Plan* toReturn = new Plan();
	
	const_iterator itr = begin();
	const_iterator itrEnd = end();
	
	list<pair<MacroAction*,int> > seen;
	
	for(; itr != itrEnd; ++itr){
		//cout << "next step" << endl;
		//ActionInstanceList macroActionsAtThisTime;
		//for(; itr != planActions.end() && (*itr).second == currTime; itr++){
		double currTime = (*itr).second;
		//cout << "Step at time " << currTime << endl;
		ActionInstance* currAI = (*itr).first;
		//cout << "Action instance is at " << currAI << endl;
		//cout << "Usage count = " << currAI->getUsageCount() << endl;
		if(currAI->isMacroAction()){
			//cout << "It's a macro action " << endl;
			const Action* currAct =  currAI->getParentAction();
			//cout << "Got the action pointer: " << currAct << endl;
			if (currAct == 0) {
				//cout << "It's a zero pointer!!!" << endl;
				
			}
			//string n = currAct->name();
			//cout << n << " claims to be a macro action" << endl;
			MacroAction* currMA = const_cast<MacroAction*>(static_cast<const MacroAction*>(currAct));
			//cout << "Updating counters..." << endl;
			currMA->incrementAppropriateCounter(numberOfMarvins, numberOfEscapeMacroActions);
			//macroActionsAtThisTime.push_back((*itr).first);
			//cout << "Getting constituent actions..." << endl;
			
			Plan* actionsToAdd = currMA->getConstituentActions((*itr).first);
			bool exemplarSeen = false;
			for (list<pair<MacroAction*, int> >::iterator csItr = seen.begin(); !exemplarSeen && csItr != seen.end(); csItr++) {
				exemplarSeen = ((*csItr).first == currMA);
				if (exemplarSeen) {
					(*csItr).second++;
				}
			}
			
			if (!exemplarSeen) {
				//cout << "; Exemplar of " << *currMA << ":" << endl;
				/*Plan::iterator expItr = actionsToAdd->begin();
				for (;expItr != actionsToAdd->end(); expItr++) {
					cout << ";    ";
					cout << (*expItr).second;
					cout << "  ";
					(*expItr).first->briefWrite(cout);
					
				}*/
				seen.push_back(pair<MacroAction*, int>(currMA, 1));
			}
			
			numberOfMacroActionSteps += actionsToAdd->length();
			iterator pItr = actionsToAdd->begin();
			double timeToAdd = currTime;
			double timeWas = currTime;
			//iterator currItrPos = itr;
			iterator currItrPos = toReturn->begin();
			bool firstTime = true;
			//for each action that is part of the macro-action
			for(;pItr != actionsToAdd->end();  pItr++){
			
				if((*pItr).second != timeWas || firstTime){
					
					timeToAdd = currTime + (*pItr).second;
					timeWas = timeToAdd - currTime;
					if(firstTime) firstTime = false;
					
					while(currItrPos != toReturn->end() && (*currItrPos).second <= timeToAdd) currItrPos++;
					(*pItr).second = timeToAdd;
					toReturn->insert(currItrPos, (*pItr));
					if(currItrPos == end()) toReturn->push_back(*pItr);
					
				} else {
					//insert before the position
					(*pItr).second = timeToAdd;
					toReturn->insert(currItrPos,(*pItr));	
				}
				
				
			
			}
			
			delete actionsToAdd;
			
		}else{
			//cout << "Normal step" << endl;
			
			iterator insItr = toReturn->begin();
			while(insItr != toReturn->end() && (*insItr).second <= (*itr).second) insItr++;
				
			//cout << "Inserting in right place" << endl;
			
			
			toReturn->insert(insItr, (*itr));
			
			//cout << "Seeing if it's a sub action of anything" << endl;
			
			bool subAction = false;
			
			const Action* c = (*itr).first->getParentAction();
			
			for (int i = 0; !subAction && i < staticInf->numberOfActions; i++) {
				if (staticInf->Actions[i]->isMacroAction) {
					subAction = ((MacroAction*) (staticInf->Actions[i]))->containsActionOfType(c);
				}
			}
			
			if (subAction) potentialMacroSteps++;
			
			//cout << "Done that bit" << endl;
		}
			
		//}
	
	}
	
	for (	list<pair<MacroAction*, int> >::iterator mauItr = seen.begin();
		mauItr != seen.end();
		mauItr++) {
		//cout << ";  Macro action " << *((*mauItr).first) << ";  is used " << (*mauItr).second << " times" << endl;
	}
	/*cout << "; " << numberOfMarvins << " marvin";
	if (numberOfMarvins != 1) cout << "s";
	cout << " in solution plan" << endl;
	
	cout << "; "<< numberOfEscapeMacroActions << " plateau-escaping macro action";
	if (numberOfEscapeMacroActions != 1) cout << "s";
	cout << " in solution plan" << endl;
	
	cout << "; " << numberOfMacroActionSteps << " step";
	if (numberOfMacroActionSteps != 1) cout << "s";
	cout << " in the solution plan accounted for by macro actions" << endl;
	
	cout << "; " << potentialMacroSteps << " step";
	if (potentialMacroSteps != 1) cout << "s";
	cout << " in the solution plan are sub-actions of macro actions (but added not as part of a macro action)" << endl;
	*/
	return toReturn;

}

list<Plan *> Plan::untangle(list<pair<ActionInstanceList, double> > & stepsAndSubActions) {

	
	list<ADLPONode*> nodes;
	list<pair<ActionInstanceList, double> >::const_iterator opItr = stepsAndSubActions.begin();
	list<pair<ActionInstanceList, double> >::const_iterator opEnd = stepsAndSubActions.end();
	
	int created = 0;
	for (; opItr != opEnd; opItr++) {
		nodes.push_back(new ADLPONode((*opItr).first));
		created++;
	}

	//cout << "Created " << created << " ADL PO Nodes" << endl;
	
	list<ADLPONode*>::iterator rItr = nodes.end();
	rItr--;
	
	
	//cout << "Creating causal links" << endl;
	created = 0;
	for (	list<ADLPONode*>::reverse_iterator tracker = nodes.rbegin();
		tracker != nodes.rend();
		rItr--, tracker++) {
		
		ADLPONode* currNode = *rItr;
		
		
		ActionInstanceList::iterator ialItr = currNode->act.begin();
		ActionInstanceList::iterator ialEnd = currNode->act.end();
		
		for (; ialItr != ialEnd; ialItr++) {
			PredicateInstance** p;
			int looplimit;
			
			p = (*ialItr)->internalPreconditions();
			looplimit = (*ialItr)->getPreconditionCount();				
			for (int i = 0; i < looplimit; i++) {
				if (p[i] != 0 && !p[i]->getParentPredicate()->isStatic()) {
				
					bool exists = false;
					ADLPONode* establishedBy;
					for (	list<ADLPONode*>::iterator fItr = nodes.begin();
						fItr != rItr;
						fItr++) {
						
						ADLPONode* sNode = *fItr;
						ActionInstanceList::iterator snItr;
						ActionInstanceList::iterator snEnd;
						
						
						if (exists) {
							snItr = sNode->act.begin();
							snEnd = sNode->act.end();
							for (;snItr != snEnd; snItr++) {
								if ((*snItr)->deletes(p[i])) {
									exists = false;
								}
							}
						} else {
							snItr = sNode->act.begin();
							snEnd = sNode->act.end();
							for (;snItr != snEnd; snItr++) {
								if ((*snItr)->achieves(p[i])) {
									exists = true;
									establishedBy = sNode;
								}
							}
							
						}
					}
					
					if (exists) {
						//cout << *p[i] << " achieved by ";
						//establishedBy->act.front()->briefWrite(cout);
						
						bool found = false;
				
						for (	list<ADLPONode*>::iterator cpItr = establishedBy->children.begin();
							!found && cpItr != establishedBy->children.end();
							cpItr++) {
							
							found = (currNode == *cpItr);
							
								
						}
						
						if (!found) {
						
							establishedBy->children.push_back(currNode);
							currNode->parents.push_back(establishedBy);
							currNode->fanIn++;
						
						}
						
						
					}
				
				}
				
				
			}
			
			p = (*ialItr)->internalNegativePreconditions();
			looplimit = (*ialItr)->getNegativePreconditionCount();				
			
			for (int i = 0; i < looplimit; i++) {
				if (p[i] != 0 && !p[i]->getParentPredicate()->isStatic()) {
				
					bool notExists = false;
					ADLPONode* establishedBy;
					for (	list<ADLPONode*>::iterator fItr = nodes.begin();
						fItr != rItr;
						fItr++) {
						
						ADLPONode* sNode = *fItr;
						ActionInstanceList::iterator snItr;
						ActionInstanceList::iterator snEnd;
						
						
						if (!notExists) {
							snItr = sNode->act.begin();
							snEnd = sNode->act.end();
							for (;snItr != snEnd; snItr++) {
								if ((*snItr)->deletes(p[i])) {
									notExists = true;
									establishedBy = sNode;
								}
							}
						} else {
							snItr = sNode->act.begin();
							snEnd = sNode->act.end();
							for (;snItr != snEnd; snItr++) {
								if ((*snItr)->achieves(p[i])) {
									notExists = false;
								}
							}
							
						}
					}
					
					if (notExists) {
						//cout << *p[i] << " negative-achieved by ";
						//establishedBy->act.front()->briefWrite(cout);
						
						
						
						bool found = false;
				
						for (	list<ADLPONode*>::iterator cpItr = establishedBy->children.begin();
							!found && cpItr != establishedBy->children.end();
							cpItr++) {
							
							found = (currNode == *cpItr);
							
								
						}
						
						if (!found) {
						
							establishedBy->children.push_back(currNode);
							currNode->parents.push_back(establishedBy);
							currNode->fanIn++;
						
						}
					}
				
				}
				
				
			}
			
			
		}
		created++;
		
	}
	//cout << "Created causal links into " << created << " nodes" << endl;
	//cout << "Creating ordering constraints" << endl;
	
	rItr = nodes.begin();
	created = 0;
	for (; rItr != nodes.end(); rItr++) {
	
		ADLPONode * currNode = *rItr;
		
		list<ADLPONode*>::iterator ultItr = rItr;
		ultItr++;
		
		for (; ultItr != nodes.end(); ultItr++) {
		
			ADLPONode * compNode = *ultItr;
			
			if (currNode->isMutexWith(compNode)) {
			
				bool found = false;
				
				for (	list<ADLPONode*>::iterator cpItr = currNode->children.begin();
					!found && cpItr != currNode->children.end();
					cpItr++) {
					
					found = (compNode == *cpItr);
					
						
				}
				
				if (!found) {
					/*currNode->act.front()->briefWrite(cout);
					cout << " --- must come before ---" << endl;
					compNode->act.front()->briefWrite(cout);*/
					
										
					currNode->children.push_back(compNode);
					compNode->parents.push_back(currNode);
					compNode->fanIn++;
				
				}
			
			}
		
		}
		created++;
	
	}
	//cout << "Created ordering constraints from " << created << " nodes" << endl;
	for (list<ADLPONode*>::iterator ultItr = nodes.begin(); ultItr != nodes.end(); ultItr++) {
		(*ultItr)->registerUltimate(0);
	}
	
	//cout << "Extracting independent plans" << endl;
	
	list<ADLPONode*> topLevel;
	
	created = 0;
	for (list<ADLPONode*>::iterator ultItr = nodes.begin(); ultItr != nodes.end(); ultItr++) {
		if ((*ultItr)->parents.empty()) {
			//cout << "Found top-level node: fan-in = " << (*ultItr)->fanIn << endl;
			//(*ultItr)->act.front()->briefWrite(cout);
			topLevel.push_back(*ultItr);
		}
		created++;
	}
	
	//cout << "Inspected " << created << " nodes to find top-level ones" << endl;
	
	list<ADLPONodeSet* > nodePlans;
	
	for (list<ADLPONode*>::iterator ultItr = topLevel.begin(); ultItr != topLevel.end(); ultItr++) {
		bool independent = true;
		for (	list<ADLPONodeSet* >::iterator fmItr = nodePlans.begin();
			independent && fmItr != nodePlans.end();
			fmItr++) {
			
			ADLPONodeSet * compSet = *fmItr;
			
			ADLPONodeSet * isect = intersection(&((*ultItr)->ultimateChildren), compSet);

			if (!isect->empty()) {
				//cout << "Non-independent plan stream dependent on ";
				//(*ultItr)->act.front()->briefWrite(cout);
				independent = false;
				compSet->insert(*ultItr);
				
				for (	ADLPONodeSet::iterator ccItr = (*ultItr)->ultimateChildren.begin();
					ccItr != (*ultItr)->ultimateChildren.end();
					ccItr++) {
				
					compSet->insert(*ccItr);
				
				}
				
				//cout << "Plan now contains " << compSet->size() << " elements" << endl;
			
			}
			
			delete isect;
			
		}
		if (independent) {
			/*cout << "Independent plan stream dependent on ";
			(*ultItr)->act.front()->briefWrite(cout);*/
			ADLPONodeSet * curr = new ADLPONodeSet();
			curr->insert(*ultItr);
			
			for (	ADLPONodeSet::iterator ccItr = (*ultItr)->ultimateChildren.begin();
				ccItr != (*ultItr)->ultimateChildren.end();
				ccItr++) {
				
				curr->insert(*ccItr);
				
			}
			
			nodePlans.push_back(curr);
			
			//cout << "Plan now contains " << curr->size() << " elements" << endl;
		}
	}
	
	list<Plan*> toReturn;
	
	//cout << "Creating output plans" << endl;
	int planID = 0;
	for (	list<ADLPONodeSet* >::iterator fmItr = nodePlans.begin();
		fmItr != nodePlans.end();
		fmItr++, planID++) {
		//cout << "\t plan " << planID << endl;
		
		Plan* newPlan = new Plan();
		list<ADLPONode*> searchQueue;
		
		for (	ADLPONodeSet::iterator fftsItr = (*fmItr)->begin();
			fftsItr != (*fmItr)->end();
			fftsItr++) {
		
			if ((*fftsItr)->fanIn == 0) {
				searchQueue.push_back(*fftsItr);
				/*cout << "Found a time-zero action: ";
				(*fftsItr)->act.front()->briefWrite(cout);*/
			}
				
		}
		
		while (!searchQueue.empty()) {
		
			ADLPONode* frontNode = searchQueue.front();
			searchQueue.pop_front();
			
			double currTime = frontNode->timeStamp;
			
			newPlan->addStepsAtEnd(frontNode->act, currTime);
			
			for (list<ADLPONode*>::iterator fanInDecr = frontNode->children.begin();
				fanInDecr != frontNode->children.end();
				fanInDecr++) {
			
				(*fanInDecr)->updateTime(currTime + 1.0);
				(*fanInDecr)->fanIn--;
				
				if ((*fanInDecr)->fanIn == 0) {
					searchQueue.push_back(*fanInDecr);
				}
					
			}
		
		}
		
		toReturn.push_back(newPlan);
		
		delete (*fmItr);
		
					
	}
	
	while (!nodes.empty()) {
		delete nodes.front();
		nodes.pop_front();
	}
	
	return toReturn;
	

}

list<Plan *> Plan::untangle() const {

	list<PONode*> nodes;
	for (Plan::const_iterator itr = begin();
		itr != end();
		itr++)  {
	
		nodes.push_back(new PONode((*itr).first));
			
	}
	
	list<PONode*>::iterator rItr = nodes.end();
	rItr--;
	
	
	cout << "Creating causal links" << endl;
	
	for (	list<PONode*>::reverse_iterator tracker = nodes.rbegin();
		tracker != nodes.rend();
		rItr--, tracker++) {
		
		PONode* currNode = *rItr;
		int looplimit = currNode->act->getPreconditionCount();
		PredicateInstance** p = currNode->act->getPreconditionsAsPredicateInstances();
		
		for (int i = 0; i < looplimit; i++) {
			if (!p[i]->getParentPredicate()->isStatic()) {
			
				bool exists = false;
				PONode* establishedBy;
				for (	list<PONode*>::iterator fItr = nodes.begin();
					fItr != rItr;
					fItr++) {
					
					PONode* sNode = *fItr;
					
					if (exists) {
						if (sNode->act->deletes(p[i])) {
							exists = false;
						}
					} else {
						if (sNode->act->achieves(p[i])) {
							exists = true;
							establishedBy = sNode;
						}
					}
				}
				
				if (exists) {
					establishedBy->children.push_back(currNode);
					currNode->parents.push_back(establishedBy);
					currNode->fanIn++;
				}
			
			}
			
			if (p[i]->decrUsage()) delete p[i];
		}
		
		delete [] p;		
		
	}
	
	cout << "Creating ordering constraints" << endl;
	
	rItr = nodes.begin();
	
	for (; rItr != nodes.end(); rItr++) {
	
		PONode * currNode = *rItr;
		
		list<PONode*>::iterator ultItr = rItr;
		ultItr++;
		
		for (; ultItr != nodes.end(); ultItr++) {
		
			PONode * compNode = *ultItr;
			
			if (currNode->act->isMutexWith(*(compNode->act))) {
			
				bool found = false;
				
				for (	list<PONode*>::iterator cpItr = currNode->children.begin();
					!found && cpItr != currNode->children.end();
					cpItr++) {
					
					found = (compNode == *cpItr);
					
						
				}
				
				if (!found) {
				
					currNode->children.push_back(compNode);
					compNode->parents.push_back(currNode);
					compNode->fanIn++;
				
				}
			
			}
		
		}
	
	}
	
	for (list<PONode*>::iterator ultItr = nodes.begin(); ultItr != nodes.end(); ultItr++) {
		(*ultItr)->registerUltimate(0);
	}
	
	
	cout << "Extracting independent plans" << endl;
	
	list<PONode*> topLevel;
	
	for (list<PONode*>::iterator ultItr = nodes.begin(); ultItr != nodes.end(); ultItr++) {
		if ((*ultItr)->parents.empty()) topLevel.push_back(*ultItr);
	}
	
	list<PONodeSet* > nodePlans;
	
	for (list<PONode*>::iterator ultItr = topLevel.begin(); ultItr != topLevel.end(); ultItr++) {
		bool independent = true;
		for (	list<PONodeSet* >::iterator fmItr = nodePlans.begin();
			independent && fmItr != nodePlans.end();
			fmItr++) {
			
			PONodeSet * compSet = *fmItr;
			
			PONodeSet * isect = intersection(&((*ultItr)->ultimateChildren), compSet);

			if (!isect->empty()) {
			
				independent = false;
				
				for (	PONodeSet::iterator ccItr = (*ultItr)->ultimateChildren.begin();
					ccItr != (*ultItr)->ultimateChildren.end();
					ccItr++) {
				
					compSet->insert(*ccItr);
				
				}
			
			}
			
			delete isect;
			
		}
		if (independent) {
			PONodeSet * curr = new PONodeSet();
			curr->insert(*ultItr);
			
			for (	PONodeSet::iterator ccItr = (*ultItr)->ultimateChildren.begin();
				ccItr != (*ultItr)->ultimateChildren.end();
				ccItr++) {
				
				curr->insert(*ccItr);
				
			}
			
			nodePlans.push_back(curr);
		}
	}
	
	list<Plan*> toReturn;
	
	for (	list<PONodeSet* >::iterator fmItr = nodePlans.begin();
		fmItr != nodePlans.end();
		fmItr++) {
	
		Plan* newPlan = new Plan();
		list<PONode*> searchQueue;
		
		for (	PONodeSet::iterator fftsItr = (*fmItr)->begin();
			fftsItr != (*fmItr)->end();
			fftsItr++) {
		
			if ((*fftsItr)->fanIn == 0) searchQueue.push_back(*fftsItr);
				
		}
		
		while (!searchQueue.empty()) {
		
			PONode* frontNode = searchQueue.front();
			searchQueue.pop_front();
			
			double currTime = frontNode->timeStamp;
			
			newPlan->addStepAtEnd(frontNode->act, currTime);
			
			for (list<PONode*>::iterator fanInDecr = frontNode->children.begin();
				fanInDecr != frontNode->children.end();
				fanInDecr++) {
			
				(*fanInDecr)->updateTime(currTime + 1.0);
				(*fanInDecr)->fanIn--;
				
				if ((*fanInDecr)->fanIn == 0) {
					searchQueue.push_back(*fanInDecr);
				}
					
			}
		
		}
		
		toReturn.push_back(newPlan);
		
		delete (*fmItr);
		
					
	}
	
	while (!nodes.empty()) {
		delete nodes.front();
		nodes.pop_front();
	}
	
	return toReturn;
	

}

PONodeSet * Plan::intersection(const PONodeSet * first, const PONodeSet * second) {

	PONodeSet * toReturn = new PONodeSet();
	
	
	PONodeSet::const_iterator fStart = first->begin();
	PONodeSet::const_iterator fEnd = first->end();
	
	for (;fStart != fEnd; fStart++) {
		if (second->find(*fStart) != second->end()) {
			toReturn->insert(*fStart);
		}
		
	}
	return toReturn;
};


ADLPONodeSet * Plan::intersection(const ADLPONodeSet * first, const ADLPONodeSet * second) {

	ADLPONodeSet * toReturn = toReturn = new ADLPONodeSet();
	
	
	ADLPONodeSet::const_iterator fStart = first->begin();
	ADLPONodeSet::const_iterator fEnd = first->end();
	
	for (;fStart != fEnd; fStart++) {
		if (second->find(*fStart) != second->end()) {
			toReturn->insert(*fStart);
		}
		
	}
	return toReturn;
};


ostream & operator <<(ostream & o, const Plan & p) {
	p.write(o);
	return o;
}

Plan* Plan::removeSimilarAdjacentSteps() const {
	
	const_iterator itr = planActions.begin();
	
	const Action* comparison = planActions.front().first->getParentAction();
	//double timeOfAction = planActions.front().second;
	Plan* toReturn = new Plan();
	toReturn->push_back(planActions.front());
	double subtractFromTimeStamp = 0;
	double currTimeStamp = 0;
	for(; itr != planActions.end(); itr++){
		
		if((*itr).first->getParentAction() != comparison){
		
			comparison = (*itr).first->getParentAction();
			currTimeStamp = (*itr).second;
			toReturn->push_back(pair<ActionInstance*,double>((*itr).first, (*itr).second - subtractFromTimeStamp));
			
			
		}else{
		
			if(currTimeStamp != (*itr).second){
				
				subtractFromTimeStamp += 1; 
				//eventually this needs to be += (*itr).first->duration
				currTimeStamp = (*itr).second;
			}
		}
		
	}
	if(toReturn->length() == 1) toReturn = new Plan(*this);
	return toReturn;

};

void Plan::addStepsAtEnd(const ActionInstanceList & actionsToAdd, const double & timeStamp) {
	
	ActionInstanceList::const_iterator actItr = actionsToAdd.begin();
	const ActionInstanceList::const_iterator actEnd = actionsToAdd.end();
	
	list<Plan::iterator> pos;
	list<Plan::iterator> ends;
	list<Plan*> toDelete;
	
	for(;actItr != actEnd; ++actItr){

		if ((*actItr)->getParentAction()->transformationalOperator) {
			
			Plan* toAdd = (*actItr)->getParentAction()->sequenceValidator->GetActionsToApply((*actItr)->getParameter((*actItr)->getParentAction()->sequenceValidator->getParameterOne())->boundTo(), (*actItr)->getParameter((*actItr)->getParentAction()->sequenceValidator->getParameterTwo())->boundTo(),timeStamp, (*actItr)); 
			toDelete.push_back(toAdd);
			pos.push_back(toAdd->begin());
			ends.push_back(toAdd->end());
		} else {
			planActions.push_back(pair<ActionInstance*, double>(*actItr, timeStamp));
			(*actItr)->incrUsage();
		}
	}
	
	bool carryOn = true;
	
	while (carryOn) {
		carryOn = false;
		//cout << "Carrying on" << endl;
		list<Plan::iterator>::iterator posItr = pos.begin();
		list<Plan::iterator>::iterator posEnd = pos.end();
		
		list<Plan::iterator>::iterator endsItr = ends.begin();
					
		for (; posItr != posEnd; ++posItr, ++endsItr) {
		
			if ((*posItr) != (*endsItr)) {
				carryOn = true;
				planActions.push_back(*(*posItr));
				(*(*posItr)).first->incrUsage();
				++(*posItr);
			}
		
		}
	}
	
	list<Plan*>::iterator delItr = toDelete.begin();
	list<Plan*>::iterator delEnd = toDelete.end();
	
	for (; delItr != delEnd; ++delItr) delete (*delItr);
	
}

void Plan::blindlyAddStepAtEnd(ActionInstance * actionToAdd, const double & timeStamp) {
	planActions.push_back(pair<ActionInstance*, double>(actionToAdd, timeStamp));
	actionToAdd->incrUsage();
}

void Plan::addStepAtEnd(ActionInstance * actionToAdd, const double & timeStamp) {
	
	if (actionToAdd->getParentAction()->transformationalOperator) {
	
		Plan* toAdd = actionToAdd->getParentAction()->sequenceValidator->GetActionsToApply(actionToAdd->getParameter(actionToAdd->getParentAction()->sequenceValidator->getParameterOne())->boundTo(), actionToAdd->getParameter(actionToAdd->getParentAction()->sequenceValidator->getParameterTwo())->boundTo(),timeStamp, actionToAdd); 

		Plan::iterator itr = toAdd->begin();
		Plan::iterator itrEnd = toAdd->end();
		
		for(;itr != itrEnd; ++itr){
			
			planActions.push_back(*itr);
			planActions.back().first->incrUsage();
		
		}
		
		delete toAdd;
			
	} else {
		planActions.push_back(pair<ActionInstance*, double>(actionToAdd, timeStamp));
		actionToAdd->incrUsage();
	}
}

Plan* Plan::expandTOs() {

	bool changed = false;
	
	Plan* toReturn = new Plan();
	
	iterator itr = begin();
	const iterator itrEnd = end();
	
	if (itr == itrEnd) return toReturn;
	
	double offset = 0;
	double lastTime = (*itr).second;
	
	
	for(; itr != itrEnd; ++itr){
		
		double currTime = (*itr).second;
		ActionInstance* currAI = (*itr).first;
		
		if (currTime > lastTime) {
			if (offset > 0) {
				iterator tItr = itr;
				for (; tItr != itrEnd; ++tItr) {
					cout << "Doing a spot of shuffling" << endl;
					double oldTime = (*tItr).second;
					cout << "Action was at " << oldTime << endl;
					oldTime += offset;
					(*tItr).second = oldTime;
					cout << "Now at " << oldTime << endl;
					
				}
				
				currTime = (*itr).second;
		
				changed = true;		
				offset = 0;
			}
			lastTime = currTime;
		}
		
		if(currAI->getParentAction()->transformationalOperator){
			
			Plan* actionsToAdd = currAI->getParentAction()->sequenceValidator->GetActionsToApply(currAI->getParameter(currAI->getParentAction()->sequenceValidator->getParameterOne())->boundTo(), currAI->getParameter(currAI->getParentAction()->sequenceValidator->getParameterTwo())->boundTo(),currTime, currAI); 
			//cout << "TO Steps:" << endl;
			//cout << *actionsToAdd << endl;
			
			if (actionsToAdd->length() > 1) {
			
				//double newOffset = actionsToAdd->makeSpan() - 1;
				//if (offset < newOffset) offset = newOffset;
			}
			
			iterator pItr = actionsToAdd->begin();
			const iterator pItrEnd = actionsToAdd->end();
			
			//double timeToAdd = currTime;
			//double timeWas = 0;
			
			iterator currItrPos = toReturn->begin();
			//bool firstTime = true;
			
			for(;pItr != pItrEnd; ++pItr){
				
				if (currItrPos == toReturn->end()) {
					toReturn->addStepAtEnd((*pItr).first, (*pItr).second);
				} else {
					while(currItrPos != toReturn->end() && (*currItrPos).second <= (*pItr).second) ++currItrPos;
					
					if (currItrPos == toReturn->end()) {
						toReturn->addStepAtEnd((*pItr).first, (*pItr).second);
					} else {
						//(*pItr).second = timeToAdd;
						toReturn->insert(currItrPos, (*pItr));
					}
				}/*
				if((*pItr).second != timeWas || firstTime){
					
					timeToAdd = currTime + (*pItr).second;
					timeWas = timeToAdd - currTime;
					if(firstTime) firstTime = false;
					
					while(currItrPos != toReturn->end() && (*currItrPos).second <= timeToAdd) currItrPos++;
					(*pItr).second = timeToAdd;
					toReturn->insert(currItrPos, (*pItr));
					if(currItrPos == end()) toReturn->push_back(*pItr);
					
				} else {
					//insert before the position
					(*pItr).second = timeToAdd;
					toReturn->insert(currItrPos,(*pItr));	
				}*/
				
				
			
			}
			
			delete actionsToAdd;
			
		}else{
			//cout << "Normal step" << endl;
			//cout << "Step at time " << currTime << endl;
			iterator insItr = toReturn->begin();
			const iterator insEnd = toReturn->end();
			
			if (insItr == insEnd) {
				toReturn->addStepAtEnd((*itr).first, (*itr).second);
			} else {
				while(insItr != insEnd && (*insItr).second <= (*itr).second) ++insItr;
				
				if (insItr == insEnd) {
					toReturn->addStepAtEnd((*itr).first, (*itr).second);
				} else {
					toReturn->insert(insItr, (*itr));
				}
			}
			
			
		
		}
			
		
	
	}
	
	if (changed) {
		//cout << "Changed; new plan: " << endl;
		//cout << *toReturn << endl;
	} else {
		//cout << "No TOs needed changing" << endl;
	}
	
	return toReturn;


};

list<MacroAction*> Plan::findPlateauEscapingMacros(StaticInformation * staticInf, RelaxedPlanningGraph * rpg, bool annotate) const {

	const bool localVerbose = false;
	
	if (localVerbose) {
		cout << "Finding Plateau-Escaping Macros from plan so far " << endl;
	}
	
	list<MacroAction*> toReturn;
	
	Plan segment;
	int bestHeuristic;
	bool onPlateau = false;

	double previousTimeStamp = 0.0;
	double plateauTimeStamp = 0.0;
	State* currState = staticInf->InitialState->copy();
	auto_ptr<ActionInstanceList> freeActions;
	
	if (staticInf->hasDerivedPredicates) {
			
		freeActions = rpg->freeActions(*staticInf, currState);
			
		while (!freeActions->empty()) {
			//cout << freeActions->size() << " free actions at the start" << endl;
			currState->applyActionsLocally(*(freeActions.get()), staticInf);
			currState->advanceTime();
			currState->timeBackOne();
			freeActions = rpg->freeActions(*staticInf, currState);
		} 
			
			//cout << "No more free actions at the start" << endl;
	}
	
	RelaxedPlanningGraph * startRPG = new RelaxedPlanningGraph(*rpg, *staticInf, currState);
	Plan relaxedPlan(startRPG->extractRelaxedPlanQuickly(*rpg, *staticInf, bestHeuristic));
	
	delete startRPG;
	
	const EntitySet* const entSet  = 0;
	
	rpg->applicableRelaxedActions(*staticInf, currState, entSet);
	
	Plan::const_iterator itr = begin();
	const Plan::const_iterator itrEnd = end();
	
	if (annotate) cout << "0 " << bestHeuristic << "" << endl;
	for (;itr != itrEnd; ++itr) {
	
		if (itr->second > previousTimeStamp) {
			do {
				currState->advanceTime();
				++previousTimeStamp;
			} while (itr->second > previousTimeStamp);
	
			if (staticInf->hasDerivedPredicates) {
			
				freeActions = rpg->freeActions(*staticInf, currState);
					
				while (!freeActions->empty()) {
					//cout << freeActions->size() << " free actions at the start" << endl;
					currState->applyActionsLocally(*(freeActions.get()), staticInf);
					currState->advanceTime();
					currState->timeBackOne();
					freeActions = rpg->freeActions(*staticInf, currState);
				} 
					
					//cout << "No more free actions at the start" << endl;
			}

			rpg->applicableRelaxedActions(*staticInf, currState, entSet);
			
		}
		
		if (itr->first->getParentAction()->rootAction()) {
		
			ActionInstanceList toApply;
			toApply.push_back(itr->first);
			
			rpg->getApplicableSubActions(itr->first, toApply);
			currState->applyActionsLocally(toApply, rpg, staticInf);
			
			int newHeuristic;
			
			startRPG = new RelaxedPlanningGraph(*rpg, *staticInf, currState);
			relaxedPlan = startRPG->extractRelaxedPlanQuickly(*rpg, *staticInf, newHeuristic);
			if (annotate) cout << (itr->second + 1) << " " << newHeuristic << endl;
			delete startRPG;
			//if (localVerbose) cout << "\t" << previousTimeStamp << ": " << newHeuristic << endl;
			
			if (newHeuristic < bestHeuristic) {
				if (onPlateau) {
					const double ats = (itr->second) - plateauTimeStamp;
					segment.addStepAtEnd(itr->first, ats);
					//if (localVerbose) cout << "\t --> Now off a plateau" << endl;
					MacroAction* currMA = new MacroAction(&segment, *staticInf);
					toReturn.push_back(currMA);
				}
				onPlateau = false;
				segment.clear();
				bestHeuristic = newHeuristic;
			} else {
				if (!onPlateau) {
					//if (localVerbose) cout << "\t --> Now on a plateau" << endl;
					onPlateau = true;
					plateauTimeStamp = previousTimeStamp;
				}
				const double ats = (itr->second) - plateauTimeStamp;
				segment.addStepAtEnd(itr->first, ats);
			}
					
			
		}
		
	
	};
	
	delete currState;
	
	return toReturn;

};
