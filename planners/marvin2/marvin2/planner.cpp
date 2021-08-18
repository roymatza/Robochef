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
#include <string>

#include <cstdio>
#include <climits>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/times.h>
#include <unistd.h>

//#include "main.h"
#include <vector>
#include <ext/hash_map>

using __gnu_cxx::hash_map;

#include "staticinformation.h"
#include "State.h"
#include "plan.h"
#include "planningAlgorithms.h"
#include "MacroAction.h"

using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::cerr;
using std::cout;
using std::for_each;
using std::copy;

using std::vector;





void usage(char * argv[])
{
	cout << "Marvin, Public Release 2" << endl;
	cout << "By releasing this code we imply no warranty as to its reliability" << endl;
	cout << "and its use is entirely at your own risk." << endl;
	cout << endl;
	cout << "Usage: " << argv[0] << " [OPTION] domainfile problemfile <macrolist>" << endl << endl;
	cout << "Options are: " << endl << endl;
	cout << "\t" << "-b" << "\t" << "Go straight for best-first search - skip EHC step;" << endl;
	cout << "\t" << "-m" << "\t" << "Do not memoise plateau-escaping action sequences;" << endl;
	cout << "\t" << "-p" << "\t" << "Disable symmetric action choice pruning;" << endl;
	cout << "\t" << "-s" << "\t" << "Sequential (implicit if domain contains derived predicates);" << endl;
	cout << "\t" << "-t" << "\t" << "Do not recognise transformational operators;" << endl;
	cout << "\t" << "-v" << "\t" << "Verbose." << endl;
	cout << "\n\nMacro library management options (recommended: -l -n10):\n";
	cout << "\t<macrolist>\tLoad macro actions listed in the specified file;\n";
	cout << "\t" << "-l" << "\t\t" << "Save all macro actions (by default, saves all);" << endl;
	cout << "\t" << "-n<number>" << "\t" << "If used with -l, save the <number> most-used macro actions after search;" << endl;
	cout << "\t" << "-u<number>" << "\t" << "If used with -l, save only the macro-actions used in solving at least one of the <number> most recent problems." << endl;
	cout << endl;
  };



