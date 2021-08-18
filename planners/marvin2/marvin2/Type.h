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
#ifndef TYPE_H
#define TYPE_H

#include "ptree.h"
#include <ext/hash_map>

class Type;
class Entity;



#include "hashdefinitions.h"


/**
@author Andrew Ian Coles, Amanda Smith
*/
class Type{

private:

	pddl_type* originalTypePointer;
	
	Type* parentType;
	Type** subTypes;
	hash_map<Type*, bool> subTypeHash;
	
	int subTypeCount;
	int allocatedSubTypeCount;
	
	Entity** instances;
	EntitySet* entitySet;
	EntitySet* wholeEntitySet;
	
	
	int instanceCount;
	int allocatedInstanceCount;
	int typeID;
	
public:
    
    	Type(pddl_type* & t) : 	originalTypePointer(t),
				parentType(0),
				subTypes(0),
				subTypeCount(0),
				allocatedSubTypeCount(0),
				instances(0),
				entitySet(0),
				wholeEntitySet(0),
				instanceCount(0),
				allocatedInstanceCount(0) {};

    	~Type();
    
    	const string name() const {
		if (originalTypePointer != 0) {
			return (originalTypePointer->getName());
		} else {
			return "Object";
		}
	};
 	void setParent(Type* & t) { parentType = t; };   
	Type* getParent() const { return parentType; };
	
	int getSubTypeCount() const { return subTypeCount; };
	void setSubTypeCount(const int & c) { subTypeCount = c; };
	
	inline void incrSubTypeCounter() {
		subTypeCount++;
		if (parentType != 0) parentType->incrSubTypeCounter();
	};
	
	void allocSubTypeArray() {
		if (subTypeCount != 0) subTypes = new Type*[subTypeCount];
	};
	
	void registerSubType(Type * t) {
		subTypes[allocatedSubTypeCount++] = t;
		subTypeHash[t] = true;
		if (parentType != 0) parentType->registerSubType(t);
	};
	
	void registerWithParent() {
		if (parentType != 0) parentType->registerSubType(this);
	}
	
	int getInstanceCount() const { return instanceCount; };
	
	void incrInstanceCount() { 
		instanceCount++;
		if (parentType != 0) parentType->incrInstanceCount();
	};
	
	void allocateInstanceArray() {
		if (instances != 0) delete [] instances;
		instances = new Entity*[instanceCount];
		allocatedInstanceCount = 0;
	};
	
	void resetInstanceCount() { instanceCount = 0; };
	
	void setInstanceCount(const int & c) {
		instanceCount = c;
		instances = new Entity*[c];
	}
	
	void registerInstance(Entity * e) {
		instances[allocatedInstanceCount++] = e;
		if (parentType != 0) parentType->registerInstance(e);
		if (instanceCount == allocatedInstanceCount) sortInstancePointers();
	}
	
	void sortInstancePointers();
	
	Entity* getInstance(const int & i) const { return instances[i]; };
	
	Entity** getInstances() const { return instances; };

	void makeEntitySet();	
	void trimEntitySet(const EntitySet* trimTo);
	void resetEntitySet();
	
	EntitySet* getEntitySet() const {
		return entitySet;
	}
	
	void write(ostream & o) const;
	void detailedWrite(ostream & o) const;
	
	void setID(const int & i) { typeID = i; };
	int getID() const { return typeID; };
	
	bool isASubType(Type* toCheck) {
		return (subTypeHash.find(toCheck) != subTypeHash.end());
	}

	bool isASubTypeOf(Type* toCheck){

		return (toCheck->isASubType(this));

	}
	
};

ostream & operator <<(ostream & o, const Type & a);

#endif
