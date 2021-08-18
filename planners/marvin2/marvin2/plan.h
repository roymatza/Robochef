#ifndef PLAN_H
#define PLAN_H

#include <list>
#include "ActionInstance.h"
class MacroAction;

using std::list;

class StaticInformation;

class RelaxedPlanningGraph;

class Plan{

private:
	
	list<pair<ActionInstance*, double> > planActions;

	static PONodeSet * intersection(const PONodeSet * first, const PONodeSet * second);
	static ADLPONodeSet * intersection(const ADLPONodeSet * first, const ADLPONodeSet * second);

public:

	typedef list<pair<ActionInstance*, double> >::iterator iterator;
	typedef list<pair<ActionInstance*, double> >::const_iterator const_iterator;

	iterator begin() { return planActions.begin(); };
	const_iterator begin() const { return planActions.begin(); };

	iterator end() { return planActions.end(); };
	const_iterator end() const { return planActions.end(); };
	
	Plan() {
		//cout << "Default-constructing plan at " << this << endl;
	};
	
	Plan(const Plan & p) : planActions(p.planActions) {
		for (	list<pair<ActionInstance*, double> >::iterator itr = planActions.begin();
			itr != planActions.end();
			itr++) {
		
			(*itr).first->incrUsage();	
		}
		
	}
	
	
	Plan(const ActionInstanceList & a)  {
		for (	ActionInstanceList::const_iterator itr = a.begin();
			itr != a.end();
			itr++) {
		
			planActions.push_back(pair<ActionInstance*, double>(*itr, -1.0));
			(*itr)->incrUsage();
		}
		
			
	};
	
	~Plan() {
		for (	list<pair<ActionInstance*, double> >::iterator itr = planActions.begin();
			itr != planActions.end();
			itr++) {
		
			if ((*itr).first->decrUsage()) delete (*itr).first;
		}
	}
	
	
	Plan & operator =(const Plan & p) {
	
		//cout << "Assigning to plan at " << this << endl;
		for (	list<pair<ActionInstance*, double> >::iterator itr = planActions.begin();
			itr != planActions.end();
			itr++) {
		
			if ((*itr).first->decrUsage()) delete (*itr).first;
		}
		
		planActions.clear();
		
		for (	list<pair<ActionInstance*, double> >::const_iterator itr = p.planActions.begin();
			itr != p.planActions.end();
			itr++) {
		
			planActions.push_back(*itr);
			planActions.back().first->incrUsage();	
		}
		
		
		
		return *this;
	}
	
	void addStepAtEnd(ActionInstance * actionToAdd, const double & timeStamp);
	void blindlyAddStepAtEnd(ActionInstance * actionToAdd, const double & timeStamp);
	
	void push_back(pair<ActionInstance*, double> toAdd){
		
		planActions.push_back(toAdd);
		toAdd.first->incrUsage();
	}

	
	void addStepsAtEnd(const ActionInstanceList & actionsToAdd, const double & timeStamp);
	
	
	void addStepsAtEnd(const ActionInstanceList & actionsToAdd, const list<double> & timeStampValues){
		cerr << "Deprecated" << endl;
		exit(0);
		ActionInstanceList::const_iterator actItr = actionsToAdd.begin();
		list<double>::const_iterator timeItr = timeStampValues.begin();

		for(;actItr != actionsToAdd.end() && timeItr != timeStampValues.end(); actItr++, timeItr++){

			
			planActions.push_back(pair<ActionInstance*, double>(*actItr, *timeItr));
			(*actItr)->incrUsage();
		}
	}
	
	
	void addStepsAtBeginning(const ActionInstanceList & actionsToAdd, const list<double> & timeStampValues){
		
		ActionInstanceList::const_iterator actItr = actionsToAdd.end();
		actItr--;
		list<double>::const_iterator timeItr = timeStampValues.end();
		timeItr--;
		
		for(; actItr != actionsToAdd.begin() && timeItr != timeStampValues.begin(); actItr--, timeItr--){
			planActions.push_front(pair<ActionInstance*, double>(*actItr, *timeItr));
			(*actItr)->incrUsage();
		}
		
		if (!actionsToAdd.empty()) {
			planActions.push_front(pair<ActionInstance*, double>(*actItr, *timeItr));
			(*actItr)->incrUsage();
		
		}
		
	}
	
	void addStepAtBeginning(ActionInstance * actionToAdd, const double & timeStamp){
		planActions.push_front(pair<ActionInstance*, double>(actionToAdd, timeStamp));
		actionToAdd->incrUsage();
	}
	
	Plan* unmacrify(StaticInformation * staticInf) const;
	
	void write(ostream & o) const {
		//o << "Plan at address " << this << endl;
		list<pair<ActionInstance*, double> >::const_iterator actItr = planActions.begin();
		

		for(; actItr != planActions.end(); actItr++){

			if ((*actItr).first->getParentAction()->rootAction()) {
				o << (*actItr).second << ": ";
				(*actItr).first->standardWrite(o);
				o << " [1]" << endl;
			} else {
				/*o << "; " << (*actItr).second << ": ";
				(*actItr).first->standardWrite(o);
				o << " [1]" << endl;*/
			}
			
		}
		
		
		
	}
	
	void insert(list<pair<ActionInstance*, double> >::iterator itrPos, pair<ActionInstance*, double> toInsert){
		
		planActions.insert(itrPos, toInsert);
		toInsert.first->incrUsage();
	
	}
	
	int length() const {
		return planActions.size();
	}

	ActionInstanceList helpfulActions() const;
	
	const list<pair<ActionInstance*, double> > & planSteps() const {
		return planActions;
	}
	
	double makeSpan() const{
		return ((planActions.back().second - planActions.front().second) + 1);
		//return ((planActions.back().second) + 1);
	}
	
	bool empty() const {
		return (planActions.empty());
	}
	
	void clear() {
		for (	list<pair<ActionInstance*, double> >::iterator itr = planActions.begin();
			itr != planActions.end();
			itr++) {
		
			if ((*itr).first->decrUsage()) delete (*itr).first;
		}
		planActions.clear();
	}
	list<Plan *> untangle() const;
	
	static list<Plan *> untangle(list<pair<ActionInstanceList, double> > & actionsAndSubActions);
	
	Plan* removeSimilarAdjacentSteps() const;
	
	Plan* expandTOs();

	list<MacroAction*> findPlateauEscapingMacros(StaticInformation * staticInf, RelaxedPlanningGraph * rpg,bool annotate=false) const;

	struct MALT {
	
		bool operator()(const MacroAction* const & a, const MacroAction* const & b) const {
			return (reinterpret_cast<unsigned int>(a) < reinterpret_cast<unsigned int>(b));
		};
	
	};

	map<MacroAction*, unsigned int, MALT> used;

	void oneMoreMacro(MacroAction* a) {
		map<MacroAction*, unsigned int, MALT>::iterator itr = used.find(a);
		if (itr == used.end()) {
			used[a] = 1;
		} else {
			++(itr->second);
		}
	};	
};


ostream & operator <<(ostream & o, const Plan & p);

#endif
