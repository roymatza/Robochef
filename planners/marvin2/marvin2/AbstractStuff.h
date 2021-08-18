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

#ifndef ABSTRACTSTUFF_H
#define ABSTRACTSTUFF_H

#include "Predicate.h"
#include "Entity.h"
#include "hashdefinitions.h"
#include <set>
#include <cstring>

using std::set;

#include<iostream>

using std::cout;
using std::cerr;
using std::endl;

#include<string>

using std::string;

#include<sstream>

using std::stringstream;



struct AbstractPredicate {

	const Predicate* predType;
	int* parameters;
	Entity** hardParameters;
	
	AbstractPredicate() : parameters(0), hardParameters(0) {};
	
	~AbstractPredicate() {
		if (parameters != 0) delete [] parameters;
		if (hardParameters != 0) delete [] hardParameters;
	}
	
	AbstractPredicate(const AbstractPredicate & a)
	 : predType(a.predType), parameters(new int[predType->getArgumentCount()]), hardParameters(new Entity*[predType->getArgumentCount()]) {
	
		for (int i = 0; i < predType->getArgumentCount(); i++) {
			parameters[i] = a.parameters[i];
			hardParameters[i] = a.hardParameters[i];
		} 
		
	};
	
	
	AbstractPredicate(const Predicate * p)
	 : predType(p) {
		const int limit = predType->getArgumentCount();
		
		parameters = new int[limit];
		hardParameters = new Entity*[limit];
		
	 	memset(hardParameters, 0, sizeof(Entity*) * limit);
		 
				
	};
	
	AbstractPredicate(const Predicate * p, const int & limit, Entity** & hp)
	 : predType(p), parameters(new int[limit]), hardParameters(hp) {
			
	 	for (int i = 0; i < limit; ++i) parameters[i] = -1;
		 
				
	};
	
	AbstractPredicate & operator=(const AbstractPredicate & a) {
	
		delete [] parameters;
		delete [] hardParameters;
		predType = a.predType;
		const int limit = predType->getArgumentCount();
		
		parameters = new int[limit];
		hardParameters = new Entity*[limit];
		
		memcpy(parameters, a.parameters, sizeof(int) * limit);
		memcpy(hardParameters, a.hardParameters, sizeof(Entity*) * limit);
		
		return *this;
	
	}
	
	bool operator==(const AbstractPredicate & other) const {
		if (predType != other.predType) return false;
		const int limit = predType->getArgumentCount();
		for (int i = 0; i < limit; ++i) {
			if (hardParameters[i]) {
				if (hardParameters[i] != other.hardParameters[i]) return false;
			} else {
				if (parameters[i] != -1 && other.parameters[i] != -1) {
					if (parameters[i] != other.parameters[i]) return false;
				}
			}
		}
		return true;
		
	}
	
	bool operator<(const AbstractPredicate & other) const {
	
		if (predType != other.predType) {
			
			return (predType < other.predType);
		}
		const int limit = predType->getArgumentCount();
		for (int i = 0; i < limit; ++i) {
			if (hardParameters[i]) {
				if (hardParameters[i] > other.hardParameters[i]) {
					
					return false;
				} else if (hardParameters[i] < other.hardParameters[i]) {
					
					return true;
				}
			} else {
				if (parameters[i] > other.parameters[i]) {
					
					return false;
				} else if (parameters[i] < other.parameters[i]) {
					
					return true;
				}
				
			}
		}
		
		return false;
	
	}
	
	bool verboseLT(const AbstractPredicate & other) const {
	
		if (predType != other.predType) {
			cout << "Two predicates are not of the same type" << endl;
			cout << "Returning predType < other.predType" << endl;
			return (predType < other.predType);
		}
		
		for (int i = 0; i < predType->getArgumentCount(); i++) {
			if (hardParameters[i] != 0) {
				if (hardParameters[i] > other.hardParameters[i]) {
					cout << "hardParameter " << i << " of this is > hardParameter " << i << " of the other" << endl;
					return false;
				} else if (hardParameters[i] < other.hardParameters[i]) {
					cout << "hardParameter " << i << " of this is < hardParameter " << i << " of the other" << endl;
					return true;
				}
			} else {
				if (parameters[i] > other.parameters[i]) {
					cout << "parameter " << i << " of this is > parameter " << i << " of the other" << endl;
					return false;
				} else if (parameters[i] < other.parameters[i]) {
					cout << "parameter " << i << " of this is < parameter " << i << " of the other" << endl;
					return true;
				}
				
			}
		}
		
		return false;
	
	}
	
	void write(ostream & o) const {
	
		o << (predType->name());
		for (int i = 0; i < predType->getArgumentCount(); i++) {
			if (hardParameters[i] != 0) {
				o << " " << hardParameters[i]->name();
			} else {
				o << " " << parameters[i];
			}
		}
	
	}
	string getLabel() const {
	
		string toReturn = predType->name();
		for (int i = 0; i < predType->getArgumentCount(); i++) {
			toReturn += " ";
			if (hardParameters[i] != 0) {
				toReturn += hardParameters[i]->name();
			} else {
				stringstream s;
				s << parameters[i];

				toReturn += s.str();
			}
		}
		
		return toReturn;
	
	}
	
	bool interactsWith(const int & c) const {
		bool toReturn = false;
		const int limit = predType->getArgumentCount();
		for (int i = 0; i < limit; ++i) {
			if (!hardParameters[i] && parameters[i] == c) return true;
		}
		
		return toReturn;
	}
};

ostream & operator <<(ostream & o, const AbstractPredicate & a);

struct AbstractEventQueueItem {

	set<AbstractPredicate> addEffects;
	set<AbstractPredicate> deleteEffects;
	set<AbstractPredicate> invariants;
	set<AbstractPredicate> negativeInvariants;

};

class Action;

struct AbstractAction {

	const Action* actType;
	int* parameters;
	Entity** hardParameters;
	
	AbstractAction(const Action * p);
	
	AbstractAction() : actType(0), parameters(0), hardParameters(0) {
		
	};
	
	~AbstractAction() {
		delete [] parameters;
		delete [] hardParameters;
	}
	
	
	AbstractAction(const AbstractAction & a);
	
	AbstractAction & operator=(const AbstractAction & a);
	
	int getAddEffectCount() const;
	int getDeleteEffectCount() const;
	int getPreconditionCount() const;
	int getNegativePreconditionCount() const;
	
	bool operator<(const AbstractAction & other) const; 
	
};

#endif
