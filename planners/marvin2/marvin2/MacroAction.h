#ifndef MACROACTION_H
#define MACROACTION_H

#include "Action.h"
#include "staticinformation.h"
#include "hashdefinitions.h"
#include "plan.h"
#include <set>

using std::set;

#include <vector>

using std::vector;

using std::istream;

#include "State.h"
#include "AbstractStuff.h"

#include "ActionInstanceWithSubactions.h"

class ActionInstance;

class RelaxedPlanningGraph;

class DASearchStatus;

class MacroAction : public Action {

private:

	list<pair<list<AbstractAction>, double> > abstractSteps;
	
	
	Entity** originalBindings;
	

	bool marvinAction;
	bool escapeAction;

	bool loadedFromDisk;
	unsigned int timesSinceUse;
	unsigned int usageCount;
	
	static int calculateParameterCount(Plan* toUse, StaticInformation & problemInformation);
	
public:


	MacroAction(Plan* toUse, StaticInformation & problemInformation);

	void write(ostream & o) const;
	
	Plan* getConstituentActions(const ActionInstance * a) const;
	
	const string name() const {
		//cout << "Asking for name of a macro action at " << this << endl;
		return definedName;
	};

	~MacroAction(){
		
		delete [] originalBindings;

		
	}

	double duration() const;
	
	void setMarvinAction() {
		marvinAction = true;
		escapeAction = false;
	}
	
	void setEscapeAction() {
		marvinAction = false;
		escapeAction = true;
	}
	
	void incrementAppropriateCounter(int & a, int & b) const {
		if (marvinAction) a++;
		if (escapeAction) b++;
	}
	
	inline const bool & isMarvinAction() const { return marvinAction; };
	
	bool containsActionOfType(const Action* c) const {
		list<pair<list<AbstractAction>, double> >::const_iterator itr = abstractSteps.begin();
		for (; itr != abstractSteps.end(); itr++) {
			for (	list<AbstractAction>::const_iterator iItr = (*itr).first.begin();
				iItr != (*itr).first.end();
				iItr++) {
			
				if ((*iItr).actType == c) return true;
					
			}
		}
		
		return false;
	}
	void writeAbstractActions(ostream & o) const;
	
	auto_ptr<ActionInstanceList> derivedActions(ActionInstanceWithSubActionsList & unitActions, State *&  startAt, RelaxedPlanningGraph *& masterRPG, StaticInformation *& staticInf);
	
	bool operator ==(const MacroAction & o) const;
	
	bool isTransformationalOperator(const Predicate* & staticTransPred, int & loc1, int & loc2) {
		return false;
	}
	
	void updateActions(hash_map<Action*, Action*> & convertActions);

	static MacroAction* loadMacroAction(istream & o, StaticInformation & problemInformation, const unsigned int & tsu, const unsigned int & uc);
	
	const bool & isLoadedFromDisk() const { return loadedFromDisk; };
	const unsigned int & getTimesSinceUse() const { return timesSinceUse; };
	const unsigned int & getUsageCount() const { return usageCount; };
	
	void updateTimesSinceUse(const unsigned int & tsu) { timesSinceUse = tsu; };
	void updateUsageCount(const unsigned int & uc) { usageCount = uc; };
	void incrementUsageCount() { ++usageCount; };
	void save(ostream & o) const;

	MacroAction(const int & pcIn, const vector<Type*> & ptIn, const list<pair<list<AbstractAction>, double> > & asIn, StaticInformation & problemInformation, const unsigned int & tsu, const unsigned int & uc);

	DASearchStatus beginDerivedActions( ActionInstanceWithSubActionsList & unitActions, State *& startAt);
	
	ActionInstance* derivedActions(DASearchStatus & status, RelaxedPlanningGraph *& masterRPG, StaticInformation *& staticInf);

};

class GroundOutActionData;
class DASearchStatus {

public:
	list<pair<list<AbstractAction>, double> >::iterator asItr;
	const list<pair<list<AbstractAction>, double> >::iterator asEnd;
	
	
	slist<State*> toDelete;
	slist<ActionInstanceWithSubActionsList*> falToDelete;
	
	
	slist<GroundOutActionData*> asQueue;
	

	DASearchStatus( const list<pair<list<AbstractAction>, double> >::iterator & asItr_in,
			const list<pair<list<AbstractAction>, double> >::iterator & asEnd_in)
			: asItr(asItr_in), asEnd(asEnd_in) {};
	
	~DASearchStatus();
};
ostream & operator <<(ostream & o, const MacroAction & a);

#endif
