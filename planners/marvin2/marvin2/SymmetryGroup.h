#ifndef SYMMETRYGROUP_H
#define SYMMETRYGROUP_H

#include "PredicateInstance.h"
#include <list>


class SymmetryGroup{

private:

	PredicateInstanceList definingPredicates;
	PredicateInstanceList definingGoalPredicates;
	list<SymmetryGroup*> relatedGroups;
	PredicateInstanceList differingRelatedGroupPredicates;
	PredicateInstanceList differingRelatedGroupGoalPredicates;
	list<Entity*> groupMembers;
	Type* typeToUse;
	SymmetryGroup* parentGroup;


public:

	//SymmetryGroup(PredicateInstanceList & defining, list <PredicateInstance*> definingGoals);

	SymmetryGroup():typeToUse(0), parentGroup(0){};

	SymmetryGroup(	const PredicateInstanceList & defining,
					const PredicateInstanceList & definingGoal,
					const list<Entity*> & entities,
					Type* groupType)
					:	definingPredicates(defining),
						definingGoalPredicates(definingGoal),
						groupMembers(entities),
						typeToUse(groupType), parentGroup(0){};

	SymmetryGroup(	Entity* theEntity ) : parentGroup(0) {
		groupMembers.push_back(theEntity);
		typeToUse = theEntity->getEntityType();
	};

	PredicateInstanceList & getDefiningPredicates(){
		return definingPredicates;
	}

	PredicateInstanceList & getDefiningGoalPredicates(){
		return definingGoalPredicates;
	}

	list<SymmetryGroup*> & getRelatedGroups(){
		return relatedGroups;
	}

	const list<SymmetryGroup*> & getRelatedGroups() const {
		return relatedGroups;
	}

	SymmetryGroup* getParentGroup() const {
		return parentGroup;
	}

	void setParentGroup(SymmetryGroup* parent){

		parentGroup = parent;

	}

	Entity* getExemplar() const {
		return groupMembers.front();
	}



	PredicateInstanceList & getDifferingPredicates(){
		return differingRelatedGroupPredicates;
	}

	PredicateInstanceList & getDifferingGoalPredicates(){
		return differingRelatedGroupGoalPredicates;
	}


	list<Entity*> & getEntities(){
		return groupMembers;
	}

	const list<Entity*> & getEntities() const{
		return groupMembers;
	}

	void addEntity(Entity* toAdd){
		groupMembers.push_back(toAdd);
		if(typeToUse == 0) typeToUse = toAdd->getEntityType();
	}

	Type* getType(){
		return typeToUse;
	}

	void setType (Type* newType){
		typeToUse = newType;
	}

	void setDefiningPredicates(PredicateInstanceList defining){

		definingPredicates.clear();

		PredicateInstanceList::iterator itr = defining.begin();
		for(; itr != defining.end(); itr++){
			if((*itr) != 0) definingPredicates.push_back((*itr));
		}
	}

	void setDefiningGoalPredicates(PredicateInstanceList defining){

		definingGoalPredicates.clear();

		PredicateInstanceList::iterator itr = defining.begin();
		for(; itr != defining.end(); itr++){
			//cout << "pushing " << (*(*itr)) <<  " onto defining goal predicates" << endl;
			if((*itr) != 0) definingGoalPredicates.push_back((*itr));
		}


	}

	PredicateInstance* difference(SymmetryGroup * toCompare){
		list<SymmetryGroup*>::iterator itr = relatedGroups.begin();
		bool found = false;
		int count = -1;
		for(; itr != relatedGroups.end() && !found; count++, itr++){
			if((*itr) == toCompare) found = true;
		}
		PredicateInstanceList::iterator pItr = differingRelatedGroupPredicates.begin();
		int i = 0;
		while(i < count){
			i++;
			pItr++;
		}

		return (*pItr);

	}

	bool isExemplarGroup(){
		return (relatedGroups.size() > 0);
	}

