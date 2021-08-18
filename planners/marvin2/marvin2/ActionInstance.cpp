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
#include "ActionInstance.h"

#ifdef SANITYCHECK
	ActionInstanceSet ActionInstance::allCreated;
	ActionInstanceSet ActionInstance::allDeleted;
#endif

#include "PredicateInstance.h"

void ActionInstance::write(ostream & o) const {
#ifdef SANITYCHECK
		sanityCheck();
#endif	
	o << "Action instance " << parentAction->name();
	for (int i = 0; i < parentAction->parameterCount; i++) {
		o << " " << *(parameters[i]);
	}
	o << endl;
	
	o << "- Preconditions:" << endl;
	for (int i = 0; i < getPreconditionCount(); i++) {
		o << "\t" << getPreconditionType(i)->name();
		for (int j = 0; j < getPreconditionArgumentCount(i); j++) {
			o << " " << *(getPreconditionArgument(i,j));
		}
		o << endl;
	}
	o << "- Effects:" << endl;
	for (int i = 0; i < getAddEffectCount(); i++) {
		o << "\t" << getAddEffectType(i)->name();
		for (int j = 0; j < getAddEffectArgumentCount(i); j++) {
			o << " " << *(getAddEffectArgument(i,j));
		}
		o << endl;
	}
	
	for (int i = 0; i < getDeleteEffectCount(); i++) {
		o << "\t¬(" << getDeleteEffectType(i)->name();
		for (int j = 0; j < getDeleteEffectArgumentCount(i); j++) {
			o << " " << *(getDeleteEffectArgument(i,j));
		}
		o << ")" << endl;
	}

};
void ActionInstance::briefWrite(ostream & o) const {
#ifdef SANITYCHECK
		sanityCheck();
#endif	
	if (parentAction->subActionTypes == 0) {
		o << parentAction->name();
		for (int i = 0; i < parentAction->parameterCount; i++) {
			o << " " << *(parameters[i]);
		}
		o << endl;
	} else {
		for (int j = 0; j < parentAction->subActionCount; j++) {
			o << parentAction->subActionTypes[j]->name();
			for (int i = 0; i < parentAction->subActionTypes[j]->parameterCount; i++) {
				o << " " << *(parameters[parentAction->subActionArguments[j][i]]);
			}
			o << endl;
		}
	}

};

void ActionInstance::standardWrite(ostream & o) const {
#ifdef SANITYCHECK
		sanityCheck();
#endif	
	if (parentAction->subActionTypes == 0) {
		o << "(";
		o << parentAction->name();
		
		for (int i = 0; i < parentAction->parameterCount; i++) {
			o << " " << *(parameters[i]);
		}
		o << ")";
		
	} else {
		for (int j = 0; j < parentAction->subActionCount; j++) {
			o << parentAction->subActionTypes[j]->name();
			for (int i = 0; i < parentAction->subActionTypes[j]->parameterCount; i++) {
				o << " " << *(parameters[parentAction->subActionArguments[j][i]]);
			}
			o << endl;
		}
	}

};

ostream & operator <<(ostream & o, const ActionInstance & a) {
	a.write(o);
	return o;
};


bool ActionInstance::checkClash(const int & looplimit, const int & otherlimit, PredicateInstance** const thisTypes, PredicateInstance** const otherTypes) {

	for (int a = 0; a < looplimit; ++a) {
		
		if (thisTypes[a] != 0) {
			for (int d = 0; d < otherlimit; ++d) {
				if (otherTypes[d] != 0 && *thisTypes[a] == *otherTypes[d]) {
					return true;
				}
			}
		}
	}
	
	return false;
};

