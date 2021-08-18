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
#ifndef RESOURCEINSTANCE_H
#define RESOURCEINSTANCE_H

#include "Resource.h"
#include "Parameter.h"

/**
@author Andrew Ian Coles, Amanda Smith
*/
class ResourceInstance{
private:

	Resource* parentResource;

	Parameter** parameters;
	
	int usageCount;
	
	double value;
	
public:

	ResourceInstance(Resource* p)
	   : 	parentResource(p),
		parameters(new Parameter*[p->argumentCount]),
		usageCount(0),
		value(0) {

		for (int i = 0; i < parentResource->argumentCount; i++) parameters[i] = 0;

	};
	
	inline void incrUsage() { usageCount++; };
	inline bool decrUsage() { return ((--usageCount) <= 0); };

    	~ResourceInstance() {
		for (int i = 0; i < parentResource->argumentCount; i++) {
			if (parameters[i] != 0) {

				if (parameters[i]->decrUsage()) delete parameters[i];
			}
		}
		delete [] parameters;
	};
	
	void setParameter(int index, Parameter* p) {
		if (parameters[index] != 0) {
			if (parameters[index]->decrUsage()) delete parameters[index];
		}
		parameters[index] = p;
		p->incrUsage();
	}
	
	double setValue(const double & v) { value = v; };
	double getValue() const { return value; };
	
	friend double & operator +=(ResourceInstance & r, const double & v);
	friend double & operator -=(ResourceInstance & r, const double & v);
	friend double & operator /=(ResourceInstance & r, const double & v);
	friend double & operator *=(ResourceInstance & r, const double & v);

	

	void write(ostream & o) const {
		o << parentResource->name();
		for (int i = 0; i < parentResource->argumentCount; i++) {
			o << " " << *(parameters[i]);
		}
	};
};

ostream & operator <<(ostream & o, const ResourceInstance & a);

double & operator +=(ResourceInstance & r, const double & v);
double & operator -=(ResourceInstance & r, const double & v);
double & operator /=(ResourceInstance & r, const double & v);
double & operator *=(ResourceInstance & r, const double & v);


#endif
