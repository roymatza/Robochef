#include "SymmetryExtractor.h"
#include "State.h"
#include <ext/hash_map>
#include <climits>

list<SymmetryGroup*>* SymmetryExtractor::extractSymmetryGroups(StaticInformation & problemInformation, const bool & considerGoalState){

	list<SymmetryGroup*>* toReturn = extractSymmetryGroups(problemInformation, considerGoalState, problemInformation.InitialState);
	return toReturn;
};

list<SymmetryGroup*>* SymmetryExtractor::extractSymmetryGroups(StaticInformation & problemInformation, const bool & considerGoalState, State * startState){

	symmetryGroups.clear();

	Entity** loopOver;
	int looplimit;
	
	if (problemInformation.possiblySymmetric != 0) {
	
		looplimit = problemInformation.numberOfPossiblySymmetricEntities;
		loopOver = problemInformation.possiblySymmetric;
	} else {
		looplimit = problemInformation.numberOfEntities;
		loopOver = problemInformation.Entities;
	}
	
	/*for(int eIndex= 0; eIndex < looplimit; ++eIndex){
		Entity* currEntity = problemInformation.Entities[eIndex];
		SymmetryGroup* newGroup = new SymmetryGroup();
		newGroup->addEntity(currEntity);
		symmetryGroups.push_back(newGroup);
	}
	
	return symmetryGroups;*/
	
	//for all entities
	//list< list<PredicateInstance*> > definingGroupPredicates;
	//list< list<PredicateInstance*> > definingGroupGoalPredicates;
	

	//int looplimit = problemInformation.numberOfEntities;
	
	for(int eIndex= 0; eIndex < looplimit; ++eIndex){
		//cout << " [" << eIndex << "]";
		//cout.flush();
//		problemInformation.verbose = false;
		if(problemInformation.verbose){
			cout << "----------------------------------------------------------------" << endl;
			cout << "Considering Entity " << eIndex << " of " << problemInformation.numberOfEntities << endl;
		}

		Entity* currEntity = loopOver[eIndex];

		if(problemInformation.verbose) cout << "current entity is " << (*currEntity) << endl;

		PredicateInstanceList definingPredicates;
		bool added = false;

		State::iterator pItr = startState->begin();
		State::iterator pEnd = startState->end();
		//loop over the initial state
		for(;pItr != pEnd; ++pItr){

			if (!startState->isToDelete(*pItr)) {
				if(problemInformation.verbose) cout << "checking whether " << (*currEntity) << " is an argument of " << (*(*pItr)) << endl;
	
				//if this predicate pertains to the entity in question
				bool found = false;
				int llt = (*pItr)->getParentPredicate()->getArgumentCount();
				for(int paramIndex = 0; !found &&paramIndex < llt; ++paramIndex){
	
					if(currEntity == ((*pItr)->getParameter(paramIndex))->boundTo()){
						definingPredicates.push_back((*pItr));
						found = true;
						if(problemInformation.verbose) cout << "...............it is" << endl;
					}
	
				}
			}

		}

		pItr = problemInformation.StaticInitialState->begin();
		pEnd = problemInformation.StaticInitialState->end();
		//loop over the initial state
		for(;pItr != pEnd; ++pItr){

			if(problemInformation.verbose) cout << "checking whether " << (*currEntity) << " is an argument of static precondition " << (*(*pItr)) << endl;

			bool found = false;
			//if this predicate pertains to the entity in question
			int llt = (*pItr)->getParentPredicate()->getArgumentCount();
			for(int paramIndex = 0; !found && paramIndex < llt; ++paramIndex){

				if(currEntity == ((*pItr)->getParameter(paramIndex))->boundTo()){
					definingPredicates.push_back((*pItr));
					found = true;
					if(problemInformation.verbose) cout << "...............it is" << endl;
				}

			}

		}

		PredicateInstanceList definingGoalPredicates;

		if(considerGoalState){


			pItr = problemInformation.GoalPredicates->begin();
			pEnd = problemInformation.GoalPredicates->end();
			//loop over the initial state
			for(;pItr != pEnd; ++pItr){
				//if this predicate pertains to the entity in question
				bool found = false;
				int llt = (*pItr)->getParentPredicate()->getArgumentCount();
				for(int paramIndex = 0; !found && paramIndex < llt ; ++paramIndex){

					if(currEntity == ((*pItr)->getParameter(paramIndex))->boundTo()){
						found = true;
						definingGoalPredicates.push_back((*pItr));
					}

				}

			}
		}


		list<SymmetryGroup*>::iterator groupItr = symmetryGroups.begin();
		list<SymmetryGroup*>::iterator groupEnd = symmetryGroups.end();
		//int groupItrCount = -1;
		//cout << "Investigating what " << currEntity->name() << " could be symmetric with" << endl;
		for(; groupItr != groupEnd; ++groupItr){

			Entity* potentialMatch = (*groupItr)->getExemplar();
			//groupItrCount++;

			//if the entity is of the same type as those in this group
			if(currEntity->getEntityType() == potentialMatch->getEntityType() && currEntity->possiblySymmetricWith(potentialMatch) ){

				bool check = true;
				//if they don't have the same number of defining predicates they are not symmetric
				if((*groupItr)->getDefiningPredicates().size() != definingPredicates.size()) check = false;
				if(check && considerGoalState){
					if((*groupItr)->getDefiningGoalPredicates().size() != definingGoalPredicates.size()) check = false;
				}


				if(problemInformation.verbose) cout << "found entity of the same type ," << (*potentialMatch) << ",but number of predicates doesn't match" << endl;

				if(check){
					bool inGroup = true;

					if(problemInformation.verbose) cout << "found existing entity of same type; checking predicates..." << endl;

					PredicateInstanceList::iterator currItr = definingPredicates.begin();
					PredicateInstanceList::iterator currEnd = definingPredicates.end();
					
					if(problemInformation.verbose) cout << "Entity " << (*currEntity) << " has " << definingPredicates.size() << " defining predicates" << endl;

					//for each of the current entity's defining predicates

					for(; inGroup && currItr != currEnd; ++currItr){

						//check if it is in the list for this symmetry group
						PredicateInstanceList matchList = (*groupItr)->getDefiningPredicates();
						PredicateInstanceList::iterator matchItr = matchList.begin();
						PredicateInstanceList::iterator matchEnd = matchList.end();

						bool found = false;

						for(;!found && matchItr != matchEnd; ++matchItr){

							if((*(*currItr)).equalsWithout((*(*matchItr)), currEntity, potentialMatch )){
								found = true;
								if(problemInformation.verbose) cout << "*****Entities " << (*currEntity)  << " and " << (*((*groupItr)->getEntities().front())) << " share the initial state predicate " << (*(*currItr)) << " ignoring the entity " << (*currEntity) << endl;
							}

						}

						if(!found) inGroup = false;
						if(!found && problemInformation.verbose) cout << "Entities " << (*currEntity)  << " and " << (*((*groupItr)->getEntities().front())) << " are not symmetric; they do not share the predicate " << (*(*currItr)) << endl;

					}

					if(inGroup && considerGoalState){

						currItr = definingGoalPredicates.begin();
						currEnd = definingGoalPredicates.end();
						for(; inGroup && currItr != currEnd; ++currItr){

							//check if it is in the list for this symmetry group

							PredicateInstanceList matchGoalList = (*groupItr)->getDefiningGoalPredicates();
							PredicateInstanceList::iterator matchGoalItr = matchGoalList.begin();
							PredicateInstanceList::iterator matchGoalEnd = matchGoalList.end();
							
							bool found = false;

							for(;!found && matchGoalItr != matchGoalEnd; ++matchGoalItr){

								if((*(*currItr)).equalsWithout((*(*matchGoalItr)), currEntity, potentialMatch ) ){
									found = true;
								}

							}

							if(!found) inGroup = false;
							if(!found && problemInformation.verbose) cout << "Entities " << (*currEntity)  << " and " << (*((*groupItr)->getEntities().front())) << " are not symmetric; they do not share the goal predicate " <<(*(*currItr)) << endl;
						}

					}

					if(inGroup){

						(*groupItr)->addEntity(currEntity);
						added = true;
					}
				}

			}

		}
		//if we've not found a matching group we need to create a new one
		if(!added){
			SymmetryGroup* newGroup = new SymmetryGroup();
			newGroup->addEntity(currEntity);
			symmetryGroups.push_back(newGroup);
			newGroup->setDefiningPredicates(definingPredicates);
			if(considerGoalState) newGroup->setDefiningGoalPredicates(definingGoalPredicates);
		}

	}

	if (problemInformation.Asymmetric != 0) {
		Entity** copyFrom = problemInformation.Asymmetric;
		const int thisLoopLimit = problemInformation.numberOfAsymmetricEntities;
		
		for (int i = 0; i < thisLoopLimit; ++i) {
			SymmetryGroup* newGroup = new SymmetryGroup(copyFrom[i]);
			symmetryGroups.push_back(newGroup);
		}
	}
	
	return &symmetryGroups;

};


