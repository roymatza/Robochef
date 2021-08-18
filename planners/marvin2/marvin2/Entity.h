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
#ifndef ENTITY_H
#define ENTITY_H

#include "Type.h"
#include "ptree.h"
/**
@author Andrew Ian Coles, Amanda Smith
*/

#include "hashdefinitions.h"

class Parameter;

class Entity{

private:
	
	const_symbol* originalEntity;
	Type* entityType;
	
	Parameter* entityAsParameter;
	
	bool constant;
	bool asymmetric;
	
	EntitySet symmetricWith;
	
public:

	
	
    	Entity(const_symbol* c, Type* t, const bool & b);

    	~Entity();

    	const string name() const { return (originalEntity->getName()); };
    	inline Type* getEntityType() const { return entityType; };

	void write(ostream & o) const {
		o << "Entity " << name() << " of type " << entityType->name();
	};
	
	Parameter* getEntityAsParameter() const {
		return entityAsParameter;
	}
	
	void registerInstanceWithType() {
		entityType->registerInstance(this);
	}
	
	const bool & isConstant() const { return constant; };
	const bool & isAsymmetric() const { return asymmetric; };
	void setSymmetricWith(const list<Entity*>&, const list<Entity*>&);
	bool possiblySymmetricWith(Entity* other) { return (symmetricWith.find(other) != symmetricWith.end()); };
	
};

ostream & operator <<(ostream & o, const Entity & a);

#endif