	void addRelatedGroup(SymmetryGroup * toAdd, PredicateInstance * relatedGroupDifference, PredicateInstance * relatedGroupGoalDifference){
		//here if the differing predicate from us to our parent is the same as that from this to us
		//we should add it as a related group for our parent too.

		relatedGroups.push_back(toAdd);
		differingRelatedGroupPredicates.push_back(relatedGroupDifference);
		differingRelatedGroupGoalPredicates.push_back(relatedGroupGoalDifference);

		PredicateInstanceList::iterator itr = differingRelatedGroupPredicates.begin();
		PredicateInstanceList::iterator gitr = differingRelatedGroupGoalPredicates.begin();
		list<SymmetryGroup*>::iterator sitr = relatedGroups.begin();

		if(parentGroup == 0){
			if(relatedGroupDifference == 0 && relatedGroupGoalDifference == 0){
				cout << "attempting to add related group with no difference" << endl;
				return;
			}

			if(relatedGroupDifference == 0){

				for(; itr != differingRelatedGroupPredicates.end(); itr++, gitr++, sitr++){

					if((*itr) == 0 && (*gitr)->equalsWithout(relatedGroupGoalDifference, groupMembers.front(), toAdd->getEntities().front())){

						(*sitr)->addRelatedGroup(toAdd, relatedGroupDifference, relatedGroupGoalDifference);

					}

				}

			} else if(relatedGroupGoalDifference == 0){

				for(; itr != differingRelatedGroupPredicates.end(); itr++, gitr++, sitr++){

					if( (*gitr) == 0 && (*itr)->equalsWithout(relatedGroupDifference, groupMembers.front(), toAdd->getEntities().front())){

						(*sitr)->addRelatedGroup(toAdd, relatedGroupDifference, relatedGroupGoalDifference);

					}

				}
			} else {

				for(; itr != differingRelatedGroupPredicates.end(); itr++, gitr++, sitr++){

					if(	( ((*itr) == 0)  || (*itr)->equalsWithout(relatedGroupDifference, groupMembers.front(), toAdd->getEntities().front()))
					
					     &&	(((*gitr) == 0) || (*gitr)->equalsWithout(relatedGroupGoalDifference, groupMembers.front(), toAdd->getEntities().front())) ){

						(*sitr)->addRelatedGroup(toAdd, relatedGroupDifference, relatedGroupGoalDifference);

					}

				}
			}
		}

	}

	void addDefiningPredicate(PredicateInstance * toAdd){
		definingPredicates.push_back(toAdd);
	}
	void writeGroupMembers(ostream & o) const{

		list<Entity*>::const_iterator itr = groupMembers.begin();
		o << "related group: ";
		for(; itr != groupMembers.end(); itr++){
			o << (*(*itr));
		}

	}
	void printDefiningPredicates(ostream & o) const{

		PredicateInstanceList::const_iterator itr = definingPredicates.begin();
		o << "defining predicates are: ";
		for(; itr != definingPredicates.end(); itr++){
			o << "(" << (*(*itr)) << ") ";
		}

	}

	void printDefiningGoalPredicates(ostream & o) const{

		PredicateInstanceList::const_iterator itr = definingGoalPredicates.begin();
		o << "defining goal predicates are: ";
		for(; itr != definingGoalPredicates.end(); itr++){
			o << "(" << (*(*itr)) << ") ";
		}

	}

	void writeActionSymmetries(ostream & o) const{
		o << endl <<endl << "Entities in this group are of type " << (*typeToUse) << endl;
		o << endl << "Entities in this group are:" << endl;

		list<Entity*>::const_iterator itr = groupMembers.begin();

		for(; itr != groupMembers.end(); itr++){
			o << (*(*itr)) << endl;
		}

		printDefiningPredicates(o);


	}

	void write(ostream & o) const{

		if(typeToUse != 0) o << endl << endl << "Entities in this group are of type " << (typeToUse->name()) << endl;
		o << "Entities in this group are:" << endl;

		list<Entity*>::const_iterator itr = groupMembers.begin();

		for(; itr != groupMembers.end(); itr++){
			o << (*(*itr)) << endl;
		}



		PredicateInstanceList::const_iterator pItr = differingRelatedGroupPredicates.begin();
		PredicateInstanceList::const_iterator gItr = differingRelatedGroupGoalPredicates.begin();
		list<SymmetryGroup*>::const_iterator sItr = relatedGroups.begin();

		if(relatedGroups.size() != 0){
			o << "Related groups exist with differing predicates " << endl;
			for(; sItr != relatedGroups.end(); sItr++){
				(*sItr)->writeGroupMembers(o);
			}
			cout << " ";
			for(; pItr != differingRelatedGroupPredicates.end(); pItr++){

				if((*pItr) != 0){
					o << "Difference in initial state is " << (*(*pItr)) << endl;
				} else {
					o << "Same in initial state" << endl;
				}
			}


			for(; gItr != differingRelatedGroupGoalPredicates.end(); gItr++){
				if((*gItr) != 0){
					o << "Difference in goal state is " << (*(*gItr)) << endl;
				} else {
					o << "Same in goal state" << endl;
				}
			}

		}
	};


};

ostream & operator <<(ostream & o, const SymmetryGroup & s);

#endif