int main(int argc,char * argv[])
{

	//time_t startTime = time(0);
/*  try {*/
	if(argc < 2)
	{
		usage(argv);
		return 0;
	};

	bool verbose = false;
	bool onlyBFS = false; 		// -b sets this to true
	bool prune = true;		// -p sets this to false
    	bool concurrent = true; 	// -s sets this to false
	bool hasDPs = false;		// extracted from the domain later
	bool plateauEscaping = true;	// -m sets this to false
	bool marvins = false;		// -r sets this to false	
	bool transOps = true;		// -t sets this to false
	bool saveMacros = false;	// -l sets this to true
	bool bestFSPlateau = true;

	int keepTopN = INT_MAX;
	int sinceUseLimit = INT_MAX;
	
	int argcount = 1;
    
    	while(argcount < argc && argv[argcount][0] == '-') {
		
		switch(argv[argcount][1]) {
		
		case 'b':
			onlyBFS = true;
			break;
		
		case 'p':
			prune = false;
			break;
			
		case 's':
			concurrent = false;
			break;
			
		case 'm':
			plateauEscaping = false;
			break;
		case 't':
			transOps = false;
			break;
			
		case 'v':
			verbose = true;
			break;
		case 'x':
			bestFSPlateau = false;
			break;
		case 'l':
			saveMacros = true;
			break;
		case 'n':
		{
		         if (argv[argcount][2] == 0) {
				usage(argv);
				exit(1);
			 }
		         keepTopN = atoi(&(argv[argcount][2]));
			 break;
		}
		case 'u':
		{
		         if (argv[argcount][2] == 0) {
				usage(argv);
				exit(1);
			 }
		         sinceUseLimit = atoi(&(argv[argcount][2]));
			 break;
		}

		default:
			cout << "Unrecognised command-line switch" << endl;
			usage(argv);
			exit(1);
			
		};
		++argcount;
    	};

	if(argcount>=argc) {
		usage(argv);
		return 0;
	};


	char* domainFilename = argv[argcount++];

	if(argcount>=argc)  {
		usage(argv);
		return 0;
	};

	char* problemFilename = argv[argcount++];

	char* macroFilename = 0;
	if (argcount < argc) macroFilename = argv[argcount++];

	double parsetime;

	StaticInformation s(domainFilename, problemFilename, macroFilename, verbose, parsetime);


	planningAlgorithms paInstance;

	//s.verbose = true;
	bool writeCompetitionOutput = true;
//	time_t finishParseTime = time(0);
	//Plan * solution = paInstance.planEhc(&s);

	
	if (s.hasDerivedPredicates) {
		concurrent = false;
		hasDPs = true;
	}
	Plan * solution;
	solution = paInstance.planLookAheadEhc(&s, plateauEscaping, concurrent, hasDPs, prune, onlyBFS, false, bestFSPlateau);

	

	if (solution->empty()) {
		delete solution;
		return 0;
	}
	
	//time_t finishTime = time(0);
	cout << endl;
	
	if(writeCompetitionOutput){

		//time_t totalTime = finishTime - startTime;
		tms refReturn;
		times(&refReturn);

		Plan* preForPrinting = solution->unmacrify(&s);
		Plan* forPrinting = preForPrinting->expandTOs();
		
/*		for (int i = 0; i < s.numberOfActions; i++) {
		
			if (s.Actions[i]->isMacroAction) {
				MacroAction* currMA = ((MacroAction*) s.Actions[i]);
				currMA->writeAbstractActions(cout);
			}
		
		}
		
		cout << ";" << endl;
		cout << ";" << endl;
		cout << "; Without macro-action substitutions:" << endl;
		cout << ";" << endl;
		cout << *solution << endl;
*/		
		cout << ";;;; Solution Found" << endl;
		
		double secs = ((double)refReturn.tms_utime + (double)refReturn.tms_stime) / ((double) sysconf(_SC_CLK_TCK));

		int twodp = (int) (secs * 100.0);
		int wholesecs = twodp / 100;
		int centisecs = twodp % 100;

		cout << "; Time " << wholesecs << ".";
		if (centisecs < 10) cout << "0";
		cout << centisecs << endl;
		//time_t parsingTime = finishParseTime - startTime;
		twodp = (int) (parsetime * 100.0);
		wholesecs = twodp / 100;
		centisecs = twodp % 100;

		cout << "; ParsingTime " << wholesecs << ".";
		if (centisecs < 10) cout << "0";
		cout << centisecs << endl;
		
		
		
		cout << "; NrActions " << /*forPrinting->length() <<*/ endl;
		
		cout << "; MakeSpan " << forPrinting->makeSpan() << endl;
		
		cout << "; MetricValue" << endl;
		
		
		
		
		/*cout << ";" << endl;
		cout << ";" << endl;
		cout << "; With macro-action substitutions:" << endl;
		cout << ";" << endl;*/
		
		cout << *forPrinting << endl;
		

		if (saveMacros) {
		
		  
			{
			
				Plan::iterator pItr = solution->begin();
				const Plan::iterator pEnd = solution->end();
				
				for (; pItr != pEnd; ++pItr) {
				      
					Action* p = const_cast<Action*>(pItr->first->getParentAction());
					
					if (pItr->first->isMacroAction()) {
						MacroAction * ma = static_cast<MacroAction*>(p);  
						ma->updateTimesSinceUse(0);
						ma->incrementUsageCount();
					}
				}
			    
			}
		  
			if (plateauEscaping) {
			
			
				
				//cout << "Finding extra macros from plan so far" << endl;
				
				list<MacroAction*> streams = forPrinting->findPlateauEscapingMacros(&s, paInstance.masterRPG);
				
				
				for (	list<MacroAction*>::iterator strItr = streams.begin();
					strItr != streams.end();
					++strItr) {
				
					MacroAction* hierachicalAction = *strItr;
					//cout << "Stream:" << endl;
					//cout << *currMacroPlan << endl;
					
					hierachicalAction->setEscapeAction();

						//cout << "new Action is " << (*hierachicalAction) << endl;
					bool found = false;
					for (int i = 0; !found && i < s.numberOfActions; i++) {
						if (s.Actions[i]->isMacroAction) {
							MacroAction * ma = (static_cast<MacroAction*>(s.Actions[i]));
							if (*hierachicalAction == *ma) {
								found = true;
								ma->updateTimesSinceUse(0);
								ma->incrementUsageCount();
							}
						}
					}
					if (!found) {
						cout << "New : " << (*hierachicalAction);
						s.addReserveAction(hierachicalAction);
						forPrinting->oneMoreMacro(hierachicalAction);
						hierachicalAction->updateTimesSinceUse(0);
						hierachicalAction->incrementUsageCount();
					} else {
						//cout << "Duplicate: " << (*hierachicalAction) << endl;
						delete hierachicalAction;
					}
					
				}				
				
			
			
			
			};
				
			list<pair<string, MacroAction*> > filenames;
			
			for (int i = 0; i < s.numberOfActions; i++) {
			
				if (s.Actions[i]->isMacroAction) {
					MacroAction* currMA = ((MacroAction*) s.Actions[i]);
					if (currMA->getTimesSinceUse() <= sinceUseLimit) {
						string filename = "macro.";
						unsigned int hex = reinterpret_cast<unsigned int>(currMA);
						stringstream buf;
						buf << hex;
						string suffix;
						buf >> suffix;
						filename += suffix;
						
						ofstream file(filename.c_str());
						currMA->save(file);
						file.close();
						
						
						
						if (filenames.empty()) {
							filenames.push_back(make_pair(filename, currMA));
						} else {
							list<pair<string, MacroAction*> >::iterator insItr = filenames.begin();
							const list<pair<string, MacroAction*> >::iterator insEnd = filenames.end();
							
							while (insItr != insEnd && (insItr->second->getUsageCount() >= currMA->getUsageCount())) ++insItr;
							
							filenames.insert(insItr, make_pair(filename, currMA));
							
						}
					}
				}
			
			}
			ofstream macrolist("macrolist");
			stringstream buf;
			buf << (filenames.size() < keepTopN ? filenames.size() : keepTopN);
			string linecount;
			buf >> linecount;
			macrolist << linecount << endl;
			
			list<pair<string, MacroAction*> >::const_iterator fnItr = filenames.begin();
			const list<pair<string, MacroAction*> >::const_iterator fnEnd = filenames.end();
			
			for (int s = 0; fnItr != fnEnd && s < keepTopN; ++fnItr, ++s) {
			
				macrolist << fnItr->first << endl;
				macrolist << fnItr->second->getTimesSinceUse() << endl;
				macrolist << fnItr->second->getUsageCount() << endl;
			
			};
			
			macrolist.close();
			
		}

		delete forPrinting;
		delete preForPrinting;
		
		
	
	
	}else{
		cout << *solution << endl;
	}
	
	delete solution;
	
	/*
	State* currState = s.InitialState->copy();
	
	s.verbose = true;
	
	int expansions = 0;
	cout << "currState pointer is now " << currState << endl;
	while (!currState->subsumes(s.GoalPredicates, &s)) {
		s.verbose = false;
		list<ActionInstance* > applicable = currState->applicableRelaxedActions(&s);
		
		s.verbose = false;
		
		State* nextLayer = currState->applyRelaxedActions(applicable, &s);
		if (nextLayer->achievingActions.empty()) {
			cout << "No useful applicable actions (no predicates added to state)" << endl;
			exit(0);
		}
		nextLayer->advanceTime();
		delete currState;
		currState = nextLayer;
		cout << "currState pointer is now " << currState << endl;
		expansions++;
		cout << "Expansion " << expansions << " performed" << endl;
		s.verbose = true;
	}                                               
  
	cout << expansions << " expansions until goals all appear" << endl;*/
/*
  
  }
  
  catch(exception & e)
  {
  	cerr << "Error: " << e.what() << "\n";
  	an_analysis.error_list.report();
  	return 0;
  };

    return 0;*/
};


