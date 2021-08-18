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
#ifndef ACTIONINSTANCE_H
#define ACTIONINSTANCE_H

#include "Action.h"
#include "Parameter.h"
#include "withrefcounter.h"
#include "withrefcounterlist.h"
#include "hashdefinitions.h"




/**
@author Andrew Ian Coles, Amanda Smith
*/
class ActionInstance;

typedef WithRefCounterList<ActionInstance> ActionInstanceList;

class ActionInstance : public WithRefCounter {

private:
#ifdef SANITYCHECK
	static ActionInstanceSet allCreated;
	static ActionInstanceSet allDeleted;

	void sanityCheck() const {
		bool created = (allCreated.find(const_cast<ActionInstance*>(this)) != allCreated.end());
		bool deleted = (allDeleted.find(const_cast<ActionInstance*>(this)) != allDeleted.end());
		if (!created && !deleted) {
			cout << "Error: ActionInstance at address " << this << " is purely ficticious" << endl;
			exit(0);
		}
		if (deleted) {
			cout << "Error: ActionInstance at address " << this << " has been deleted!" << endl;
			exit(0);
		}
	}
#endif
	
	const Action* parentAction;
	Parameter** parameters;
	
	mutable int addEffectCount;
	mutable int deleteEffectCount;
	
	
	
	mutable PredicateInstance** addEffectCache;
	mutable PredicateInstance** deleteEffectCache;
	
	mutable PredicateInstance** preconditionCache;
	mutable PredicateInstance** negativePreconditionCache;
	
	unsigned int masterID;
	bool haveID;

	void cacheAddDelEffects() const {

#ifdef SANITYCHECK
		sanityCheck();
#endif
		
		const Entity** tArray = new const Entity*[parentAction->parameterCount];
		for (int i = 0; i < parentAction->parameterCount; i++) tArray[i] = parameters[i]->boundTo();
		
		parentAction->createEffectsArrays(	tArray,
							addEffectCache,
							addEffectCount,
							deleteEffectCache,
							deleteEffectCount);
				
		delete [] tArray;
	}
	
	static bool checkClash(const int & looplimit, const int & otherlimit, PredicateInstance** const thisTypes, PredicateInstance** const otherTypes);


	
public:

    	ActionInstance(const Action* a) : WithRefCounter(), parentAction(a), parameters(new Parameter*[a->parameterCount]), addEffectCount(-1), deleteEffectCount(-1), addEffectCache(0), deleteEffectCache(0), preconditionCache(0), negativePreconditionCache(0), masterID(0), haveID(false) {

		for (int i = 0; i < parentAction->parameterCount; i++) {
			parameters[i] = new Parameter(parentAction->parameterTypes[i]);
			parameters[i]->incrUsage();
		}
	
#ifdef SANITYCHECK	
		allCreated.insert(this);
		allDeleted.erase(this);
#endif

	};
	
	ActionInstance(const ActionInstance & a) : WithRefCounter(), 
	parentAction(a.parentAction), parameters(new Parameter*[parentAction->parameterCount]), addEffectCount(-1), deleteEffectCount(-1), addEffectCache(0), deleteEffectCache(0), preconditionCache(0), negativePreconditionCache(0), masterID(0), haveID(false) {
	
		for (int i = 0; i < parentAction->parameterCount; i++) {
			parameters[i] = a.parameters[i];
			parameters[i]->incrUsage();
		}
#ifdef SANITYCHECK	
		allCreated.insert(this);
		allDeleted.erase(this);
#endif
	}

	ActionInstance(const Action* a, Entity** p)
		: WithRefCounter(), parentAction(a), parameters(new Parameter*[a->parameterCount]), addEffectCount(-1), deleteEffectCount(-1), addEffectCache(0), deleteEffectCache(0), preconditionCache(0), negativePreconditionCache(0), masterID(0), haveID(false) {    	
		
		for (int i = 0; i < parentAction->parameterCount; i++) {
			parameters[i] = p[i]->getEntityAsParameter();
			parameters[i]->incrUsage();
		}
		
#ifdef SANITYCHECK	
		allCreated.insert(this);
		allDeleted.erase(this);
#endif
	};
	
