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
#include "Entity.h"
#include "Parameter.h"

Entity::Entity(const_symbol* c, Type* t, const bool & b) : originalEntity(c), entityType(t), constant(b), asymmetric(b) {
	entityAsParameter = new Parameter(this);
	entityAsParameter->incrUsage();
}

Entity::~Entity() {
	if (entityAsParameter->decrUsage()) delete entityAsParameter;
};

ostream & operator <<(ostream & o, const Entity & a) {
	a.write(o);
	return o;
};

void Entity::setSymmetricWith(const list<Entity*> & head, const list<Entity*> & tail) {
	symmetricWith.clear();
	
	if (true) {
		list<Entity*>::const_iterator itr = head.begin();
		const list<Entity*>::const_iterator itrEnd = head.end();
		
		for (; itr != itrEnd; ++itr) symmetricWith.insert(*itr);
	}
	
	if (true) {
		list<Entity*>::const_iterator itr = tail.begin();
		const list<Entity*>::const_iterator itrEnd = tail.end();
		
		for (; itr != itrEnd; ++itr) symmetricWith.insert(*itr);
	}
	
	asymmetric = symmetricWith.empty();
	
	if (asymmetric) {
//		cout << name() << " couldn't possibly be symmetric with anything" << endl;
	} else {
//		cout << name() << " could be symmetric with " << symmetricWith.size() << " things" << endl;
		
	}
};