bool ActionInstance::isMutexWith(const ActionInstance & toCheck) const{
#ifdef SANITYCHECK
		sanityCheck();
#endif
	//preconditions mutex can't happen as both actions are applicable in a given state

	//add effects of one intersect delete effects of the other

	if (checkClash(getAddEffectCount(), toCheck.getAddEffectCount(),            internalAddEffects(), toCheck.internalAddEffects())) return true;
	if (checkClash(getAddEffectCount(), toCheck.getDeleteEffectCount(),         internalAddEffects(), toCheck.internalDeleteEffects())) return true;
	if (checkClash(getAddEffectCount(), toCheck.getNegativePreconditionCount(), internalAddEffects(), toCheck.internalNegativePreconditions())) return true;
	

	if (checkClash(getDeleteEffectCount(), toCheck.getAddEffectCount(),    internalDeleteEffects(), toCheck.internalAddEffects())) return true;
	if (checkClash(getDeleteEffectCount(), toCheck.getDeleteEffectCount(), internalDeleteEffects(), toCheck.internalDeleteEffects())) return true;
	if (checkClash(getDeleteEffectCount(), toCheck.getPreconditionCount(), internalDeleteEffects(), toCheck.internalPreconditions())) return true;
	
	
	if (checkClash(getPreconditionCount(), toCheck.getAddEffectCount(),    internalPreconditions(),    toCheck.internalAddEffects())) return true;
	if (checkClash(getPreconditionCount(), toCheck.getDeleteEffectCount(), internalPreconditions(), toCheck.internalDeleteEffects())) return true;
	
	if (checkClash(getNegativePreconditionCount(), toCheck.getAddEffectCount(),    internalNegativePreconditions(),    toCheck.internalAddEffects())) return true;
	if (checkClash(getNegativePreconditionCount(), toCheck.getDeleteEffectCount(), internalNegativePreconditions(), toCheck.internalDeleteEffects())) return true;
	
	
/*	for (int pass = 0; pass < 8; pass++) {
	
		int looplimit;
		int otherlimit;
		
		PredicateInstance** thisTypes;
		PredicateInstance** otherTypes;
		
	
		
		if ((pass & 4) == 0) {
			
			if ((pass & 2) == 0) {
				otherlimit = toCheck.getDeleteEffectCount();
				otherTypes = toCheck.internalDeleteEffects();
			} else {
				otherlimit = toCheck.getNegativePreconditionCount();
				otherTypes = toCheck.internalNegativePreconditions();
			}
			
			
			if ((pass & 1) == 0) {
				looplimit = getAddEffectCount();
				thisTypes = internalAddEffects();
			} else {
				looplimit = getPreconditionCount();
				thisTypes = internalPreconditions();
				
			}

			
		} else {
			if ((pass & 2) == 0) {
				looplimit = getDeleteEffectCount();
				thisTypes = internalDeleteEffects();
			} else {
				looplimit = getNegativePreconditionCount();
				thisTypes = internalNegativePreconditions();
			}
			
			
			if ((pass & 1) == 0) {
				otherlimit = toCheck.getAddEffectCount();
				otherTypes = toCheck.internalAddEffects();			
			} else {
				otherlimit = toCheck.getPreconditionCount();
				otherTypes = toCheck.internalPreconditions();			
			}
		}
	
			
	}
	
	// one more case - check for delete-delete mutexes
	
	const int looplimit = getDeleteEffectCount();
	PredicateInstance** thisTypes = internalDeleteEffects();
	
	const int otherlimit = toCheck.getDeleteEffectCount();
	PredicateInstance** otherTypes = toCheck.internalDeleteEffects();
	
	for (int a = 0; a < looplimit; ++a) {
			
		if (thisTypes[a] != 0) {
			for (int d = 0; d < otherlimit; ++d) {
				if (otherTypes[d] != 0 && *thisTypes[a] == *otherTypes[d]) {
					return true;
				}
			}
		}
	}
	
	return false;
	
*/
	return false;
};

bool ActionInstance::achieves(PredicateInstance * p ) const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
	if (addEffectCache == 0) cacheAddDelEffects();
	
	for (int i = 0; i < getAddEffectCount(); ++i) {
		if (*addEffectCache[i] == *p) return true;
	}
		
	return false;
}

bool ActionInstance::deletes(PredicateInstance * p ) const {
#ifdef SANITYCHECK
		sanityCheck();
#endif	
	if (deleteEffectCache == 0) cacheAddDelEffects();	
	
	for (int i = 0; i < getDeleteEffectCount(); i++) {
		if (*deleteEffectCache[i] == *p) return true;
	}
	
		
	return false;
}


PredicateInstance** ActionInstance::getPreconditionsAsPredicateInstances() const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
	if (preconditionCache == 0) createPreconditionArrays();
			
	PredicateInstance** toReturn = new PredicateInstance*[getPreconditionCount()];
	
	for (int i = 0; i < getPreconditionCount(); i++) {
	
		
		toReturn[i] = preconditionCache[i];
		if (toReturn[i] != 0) toReturn[i]->incrUsage();
		
	}
	
	return toReturn;

};

PredicateInstance** ActionInstance::getNegativePreconditionsAsPredicateInstances() const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
	if (negativePreconditionCache == 0) createPreconditionArrays();
	
	PredicateInstance** toReturn = new PredicateInstance*[getNegativePreconditionCount()];
	
	for (int i = 0; i < getNegativePreconditionCount(); i++) {
	
		
		toReturn[i] = negativePreconditionCache[i];
		
		
		if (toReturn[i] != 0) toReturn[i]->incrUsage();
		
	}
	
	return toReturn;

};

PredicateInstance** & ActionInstance::internalAddEffects() {
#ifdef SANITYCHECK
		sanityCheck();
#endif
	if (addEffectCache == 0) cacheAddDelEffects();
	return addEffectCache;
}

PredicateInstance** & ActionInstance::internalDeleteEffects() {
#ifdef SANITYCHECK
		sanityCheck();
#endif
	if (deleteEffectCache == 0) cacheAddDelEffects();
	return deleteEffectCache;
}

PredicateInstance** & ActionInstance::internalPreconditions() {
#ifdef SANITYCHECK
		sanityCheck();
#endif
	
	if (preconditionCache == 0) createPreconditionArrays();
	return preconditionCache;
}