list<SymmetryGroup*> SymmetryExtractor::almostSymmetryDifferenceOnePredicate(StaticInformation & problemInformation){

	bool considerGoalState = true;
	//for all entities
	list< PredicateInstanceList > definingGroupPredicates;
	list< PredicateInstanceList > definingGroupGoalPredicates;
	almostSymmetryGroups.clear();
	bool relatedGroup = false;
	PredicateInstanceList differingPredicates;
	PredicateInstanceList differingGoalPredicates;
	list<SymmetryGroup*> potentialPartialMatches;
//	problemInformation.verbose = false;
	//bool addedToRelatedGroup = false;
	list<SymmetryGroup*> trueSymmetryGroups(*extractSymmetryGroups(problemInformation, true));
	
	list<SymmetryGroup*>::iterator tsItr = trueSymmetryGroups.begin();
	const list<SymmetryGroup*>::iterator tsEnd = trueSymmetryGroups.end();
	
	for (; tsItr != tsEnd; ++tsItr) {
	
		Entity* currEntity = (*tsItr)->getExemplar();
		
		if (currEntity->isAsymmetric()) { // we have been fobbed off with a dummy group
			PredicateInstanceList definingPredicates;
			
			State* startState = problemInformation.InitialState;
			
			State::iterator pItr = startState->begin();
			State::iterator pEnd = startState->end();
			//loop over the initial state
			for(;pItr != pEnd; ++pItr){
	
				if (!startState->isToDelete(*pItr)) {
					if(problemInformation.verbose) cout << "checking whether " << (*currEntity) << " is an argument of " << (*(*pItr)) << endl;
		
					//if this predicate pertains to the entity in question
					bool found = false;
					int llt = (*pItr)->getParentPredicate()->getArgumentCount();
					for(int paramIndex = 0; !found &&paramIndex < llt; ++paramIndex){
		
						if(currEntity == ((*pItr)->getParameter(paramIndex))->boundTo()){
							definingPredicates.push_back((*pItr));
							found = true;
							if(problemInformation.verbose) cout << "...............it is" << endl;
						}
		
					}
				}
	
			}
	
			pItr = problemInformation.StaticInitialState->begin();
			pEnd = problemInformation.StaticInitialState->end();
			//loop over the initial state
			for(;pItr != pEnd; ++pItr){
	
				if(problemInformation.verbose) cout << "checking whether " << (*currEntity) << " is an argument of static precondition " << (*(*pItr)) << endl;
	
				bool found = false;
				//if this predicate pertains to the entity in question
				int llt = (*pItr)->getParentPredicate()->getArgumentCount();
				for(int paramIndex = 0; !found && paramIndex < llt; ++paramIndex){
	
					if(currEntity == ((*pItr)->getParameter(paramIndex))->boundTo()){
						definingPredicates.push_back((*pItr));
						found = true;
						if(problemInformation.verbose) cout << "...............it is" << endl;
					}
	
				}
	
			}
	
			PredicateInstanceList definingGoalPredicates;
	
			if(considerGoalState){
	
	
				pItr = problemInformation.GoalPredicates->begin();
				pEnd = problemInformation.GoalPredicates->end();
				//loop over the initial state
				for(;pItr != pEnd; ++pItr){
					//if this predicate pertains to the entity in question
					bool found = false;
					int llt = (*pItr)->getParentPredicate()->getArgumentCount();
					for(int paramIndex = 0; !found && paramIndex < llt ; ++paramIndex){
	
						if(currEntity == ((*pItr)->getParameter(paramIndex))->boundTo()){
							found = true;
							definingGoalPredicates.push_back((*pItr));
						}
	
					}
	
				}
			}
			
			(*tsItr)->setDefiningPredicates(definingPredicates);
		}
	
	}
	
//	problemInformation.verbose = false;
	Entity** entities = new Entity*[trueSymmetryGroups.size()];
	SymmetryGroup** realGroups = new SymmetryGroup*[trueSymmetryGroups.size()];
	list<SymmetryGroup*>::iterator itr = trueSymmetryGroups.begin();
	int numberOfEntities = trueSymmetryGroups.size();

	for(int i = 0; i < numberOfEntities; i++, itr++){
		entities[i] = (*itr)->getEntities().front();
		realGroups[i] = (*itr);
	}
	if(problemInformation.verbose) cout << "number of entities = " << numberOfEntities << endl;
	for(int eIndex= 0; eIndex < numberOfEntities; eIndex++){
		relatedGroup = false;
		//addedToRelatedGroup = false;

		if(problemInformation.verbose) cout << "beginning calculations for next entity" << endl;
		if(problemInformation.verbose){
			cout << "----------------------------------------------------------------" << endl;
			cout << "Considering Entity " << eIndex << " of " << problemInformation.numberOfEntities << endl;
		}

		differingPredicates.clear();
		differingGoalPredicates.clear();
		potentialPartialMatches.clear();

		Entity* currEntity = entities[eIndex];

		if(problemInformation.verbose) cout << "current entity is " << (*currEntity) << endl;

		PredicateInstanceList definingPredicates = realGroups[eIndex]->getDefiningPredicates();
		PredicateInstanceList definingGoalPredicates = realGroups[eIndex]->getDefiningGoalPredicates();
		bool added = false;

		if(problemInformation.verbose){
			cout << (*currEntity) << " has defining predicates ";
			PredicateInstanceList::iterator itr = definingPredicates.begin();
			for(; itr != definingPredicates.end(); itr++){
				cout << " (" << (*(*itr)) << ")";
			}
			cout << endl<< "and defining goal predicates ";

			itr = definingGoalPredicates.begin();
			for(; itr != definingGoalPredicates.end(); itr++){
				cout << " (" << (*(*itr)) << ")";
			}
			cout << endl;
		}


		list<SymmetryGroup *>::iterator groupItr = almostSymmetryGroups.begin();
		int groupItrCount = -1;

		for(; groupItr != almostSymmetryGroups.end(); groupItr++){
			
			Entity* potentialMatch = (*groupItr)->getEntities().front();
			groupItrCount++;

			//if the entity is of the same type as those in this group
			if(currEntity->getEntityType() == potentialMatch->getEntityType()){

				bool check = true;
				//if they don't have the same number of defining predicates they are not symmetric
				if((*groupItr)->getDefiningPredicates().size() != definingPredicates.size()) check = false;
				if(considerGoalState){
					if((*groupItr)->getDefiningGoalPredicates().size() != definingGoalPredicates.size()) check = false;
				}

				if(problemInformation.verbose && !check) cout << "found entity of the same type ," << (*potentialMatch) << ",but number of predicates doesn't match" << endl;

				if(check){

					bool inGroup = true;
					relatedGroup = false;
					if(problemInformation.verbose) cout << "Set related group to false" << endl;
					if(problemInformation.verbose) cout << "found existing entity of same type; checking predicates..." << endl;

					if(problemInformation.verbose) {
						cout << "comparing to " << (*potentialMatch) << " with ";
						(*groupItr)->printDefiningPredicates(cout);
					}

					PredicateInstanceList::iterator currItr = definingPredicates.begin();

					if(problemInformation.verbose) cout << "Entity " << (*currEntity) << " has " << definingPredicates.size() << " defining predicates" << endl;
					bool consideringMismatches = false;
					//for each of the current entity's defining predicates
					

					int mismatches = 0;
					
					bool abort = false;
					PredicateInstance* mismatchingPredicate = 0;
					
					for(; !abort && currItr != definingPredicates.end(); currItr++){

						//check if it is in the list for this symmetry group
						PredicateInstanceList matchList = (*groupItr)->getDefiningPredicates();
						PredicateInstanceList::iterator matchItr = matchList.begin();

						
						
						
						bool found = false;
						int localmismatches = INT_MAX;
						int count = 0;
						
						for(;!found && matchItr != matchList.end(); matchItr++){
							count ++;
							if(problemInformation.verbose) cout << "about to check initial state predicate number " << count << " of " << (*groupItr)->getDefiningPredicates().size() << endl;

							if(problemInformation.verbose && (*matchItr) != 0){
								cout << "checking defining predicate (" << (*(*matchItr)) << ") of " << (*potentialMatch) << endl;
							}


							if((*(*currItr)).equalsWithout((*(*matchItr)), currEntity, potentialMatch )){
								found = true;
								localmismatches = 0;
								if(problemInformation.verbose) cout << "*****Entities " << (*currEntity)  << " and " << (*potentialMatch) << " share the initial state predicate " << (*(*currItr)) << " ignoring the entity " << (*currEntity) << endl;
							} else  {
								//if we've found a non-matching predicate but the types are the same
								if((*currItr)->getParentPredicate() == (*matchItr)->getParentPredicate() ){
									int olm = localmismatches;
									localmismatches = (*(*currItr)).equalsWithoutPlusOne((*(*matchItr)), currEntity, potentialMatch );
									if (olm < localmismatches) {
										localmismatches = olm;
									} else {
										mismatchingPredicate = (*matchItr);
									}

								}

							}


						}
						abort = (localmismatches == INT_MAX);
						//if ( (localmismatches == INT_MAX) ) cout << "Setting abort to true; too many mismatches found" << endl;
						mismatches += localmismatches;
						if(!found){
							inGroup = false;
							if(problemInformation.verbose) cout << "Entities " << (*currEntity)  << " and " << (*potentialMatch) << " are not symmetric; they do not share the predicate " << (*(*currItr)) << endl;
						}
					

						
					}
					if(problemInformation.verbose) cout << "abort = " << abort << endl;
					if(!abort && mismatches == 1){
						if(problemInformation.verbose) cout << "one mismatch found in initial state, it is: " << (*mismatchingPredicate) << endl;
						
						differingPredicates.push_back(mismatchingPredicate);
						potentialPartialMatches.push_back(*groupItr);
						consideringMismatches = true;
						relatedGroup = true;
						

					}else{
						if(problemInformation.verbose) cout << "not doing the if statement that sets related group to be true" << endl << "mismatches = " << mismatches << " abort = " << abort << endl;
						
					}
					
					if(problemInformation.verbose && mismatches > 1) cout << "more than one mismatch found in initial state last one found is: " << (*mismatchingPredicate) << endl;

					
					

					if(!abort && considerGoalState){

						currItr = definingGoalPredicates.begin();
						int mismatches = 0;
						abort = false;
						PredicateInstance* mismatchingPredicate = 0;
						bool inGroupWas = inGroup;
						for(; !abort && currItr != definingGoalPredicates.end(); currItr++){

							//check if it is in the list for this symmetry group
							if(problemInformation.verbose) cout << "checking next goal predicate for potential match" << endl;
							PredicateInstanceList groupGoalPredicates = (*groupItr)->getDefiningGoalPredicates();
							PredicateInstanceList::iterator matchGoalItr = groupGoalPredicates.begin();

							bool found = false;
							int localmismatches = INT_MAX;

							if(problemInformation.verbose) cout << "There are " << groupGoalPredicates.size() << " defining goal predicates for " << (*(*groupItr)) << endl;
							for(;!found && matchGoalItr != groupGoalPredicates.end(); matchGoalItr++){

								if(problemInformation.verbose) {
									cout << "location of goal predicate on list is " << (*matchGoalItr) << endl;
									if((*matchGoalItr) != 0 ) cout << "checking goal predicate (" << (*(*matchGoalItr)) << ") of " << (*currEntity) << endl;
								}



								if((*(*currItr)).equalsWithout((*(*matchGoalItr)), currEntity, potentialMatch ) ){
									found = true;
									localmismatches = 0;
									
								} else {
								//if we've found a non-matching predicate but the types are the same
									if((*currItr)->getParentPredicate() == (*matchGoalItr)->getParentPredicate() ){
										int olm = localmismatches;
										localmismatches = (*(*currItr)).equalsWithoutPlusOne((*(*matchGoalItr)), currEntity, potentialMatch );
										if (olm < localmismatches) {
											localmismatches = olm;
										} else {
											mismatchingPredicate = (*matchGoalItr);
										}
	
									}

								}

							}
							abort = (localmismatches == INT_MAX);
								
							mismatches += localmismatches;
							//inGroupWas = inGroup;
							if(!found){
								inGroup = false;
								if(problemInformation.verbose) cout << "Entities " << (*currEntity)  << " and " << (*potentialMatch) << " are not symmetric; they do not share the predicate " << (*(*currItr)) << endl;
							}
							
							
						}
						
						if(problemInformation.verbose) cout << "mismatches = " << mismatches << endl;
						
						if (!abort) {
						
							bool groupWas = relatedGroup;
							relatedGroup = relatedGroup && (mismatches <= 1) || (inGroupWas && (mismatches == 1));
							if(problemInformation.verbose) cout << "setting relatedGroup to " << relatedGroup << " it was " << groupWas << " in group was " << inGroup << endl;
							if(problemInformation.verbose) cout << "about to check related groups" << endl;

							if(relatedGroup){

								differingGoalPredicates.push_back(mismatchingPredicate);
								if(!consideringMismatches){
									differingPredicates.push_back(0);
									potentialPartialMatches.push_back((*groupItr));

								}

								consideringMismatches = false;
							}

						} else {
							relatedGroup = false;
							inGroup = false;
							if(problemInformation.verbose) cout << "Running aborted code " << endl;
						}

						
						if(consideringMismatches) differingGoalPredicates.push_back(0);
						
						//if(definingGoalPredicates.empty() && consideringMismatches) differingGoalPredicates.push_back(0);

						if(problemInformation.verbose) cout << "exited loop" << endl;
					}

					if(problemInformation.verbose) cout << "in group is " << inGroup << endl;

					if(inGroup){
						cout << "I think that two non-symmetric things are symmetric, culprits are: " << (*currEntity) << " and " << (*potentialMatch) << endl;
						if(problemInformation.verbose) cout << "adding entity to existing group " << (*groupItr) << endl;
						//(*groupItr)->addEntity(currEntity);
						//added = true;
					}
				}

			} else {

				if(problemInformation.verbose){
					cout << (*currEntity) << " is not of the same type as " << (*potentialMatch) << endl;
				}

			}

		}
		//if we've not found a matching group we need to create a new one
		if(problemInformation.verbose) cout << "added = " << added << endl;
		if(!added){
			if(problemInformation.verbose){
			 	cout <<"not added the item yet" << endl;
				cout << "related group is " << relatedGroup << endl;
				cout << potentialPartialMatches.size() << "potential related groups" << endl;

			}

			if(relatedGroup){

				if(problemInformation.verbose) cout << "checking potential related group" << endl;
				list<SymmetryGroup*>:: iterator itr = potentialPartialMatches.begin();
				PredicateInstanceList::iterator iItr = differingPredicates.begin();
				PredicateInstanceList::iterator gItr = differingGoalPredicates.begin();



				if(problemInformation.verbose){

					cout << "checking " << potentialPartialMatches.size() << " potential matches" << endl;
					cout << differingPredicates.size() << " differing predicates" << endl;
					cout << "and " << differingGoalPredicates.size() << " differing goal predicates" << endl;
				}

				if(problemInformation.verbose){

					for(; itr != potentialPartialMatches.end(); itr++, iItr++, gItr++){
						cout << "potential matches " << endl;

						if(*iItr != 0 && *gItr !=0) cout << "group: " << *((*itr)->getEntities().front()) << " initial state difference: " << (*(*iItr)) << " goal state difference: " << (*(*gItr)) << endl;
						if(*iItr != 0 && *gItr ==0) cout << "group: " << *((*itr)->getEntities().front()) << " initial state difference: " << (*(*iItr)) << " same in goal state" << endl;
						if(*iItr == 0 && *gItr !=0) cout << "group: " << *((*itr)->getEntities().front()) << " no difference in initial state goal state difference: " << (*(*gItr)) << endl;
						if(*iItr == 0 && *gItr ==0) cout << "WARNING: adding group which has the same initial and goal predicates as related group" << endl;
					}
					itr = potentialPartialMatches.begin();
					iItr = differingPredicates.begin();
					gItr = differingGoalPredicates.begin();

				}

				//for each related group that we find we need to check that it's parent isn't on the list...
				list<SymmetryGroup*> newMatches;
				PredicateInstanceList newInitial;
				PredicateInstanceList newGoal;

				if(problemInformation.verbose) cout << "size of potential matches list is now " << potentialPartialMatches.size() << endl << "iterator at beginning of list = " << (itr == potentialPartialMatches.begin()) << endl << "iterator at end of list = " << (itr == potentialPartialMatches.end()) << endl;


				for(; itr != potentialPartialMatches.end(); itr++, iItr++, gItr++){
						if(problemInformation.verbose) cout << "checking match " << endl;
						if((*itr)->getParentGroup() != 0){
							list<SymmetryGroup*>::iterator chitr = potentialPartialMatches.begin();
							bool remove = false;
							for(; chitr != potentialPartialMatches.end(); chitr ++){
								if((*chitr) == ((*itr)->getParentGroup())){
									remove = true;
									if(problemInformation.verbose) cout << "parent is in list" << endl;
								}
							}

							if(!remove){
								newMatches.push_back(*itr);
								newInitial.push_back(*iItr);
								newGoal.push_back(*gItr);
							}

						}else{
							newMatches.push_back(*itr);
							newInitial.push_back(*iItr);
							newGoal.push_back(*gItr);
						}

				}



				potentialPartialMatches = newMatches;
				differingPredicates = newInitial;
				differingGoalPredicates = newGoal;

				itr = potentialPartialMatches.begin();
				iItr = differingPredicates.begin();
				gItr = differingGoalPredicates.begin();

//				int printCount = 1;

				if(problemInformation.verbose) cout << "size of potential matches list is now " << potentialPartialMatches.size() << endl;


				if(problemInformation.verbose) cout << "this item has not yet been insterted" << endl;

				if(!added){

					almostSymmetryGroups.push_back(realGroups[eIndex]);
					(*itr)->addRelatedGroup(realGroups[eIndex], (*iItr), (*gItr));
					realGroups[eIndex]->setParentGroup((*itr));

					added = true;
				}else{

					(*itr)->addRelatedGroup(realGroups[eIndex], (*iItr), (*gItr));

				}


			}else{

				almostSymmetryGroups.push_back(realGroups[eIndex]);
				realGroups[eIndex]->setParentGroup(0);
				added = true;
			}

		}

	}

	delete[] entities;
	delete[] realGroups;

	return almostSymmetryGroups;
}

