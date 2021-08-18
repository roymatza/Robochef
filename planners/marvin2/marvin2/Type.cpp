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
#include "Type.h"
#include "Entity.h"
#include "Action.h"
#include <algorithm>

using std::sort;

void Type::write(ostream & o) const {

	o << name();

}

void Type::detailedWrite(ostream & o) const {



	o << name() << ":";


	if (subTypeCount != 0) {
		if (subTypeCount == 1) {
			o << "\tRegistered subtype:";
		} else {
			o << "\tRegistered subtypes:";
		}
		for (int i = 0; i < allocatedSubTypeCount; i++) {
			o << " " << subTypes[i]->name();
		}
		if (allocatedSubTypeCount < subTypeCount) {

			o << " (+" << (subTypeCount - allocatedSubTypeCount) << " unregistered)";
		}

		o << endl;
	}


	if (instanceCount != 0) {
		if (instanceCount == 1) {
			o << "\tEntity of this type:";
		} else {
			o << "\tEntities of this type:"; 
		} 
		for (int i = 0; i < allocatedInstanceCount; i++) {
			Entity* curr = instances[i];
			const string n = curr->name();
			o << " " << n;
		}
		
		if (allocatedInstanceCount < instanceCount) {
		
			o << " (+" << (instanceCount - allocatedInstanceCount) << " unregistered)";
		}
		o << endl;
		
	}

};

ostream & operator <<(ostream & o, const Type & a) {
	a.write(o);
	return o;
};

void Type::sortInstancePointers() {

	sort(instances, instances + allocatedInstanceCount);
	
}

void Type::makeEntitySet() {
	entitySet = new	EntitySet();
	
	for (int i = 0; i < instanceCount; i++) {
		entitySet->insert(instances[i]);
	}
}

void Type::trimEntitySet(const EntitySet * trimTo) {

	if (wholeEntitySet != 0) delete wholeEntitySet;
	wholeEntitySet = entitySet;
	
/*	cout << "Entity set for type " << name() << " was: " << endl;
	for (EntitySet::iterator esi = entitySet->begin(); esi != entitySet->end(); esi++) {
		cout << " " << (*esi)->name();
		
	}
	
	cout << endl;
*/	
	entitySet = Action::intersection(wholeEntitySet, trimTo);
	
/*	cout << "Entity set for type " << name() << " trimmed to: " << endl;
	
	for (EntitySet::iterator esi = entitySet->begin(); esi != entitySet->end(); esi++) {
		cout << " " << (*esi)->name();
		
	}
	cout << endl;
*/
}

void Type::resetEntitySet() {
	if (wholeEntitySet == 0) return;
	if (entitySet != 0) delete entitySet;
	entitySet = wholeEntitySet;
	wholeEntitySet = 0;
	/*cout << "Entity set for type " << name() << " reset to: " << endl;
	
	for (EntitySet::iterator esi = entitySet->begin(); esi != entitySet->end(); esi++) {
		cout << " " << (*esi)->name();
		
	}
	cout << endl;*/
}

Type::~Type() {
	if (subTypes != 0) delete [] subTypes;
	if (instances != 0) delete [] instances;
	if (entitySet != 0) delete entitySet;
	if (wholeEntitySet != 0) delete wholeEntitySet;
	
}