PredicateInstance** & ActionInstance::internalNegativePreconditions() {
#ifdef SANITYCHECK
		sanityCheck();
#endif
	if (negativePreconditionCache == 0) createPreconditionArrays();
	return negativePreconditionCache;
}


PredicateInstance** const ActionInstance::internalAddEffects() const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
	if (addEffectCache == 0) cacheAddDelEffects();
	return addEffectCache;
}

PredicateInstance** const ActionInstance::internalDeleteEffects() const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
	if (deleteEffectCache == 0) cacheAddDelEffects();
	return deleteEffectCache;
}

PredicateInstance** const ActionInstance::internalPreconditions() const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
	if (preconditionCache == 0) createPreconditionArrays();
	return preconditionCache;
}

PredicateInstance** const ActionInstance::internalNegativePreconditions() const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
	if (negativePreconditionCache == 0) createPreconditionArrays();
	return negativePreconditionCache;
}



PredicateInstance** ActionInstance::getAddEffectsAsPredicateInstances() const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
	if (addEffectCache == 0) cacheAddDelEffects();
	
	PredicateInstance** toReturn = new PredicateInstance*[addEffectCount];

	for (int i = 0; i < addEffectCount; i++) {

		toReturn[i] = addEffectCache[i];
		if (toReturn[i] != 0) toReturn[i]->incrUsage();

	}

	return toReturn;

};

PredicateInstance** ActionInstance::getDeleteEffectsAsPredicateInstances() const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
	if (deleteEffectCache == 0) cacheAddDelEffects();
	
	PredicateInstance** toReturn = new PredicateInstance*[deleteEffectCount];

	for (int i = 0; i < deleteEffectCount; i++) {

		toReturn[i] = deleteEffectCache[i];
		if (toReturn[i] != 0) toReturn[i]->incrUsage();

	}

	return toReturn;

};


bool ActionInstance::isInverseOf(const ActionInstance & a) const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
	if (	getAddEffectCount() != a.getDeleteEffectCount()
	     ||	getDeleteEffectCount() != a.getAddEffectCount()) return false;
	
	bool inverse = true;
	
	for (int pass = 0; inverse && pass < 2; pass++) {
	
		int looplimit;
		int otherlimit;
		const AbstractPredicate* thisTypes;
		const AbstractPredicate* otherTypes;
		
		if (pass == 0) {
			
			
			looplimit = parentAction->addEffectCount;
			thisTypes = parentAction->addEffects;
			
			
			otherlimit = a.parentAction->deleteEffectCount;
			otherTypes = a.parentAction->deleteEffects;			
		
			
		} else {
			looplimit = parentAction->deleteEffectCount;
			thisTypes = parentAction->deleteEffects;
		

			otherlimit = a.parentAction->addEffectCount;
			otherTypes = a.parentAction->addEffects;
		
		}


		for (int i = 0; inverse && i < looplimit; i++) {

			bool found = false;

			for (int j = 0; !found && j < otherlimit; j++) {

				if (thisTypes[i].predType == otherTypes[j].predType) {


					found = true;

					for (int k = 0; found && k < thisTypes[i].predType->getArgumentCount(); k++) {
						Parameter* firstParam = getParameter(thisTypes[i].parameters[k]);
						Parameter* secondParam = a.getParameter(otherTypes[j].parameters[k]);
						found = (*firstParam == *secondParam);
					}

				}

			}
			
			inverse = found;
			
		}
		
		
	}
	
	if (false && inverse) {
		briefWrite(cout);
		cout << " is the inverse of ";
		a.briefWrite(cout);
		cout << endl;
	}
	
	return inverse;

};

void ActionInstance::createPreconditionArrays() const {
#ifdef SANITYCHECK
		sanityCheck();
#endif	
	preconditionCache = new PredicateInstance*[getPreconditionCount()];
	negativePreconditionCache = new PredicateInstance*[getNegativePreconditionCount()];

	
	for (int i = 0; i < getPreconditionCount(); i++) {

		Parameter** params = new Parameter*[getPreconditionArgumentCount(i)];
		
		for (int j = 0; j < getPreconditionArgumentCount(i); j++) {
			params[j] = getPreconditionArgument(i,j);
		}
		
		preconditionCache[i] = new PredicateInstance(getPreconditionType(i), params);
		preconditionCache[i]->incrUsage();
		
	}
	
	for (int i = 0; i < getNegativePreconditionCount(); i++) {
	
		Parameter** params = new Parameter*[getNegativePreconditionArgumentCount(i)];
		
		for (int j = 0; j < getNegativePreconditionArgumentCount(i); j++) {
			params[j] = getNegativePreconditionArgument(i,j);
		}
		
		negativePreconditionCache[i] = new PredicateInstance(getNegativePreconditionType(i), params);
		negativePreconditionCache[i]->incrUsage();
		
	}	
}
