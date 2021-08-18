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

#include "AbstractStuff.h"
#include "Action.h"

ostream & operator <<(ostream & o, const AbstractPredicate & a) {
	a.write(o);
	return o;
};

AbstractAction::AbstractAction(const Action * p)
    :	actType(p),
	parameters(new int[p->parameterCount]), hardParameters(new Entity*[p->parameterCount]) {
	
		for (int i = 0; i < p->parameterCount; i++) {
			parameters[i] = 0;
			hardParameters[i] = 0;
		}
	};
	
AbstractAction::AbstractAction(const AbstractAction & a)
    : 	actType(a.actType),
    	parameters(new int[actType->parameterCount]),
	hardParameters(new Entity*[actType->parameterCount])	 {
	
	for (int i = 0; i < actType->parameterCount; ++i) {
		parameters[i] = a.parameters[i];
		hardParameters[i] = a.hardParameters[i];
	} 
	
};
	
AbstractAction & AbstractAction::operator=(const AbstractAction & a) {
	
	delete [] parameters;
	delete [] hardParameters;
	
	actType = a.actType;
	parameters = new int[actType->parameterCount];
	hardParameters = new Entity*[actType->parameterCount];
	
	for (int i = 0; i < actType->parameterCount; i++) {
		parameters[i] = a.parameters[i];
		hardParameters[i] = a.hardParameters[i];
	}		

	return *this;
}
	
int AbstractAction::getAddEffectCount() const { return actType->addEffectCount; };
int AbstractAction::getDeleteEffectCount() const { return actType->deleteEffectCount; };
int AbstractAction::getPreconditionCount() const { return actType->preconditionCount; };
int AbstractAction::getNegativePreconditionCount() const { return actType->negativePreconditionCount; };

bool AbstractAction::operator<(const AbstractAction & other) const {
	
	if (actType != other.actType) {
		return (actType < other.actType);
	}
	
	for (int i = 0; i < actType->parameterCount; i++) {
	
		if (hardParameters[i] != 0 && other.hardParameters[i] != 0) {
			
			if (hardParameters[i] > other.hardParameters[i]) {
				return false;
			
			} else if (hardParameters[i] < other.hardParameters[i]) {
				return true;
			}
			
		} else if (hardParameters[i] != 0 && other.hardParameters[i] == 0) {
			
			return false;
		
		} else if (hardParameters[i] == 0 && other.hardParameters[i] != 0) {
			
			return true;
		
		} else if (hardParameters[i] == 0 && other.hardParameters[i] == 0) {
		
			if (parameters[i] > other.parameters[i]) {
					
				return false;
			} else if (parameters[i] < other.parameters[i]) {
				
				return true;
			}
				
		}
	}
	
	return false;

}
