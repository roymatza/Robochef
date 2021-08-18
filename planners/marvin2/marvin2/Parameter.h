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
#ifndef PARAMETER_H
#define PARAMETER_H

#include "Type.h"
#include "Entity.h"

#include "withrefcounter.h"
/**
@author Andrew Ian Coles, Amanda Smith
*/
class Parameter : public WithRefCounter {

private:

	Type* parameterType;
	Entity* boundEntity;
	
	
public:
    
	Parameter() : WithRefCounter(), parameterType(0), boundEntity(0) {};
	
	Parameter(Type* t) : WithRefCounter(), parameterType(t), boundEntity(0) {};
	Parameter(Entity* e) : WithRefCounter(), parameterType(e->getEntityType()), boundEntity(e) {};    

	~Parameter() {};
	
	bool isBound() const { return (boundEntity != 0); };
	
	void bindTo(Entity* e) { boundEntity = e; };
	Entity* boundTo() const { return boundEntity; };
    
	inline void incrUsage() { usageCount++; };
	inline bool decrUsage() {
		usageCount--;
		if (usageCount < 0) {
		
			cout << "Parameter at " << this << " has negative usage!" << endl;
			exit(0);
		}
		return (usageCount == 0);
	};
	
	void write(ostream & o) const {
		if (boundEntity == 0) {
			cout << parameterType->name() << "<" << this << ">";
		} else {
			cout << boundEntity->name();
		}
	}
	
	bool operator ==(const Parameter & p) const { 
	
		if (!isBound() || !p.isBound()) return true;
		
		return (boundTo() == p.boundTo());
	
	}
};

ostream & operator <<(ostream & o, const Parameter & a);

#endif
