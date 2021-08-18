#ifndef PLANNINGALGORITHMS_H
#define PLANNINGALGORITHMS_H

#include "ActionInstance.h"
#include <list>

using std::list;
#include "plan.h"
#include "staticinformation.h"
#include "State.h"
#include "SymmetryGroup.h"
#include "relaxedPlanningGraph.h"

class planningAlgorithms{

private:

	State * pruneState(list <SymmetryGroup*> symmGroups, const State* toPrune, StaticInformation & problemInformation, bool includeStaticInitialState, list<Entity*> & entitiesToKeep, State* staticInitialState);
	
	Action* getHierachicalAction(Plan* toUse, StaticInformation & problemInformation);

	static bool isCyclical(const ActionInstanceList * pruneCheck, const ActionInstance* toApply);
	
public:

	RelaxedPlanningGraph* masterRPG;

	planningAlgorithms() : masterRPG(0) {};
	
	~planningAlgorithms() {
		delete masterRPG;
	}
	
	Plan * planEhc(StaticInformation * problemInformation);

	Plan * planLookAheadEhc(StaticInformation * problemInformation, const bool & inferMacro, const bool & concurrent, const bool & derivedPredicates, const bool & symmPrune, const bool onlyBFS, const bool noBFS, const bool bestFSPlateau=true);

	Plan * StrictSymmetryHierachyPlan(StaticInformation & problemInformation);

	Plan * AlmostSymmetryHierachyPlan(StaticInformation & problemInformation, const bool & inferMacro, const bool & concurrent, const bool & derivedPredicates, const bool & symmPrune, const bool & onlyBFS, const bool & noBFS, const bool & transOps);

	

};

#endif