void SymmetryExtractor::write(ostream & o) const{

	o << "Symmetry Groups " << endl;

	list<SymmetryGroup*>::const_iterator outerItr = symmetryGroups.begin();

	int i = 0;
	for(; outerItr != symmetryGroups.end(); outerItr++){
		o << "Symmetry Group " << i << ":" << endl;
		o << (*outerItr) << endl;

	}

};

list<SymmetryGroup*> SymmetryExtractor::symmetricWithRespectToAction(Action* actionToUse, StaticInformation & problemInformation, const State * currState){

	actionSymmetryGroups.clear();

	hash_map<const Predicate*, bool> predicatesToCheck;

	for(int i = 0 ; i < actionToUse->preconditionCount; i ++){

		predicatesToCheck[actionToUse->preconditions[i].predType] = true;
	}

//	bool was = problemInformation.verbose;
//	problemInformation.verbose = false;
	for(int eIndex= 0; eIndex < problemInformation.numberOfEntities; eIndex++){


		Entity* currEntity = problemInformation.Entities[eIndex];
		bool seen = false;
		Type* typeToUse = 0;

		for(int i = 0; i < actionToUse->parameterCount; i++){
			if((currEntity->getEntityType() == actionToUse->parameterTypes[i]) || currEntity->getEntityType()->isASubTypeOf(actionToUse->parameterTypes[i])){
				if(!seen){
					typeToUse = currEntity->getEntityType();
					seen = true;
				}

				if(typeToUse->isASubTypeOf(actionToUse->parameterTypes[i])) typeToUse = actionToUse->parameterTypes[i];

			} else if(problemInformation.verbose) cout << (*currEntity) << " is not the same type as parameter " << i << " of type " << *(actionToUse->parameterTypes[i]) << endl;
		}

		if(problemInformation.verbose && typeToUse !=0 ) cout << (*currEntity) << " abstracted to parameter type " << (*typeToUse) << endl;

		if(seen){

			PredicateInstanceList definingPredicates;
			bool added = false;

			State::const_iterator pItr = currState->begin();

			for(;pItr != currState->end(); pItr++){

				if(predicatesToCheck.find((*pItr)->getParentPredicate()) != predicatesToCheck.end()){

					for(int paramIndex = 0; paramIndex < (*pItr)->getParentPredicate()->getArgumentCount(); paramIndex++){
						if(problemInformation.verbose) cout << (*(*pItr)) << " is a potential precondition of " << actionToUse->name() << endl;
						//if(actionToUse->Preconditions[i]->Parameters[i]->getEntityType()
						if(currEntity == ((*pItr)->getParameter(paramIndex))->boundTo()){
							definingPredicates.push_back((*pItr));
						}

					}

				}

			}

			pItr = problemInformation.StaticInitialState->begin();

			for(;pItr != problemInformation.StaticInitialState->end(); pItr++){

				if(predicatesToCheck.find((*pItr)->getParentPredicate()) != predicatesToCheck.end()){//if this predicate pertains to the action in question

					if(problemInformation.verbose) cout << (*(*pItr)) << " is a potential precondition of " << (*actionToUse) << endl;

					for(int paramIndex = 0; paramIndex < (*pItr)->getParentPredicate()->getArgumentCount(); paramIndex++){

						if(currEntity == ((*pItr)->getParameter(paramIndex))->boundTo()){
							definingPredicates.push_back((*pItr));
						}

					}
				}
			}

			list<SymmetryGroup*>::iterator groupItr = actionSymmetryGroups.begin();

			for(; groupItr != actionSymmetryGroups.end(); groupItr++){

				Entity* potentialMatch = (*groupItr)->getEntities().front();

				//if(currEntity->getEntityType() == potentialMatch->getEntityType()){
				if(typeToUse == (*groupItr)->getType()){

					PredicateInstanceList toCheck = (*groupItr)->getDefiningPredicates();

					if(toCheck.size() == definingPredicates.size()){


						PredicateInstanceList::iterator checkItr = toCheck.begin();

						bool inGroup = true;

						for(; checkItr != toCheck.end(); checkItr++){


							bool found = false;

							PredicateInstanceList::iterator currItr = definingPredicates.begin();

							for(; !found && currItr != definingPredicates.end(); currItr++){

								if((*(*currItr)).equalsWithout((*(*checkItr)), currEntity, potentialMatch )){

									found = true;

								}
							}

							if(!found) inGroup = false;

						}

						if(inGroup){

							(*groupItr)->addEntity(currEntity);
							added = true;
						}
					}

				}

			}

			//if we've not found a matching group we need to create a new one
			if(!added){
				SymmetryGroup* newGroup = new SymmetryGroup();
				newGroup->addEntity(currEntity);
				actionSymmetryGroups.push_back(newGroup);
				newGroup->setType(typeToUse);
				newGroup->setDefiningPredicates(definingPredicates);
			}
		}

	}
//	problemInformation.verbose = was;
	return actionSymmetryGroups;
};

void SymmetryExtractor::writeAlmostSymmetry(ostream & o) const{

	o << "Almost-Symmetry Groups" << endl;

	list<SymmetryGroup*>::const_iterator groupItr = almostSymmetryGroups.begin();

	for(; groupItr != almostSymmetryGroups.end(); groupItr++){

		o << (*(*groupItr)) << endl;

	}

};

ostream & operator <<(ostream & o, const SymmetryExtractor & s) {
	s.writeAlmostSymmetry(o);
	return o;
}