	ActionInstance(const Action* a, Parameter** p) : WithRefCounter(), parentAction(a), parameters(p), addEffectCount(-1), deleteEffectCount(-1), addEffectCache(0), deleteEffectCache(0), preconditionCache(0), negativePreconditionCache(0), masterID(0), haveID(false) {

		for (int i = 0; i < parentAction->parameterCount; i++) {
			parameters[i]->incrUsage();
		}
#ifdef SANITYCHECK	
		allCreated.insert(this);
		allDeleted.erase(this);
#endif

	};	
	inline const int & getPreconditionCount() const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
		return parentAction->preconditionCount;
	};	

	inline const Predicate* & getPreconditionType(const int & i) const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
		return parentAction->preconditions[i].predType;
	}
	
	inline const int & getPreconditionArgumentCount(const int & i) const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
		
		return parentAction->preconditions[i].predType->getArgumentCount();
	}
	
	inline Parameter* getPreconditionArgument(const int & i, const int & j) const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
		
		if (parentAction->preconditions[i].hardParameters[j] != 0) {
			return parentAction->preconditions[i].hardParameters[j]->getEntityAsParameter();
		}
		int index = parentAction->preconditions[i].parameters[j];
		if (index >= 0) return parameters[index];
		return parentAction->constants[-1-index]->getEntityAsParameter();
		
	}
	
	inline const int & getNegativePreconditionCount() const {
	
#ifdef SANITYCHECK
		sanityCheck();
#endif

		return parentAction->negativePreconditionCount;
	};	

	inline const Predicate* & getNegativePreconditionType(const int & i) const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
		
		return parentAction->negativePreconditions[i].predType;
	}
	
	inline const int & getNegativePreconditionArgumentCount(const int & i) const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
		
		return parentAction->negativePreconditions[i].predType->getArgumentCount();
	}
	
	inline Parameter* getNegativePreconditionArgument(const int & i, const int & j) const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
		
		if (parentAction->negativePreconditions[i].hardParameters[j] != 0) {
			return parentAction->negativePreconditions[i].hardParameters[j]->getEntityAsParameter();
		}
		int index = parentAction->negativePreconditions[i].parameters[j];
		if (index >= 0) return parameters[index];
		return parentAction->constants[-1-index]->getEntityAsParameter();
		
	}
	
	
	inline const int & getAddEffectCount() const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
		
		if (addEffectCount == -1) const_cast<ActionInstance*>(this)->cacheAddDelEffects();
		return addEffectCount;
	};	

	inline const Predicate* const & getAddEffectType(const int & i) const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
		
		if (addEffectCache == 0) const_cast<ActionInstance*>(this)->cacheAddDelEffects();
		return addEffectCache[i]->getParentPredicate();
		
	}
	
	inline const int & getAddEffectArgumentCount(const int & i) const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
		
		if (addEffectCache == 0) const_cast<ActionInstance*>(this)->cacheAddDelEffects();
		return addEffectCache[i]->getParentPredicate()->getArgumentCount();
	}
	
	inline Parameter* getAddEffectArgument(const int & i, const int & j) const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
		
		if (addEffectCache == 0) const_cast<ActionInstance*>(this)->cacheAddDelEffects();
		return addEffectCache[i]->getParameter(j);
		
		
	}	
	
	inline const int & getDeleteEffectCount() const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
		
		if (deleteEffectCount == -1) const_cast<ActionInstance*>(this)->cacheAddDelEffects();
		return deleteEffectCount;
	};	

	inline const Predicate* const & getDeleteEffectType(const int & i) const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
		
		if (deleteEffectCache == 0) const_cast<ActionInstance*>(this)->cacheAddDelEffects();
		return deleteEffectCache[i]->getParentPredicate();
	}
	
	inline const int & getDeleteEffectArgumentCount(const int & i) const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
		
		if (deleteEffectCache == 0) const_cast<ActionInstance*>(this)->cacheAddDelEffects();
		return deleteEffectCache[i]->getParentPredicate()->getArgumentCount();
	}
	
	inline Parameter* getDeleteEffectArgument(const int & i, const int & j) const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
		
		if (deleteEffectCache == 0) const_cast<ActionInstance*>(this)->cacheAddDelEffects();
		return deleteEffectCache[i]->getParameter(j);
		
	}
	
	PredicateInstance** & internalAddEffects();
	PredicateInstance** & internalDeleteEffects();
	
	PredicateInstance** & internalPreconditions();
	PredicateInstance** & internalNegativePreconditions();

	PredicateInstance** const internalAddEffects() const;
	PredicateInstance** const internalDeleteEffects() const;
	
	PredicateInstance** const internalPreconditions() const;
	PredicateInstance** const internalNegativePreconditions() const;

		
    	~ActionInstance() {
		if (usageCount != 0) {
			cout << "Deleting ActionInstance with non-zero usage count" << endl;
			exit(0);
		}
		
		for (int i = 0; i < parentAction->parameterCount; ++i) {
			if (parameters[i] != 0) {
				if (parameters[i]->decrUsage()) delete parameters[i];
			};
		}
		delete [] parameters;
		
		if (addEffectCache != 0) {
		
			for (int i = 0; i < addEffectCount; ++i) {
				if (addEffectCache[i]->decrUsage()) delete addEffectCache[i];
			}
			delete [] addEffectCache;
		}
		
		if (deleteEffectCache != 0) {
		
			for (int i = 0; i < deleteEffectCount; ++i) {
				if (deleteEffectCache[i]->decrUsage()) delete deleteEffectCache[i];
			}
			delete [] deleteEffectCache;
		}
		
		if (preconditionCache != 0) {
		
			for (int i = 0; i < parentAction->preconditionCount; ++i) {
				if (preconditionCache[i] != 0 && preconditionCache[i]->decrUsage()) delete preconditionCache[i];
			}
			
			delete [] preconditionCache;
		}

		if (negativePreconditionCache != 0) {
		
			for (int i = 0; i < parentAction->negativePreconditionCount; ++i) {
				if (negativePreconditionCache[i] != 0 && negativePreconditionCache[i]->decrUsage()) delete negativePreconditionCache[i];
			}
			
			delete [] negativePreconditionCache;
		}
#ifdef SANITYCHECK	
		allCreated.erase(this);
		allDeleted.insert(this);
#endif
	}

	void write(ostream & o) const;
	void briefWrite(ostream & o) const;
	void standardWrite(ostream & o) const;
	
	bool isMutexWith(const ActionInstance & toCheck) const;
	
	int getParameterCount() const {
#ifdef SANITYCHECK
		sanityCheck();
#endif

		return parentAction->parameterCount;
	}

	const Action* getParentAction() const {
#ifdef SANITYCHECK
		sanityCheck();
#endif

		return parentAction;
	}

	Parameter* getParameter(const int & i) const {
#ifdef SANITYCHECK
		sanityCheck();
#endif

		if (i >= 0) return parameters[i];
		return parentAction->constants[-1-i]->getEntityAsParameter();
	}

	ActionInstance& operator =(const ActionInstance & a) {
		
		cerr << "Bother to write this" << endl;
		exit(0);
		for (int i = 0; i < parentAction->parameterCount; i++) {
			if (parameters[i]->decrUsage()) delete parameters[i];
		}
		
		delete [] parameters;
		
		parentAction = a.parentAction;
		parameters = new Parameter*[parentAction->parameterCount];
		
		for (int i = 0; i < parentAction->parameterCount; i++) {
			parameters[i] = a.parameters[i];
			parameters[i]->incrUsage();
		}
		
		return *this;	

	}
	
	bool achieves(PredicateInstance * p ) const;
	bool deletes(PredicateInstance * p ) const;
	

	PredicateInstance ** getPreconditionsAsPredicateInstances() const;
	PredicateInstance ** getNegativePreconditionsAsPredicateInstances() const;
	PredicateInstance ** getAddEffectsAsPredicateInstances() const;
	PredicateInstance ** getDeleteEffectsAsPredicateInstances() const;
	
	
    	bool operator ==(const ActionInstance & a) const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
	
		if (parentAction != a.parentAction) return false;
		for (int i = 0; i < parentAction->parameterCount; ++i) {
			if (!( *(parameters[i]) == *(a.parameters[i]))) return false;
		}
		return true;
	
	};
	
	
	
	int getUsageCount() const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
	
	return usageCount;
		
	};
	
	bool isInverseOf(const ActionInstance & a) const;
	
	inline const bool & hasID() const { return haveID; };
	inline const unsigned int & getID() const { return masterID; };
	inline void setID(const unsigned int & i) { masterID = i; haveID = true; };
	
	const bool & isMacroAction() const { return parentAction->isMacroAction; };
	
	size_t getHashValue() const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
	
		size_t toReturn = 0;
		
		toReturn += reinterpret_cast<size_t>(parentAction);
		for (int i = 0; i < parentAction->parameterCount; i++){
			toReturn += reinterpret_cast<size_t>(parameters[i]->boundTo());
		}
		
		return toReturn;
		
	}

	void replaceCachedAddEffect(const int & i, PredicateInstance* newPI) const {
#ifdef SANITYCHECK
		sanityCheck();
#endif

		if (addEffectCache == 0) return;
		if (addEffectCache[i]->decrUsage()) delete addEffectCache[i];
		addEffectCache[i] = newPI;
		newPI->incrUsage();
	}
	
	void replaceCachedDeleteEffect(const int & i, PredicateInstance* newPI) const {
#ifdef SANITYCHECK
		sanityCheck();
#endif
		
		if (deleteEffectCache == 0) return;
		if (deleteEffectCache[i]->decrUsage()) delete deleteEffectCache[i];
		deleteEffectCache[i] = newPI;
		newPI->incrUsage();
	}
	
	void replaceCachedPrecondition(const int & i, PredicateInstance* newPI) const {
#ifdef SANITYCHECK
		sanityCheck();
#endif

		if (preconditionCache == 0) return;
		if (preconditionCache[i]->decrUsage()) delete preconditionCache[i];
		preconditionCache[i] = newPI;
		if (newPI != 0) newPI->incrUsage();
	}
	
	void replaceCachedNegativePrecondition(const int & i, PredicateInstance* newPI) const {
#ifdef SANITYCHECK
		sanityCheck();
#endif

		if (negativePreconditionCache == 0) return;
		if (negativePreconditionCache[i]->decrUsage()) delete negativePreconditionCache[i];
		negativePreconditionCache[i] = newPI;
		if (newPI != 0) newPI->incrUsage();
	}
		
	void createPreconditionArrays() const;
#ifdef SANITYCHECK
	void incrUsage() {
		sanityCheck();
		usageCount++;
	};
	
	bool decrUsage() {
		sanityCheck();
		usageCount--;
		if (usageCount < 0) {
		
			cout << "ActionInstance at " << this << " has negative usage!" << endl;
			exit(0);
		}
		return (usageCount == 0);
	};

#endif

};

struct ActionInstanceHash {

	size_t operator()(const ActionInstance* const & x) const {
		return x->getHashValue();
	} 

};

struct ActionInstanceEq {

	bool operator()(const ActionInstance* const &  a, const ActionInstance * const & b) const {
	
		return (*a == *b);
	
	}

};
 
ostream & operator <<(ostream & o, const ActionInstance & a);


#endif
