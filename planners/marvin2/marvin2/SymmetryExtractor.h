#ifndef SYMMETRYEXTRACTOR_H
#define SYMMETRYEXTRACTOR_H

#include <list>

using std::list;

#include "staticinformation.h"
#include "SymmetryGroup.h"


class SymmetryExtractor{

private:

	list<SymmetryGroup*> symmetryGroups;
	list<SymmetryGroup*> almostSymmetryGroups;
	list<SymmetryGroup*> actionSymmetryGroups;

public:

	list<SymmetryGroup*>* extractSymmetryGroups(StaticInformation & problemInformation, const bool & considerGoalState);
	list<SymmetryGroup*>* extractSymmetryGroups(StaticInformation & problemInformation, const bool & considerGoalState, State * startState);
	list<SymmetryGroup*> almostSymmetryDifferenceOnePredicate(StaticInformation & problemInformation);
	list<SymmetryGroup*> symmetricWithRespectToAction(Action* actionToUse, StaticInformation & problemInformation, const State * currState);
	void write(ostream & o) const;
	void writeAlmostSymmetry(ostream & o) const;

};

ostream & operator <<(ostream & o, const SymmetryExtractor & s);

#endif
