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
#include "ADLAction.h"
#include "ActionSequenceValidator.h"
#include "staticinformation.h"
#include "State.h"

#include "PredicateInstance.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using std::cerr;
using std::cout;
using std::endl;

using std::string;

using std::fstream;

class SatisfactionNode;
class SatisfactionLeaf;
class SatisfactionContainerNode;
class PositiveSatisfactionLeaf;
class NegativeSatisfactionLeaf;
class ConjunctiveSatisfactionNode;
class DisjunctiveSatisfactionNode;
class NegationNode;

class EffectNode;
class EffectContainerNode;
class ConditionalEffectNode;
class ConjunctiveEffectNode;
class NegativeEffectNode;
class EffectLeaf;
class PositiveEffectLeaf;
class NegativeEffectLeaf;

enum NodeTypes { ST_UNDEFINED, ST_POSITIVELEAF, ST_NEGATIVELEAF, ST_CONJNODE, ST_DISJNODE, ST_NEGNODE};
enum EffectNodeTypes { EF_UNDEFINED, EF_POSITIVELEAF, EF_NEGATIVELEAF, EF_CONJNODE, EF_CONDNODE, EF_NEGNODE};

class SatisfactionTreeVisitor {

public:
	virtual void visitPositiveSatisfactionLeaf(SatisfactionLeaf * p) {
		cout << "Visiting 'positive satisfaction leaf'" << endl;
	};
	virtual void visitNegativeSatisfactionLeaf(SatisfactionLeaf * p) {
		cout << "Visiting 'negative satisfaction leaf'" << endl;
	};
	virtual void visitConjunctiveSatisfactionNode(SatisfactionContainerNode * p) {
		cout << "Visiting 'conjunction node'" << endl;
	};
	virtual void visitDisjunctiveSatisfactionNode(SatisfactionContainerNode * p) {
		cout << "Visiting 'disjunction node'" << endl;
	};
	virtual void visitNegationNode(SatisfactionContainerNode * p) {
		cout << "Visiting 'negation node'" << endl;
	};
		

	


};

class EffectTreeVisitor {

public:
	virtual void visitPositiveEffectLeaf(EffectLeaf * p) {
		cout << "Visiting 'positive effect leaf'" << endl;
	};
	virtual void visitNegativeEffectLeaf(EffectLeaf * p) {
		cout << "Visiting 'negative effect leaf'" << endl;
	};
	virtual void visitConjunctiveEffectNode(ConjunctiveEffectNode * p) {
		cout << "Visiting 'conjunction node'" << endl;
	};
	virtual void visitConditionalEffectNode(ConditionalEffectNode * p) {
		cout << "Visiting 'conditional effect node'" << endl;
	};
	virtual void visitNegativeEffectNode(NegativeEffectNode * p) {
		cout << "Visiting 'negative effect node'" << endl;
	};
		

	


};


class SatisfactionNode {

protected:
	SatisfactionNode* parent;
	int ourID;

public:
	SatisfactionNode(SatisfactionNode* s, int & IDs) : parent(s), ourID(IDs++) {};
	SatisfactionNode(const SatisfactionNode & toCopy, SatisfactionNode* newParent) : parent(newParent), ourID(toCopy.ourID) {};
	
	virtual ~SatisfactionNode() {};
	
	virtual void flatten(	list<int> & dependencies,
				list<int> & satisfactionCounts,
				list<pair<int, int> > & internalSatisfactionCounts, 
				const int & parentDIndex,
				pair<int, int> * parentDCounters,
				int & nextDIndex,
				bool polarity,
				list<pair<AbstractPredicate, int> > & preconditions,
				list<pair<AbstractPredicate, int> > & negativePreconditions,
				list<bool> & essentialContainers,
				list<bool> & disjunctiveContainers,
				list<bool> & essentialPreconditions,
				list<bool> & essentialNegativePreconditions,
				bool currentEssential) {};			
	
	virtual void write(ostream & o) const {};
	
	
	virtual void saveDot(ostream & o) const {
		o << "digraph Preconditions {\n";
		o << "\tnode [shape=box, fontsize=10, color=black, fillcolor=white, fontcolor=black];\n";
		o << "\tedge [style=solid, color=black];\n";
		writeDot(o);
		o << "}\n";
	}

	
	virtual void writeDot(ostream & o) const {};
	virtual void writeReducedDot(ostream & o, bool polarity, bool contentPolarity, int parentID) const {};

	inline const int & getID() const { return ourID; };
	
	virtual void visit(SatisfactionTreeVisitor * v) {
		// generic visit - do nothing
	}
	
	virtual void setParent(SatisfactionNode * n) {
		parent = n;
	}
	
};

class SatisfactionContainerNode : public SatisfactionNode {

protected:
	list<SatisfactionNode*> children;
	
public:

	SatisfactionContainerNode(SatisfactionNode* sparent, int & IDs) : SatisfactionNode(sparent, IDs) {};
	
	SatisfactionContainerNode(const SatisfactionContainerNode & toCopy, SatisfactionNode* sparent) : SatisfactionNode(toCopy, sparent) {};
	
	virtual ~SatisfactionContainerNode() {
		for (	list<SatisfactionNode*>::iterator itr = children.begin();
			itr != children.end();
			itr++) {
		
			delete *itr;
				
		}
	}
	
	virtual void addChild(PositiveSatisfactionLeaf * const c) {
		children.push_back((SatisfactionNode*) c);
	}
	
	virtual void addChild(NegativeSatisfactionLeaf * const c) {
		children.push_back((SatisfactionNode*) c);
	}
	
	virtual void addChild(NegationNode* const c) {
		children.push_back((SatisfactionNode*) c);
	}

	virtual void addChild(ConjunctiveSatisfactionNode* const c) {
		children.push_back((SatisfactionNode*) c);
	}

	
	virtual void addChild(DisjunctiveSatisfactionNode* const c) {
		children.push_back((SatisfactionNode*) c);
	}


	
	virtual void visitChildren(SatisfactionTreeVisitor * v) {
		for (	list<SatisfactionNode*>::iterator itr = children.begin();
			itr != children.end();
			itr++) {
			(*itr)->visit(v);
		}
	}
	
	virtual list<SatisfactionNode*> & getChildren() {
		return children;
	}
	
};

class SatisfactionLeaf : public SatisfactionNode {

protected:
	AbstractPredicate currPred;
	
public:

	SatisfactionLeaf(SatisfactionNode* s, const AbstractPredicate & p, int & IDs) : SatisfactionNode(s, IDs), currPred(p) {};
	SatisfactionLeaf(const SatisfactionLeaf & toCopy, SatisfactionNode * s): SatisfactionNode(toCopy, s), currPred(toCopy.currPred) {};
	
	virtual void writeReducedDot(ostream & o, bool polarity, bool contentPolarity,  int parentID) const {
		o << "// Writing reduced dot from generic satisfaction leaf" << endl;
	};
	
	virtual ~SatisfactionLeaf() {};
	
	AbstractPredicate & getAbstractPredicate() { return currPred; };

};

class PositiveSatisfactionLeaf : public SatisfactionLeaf {

public:
	PositiveSatisfactionLeaf(SatisfactionNode* sparent, const AbstractPredicate & p, int & IDs) : SatisfactionLeaf(sparent,p,IDs) {};
	PositiveSatisfactionLeaf(const SatisfactionLeaf & toCopy, SatisfactionNode * s) : SatisfactionLeaf(toCopy, s) {};
	
	virtual void flatten(	list<int> & dependencies,
				list<int> & satisfactionCounts,
				list<pair<int, int> > & internalSatisfactionCounts, 
				const int & parentDIndex,
				pair<int, int> * parentDCounters,
				int & nextDIndex,
				bool polarity,
				list<pair<AbstractPredicate, int> > & preconditions,
				list<pair<AbstractPredicate, int> > & negativePreconditions,
				list<bool> & essentialContainers,
				list<bool> & disjunctiveContainers,
				list<bool> & essentialPreconditions,
				list<bool> & essentialNegativePreconditions,
				bool currentEssential);

	virtual void write(ostream & o) const {
	
		o << "(";
		currPred.write(o);
		o << ")";
	}
	
	virtual void writeDot(ostream & o) const {
		
		o << "\tnode" << ourID << " [label=\"";
		o << currPred.getLabel();
		o << "\"];\n";
	}
	
	
	void visit(SatisfactionTreeVisitor * p) {
		p->visitPositiveSatisfactionLeaf(this);
	}
	
	virtual ~PositiveSatisfactionLeaf() {};
	
	
};

class NegativeSatisfactionLeaf : public SatisfactionLeaf {
	
public:
	NegativeSatisfactionLeaf(SatisfactionNode* sparent, const AbstractPredicate & p, int & IDs) : SatisfactionLeaf(sparent,p,IDs) {}
	NegativeSatisfactionLeaf(const SatisfactionLeaf & toCopy, SatisfactionNode * s) : SatisfactionLeaf(toCopy, s) {};

	virtual void flatten(	list<int> & dependencies,
				list<int> & satisfactionCounts,
				list<pair<int, int> > & internalSatisfactionCounts, 
				const int & parentDIndex,
				pair<int, int> * parentDCounters,
				int & nextDIndex,
				bool polarity,
				list<pair<AbstractPredicate, int> > & preconditions,
				list<pair<AbstractPredicate, int> > & negativePreconditions,
				list<bool> & essentialContainers,
				list<bool> & disjunctiveContainers,
				list<bool> & essentialPreconditions,
				list<bool> & essentialNegativePreconditions,
				bool currentEssential);
				
	virtual void write(ostream & o) const {
	
		o << "(�";
		currPred.write(o);
		o << ")";
	}
	
	virtual void writeDot(ostream & o) const {
		
		
		o << "\tnode" << ourID << " [label=\"�(";
		o << currPred.getLabel();
		o << ")\"];\n";
	}
	
	void visit(SatisfactionTreeVisitor * p) {
		p->visitNegativeSatisfactionLeaf(this);
	}
	virtual ~NegativeSatisfactionLeaf() {};
	
};



class ConjunctiveSatisfactionNode : public SatisfactionContainerNode {

public:

	ConjunctiveSatisfactionNode(SatisfactionNode* sparent, int & IDs) : SatisfactionContainerNode(sparent, IDs) {}	
	ConjunctiveSatisfactionNode(const SatisfactionContainerNode & toCopy, SatisfactionNode* sparent) : SatisfactionContainerNode(toCopy, sparent) {};
	
	virtual void flatten(	list<int> & dependencies,
				list<int> & satisfactionCounts,
				list<pair<int, int> > & internalSatisfactionCounts, 
				const int & parentDIndex,
				pair<int, int> * parentDCounters,
				int & nextDIndex,
				bool polarity,
				list<pair<AbstractPredicate, int> > & preconditions,
				list<pair<AbstractPredicate, int> > & negativePreconditions,
				list<bool> & essentialContainers,
				list<bool> & disjunctiveContainers,
				list<bool> & essentialPreconditions,
				list<bool> & essentialNegativePreconditions,
				bool currentEssential) {
		
		essentialContainers.push_back(currentEssential);
		disjunctiveContainers.push_back(false);
		bool nextEssential = (currentEssential && true);
		
		dependencies.push_back(parentDIndex);
		satisfactionCounts.push_back(0);
		internalSatisfactionCounts.push_back(pair<int, int>(0, 0) );
		
		int ourDIndex = nextDIndex;
		nextDIndex++;
		
		pair<int, int>* ourDCounter = &(internalSatisfactionCounts.back());
		int* ourDCounter_final = &(satisfactionCounts.back());
		
		
		for (list<SatisfactionNode*>::iterator itr = children.begin();
			itr != children.end();
			itr++) {
		
			(*itr)->flatten(dependencies, satisfactionCounts, internalSatisfactionCounts, ourDIndex, ourDCounter, nextDIndex, polarity, preconditions, negativePreconditions, essentialContainers, disjunctiveContainers, essentialPreconditions, essentialNegativePreconditions, nextEssential);
				
		}

		*ourDCounter_final = ourDCounter->first;
		
		if (parentDIndex != -1) {
			if (*ourDCounter_final > 0) {
				parentDCounters->first++;
			} else {
				parentDCounters->second++;	
			}
		}
						
	}
	virtual void write(ostream & o) const {
	
		o << "(and";
		for (list<SatisfactionNode*>::const_iterator itr = children.begin();
			itr != children.end();
			itr++) {
			o << " ";
			(*itr)->write(o);
				
		}
		o << ")";
	}
	
	virtual void writeDot(ostream & o) const {
		
		o << "\tnode" << ourID << " [label=\"and\" color=green];\n";
		for (	list<SatisfactionNode*>::const_iterator itr = children.begin();
			itr != children.end();
			itr++) {
			o << "\tnode" << ourID << " -> node" << (*itr)->getID() << ";\n";
			(*itr)->writeDot(o);
		}
		
		
	}
	
	void visit(SatisfactionTreeVisitor * p) {
		p->visitConjunctiveSatisfactionNode(this);
	}
	
	
	
};

class DisjunctiveSatisfactionNode : public SatisfactionContainerNode {

public:	
	
	DisjunctiveSatisfactionNode(SatisfactionNode* sparent, int & IDs) : SatisfactionContainerNode(sparent, IDs) {}	
	DisjunctiveSatisfactionNode(const SatisfactionContainerNode & toCopy, SatisfactionNode* sparent) : SatisfactionContainerNode(toCopy, sparent) {};

	virtual void flatten(	list<int> & dependencies,
				list<int> & satisfactionCounts,
				list<pair<int, int> > & internalSatisfactionCounts, 
				const int & parentDIndex,
				pair<int, int> * parentDCounters,
				int & nextDIndex,
				bool polarity,
				list<pair<AbstractPredicate, int> > & preconditions,
				list<pair<AbstractPredicate, int> > & negativePreconditions,
				list<bool> & essentialContainers,
				list<bool> & disjunctiveContainers,
				list<bool> & essentialPreconditions,
				list<bool> & essentialNegativePreconditions,
				bool currentEssential) {

		essentialContainers.push_back(currentEssential);
		disjunctiveContainers.push_back(true);
		
		bool nextEssential = (currentEssential && false);
					
		dependencies.push_back(parentDIndex);
		satisfactionCounts.push_back(0);
		internalSatisfactionCounts.push_back(pair<int, int>(0, 0) );
		
		int ourDIndex = nextDIndex;
		nextDIndex++;
		
		pair<int, int>* ourDCounter = &(internalSatisfactionCounts.back());
		int* ourDCounter_final = &(satisfactionCounts.back());
		
		
		for (list<SatisfactionNode*>::iterator itr = children.begin();
			itr != children.end();
			itr++) {
		
			(*itr)->flatten(dependencies, satisfactionCounts, internalSatisfactionCounts, ourDIndex, ourDCounter, nextDIndex, polarity, preconditions, negativePreconditions, essentialContainers, disjunctiveContainers, essentialPreconditions, essentialNegativePreconditions, nextEssential);
				
		}

		if (ourDCounter->first > 0) {
			*ourDCounter_final = 1 - ourDCounter->second;
		} else {
			if (ourDCounter->second > 0) {
				*ourDCounter_final = 1 - ourDCounter->second;
			} else { // no children at all
				*ourDCounter_final = 0;
			}
		}
		
		if (parentDIndex != -1) {
			if (*ourDCounter_final > 0) {
				parentDCounters->first++;
			} else {
				parentDCounters->second++;
			}
		}					
	}
	virtual void write(ostream & o) const {
	
		o << "(or";
		for (list<SatisfactionNode*>::const_iterator itr = children.begin();
			itr != children.end();
			itr++) {
			o << " ";
			(*itr)->write(o);
				
		}
		o << ")";
	}	

	virtual void writeDot(ostream & o) const {
		
		o << "\tnode" << ourID << " [label=\"or\" color=blue];\n";
		for (	list<SatisfactionNode*>::const_iterator itr = children.begin();
			itr != children.end();
			itr++) {
			o << "\tnode" << ourID << " -> node" << (*itr)->getID() << ";\n";
			(*itr)->writeDot(o);
		}
		
		
	}
		
	
	void visit(SatisfactionTreeVisitor * p) {
		p->visitDisjunctiveSatisfactionNode(this);
	}
	
	
};

class NegationNode : public SatisfactionContainerNode {

private:
		
	
	bool childIsPredicate;
	bool childIsNegativePredicate;
	bool childIsConjunction;
	bool childIsDisjunction;
	bool childIsNegation;
	
public:
	NegationNode(SatisfactionNode* sparent, int & IDs) : SatisfactionContainerNode(sparent, IDs),
		childIsPredicate(false), childIsNegativePredicate(false), childIsConjunction(false), childIsDisjunction(false), childIsNegation(false) {}
	
	void addChild(PositiveSatisfactionLeaf * const c) {
		
		children.clear();
		children.push_back(c);
		
		childIsPredicate = true;
	}
	
	void addChild(NegativeSatisfactionLeaf * const c) {
		
		children.clear();
		children.push_back(c);
		childIsNegativePredicate = true;
	}
	
	void addChild(NegationNode* const c) {
		children.clear();
		children.push_back(c);
		childIsNegation = true;
	}

	void addChild(ConjunctiveSatisfactionNode* const c) {
		children.clear();
		children.push_back(c);
		childIsConjunction = true;
	}
	
	void addChild(DisjunctiveSatisfactionNode* const c) {
		children.clear();
		children.push_back(c);
		childIsDisjunction = true;
	}
	
	
	/*
	virtual void flatten(	list<int> & dependencies,
				list<int> & satisfactionCounts,
				list<pair<int, int> > & internalSatisfactionCounts, 
				const int & parentDIndex,
				pair<int, int> * parentDCounters,
				int & nextDIndex,
				bool polarity,
				list<pair<AbstractPredicate, int> > & preconditions,
				list<pair<AbstractPredicate, int> > & negativePreconditions,
				list<bool> & essentialPreconditions,
				list<bool> & essentialNegativePreconditions,
				bool currentEssential) {
	
		if (children.empty()) return;
		
		bool newPolarity = !polarity;
		
		SatisfactionNode* child = children.front();
		
		if (childIsNegation) {
			
			reinterpret_cast<NegationNode*>(child)->flatten(dependencies, satisfactionCounts, internalSatisfactionCounts, parentDIndex, parentDCounters, nextDIndex, newPolarity, preconditions, negativePreconditions, currentEssential); 
		}
		
		if (childIsPredicate) {
			reinterpret_cast<PositiveSatisfactionLeaf*>(child)->flatten(dependencies, satisfactionCounts, internalSatisfactionCounts, parentDIndex, parentDCounters, nextDIndex, newPolarity, preconditions, negativePreconditions); 
			
		}
		
		if (childIsNegativePredicate) {
			reinterpret_cast<NegativeSatisfactionLeaf*>(child)->flatten(dependencies, satisfactionCounts, internalSatisfactionCounts, parentDIndex, parentDCounters, nextDIndex, newPolarity, preconditions, negativePreconditions); 
		}
		
		if (childIsConjunction) {
			if (newPolarity) { // i.e. is now positive: take as a straight conjunction
				reinterpret_cast<ConjunctiveSatisfactionNode*>(child)->flatten(dependencies, satisfactionCounts, internalSatisfactionCounts, parentDIndex, parentDCounters, nextDIndex, newPolarity, preconditions, negativePreconditions); 
			} else {
				// i.e. is now negative: use De Morgan's law
				// not (A & B & C) == ((not A) || (not B) || (not C))
				
				reinterpret_cast<DisjunctiveSatisfactionNode*>(child)->DisjunctiveSatisfactionNode::flatten(dependencies, satisfactionCounts, internalSatisfactionCounts, parentDIndex, parentDCounters, nextDIndex, newPolarity, preconditions, negativePreconditions); 
			}
		}
		if (childIsDisjunction) {
			if (newPolarity) { // i.e. is now positive: take as a straight disjunction
				reinterpret_cast<DisjunctiveSatisfactionNode*>(child)->flatten(dependencies, satisfactionCounts, internalSatisfactionCounts, parentDIndex, parentDCounters, nextDIndex, newPolarity, preconditions, negativePreconditions); 
			} else {
				// i.e. is now negative: use De Morgan's law
				// not (A || B || C) == ((not A) && (not B) && (not C))
				reinterpret_cast<ConjunctiveSatisfactionNode*>(child)->ConjunctiveSatisfactionNode::flatten(dependencies, satisfactionCounts, internalSatisfactionCounts, parentDIndex, parentDCounters, nextDIndex, newPolarity, preconditions, negativePreconditions); 
			}
		}					
	
	}*/
	
	virtual void write(ostream & o) const {
	
		o << "(not";
		for (list<SatisfactionNode*>::const_iterator itr = children.begin();
			itr != children.end();
			itr++) {
			o << " ";
			(*itr)->write(o);
				
		}
		o << ")";
	}
	
	virtual void writeDot(ostream & o) const {
		
		o << "\tnode" << ourID << " [label=\"not\" color=red];\n";
		for (	list<SatisfactionNode*>::const_iterator itr = children.begin();
			itr != children.end();
			itr++) {
			o << "\tnode" << ourID << " -> node" << (*itr)->getID() << ";\n";
			(*itr)->writeDot(o);
		}
		
		
	}

	
	void visit(SatisfactionTreeVisitor * p) {
		p->visitNegationNode(this);
	}
	
	
};

class EffectNode {

protected:
	int ourID;
	EffectNode* parent;
public:
	EffectNode(EffectNode* s, int & IDs) : ourID(IDs++), parent(s) {};
	EffectNode(const EffectNode & toCopy, EffectNode* newParent) : ourID(toCopy.ourID), parent(newParent)  {};

	virtual ~EffectNode() {};
		
	inline const int & getID() const { return ourID; };
	
	virtual void writeDot(ostream & o) const {};
	
	virtual void saveDot(ostream & o) const {
		o << "digraph Effects {\n";
		o << "\tnode [shape=box, fontsize=10, color=black, fillcolor=white, fontcolor=black];\n";
		o << "\tedge [style=solid, color=black];\n";
		writeDot(o);
		o << "}\n";
	}
	virtual void visit(EffectTreeVisitor * v) {
		// generic visit - do nothing
	}
};

class EffectContainerNode : public EffectNode {

protected:

	list<EffectNode*> children;
	
public:
	EffectContainerNode(EffectNode* sparent, int & IDs) : EffectNode(sparent, IDs) {};
	
	EffectContainerNode(const EffectContainerNode & toCopy, EffectNode* sparent) : EffectNode(toCopy, sparent) {};
	
	virtual ~EffectContainerNode() {
		for (	list<EffectNode*>::iterator itr = children.begin();
			itr != children.end();
			itr++) {
		
			delete *itr;
				
		}
	}
	
	virtual void visitChildren(EffectTreeVisitor * v) {
		
		for (	list<EffectNode*>::iterator itr = children.begin();
			itr != children.end();
			itr++) {
			
			(*itr)->visit(v);
		}
	}
	
	virtual void addChild(EffectNode * c) {
		children.push_back(c);
	}

	virtual list<EffectNode*> & getChildren() {
		return children;
	}
};

class ConditionalEffectNode : public EffectContainerNode {

private:
	SatisfactionNode* when;

public:
	ConditionalEffectNode(EffectNode* sparent, int & IDs) : EffectContainerNode(sparent, IDs), when(0) {};
	ConditionalEffectNode(const ConditionalEffectNode & toCopy, EffectNode* sparent) : EffectContainerNode(toCopy, sparent), when(0) {};

	virtual ~ConditionalEffectNode() {
		if (when != 0) delete when;
	}
	virtual SatisfactionNode* getWhenBranch() {
		return when;
	}
	
	virtual void setWhenBranch(SatisfactionNode* newWhen) {
		when = newWhen;
	}
	
	virtual void writeDot(ostream & o) const {
		o << "\tnode" << ourID << " [label=\"when\" color=\".7 .3 1.0\"];\n";
		when->writeDot(o);
		o << "\tnode" << ourID << " -> node" << when->getID() << " [style=dashed];\n";
		for (	list<EffectNode*>::const_iterator itr = children.begin();
			itr != children.end();
			itr++) {
			o << "\tnode" << ourID << " -> node" << (*itr)->getID() << ";\n";
			(*itr)->writeDot(o);
		}
	}
	
	virtual void visit(EffectTreeVisitor * v) {
		v->visitConditionalEffectNode(this);
	}
	
	void clear() {
		when = 0;
		children.clear();
	}
	
};

class ConjunctiveEffectNode : public EffectContainerNode {

public:
	ConjunctiveEffectNode(EffectNode* sparent, int & IDs) : EffectContainerNode(sparent, IDs) {};
	ConjunctiveEffectNode(const ConjunctiveEffectNode & toCopy, EffectNode* sparent) : EffectContainerNode(toCopy, sparent) {};
	
	virtual void writeDot(ostream & o) const {
		o << "\tnode" << ourID << " [label=\"and\" color=green];\n";
		for (	list<EffectNode*>::const_iterator itr = children.begin();
			itr != children.end();
			itr++) {
			o << "\tnode" << ourID << " -> node" << (*itr)->getID() << ";\n";
			(*itr)->writeDot(o);
		}
	}	
	
	virtual void visit(EffectTreeVisitor * v) {
		v->visitConjunctiveEffectNode(this);
	}

	
};

class NegativeEffectNode : public EffectContainerNode {

public:

	NegativeEffectNode(EffectNode* sparent, int & IDs) : EffectContainerNode(sparent, IDs) {};
	NegativeEffectNode(const NegativeEffectNode & toCopy, EffectNode* sparent) : EffectContainerNode(toCopy, sparent) {};
	
	virtual void writeDot(ostream & o) const {
		o << "\tnode" << ourID << " [label=\"not\" color=red];\n";
		for (	list<EffectNode*>::const_iterator itr = children.begin();
			itr != children.end();
			itr++) {
			o << "\tnode" << ourID << " -> node" << (*itr)->getID() << ";\n";
			(*itr)->writeDot(o);
		}
	}
	
	virtual void visit(EffectTreeVisitor * v) {
		v->visitNegativeEffectNode(this);
	}


};



class EffectLeaf : public EffectNode {

protected:

	AbstractPredicate currPred;

public:

	EffectLeaf(EffectNode* sparent, const AbstractPredicate & p, int & IDs) : EffectNode(sparent, IDs), currPred(p) {};
	EffectLeaf(const EffectLeaf & toCopy, EffectNode* sparent) : EffectNode(toCopy, sparent), currPred(toCopy.currPred) {};
	
	virtual ~EffectLeaf() {};
	AbstractPredicate & getAbstractPredicate() { return currPred; };

};

class PositiveEffectLeaf : public EffectLeaf {

public:

	PositiveEffectLeaf(EffectNode* sparent, const AbstractPredicate & p, int & IDs) : EffectLeaf(sparent, p, IDs) {};
	PositiveEffectLeaf(const EffectLeaf & toCopy, EffectNode* sparent) : EffectLeaf(toCopy, sparent) {};
	
	virtual void writeDot(ostream & o) const {
		o << "\tnode" << ourID << " [label=\"";
		o << currPred.getLabel();
		o << "\"];\n";
	}

	virtual ~PositiveEffectLeaf() {};
	
	virtual void visit(EffectTreeVisitor * v) {
		v->visitPositiveEffectLeaf(this);
	}
};

class NegativeEffectLeaf : public EffectLeaf {

public:

	NegativeEffectLeaf(EffectNode* sparent, const AbstractPredicate & p, int & IDs) : EffectLeaf(sparent, p, IDs) {};
	NegativeEffectLeaf(const EffectLeaf & toCopy, EffectNode* sparent) : EffectLeaf(toCopy, sparent) {};

	virtual void writeDot(ostream & o) const {
		o << "\tnode" << ourID << " [label=\"�(";
		o << currPred.getLabel();
		o << ")\"];\n";
	}
	virtual ~NegativeEffectLeaf() {};

	virtual void visit(EffectTreeVisitor * v) {
		v->visitNegativeEffectLeaf(this);
	}

};




class MakeADLGoalListVisitor : public VisitController {

private:
	list<SatisfactionContainerNode*> parentNodes;
	int * IDs;
	bool ownIDs;
	SatisfactionNode* rootNode;
	
	hash_map<pddl_type*, Type* >* pddl_typeToType;
	
	hash_map<parameter_symbol*, int>* parameter_symbolToIndex;
	hash_map<parameter_symbol*, Entity*>* parameter_symbolToEntity;
	
	hash_map<const pred_symbol*, Predicate* > * pred_symbolToPredicate;
	hash_map<const const_symbol*, int> * const_symbolToConstantIndex;
	

public:

	int nextArg;
	Entity* nextEnt;
	
	MakeADLGoalListVisitor() : IDs(new int(0)), ownIDs(true), rootNode(0) {};
	MakeADLGoalListVisitor(int & i) : IDs(&i), ownIDs(false), rootNode(0) {};

	~MakeADLGoalListVisitor() {
		if (ownIDs) delete IDs;
	}
	
	SatisfactionNode* getRoot() {return rootNode; };

	void parseGoal(	goal* topGoal,
			hash_map<pddl_type*, Type* >* const convType,
			hash_map<parameter_symbol*, int> * const conv,
			hash_map<const pred_symbol*, Predicate* > * const predConv,
			hash_map<const const_symbol*, int> * const constConv) {

		pddl_typeToType = convType;
		parameter_symbolToIndex = conv;
		parameter_symbolToEntity = new hash_map<parameter_symbol*, Entity*>();
		pred_symbolToPredicate = predConv;
		const_symbolToConstantIndex = constConv;
		
		parentNodes.clear();
		rootNode = 0;
		
		topGoal->visit(this);
		delete parameter_symbolToEntity;

	}
	
	void parseDPGoal(	goal* topGoal,
				AbstractPredicate & extraGoal,
				hash_map<pddl_type*, Type* >* const convType,
				hash_map<parameter_symbol*, int> * const conv,
				hash_map<const pred_symbol*, Predicate* > * const predConv,
				hash_map<const const_symbol*, int> * const constConv) {

		pddl_typeToType = convType;
		parameter_symbolToIndex = conv;
		parameter_symbolToEntity = new hash_map<parameter_symbol*, Entity*>();
		pred_symbolToPredicate = predConv;
		const_symbolToConstantIndex = constConv;
		
		parentNodes.clear();
		rootNode = 0;
		ConjunctiveSatisfactionNode* dpRoot = new ConjunctiveSatisfactionNode(0, *IDs);
		
		NegativeSatisfactionLeaf* dum = new NegativeSatisfactionLeaf(dpRoot, extraGoal, *IDs);
		
		dpRoot->addChild(dum);
		
		rootNode = dpRoot;
		
		parentNodes.push_front(dpRoot);
				
		topGoal->visit(this);
		
		parentNodes.pop_front();
		
		delete parameter_symbolToEntity;

	}
	
	
	void parseGoal(	goal* topGoal,
			hash_map<pddl_type*, Type* >* const convType,
			hash_map<parameter_symbol*, int> * const conv,
			hash_map<parameter_symbol*, Entity*> * const convEnt,
			hash_map<const pred_symbol*, Predicate* > * const predConv,
			hash_map<const const_symbol*, int> * const constConv) {
	
		pddl_typeToType = convType;
		parameter_symbolToIndex = conv;
		parameter_symbolToEntity = convEnt;
		
		pred_symbolToPredicate = predConv;
		const_symbolToConstantIndex = constConv;
		
		parentNodes.clear();
		rootNode = 0;
		
		topGoal->visit(this);
		
					
	};
	
	
	void visit_simple_goal(simple_goal * s) {

		const proposition* prop = s->getProp();

		Predicate* currPredicate = (*pred_symbolToPredicate)[prop->head];
		
		AbstractPredicate currAP(currPredicate);
		
				
		parameter_symbol_list::iterator itr = prop->args->begin();

		for (int i = 0; itr != prop->args->end(); i++, itr++) {

			parameter_symbol* curr = (*itr);
			curr->visit(this);
			if (nextEnt == 0) {
				currAP.parameters[i] = nextArg;
			} else {
				currAP.hardParameters[i] = nextEnt;
			}

		}
//		cout << "Precondition: " << currAP << endl;
		
		bool p = (s->getPolarity() == E_POS);
		
		SatisfactionContainerNode* parentNode;
		
		
		if (rootNode == 0) { // simple goal at root
			parentNode = 0;
		} else {
			parentNode = parentNodes.front();
		}
		
		if (p) {
			if (parentNode != 0) {
				PositiveSatisfactionLeaf * psl = new PositiveSatisfactionLeaf(parentNode, currAP, *IDs);
				parentNode->addChild(psl);
			} else {		
				rootNode = new PositiveSatisfactionLeaf(parentNode, currAP, *IDs);
			}
		} else {
			if (parentNode != 0) {
				NegativeSatisfactionLeaf * psl = new NegativeSatisfactionLeaf(parentNode, currAP, *IDs);
				parentNode->addChild(psl);
			} else {		
				rootNode = new NegativeSatisfactionLeaf(parentNode, currAP, *IDs);;
			}
			
		}
		
		

	};
	
	void visit_conj_goal(conj_goal * c) {
	
		SatisfactionContainerNode* parentNode;
		
		if (rootNode == 0) { // simple goal at root
			parentNode = 0;
		} else {
			parentNode = parentNodes.front();
		}
		
		ConjunctiveSatisfactionNode* newNode = new ConjunctiveSatisfactionNode(parentNode, *IDs);
		
		if (parentNode != 0) {
			parentNode->addChild(newNode);
		} else {		
			rootNode = newNode;
		}
		
		parentNodes.push_front(newNode);
			
		c->getGoals()->visit(this);
		
		parentNodes.pop_front();
	
	};
	
	void visit_disj_goal(disj_goal * c) {
	
		SatisfactionContainerNode* parentNode;
		
		if (rootNode == 0) { // simple goal at root
			parentNode = 0;
		} else {
			parentNode = parentNodes.front();
		}
		
		DisjunctiveSatisfactionNode* newNode = new DisjunctiveSatisfactionNode(parentNode, *IDs);
		
		if (parentNode != 0) {
			parentNode->addChild(newNode);
		} else {		
			rootNode = newNode;
		}
		
		parentNodes.push_front(newNode);
			
		c->getGoals()->visit(this);
		
		parentNodes.pop_front();
	
	};
	
	void visit_neg_goal(neg_goal * n) {
	
		SatisfactionContainerNode* parentNode;
		
		if (rootNode == 0) { // simple goal at root
			parentNode = 0;
		} else {
			parentNode = parentNodes.front();
		}
		
		NegationNode* newNode = new NegationNode(parentNode, *IDs);
		
		if (parentNode != 0) {
			parentNode->addChild(newNode);
		} else {		
			rootNode = newNode;
		}
		
		parentNodes.push_front(newNode);
			
		n->getGoal()->visit(this);
		
		parentNodes.pop_front();

	};
	
	/**
	*  replace
	*		imply_goal
	*               /       \
	*	antecedent    consequent
	* with
	*	    Disjunction
	*	   /           \
	*	Not	   consequent
	*	 |
	*  antecedent
	*
	*/
	void visit_imply_goal(imply_goal * i) {
	
	
		SatisfactionContainerNode* parentNode;
		
		if (rootNode == 0) { // simple goal at root
			parentNode = 0;
		} else {
			parentNode = parentNodes.front();
		}
		
		DisjunctiveSatisfactionNode* newNode = new DisjunctiveSatisfactionNode(parentNode, *IDs);
		
		if (parentNode != 0) {
			parentNode->addChild(newNode);
		} else {		
			rootNode = newNode;
		}
		
		NegationNode* newNegNode = new NegationNode(newNode, *IDs);
		newNode->addChild(newNegNode);
		
		parentNodes.push_front(newNegNode);
		
			i->getAntecedent()->visit(this);
		
		parentNodes.pop_front();
		
		parentNodes.push_front(newNode);
		
			i->getConsequent()->visit(this);
		
		parentNodes.pop_front();
		
	
	}

	void visit_qfied_goal(qfied_goal * q) {
	
		SatisfactionContainerNode* parentNode;
		
		if (rootNode == 0) { // simple goal at root
			parentNode = 0;
		} else {
			parentNode = parentNodes.front();
		}
		SatisfactionContainerNode* newNode;
		
		if (q->getQuantifier() == E_FORALL) {
			ConjunctiveSatisfactionNode * lnn = new ConjunctiveSatisfactionNode(parentNode, *IDs);
			if (parentNode != 0) {
				parentNode->addChild(lnn);
			} else {		
				rootNode = lnn;
			}
			newNode = lnn;
		} else { // there exists
		
			DisjunctiveSatisfactionNode * lnn = new DisjunctiveSatisfactionNode(parentNode, *IDs);
			if (parentNode != 0) {
				parentNode->addChild(lnn);
			} else {		
				rootNode = lnn;
			}
			newNode = lnn;
		}
		
		
		
		hash_map<parameter_symbol*, Entity*> * old_parameter_symbolToEntity = parameter_symbolToEntity;
		
		parameter_symbolToEntity = new hash_map<parameter_symbol*, Entity*>(*old_parameter_symbolToEntity);
				
		parentNodes.push_front(newNode);
		
		
		all_combinations(q, q->getVars()->begin());

		parentNodes.pop_front();
		
		delete parameter_symbolToEntity;
		
		parameter_symbolToEntity = old_parameter_symbolToEntity;
		
	
	}
	
		
	void all_combinations(qfied_goal * const l, var_symbol_list::const_iterator itr) {
		if (itr == l->getVars()->end()) {
			l->getGoal()->visit(this);
			return;
		}
		
		const Type* currType = (*pddl_typeToType)[(*itr)->type];
		var_symbol_list::const_iterator nextItr = itr;
		nextItr++;
		for (int i = 0; i < currType->getInstanceCount(); i++) {
		
			(*parameter_symbolToEntity)[(*itr)] = currType->getInstance(i);
			all_combinations(l, nextItr);
		}
	
	}
	
	void visit_const_symbol(const_symbol * c) {
		nextArg = -1 - (*const_symbolToConstantIndex)[c];
		nextEnt = 0;
	}

	void visit_var_symbol(var_symbol * c) {
		hash_map<parameter_symbol*, Entity*>::iterator itr = parameter_symbolToEntity->find(c);
		if (itr != parameter_symbolToEntity->end()) {
			nextArg = 0;
			nextEnt = (*itr).second;
		
		} else {
			nextArg = (*parameter_symbolToIndex)[c];
			nextEnt = 0;
		}
	}
};





void PositiveSatisfactionLeaf::flatten(	list<int> & dependencies,
				list<int> & satisfactionCounts,
				list<pair<int, int> > & internalSatisfactionCounts, 
				const int & parentDIndex,
				pair<int, int> * parentDCounters,
				int & nextDIndex,
				bool polarity,
				list<pair<AbstractPredicate, int> > & preconditions,
				list<pair<AbstractPredicate, int> > & negativePreconditions,
				list<bool> & essentialContainers,
				list<bool> & disjunctiveContainers,
				list<bool> & essentialPreconditions,
				list<bool> & essentialNegativePreconditions,
				bool currEssential) {

	if (!polarity) {
		bool newPolarity = !polarity;
		(reinterpret_cast<NegativeSatisfactionLeaf*>(this))->NegativeSatisfactionLeaf::flatten(dependencies, satisfactionCounts, internalSatisfactionCounts, parentDIndex, parentDCounters, nextDIndex, newPolarity, preconditions, negativePreconditions, essentialContainers, disjunctiveContainers, essentialPreconditions, essentialNegativePreconditions, currEssential); 
		return;
	}
			
	if (parentDIndex == -1) {
	
		dependencies.push_back(-1);
		satisfactionCounts.push_back(1);
		internalSatisfactionCounts.push_back(pair<int, int>(1, 0) );
		preconditions.push_back(pair<AbstractPredicate, int>(currPred, 0));
		essentialPreconditions.push_back(currEssential);
		return;
	}
	
	preconditions.push_back(pair<AbstractPredicate, int>(currPred, parentDIndex));
	essentialPreconditions.push_back(currEssential);
	(*parentDCounters).first++;
	
	
}



void NegativeSatisfactionLeaf::flatten(	list<int> & dependencies,
				list<int> & satisfactionCounts,
				list<pair<int, int> > & internalSatisfactionCounts, 
				const int & parentDIndex,
				pair<int, int> * parentDCounters,
				int & nextDIndex,
				bool polarity,
				list<pair<AbstractPredicate, int> > & preconditions,
				list<pair<AbstractPredicate, int> > & negativePreconditions,
				list<bool> & essentialContainers,
				list<bool> & disjunctiveContainers,
				list<bool> & essentialPreconditions,
				list<bool> & essentialNegativePreconditions,
				bool currEssential) {
		
	if (!polarity) {
		bool newPolarity = !polarity;
		(reinterpret_cast<PositiveSatisfactionLeaf*>(this))->PositiveSatisfactionLeaf::flatten(dependencies, satisfactionCounts, internalSatisfactionCounts, parentDIndex, parentDCounters, nextDIndex, newPolarity, preconditions, negativePreconditions, essentialContainers, disjunctiveContainers, essentialPreconditions, essentialNegativePreconditions, currEssential); 
		return;
	}
	if (parentDIndex == -1) {
	
		dependencies.push_back(-1);
		satisfactionCounts.push_back(0);
		internalSatisfactionCounts.push_back(pair<int, int>(0, 1) );
		negativePreconditions.push_back(pair<AbstractPredicate, int>(currPred, 0));
		essentialNegativePreconditions.push_back(currEssential);
		return;
	}
	
	negativePreconditions.push_back(pair<AbstractPredicate, int>(currPred, parentDIndex));
	essentialNegativePreconditions.push_back(currEssential);
	(*parentDCounters).second++;
}




class TypeIdentifier : public SatisfactionTreeVisitor {

private:

	NodeTypes ans;

public:
	TypeIdentifier() : ans(ST_UNDEFINED) {};
	
	void visitPositiveSatisfactionLeaf(SatisfactionLeaf * p) {ans = ST_POSITIVELEAF;};
	void visitNegativeSatisfactionLeaf(SatisfactionLeaf * p) {ans = ST_NEGATIVELEAF;};
	void visitConjunctiveSatisfactionNode(SatisfactionContainerNode * p) {ans = ST_CONJNODE;};
	void visitDisjunctiveSatisfactionNode(SatisfactionContainerNode * p) {ans = ST_DISJNODE;};
	void visitNegationNode(SatisfactionContainerNode * p) {ans = ST_NEGNODE;};
	
	
	NodeTypes getType(SatisfactionNode * n) {
		n->visit(this);
		return ans;
	}
};

class TreeReducer : public SatisfactionTreeVisitor {

private:
	slist<pair<bool, bool> > polarityStack;
	slist<SatisfactionContainerNode *> parentNodeStack;
	SatisfactionNode* newRoot;
	TypeIdentifier ti;

public:
	
	
	void visitPositiveSatisfactionLeaf(SatisfactionLeaf * p);
	void visitNegativeSatisfactionLeaf(SatisfactionLeaf * p);
	
	void visitConjunctiveSatisfactionNode(SatisfactionContainerNode * p);
	void visitDisjunctiveSatisfactionNode(SatisfactionContainerNode * p);	
	
	void visitNegationNode(SatisfactionContainerNode * p);

	TreeReducer() {};
	virtual ~TreeReducer() {};
	
	SatisfactionNode* reduceTree(SatisfactionNode * root) {
		polarityStack.clear();
		parentNodeStack.clear();
		polarityStack.push_front(pair<bool, bool>(true, true));
		parentNodeStack.push_front(0);	
		newRoot = 0;	
		root->visit(this);
		return newRoot;
	}
	
	
	
};

void TreeReducer::visitPositiveSatisfactionLeaf(SatisfactionLeaf * p) {
	//cout << "Visiting +ve leaf" << endl;
	if (!(polarityStack.front().first)) {
		//cout << "- Revisiting as a -ve leaf" << endl;
		polarityStack.push_front(pair<bool, bool>(true, true));
		visitNegativeSatisfactionLeaf(p);
		polarityStack.pop_front();
		return;
	}
	
	SatisfactionContainerNode* parent = parentNodeStack.front();
	PositiveSatisfactionLeaf * newNode = new PositiveSatisfactionLeaf(*p,parent);
	
	if (parent == 0) {
		newRoot = newNode;
	} else {
		parent->addChild(newNode);
	}
	
};

void TreeReducer::visitNegativeSatisfactionLeaf(SatisfactionLeaf * p) {
	//cout << "Visiting -ve leaf" << endl;
	if (!(polarityStack.front().first)) {
		//cout << "Revisiting as a +ve leaf" << endl;
		polarityStack.push_front(pair<bool, bool>(true, true));
		visitPositiveSatisfactionLeaf(p);
		polarityStack.pop_front();
		return;
	}
	
	SatisfactionContainerNode* parent = parentNodeStack.front();
	NegativeSatisfactionLeaf * newNode = new NegativeSatisfactionLeaf(*p,parent);
	
	if (parent == 0) {
		newRoot = newNode;
	} else {
		parent->addChild(newNode);
	}
	
};

void TreeReducer::visitConjunctiveSatisfactionNode(SatisfactionContainerNode * p) {
	//cout << "Visiting conjunction node" << endl;
	if (!(polarityStack.front().first)) {
		//cout << "- Revisiting as a disjunction-of-negative node" << endl;
		polarityStack.push_front(pair<bool, bool>(true, false));
		visitDisjunctiveSatisfactionNode(p);
		polarityStack.pop_front();
		return;
	}
	SatisfactionContainerNode* parent = parentNodeStack.front();
		
	if (parent == 0) {
		ConjunctiveSatisfactionNode* newNode = new ConjunctiveSatisfactionNode(*p, parent);
		newRoot = newNode;
		polarityStack.push_front(pair<bool, bool>(polarityStack.front().second, true));
		parentNodeStack.push_front(newNode);
		
		p->visitChildren(this);
		
		polarityStack.pop_front();
		parentNodeStack.pop_front();
		return;	
	}
	
	if (ti.getType(parent) == ST_CONJNODE) {
		//cout << "Visiting conjunction; parent is also a conjunction" << endl;
		polarityStack.push_front(pair<bool, bool>(polarityStack.front().second, true));
		p->visitChildren(this);
		polarityStack.pop_front();
		return;
	
	}
	
	ConjunctiveSatisfactionNode* newNode = new ConjunctiveSatisfactionNode(*p, parent);
	
	parent->addChild(newNode);
	
	polarityStack.push_front(pair<bool, bool>(polarityStack.front().second, true));
	parentNodeStack.push_front(newNode);
	
	p->visitChildren(this);
	
	polarityStack.pop_front();
	parentNodeStack.pop_front();

};

void TreeReducer::visitDisjunctiveSatisfactionNode(SatisfactionContainerNode * p) {
	//cout << "Visiting disjunction node" << endl;
	if (!(polarityStack.front().first)) {
		//cout << "- Revisiting as a conjunction-of-negative node" << endl;
		polarityStack.push_front(pair<bool, bool>(true, false));
		visitConjunctiveSatisfactionNode(p);
		polarityStack.pop_front();
		return;
	}
	SatisfactionContainerNode* parent = parentNodeStack.front();
	
	if (parent == 0) {
	
		DisjunctiveSatisfactionNode* newNode = new DisjunctiveSatisfactionNode(*p, parent);
		newRoot = newNode;
		polarityStack.push_front(pair<bool, bool>(polarityStack.front().second, true));
		parentNodeStack.push_front(newNode);
	
		p->visitChildren(this);
	
		polarityStack.pop_front();
		parentNodeStack.pop_front();
		return;
	
	}
	
	if (ti.getType(parent) == ST_DISJNODE) {
		//cout << "Visiting disjunction; parent is also disjunction" << endl;
		polarityStack.push_front(pair<bool, bool>(polarityStack.front().second, true));
		p->visitChildren(this);
		polarityStack.pop_front();
		return;
	}
	
	DisjunctiveSatisfactionNode* newNode = new DisjunctiveSatisfactionNode(*p, parent);
	parent->addChild(newNode);
	
	polarityStack.push_front(pair<bool, bool>(polarityStack.front().second, true));
	parentNodeStack.push_front(newNode);
	
	p->visitChildren(this);
	
	polarityStack.pop_front();
	parentNodeStack.pop_front();
	
	

};

void TreeReducer::visitNegationNode(SatisfactionContainerNode * p) {

	bool newPolarity = !polarityStack.front().first;
	
	polarityStack.push_front(pair<bool, bool>(newPolarity, true));
		
	p->visitChildren(this);
	
	polarityStack.pop_front();
	

};

class MakeADLEffectsVisitor : public VisitController {

private:
	bool polarity;
	list<EffectContainerNode*> parentNodes;
	int IDs;
	EffectNode* rootNode;
	
	hash_map<pddl_type*, Type* >* pddl_typeToType;
	
	hash_map<parameter_symbol*, int>* parameter_symbolToIndex;
	hash_map<parameter_symbol*, Entity*> parameter_symbolToEntity;
	
	hash_map<const pred_symbol*, Predicate* > * pred_symbolToPredicate;
	hash_map<const const_symbol*, int> * const_symbolToConstantIndex;
	

public:

	int nextArg;
	Entity* nextEnt;
	
	MakeADLEffectsVisitor() : polarity(true), IDs(0), rootNode(0) {};

	EffectNode* getRoot() {return rootNode; };

	void parseEffects(	effect_lists* topEffect,
			hash_map<pddl_type*, Type* >* const convType,
			hash_map<parameter_symbol*, int> * const conv,
			hash_map<const pred_symbol*, Predicate* > * const predConv,
			hash_map<const const_symbol*, int> * const constConv) {

		
		pddl_typeToType = convType;
		parameter_symbolToIndex = conv;
		parameter_symbolToEntity.clear();
		pred_symbolToPredicate = predConv;
		const_symbolToConstantIndex = constConv;
		
		parentNodes.clear();
		rootNode = 0;
		polarity = true;
		
		topEffect->visit(this);

	}
	
	void addDeletes(AbstractPredicate & seed, int argIndex, ConjunctiveEffectNode* parent) {
	
		if (argIndex == seed.predType->getArgumentCount()) {
			NegativeEffectLeaf* nel = new NegativeEffectLeaf(parent, seed, IDs);
			parent->addChild(nel);
			return;
		}
		int looplimit = seed.predType->getArgumentType(argIndex)->getInstanceCount();
		Entity** instances = seed.predType->getArgumentType(argIndex)->getInstances();
		
		for (int i = 0; i < looplimit; ++i) {
			seed.hardParameters[argIndex] = instances[i];
			addDeletes(seed, argIndex + 1, parent);
		}
		
	
	}
	
	void parseEffectsWithDP(
			effect_lists* topEffect,
			StaticInformation * staticInf,
			hash_map<pddl_type*, Type* >* const convType,
			hash_map<parameter_symbol*, int> * const conv,
			hash_map<const pred_symbol*, Predicate* > * const predConv,
			hash_map<const const_symbol*, int> * const constConv) {

		
		pddl_typeToType = convType;
		parameter_symbolToIndex = conv;
		parameter_symbolToEntity.clear();
		pred_symbolToPredicate = predConv;
		const_symbolToConstantIndex = constConv;
		
		parentNodes.clear();
		rootNode = 0;
		
		ConjunctiveEffectNode* conjRoot = new ConjunctiveEffectNode(0, IDs);
		
		for (int i = 0; i < staticInf->numberOfPredicates; ++i) {
			if (staticInf->Predicates[i]->isDerived()) {
				AbstractPredicate seed(staticInf->Predicates[i]);
				addDeletes(seed, 0, conjRoot);
			}
		}
		
		rootNode = conjRoot;
		
		parentNodes.push_front(conjRoot);
		
		polarity = true;
		
		topEffect->visit(this);
		
		parentNodes.pop_front();

	}
	void visit_effect_lists(effect_lists * s) {
		//cout << "Effect list" << endl;
		int childCount = 0;
		childCount += s->add_effects.size();
		childCount += s->del_effects.size();
		childCount += s->cond_effects.size();
		childCount += s->forall_effects.size();
		
		
		if (childCount > 1) {
		
			EffectContainerNode* parentNode;
			if (rootNode == 0) { // simple goal at root
				parentNode = 0;
			} else {
				parentNode = parentNodes.front();
			}
			ConjunctiveEffectNode * localParent = new ConjunctiveEffectNode(parentNode, IDs);
			if (parentNode == 0) { // simple goal at root
				rootNode = localParent;
			} else {
				parentNode->addChild(localParent);
			}
			parentNodes.push_front(localParent);
		}
		polarity = true;
		s->add_effects.visit(this);
		polarity = false;
		s->del_effects.visit(this);
		polarity = true;
		s->cond_effects.visit(this);
		s->forall_effects.visit(this);
		
		if (childCount > 1) parentNodes.pop_front();
	}
	
	void visit_simple_effect(simple_effect * s) {
		//cout << "Simple effect" << endl;
		const proposition* prop = s->prop;
		Predicate* currPredicate = (*pred_symbolToPredicate)[prop->head];
		AbstractPredicate currAP(currPredicate);
		parameter_symbol_list::iterator itr = prop->args->begin();

		for (int i = 0; itr != prop->args->end(); i++, itr++) {

			parameter_symbol* curr = (*itr);
			curr->visit(this);
			if (nextEnt == 0) {
				currAP.parameters[i] = nextArg;
			} else {
				currAP.hardParameters[i] = nextEnt;
			}

		}
		EffectContainerNode* parentNode;
		
		
		if (rootNode == 0) { // simple goal at root
			parentNode = 0;
		} else {
			parentNode = parentNodes.front();
		}
		
		if (polarity) {
			if (parentNode != 0) {
				parentNode->addChild(new PositiveEffectLeaf(parentNode, currAP, IDs));
			} else {		
				rootNode = new PositiveEffectLeaf(parentNode, currAP, IDs);
			}
		} else {
			if (parentNode != 0) {
				parentNode->addChild(new NegativeEffectLeaf(parentNode, currAP, IDs));
			} else {		
				rootNode = new NegativeEffectLeaf(parentNode, currAP, IDs);
			}
			
		}		
		
	}
	
	

	void visit_forall_effect(forall_effect * q) {
		//cout << "forall effect" << endl;
	
		EffectContainerNode* parentNode;
		
		bool pushed = false;
		
		if (rootNode == 0) { // simple goal at root
			parentNode = 0;
		} else {
			parentNode = parentNodes.front();
		}
		
		if (parentNode == 0) {
			ConjunctiveEffectNode* newNode = new ConjunctiveEffectNode(parentNode, IDs);
			rootNode = newNode;
			
			parentNodes.push_front(newNode);
			pushed = true;
		}

		
		hash_map<parameter_symbol*, Entity*> old_parameter_symbolToEntity = parameter_symbolToEntity;
		
				

		
		
		all_combinations(q, q->getVarsList()->begin());

		if (pushed) parentNodes.pop_front();
		
		parameter_symbolToEntity = old_parameter_symbolToEntity;
		
	
	}
	
		
	void all_combinations(forall_effect * const q, var_symbol_list::const_iterator itr) {
		if (itr == q->getVarsList()->end()) {
			q->getEffects()->visit(this);
			return;
		}
		
		const Type* currType = (*pddl_typeToType)[(*itr)->type];
		var_symbol_list::const_iterator nextItr = itr;
		nextItr++;
		for (int i = 0; i < currType->getInstanceCount(); i++) {
		
			parameter_symbolToEntity[(*itr)] = currType->getInstance(i);
			all_combinations(q, nextItr);
		}
	
	}
	
	void visit_cond_effect(cond_effect * s) {
		//cout << "Conditional Effect" << endl;
		EffectContainerNode* parentNode;
		
		if (rootNode == 0) { // simple goal at root
			parentNode = 0;
		} else {
			parentNode = parentNodes.front();
		}
		
		ConditionalEffectNode* newNode = new ConditionalEffectNode(parentNode, IDs);
		if (parentNode != 0) {
			parentNode->addChild(newNode);
		} else {		
			rootNode = newNode;
		}
		
		MakeADLGoalListVisitor processConditionBranch(IDs);

		hash_map<parameter_symbol*, Entity*> * convEnt = &parameter_symbolToEntity;
				
		processConditionBranch.parseGoal(
			const_cast<goal*>(s->getCondition()),
			pddl_typeToType,
			parameter_symbolToIndex,
			convEnt,
			pred_symbolToPredicate,
			const_symbolToConstantIndex);
		
	
		SatisfactionNode* conditionBranch = processConditionBranch.getRoot();
		
		TreeReducer tr;
		
		SatisfactionNode* reducedRoot = tr.reduceTree(conditionBranch);

		delete conditionBranch;
		
		
		
		newNode->setWhenBranch(reducedRoot);
		
		parentNodes.push_front(newNode);
		
		s->getEffects()->visit(this);
		
		parentNodes.pop_front();
		
	
	}
	
	
	void visit_const_symbol(const_symbol * c) {
		nextArg = -1 - (*const_symbolToConstantIndex)[c];
		nextEnt = 0;
	}

	void visit_var_symbol(var_symbol * c) {
		hash_map<parameter_symbol*, Entity*>::iterator itr = parameter_symbolToEntity.find(c);
		if (itr != parameter_symbolToEntity.end()) {
			nextArg = 0;
			nextEnt = (*itr).second;
		
		} else {
			nextArg = (*parameter_symbolToIndex)[c];
			nextEnt = 0;
		}
	}
};

class MakeSubActionVisitor : public EffectTreeVisitor, public SatisfactionTreeVisitor {

private:

	list<ADLAction*> subActions;
	
	bool dynamic;
	bool identifyingDynamicity;
	
	ADLAction * parentAction;
	
	bool deleteRoot;
	
	
public:
	
	MakeSubActionVisitor(ADLAction * const& paIn) : dynamic(false), identifyingDynamicity(false), parentAction(paIn), deleteRoot(false) {};

	virtual ~MakeSubActionVisitor() {};
	
	bool & isDynamic(EffectNode * satRoot) {
		dynamic = false;
		identifyingDynamicity = true;
		satRoot->visit(this);
		identifyingDynamicity = false;
		return dynamic;
	};

	void MakeSubActions() {
		deleteRoot = false;
		EffectNode* root = parentAction->rootEffectNode;
		//cout << "Visiting root node: ";
		//root->writeDot(cout);
		root->visit(this);
		if (deleteRoot) delete root;
		
		parentAction->subActionCount = subActions.size();
		parentAction->subActions = new ADLAction*[parentAction->subActionCount];
		list<ADLAction*>::iterator itr = subActions.begin();
		for (int i = 0; i < parentAction->subActionCount; i++, itr++) {
			parentAction->subActions[i] = *itr;	
		}
		
		
		
	}
	void visitPositiveSatisfactionLeaf(SatisfactionLeaf * p) {
		
		if (!p->getAbstractPredicate().predType->isStatic()) {
			dynamic = true;
		}
		
		
	};
	
	void visitNegativeSatisfactionLeaf(SatisfactionLeaf * p) {
		if (!p->getAbstractPredicate().predType->isStatic()) {
			dynamic = true;
		}
	};
	
	void visitConjunctiveSatisfactionNode(SatisfactionContainerNode * p) {
		for (list<SatisfactionNode*>::iterator itr = p->getChildren().begin();
			!dynamic && itr != p->getChildren().end();
			itr++) {
			
			(*itr)->visit(this);
					
		}
		
		
	};
	void visitDisjunctiveSatisfactionNode(SatisfactionContainerNode * p) {
		
		for (list<SatisfactionNode*>::iterator itr = p->getChildren().begin();
			!dynamic && itr != p->getChildren().end();
			itr++) {
			
			(*itr)->visit(this);
					
		}

	};
	
	
	void visitConjunctiveEffectNode(ConjunctiveEffectNode * p) {
		
		if (identifyingDynamicity) {
			for (list<EffectNode*>::iterator itr = p->getChildren().begin();
				!dynamic && itr != p->getChildren().end();
				itr++) {
			
				(*itr)->visit(this);
					
			}
			return;
		}
		list<EffectNode*>::iterator itr = p->getChildren().begin();
		list<EffectNode*>::iterator itrPrev = p->getChildren().begin();
		list<EffectNode*>::iterator itrEnd = p->getChildren().end();
		
		while (itr != itrEnd) {
		
			EffectNode* cn = *itr;
				
			if (isDynamic(cn)) {
					//cout << "Found dynamic branch of action " << parentAction->name() << endl;
				
				// child can only be dynamic if it's a when node
				// conj nodes would have been reduced into this node
				// effect nodes aren't dynamic (visitor doesn't do anything...)
				//cn->writeDot(cout);
				
				ConditionalEffectNode* cen = static_cast<ConditionalEffectNode*>(cn);
				
				//cout << "Making sub action" << endl;
				ADLAction* newAct = new ADLAction(*parentAction, cen->getWhenBranch(), cen->getChildren().front());
				//cout << "Made sub action" << endl;
				
				cen->clear();
				
				bool firstThing = (itr == p->getChildren().begin());
				
								
				subActions.push_back(newAct);
				
				delete cn;
				
				//cout << "Calculating its sub actions" << endl;
				
				newAct->createSubActions();
				
				if (firstThing) {
					p->getChildren().pop_front();
					itr = p->getChildren().begin();
					itrPrev = p->getChildren().begin();	
				} else {
					p->getChildren().erase(itr);
					itr = itrPrev;
					itr++;
				}
			} else {
				itrPrev = itr;
				itr++;
			}
					
		}
		
		
		
		
	};
	
	void visitConditionalEffectNode(ConditionalEffectNode * p) {
		//cout << "Investigating whether conditional effect applies" << endl;
		if (identifyingDynamicity) {
			p->getWhenBranch()->visit(this);
			if (!dynamic) p->visitChildren(this);
			return;
		}
		// if we ever get here we have a conditional node at the root - collapse it down!
		
		//should never get here - conditional effects at roots are merged
		cout << "Oh dear, should never get here!" << endl;
		exit(0);
		
	};
	
	virtual void visitPositiveEffectLeaf(EffectLeaf * p) {
		// do nothing
	};
	virtual void visitNegativeEffectLeaf(EffectLeaf * p) {
		// do nothing
	};

	

};

class GroundEffectsVisitor : public EffectTreeVisitor, public SatisfactionTreeVisitor {

private:

	PredicateInstanceList addEffects;
	PredicateInstanceList deleteEffects;
	StaticInformation * staticInf;
	const Entity** argAssignments;
	bool satisfied;
	bool warned;
	
	
	
	inline void warn(PredicateInstance * const tmpPI) {
		if (warned) return;
		cerr << "#WARNING:  Conditional effects contingent on dynamic predicates not supported yet!" << endl;
		cerr << "#       :  Predicate " << tmpPI->getParentPredicate()->name() << " is not static" << endl;
		warned = true;
		
				
	}
public:

	GroundEffectsVisitor(	const Entity** & argAssignmentsIn,
				StaticInformation * staticInfIn)
				: 	staticInf(staticInfIn),
					argAssignments(argAssignmentsIn),
					satisfied(true),
					warned(false) {};
	
	virtual ~GroundEffectsVisitor() {
		addEffects.clear();
		deleteEffects.clear();
	};
	
	
	void groundEffects(EffectNode* rootEffectNode) {
	
		addEffects.clear();
		deleteEffects.clear();
		satisfied = true;
		
		rootEffectNode->visit(this);
					
	}
	
	PredicateInstanceList & getAddEffects() { return addEffects; };
	PredicateInstanceList & getDeleteEffects() { return deleteEffects; };
	
	PredicateInstance* groundAbstractPredicate(const AbstractPredicate & p) {
		int loopLimit = p.predType->getArgumentCount();
		const Entity** entities = new const Entity*[loopLimit];
		
		for (int i = 0; i < p.predType->getArgumentCount(); i++) {
			if (p.hardParameters[i] != 0) {
				entities[i] = p.hardParameters[i];
			} else {
				int cp = p.parameters[i];
				if (cp < 0) {
					entities[i] = staticInf->Constants[-1 - cp];
				} else {
					entities[i] = argAssignments[cp];
				}
			}
		}
		PredicateInstance* newPI = new PredicateInstance(p.predType, const_cast<Entity**>(entities));
		
		delete [] entities;
		
		return newPI;
	}
	bool & isSatisfied(SatisfactionNode * satRoot) {
		satisfied = true;
		satRoot->visit(this);
		return satisfied;
	};
	
	void visitPositiveSatisfactionLeaf(SatisfactionLeaf * p) {
		//cout << "Visiting positive satisfaction leaf" << endl;
		PredicateInstance * tmpPI = groundAbstractPredicate(p->getAbstractPredicate());
		//cout << "Searching for " << *tmpPI << endl;
		satisfied = staticInf->StaticInitialState->containsPredicateInstance(tmpPI);
		if (!tmpPI->getParentPredicate()->isStatic()) {
			warn(tmpPI);
		}
		delete tmpPI;
		
	};
	
	void visitNegativeSatisfactionLeaf(SatisfactionLeaf * p) {
		//cout << "Visiting negative satisfaction leaf" << endl;
		PredicateInstance * tmpPI = groundAbstractPredicate(p->getAbstractPredicate());
		//cout << "Searching for �(" << *tmpPI << ")" << endl;
		satisfied = !staticInf->StaticInitialState->containsPredicateInstance(tmpPI);
		if (!tmpPI->getParentPredicate()->isStatic()) {
			warn(tmpPI);
		}
		delete tmpPI;

	};
	
	void visitConjunctiveSatisfactionNode(SatisfactionContainerNode * p) {
		satisfied = true;
		for (list<SatisfactionNode*>::iterator itr = p->getChildren().begin();
			satisfied && itr != p->getChildren().end();
			itr++) {
			
			(*itr)->visit(this);
					
		}
		
		
	};
	void visitDisjunctiveSatisfactionNode(SatisfactionContainerNode * p) {
		
		satisfied = false;
		for (list<SatisfactionNode*>::iterator itr = p->getChildren().begin();
			!satisfied && itr != p->getChildren().end();
			itr++) {
			
			(*itr)->visit(this);
					
		}

	};
	
	
	void visitPositiveEffectLeaf(EffectLeaf * p) {
		addEffects.push_back(groundAbstractPredicate(p->getAbstractPredicate()));
		
		//cout << "Add Effect: " << *(addEffects.back()) << endl;
	};
	
	void visitNegativeEffectLeaf(EffectLeaf * p) {
		deleteEffects.push_back(groundAbstractPredicate(p->getAbstractPredicate()));
		//cout << "Delete Effect: " << *(deleteEffects.back()) << endl;
	};
	
	void visitConjunctiveEffectNode(ConjunctiveEffectNode * p) {
		p->visitChildren(this);
	};
	void visitConditionalEffectNode(ConditionalEffectNode * p) {
		//cout << "Investigating whether conditional effect applies" << endl;
		if (isSatisfied(p->getWhenBranch())) {
			/*cout << "88888888 Conditional branch applies:" << endl;
			p->getWhenBranch()->write(cout);
			cout << endl;*/
			//cout << "Conditional effects: " << endl;
			p->visitChildren(this);		
			//cout << "End of conditional effects" << endl;
		} else {
			//cout << "88888888 Conditional branch doesn't apply" << endl;
		}
	};
	
	
	
		

};

class MergeConditions : public EffectTreeVisitor, SatisfactionTreeVisitor {

private:
	
	SatisfactionNode* existingConditions;
	SatisfactionNode** destinationSatNode;
	EffectNode** destinationEffectNode;
	
	bool addingMode;
	ConjunctiveSatisfactionNode* tSatRoot;
	
public:

	bool allMerged;
	
	MergeConditions(SatisfactionNode** intoIn, EffectNode** destNodeIn, SatisfactionNode* snodeIn, EffectNode* enodeIn)
	  : existingConditions(snodeIn),
	    destinationSatNode(intoIn),
	    destinationEffectNode(destNodeIn),
	    addingMode(false),
	    allMerged(true) {
	
		enodeIn->visit(this);
		if (!allMerged) {
			delete enodeIn;
		}
	    
	}
	
	void visitPositiveSatisfactionLeaf(SatisfactionLeaf * p) {
		if (addingMode) {
			tSatRoot->addChild(static_cast<PositiveSatisfactionLeaf*>(p));
			p->setParent(tSatRoot);
			return;	
		}
	};
	
	void visitNegativeSatisfactionLeaf(SatisfactionLeaf * p) {
		if (addingMode) {
			tSatRoot->addChild(static_cast<NegativeSatisfactionLeaf*>(p));
			p->setParent(tSatRoot);
			return;	
		}
	};
	
	void visitConjunctiveSatisfactionNode(SatisfactionContainerNode * p) {
		if (addingMode) {
			tSatRoot->addChild(static_cast<ConjunctiveSatisfactionNode*>(p));
			p->setParent(tSatRoot);
			return;	
		}
	};
	
	void visitDisjunctiveSatisfactionNode(SatisfactionContainerNode * p) {
		if (addingMode) {
			tSatRoot->addChild(static_cast<DisjunctiveSatisfactionNode*>(p));
			p->setParent(tSatRoot);
			return;	
		}
	};
	
	
	void visitPositiveEffectLeaf(EffectLeaf * p) {
		
		*destinationEffectNode = p;
		*destinationSatNode = existingConditions;
	};
	
	void visitNegativeEffectLeaf(EffectLeaf * p) {
		
		*destinationEffectNode = p;
		*destinationSatNode = existingConditions;
	};
	
	void visitConjunctiveEffectNode(ConjunctiveEffectNode * p) {
		
		*destinationEffectNode = p;
		*destinationSatNode = existingConditions;
	};
	
	void visitConditionalEffectNode(ConditionalEffectNode * p) {
		
		allMerged = false;
		int IDs = 0;
		
		tSatRoot = new ConjunctiveSatisfactionNode(0, IDs);
		addingMode = true;
		
		existingConditions->visit(this);
		p->getWhenBranch()->visit(this);
		
		addingMode = false;
		
		*destinationEffectNode = p->getChildren().front();
		
		p->clear();
		
		
		
		TreeReducer tReduce;
		
		SatisfactionNode* newRoot = tReduce.reduceTree(tSatRoot);
		delete tSatRoot;
		*destinationSatNode = newRoot;
		
		
		
		
		
	};
	
};


class StaticCheckVisitor : public EffectTreeVisitor {

private:

	bool containsPred;
	const Predicate* currPred;
	
	
public:

	StaticCheckVisitor() : containsPred(false) {};
	
	virtual ~StaticCheckVisitor() {
	
	};
	
	bool containsPredicate(const Predicate * p, EffectNode* rootEffectNode) {
	
		currPred = p;
		rootEffectNode->visit(this);
		return containsPred;
		
	};

	
	void visitPositiveEffectLeaf(EffectLeaf * p) {
		if (p->getAbstractPredicate().predType == currPred) {
			//cout << "Found effect match for " << currPred->name() << endl;
		} else {
			//cout << "> " << currPred->name() << " does not match " << p->getAbstractPredicate().predType->name() << endl;
		}
		containsPred = containsPred || (p->getAbstractPredicate().predType == currPred);
	};
	
	void visitNegativeEffectLeaf(EffectLeaf * p) {
		if (p->getAbstractPredicate().predType == currPred) {
			//cout << "Found effect match for " << currPred->name() << endl;
		} else {
			//cout << "> " << currPred->name() << " does not match " << p->getAbstractPredicate().predType->name() << endl;
		}
		containsPred = containsPred || (p->getAbstractPredicate().predType == currPred);
	};
	
	void visitConjunctiveEffectNode(ConjunctiveEffectNode * p) {
		//cout << "> Visiting container node" << endl;
		p->visitChildren(this);
	};
	void visitConditionalEffectNode(ConditionalEffectNode * p) {
		//cout << "> Visiting container node" << endl;
		p->visitChildren(this);		
		
	};
	
	
	
		

};


class AddCheckVisitor : public EffectTreeVisitor {

private:

	const Predicate* currPred;
	list<AbstractPredicate> toReturn;
	
public:

	AddCheckVisitor() {};
	
	virtual ~AddCheckVisitor() {
	
	};
	
	list<AbstractPredicate> containsPredicate(const Predicate * p, EffectNode* rootEffectNode) {
	
		currPred = p;
		rootEffectNode->visit(this);
		return toReturn;
		
	};

	
	void visitPositiveEffectLeaf(EffectLeaf * p) {
		if (p->getAbstractPredicate().predType == currPred) {
			toReturn.push_back(p->getAbstractPredicate());
		}
	};
	
	void visitNegativeEffectLeaf(EffectLeaf * p) {
		
	};
	
	void visitConjunctiveEffectNode(ConjunctiveEffectNode * p) {
		//cout << "> Visiting container node" << endl;
		p->visitChildren(this);
	};
	void visitConditionalEffectNode(ConditionalEffectNode * p) {
		
		
	};
	
	
	
		

};

class DeleteCheckVisitor : public EffectTreeVisitor {

private:

	const Predicate* currPred;
	list<AbstractPredicate> toReturn;
	
public:

	DeleteCheckVisitor() {};
	
	virtual ~DeleteCheckVisitor() {
	
	};
	
	list<AbstractPredicate> containsPredicate(const Predicate * p, EffectNode* rootEffectNode) {
	
		currPred = p;
		rootEffectNode->visit(this);
		return toReturn;
		
	};

	
	void visitPositiveEffectLeaf(EffectLeaf * p) {
		
	};
	
	void visitNegativeEffectLeaf(EffectLeaf * p) {
		if (p->getAbstractPredicate().predType == currPred) {
			toReturn.push_back(p->getAbstractPredicate());
		}
	};
	
	void visitConjunctiveEffectNode(ConjunctiveEffectNode * p) {
		//cout << "> Visiting container node" << endl;
		p->visitChildren(this);
	};
	void visitConditionalEffectNode(ConditionalEffectNode * p) {
			
		
	};
	
	
	
		

};

ADLAction::ADLAction(operator_ * & o,
	hash_map<pddl_type*, Type* > & pddl_typeToType,
	hash_map<const pred_symbol*, Predicate* > & pred_symbolToPredicate,
	hash_map<const const_symbol*, int > & const_symbolToConstantIndex,
	Entity** constantsIn,
	EntitySet** constantDomainsIn,
	StaticInformation* staticInfIn)
	: Action(o, pddl_typeToType, pred_symbolToPredicate, const_symbolToConstantIndex, constantsIn, constantDomainsIn, true, o->parameters->size()), subActionCount(0), subActions(0),  isRootAction(true), staticInf(staticInfIn) {
	
	

	var_symbol_list* parameters = o->parameters;
	//parameterCount = parameters->size();
	//parameterTypes = new Type*[parameterCount];
	parameterDomains = new EntitySet*[parameterCount];
	ownParameterDomains = new bool[parameterCount];
	/*
	jointParameterDomainConstraints = new hash_map<Entity*, EntitySet**>*[parameterCount];
	

	for (int i = 0; i < parameterCount; i++) {
	
		hash_map<Entity*, EntitySet**> * eHash = new hash_map<Entity*, EntitySet**>();
		
		jointParameterDomainConstraints[i] = eHash;
		
	}
	*/
	hash_map<parameter_symbol*, int> parameter_symbolToIndex;
	
	var_symbol_list::const_iterator itr = parameters->begin();

	for (int i = 0; itr != parameters->end(); itr++, i++) {

		parameterTypes[i] = pddl_typeToType[(*itr)->type];
		parameter_symbolToIndex[(*itr)] = i;
		parameterDomains[i] = 0;
		ownParameterDomains[i] = false;
	}

	jointParameterDomainConstraints = new hash_map<Entity*, EntitySet**>*[parameterCount];
	
		
	for (int i = 0; i < parameterCount; i++) {
	
		hash_map<Entity*, EntitySet**> * eHash = new hash_map<Entity*, EntitySet**>();
		
		jointParameterDomainConstraints[i] = eHash;
		
	}
	MakeADLGoalListVisitor vis;
	hash_map<pddl_type*, Type* > * t1 = &pddl_typeToType;
	hash_map<parameter_symbol*, int> * t2 = &parameter_symbolToIndex;
	hash_map<const pred_symbol*, Predicate* > * t3 = &pred_symbolToPredicate;
	hash_map<const const_symbol*, int > * t4 = &const_symbolToConstantIndex;
	
	vis.parseGoal(	o->precondition,
			t1, t2, t3, t4);
			
	SatisfactionNode* rootNode = vis.getRoot();
	
	TreeReducer tr;
	
	SatisfactionNode* reducedRoot = tr.reduceTree(rootNode);

	delete rootNode;
	
	MakeADLEffectsVisitor effVis;

	if (staticInf->hasDerivedPredicates) {			
		effVis.parseEffectsWithDP(o->effects, staticInf, t1, t2, t3, t4);
	} else {
		effVis.parseEffects(o->effects, t1, t2, t3, t4);
	}
	
	EffectNode* effNode = effVis.getRoot();
	
	bool allMerged = false;
	while (!allMerged) {
		SatisfactionNode* newSatRoot;
		EffectNode* newEffRoot;
	
		MergeConditions mg(&newSatRoot, &newEffRoot, reducedRoot, effNode);
	
		reducedRoot = newSatRoot;
		effNode = newEffRoot;
		allMerged = mg.allMerged;
		
	}
	
	rootEffectNode = effNode;
		
	fstream dotWrite;
	string filename = "testdot-preconditions-";
	filename += name();
	filename += ".dot";
	
	//dotWrite.open(filename.data(), ios_base::out);
	//reducedRoot->saveDot(dotWrite);
	//dotWrite.close();
	
	//cout << endl;

	list<int> dependencies;
	list<int> satisfactionCounts;
	list<pair<int, int> > internalSatisfactionCounts;
	list<pair<AbstractPredicate, int> > preconditionsList;
	list<pair<AbstractPredicate, int> > negativePreconditionsList;
	
	list<bool> essentialContainerList;
	list<bool> disjunctiveContainerList;
	list<bool> essentialPreconditionsList;
	list<bool> essentialNegativePreconditionsList;
	
	
	int parentDIndex = -1;
	pair<int, int>* parentDCounters = 0;
	int nextDIndex = 0;		

	reducedRoot->flatten(dependencies, satisfactionCounts, internalSatisfactionCounts, parentDIndex, parentDCounters, nextDIndex, true, preconditionsList, negativePreconditionsList, essentialContainerList, disjunctiveContainerList, essentialPreconditionsList, essentialNegativePreconditionsList, true);
	
	preconditionCount = preconditionsList.size();
	negativePreconditionCount = negativePreconditionsList.size();
	
	preconditions = new AbstractPredicate[preconditionCount];
	preconditionsUpdate = new int[preconditionCount];
	essentialPrecondition = new bool[preconditionCount];
		
	negativePreconditions = new AbstractPredicate[negativePreconditionCount];
	negativePreconditionsUpdate = new int[negativePreconditionCount];
	essentialNegativePrecondition = new bool[negativePreconditionCount];
	
	containerCount = dependencies.size();
	
	subContainers = new list<int>[containerCount];
	subPreconditions = new list<int>[containerCount];
	subNegativePreconditions = new list<int>[containerCount];
	
	containersUpdate = new int[containerCount];
	essentialContainer = new bool[containerCount];
	disjunctiveContainer = new bool[containerCount];
	defaultSatisfactionCounters = new int[containerCount];
	
	int i = 0;
	for (	list<int>::iterator itr = satisfactionCounts.begin();
		itr != satisfactionCounts.end();
		itr++, i++) {
	
		defaultSatisfactionCounters[i] = *itr;
	}
	i = 0;
	list<bool>::iterator sItr = essentialContainerList.begin();
	list<bool>::iterator dcItr = disjunctiveContainerList.begin();
	for (	list<int>::iterator itr = dependencies.begin();
		itr != dependencies.end();
		++sItr, ++dcItr, ++itr, ++i) {
	
		containersUpdate[i] = *itr;
		essentialContainer[i] = *sItr;
		disjunctiveContainer[i] = *dcItr;
		if (*itr != -1) subContainers[*itr].push_back(i);
	}
	i = 0;
	
	sItr = essentialPreconditionsList.begin();
	
	for ( 	list<pair<AbstractPredicate, int> >::iterator itr = preconditionsList.begin();
		itr != preconditionsList.end();
		sItr++, itr++, i++) {
		
		preconditions[i] = (*itr).first;
		preconditionsUpdate[i] = (*itr).second;
		essentialPrecondition[i] = (*sItr);
		subPreconditions[(*itr).second].push_back(i);
		
	}
	i = 0;
	sItr = essentialNegativePreconditionsList.begin();
	for ( 	list<pair<AbstractPredicate, int> >::iterator itr = negativePreconditionsList.begin();
		itr != negativePreconditionsList.end();
		sItr++, itr++, i++) {
		
		negativePreconditions[i] = (*itr).first;
		negativePreconditionsUpdate[i] = (*itr).second;
		essentialNegativePrecondition[i] = (*sItr);
		subNegativePreconditions[(*itr).second].push_back(i);
	}
	
	
	
	filename = "testdot-effects-";
	filename += name();
	filename += ".dot";
	
///	dotWrite.open(filename.data(), ios_base::out);
///	rootEffectNode->saveDot(dotWrite);
///	dotWrite.close();
	
	delete reducedRoot;
	
		
};

ADLAction::ADLAction(derivation_rule * & o,
	hash_map<pddl_type*, Type* > & pddl_typeToType,
	hash_map<const pred_symbol*, Predicate* > & pred_symbolToPredicate,
	hash_map<const const_symbol*, int > & const_symbolToConstantIndex,
	Entity** constantsIn,
	EntitySet** constantDomainsIn,
	StaticInformation* staticInfIn)
	: Action(o, pddl_typeToType, pred_symbolToPredicate, const_symbolToConstantIndex, constantsIn, constantDomainsIn, true, o->get_head()->args->size()), subActionCount(0), subActions(0),  isRootAction(true), staticInf(staticInfIn) {
	
	Predicate* introduces = pred_symbolToPredicate[o->get_head()->head];
	
	introduces->setDerived();
	
	parameter_symbol_list* parameters = o->get_head()->args;
	//parameterCount = parameters->size();
	
	//parameterTypes = new Type*[parameterCount];
	parameterDomains = new EntitySet*[parameterCount];
	ownParameterDomains = new bool[parameterCount];
	
	hash_map<parameter_symbol*, int> parameter_symbolToIndex;
	
	parameter_symbol_list::const_iterator itr = parameters->begin();

	for (int i = 0; itr != parameters->end(); itr++, i++) {

		parameterTypes[i] = pddl_typeToType[(*itr)->type];
		parameter_symbolToIndex[(*itr)] = i;
		parameterDomains[i] = 0;
		ownParameterDomains[i] = false;
	}

	jointParameterDomainConstraints = new hash_map<Entity*, EntitySet**>*[parameterCount];
	

	for (int i = 0; i < parameterCount; i++) {
	
		hash_map<Entity*, EntitySet**> * eHash = new hash_map<Entity*, EntitySet**>();
		
		jointParameterDomainConstraints[i] = eHash;
		
	}
	
	AbstractPredicate theEffect(introduces);
	
	for (int i = 0; i < parameterCount; ++i) {
			theEffect.parameters[i] = i;
	}
	
	MakeADLGoalListVisitor vis;
	hash_map<pddl_type*, Type* > * t1 = &pddl_typeToType;
	hash_map<parameter_symbol*, int> * t2 = &parameter_symbolToIndex;
	hash_map<const pred_symbol*, Predicate* > * t3 = &pred_symbolToPredicate;
	hash_map<const const_symbol*, int > * t4 = &const_symbolToConstantIndex;
	
			
	vis.parseDPGoal(o->get_body(), theEffect,
			t1, t2, t3, t4);

	SatisfactionNode* rootNode = vis.getRoot();
	
	TreeReducer tr;
	
	SatisfactionNode* reducedRoot = tr.reduceTree(rootNode);

	delete rootNode;
	
	int dummyID = 0;
	
	
	
	rootEffectNode = new PositiveEffectLeaf(0, theEffect, dummyID);
	
	fstream dotWrite;
	string filename = "testdot-";
	filename += name();
	filename += ".dot";
	
	//dotWrite.open(filename.data(), ios_base::out);
	//reducedRoot->saveDot(dotWrite);
	//dotWrite.close();
	
	//cout << endl;

	list<int> dependencies;
	list<int> satisfactionCounts;
	list<pair<int, int> > internalSatisfactionCounts;
	list<pair<AbstractPredicate, int> > preconditionsList;
	list<pair<AbstractPredicate, int> > negativePreconditionsList;
	
	list<bool> essentialContainerList;
	list<bool> disjunctiveContainerList;
	list<bool> essentialPreconditionsList;
	list<bool> essentialNegativePreconditionsList;
	
	
	int parentDIndex = -1;
	pair<int, int>* parentDCounters = 0;
	int nextDIndex = 0;		

	reducedRoot->flatten(dependencies, satisfactionCounts, internalSatisfactionCounts, parentDIndex, parentDCounters, nextDIndex, true, preconditionsList, negativePreconditionsList, essentialContainerList, disjunctiveContainerList, essentialPreconditionsList, essentialNegativePreconditionsList, true);
	
	preconditionCount = preconditionsList.size();
	negativePreconditionCount = negativePreconditionsList.size();
	
	preconditions = new AbstractPredicate[preconditionCount];
	preconditionsUpdate = new int[preconditionCount];
	essentialPrecondition = new bool[preconditionCount];
		
	negativePreconditions = new AbstractPredicate[negativePreconditionCount];
	negativePreconditionsUpdate = new int[negativePreconditionCount];
	essentialNegativePrecondition = new bool[negativePreconditionCount];
	
	containerCount = dependencies.size();
	
	subContainers = new list<int>[containerCount];
	subPreconditions = new list<int>[containerCount];
	subNegativePreconditions = new list<int>[containerCount];
	
	containersUpdate = new int[containerCount];
	essentialContainer = new bool[containerCount];
	disjunctiveContainer = new bool[containerCount];
	defaultSatisfactionCounters = new int[containerCount];
	
	int i = 0;
	for (	list<int>::iterator itr = satisfactionCounts.begin();
		itr != satisfactionCounts.end();
		itr++, i++) {
	
		defaultSatisfactionCounters[i] = *itr;
	}
	i = 0;
	list<bool>::iterator sItr = essentialContainerList.begin();
	list<bool>::iterator dcItr = disjunctiveContainerList.begin();
	for (	list<int>::iterator itr = dependencies.begin();
		itr != dependencies.end();
		++sItr, ++dcItr, ++itr, ++i) {
	
		containersUpdate[i] = *itr;
		essentialContainer[i] = *sItr;
		disjunctiveContainer[i] = *dcItr;
		if (*itr != -1) subContainers[*itr].push_back(i);
	}
	i = 0;
	
	sItr = essentialPreconditionsList.begin();
	
	for ( 	list<pair<AbstractPredicate, int> >::iterator itr = preconditionsList.begin();
		itr != preconditionsList.end();
		sItr++, itr++, i++) {
		
		preconditions[i] = (*itr).first;
		preconditionsUpdate[i] = (*itr).second;
		essentialPrecondition[i] = (*sItr);
		subPreconditions[(*itr).second].push_back(i);
		
	}
	i = 0;
	sItr = essentialNegativePreconditionsList.begin();
	for ( 	list<pair<AbstractPredicate, int> >::iterator itr = negativePreconditionsList.begin();
		itr != negativePreconditionsList.end();
		sItr++, itr++, i++) {
		
		negativePreconditions[i] = (*itr).first;
		negativePreconditionsUpdate[i] = (*itr).second;
		essentialNegativePrecondition[i] = (*sItr);
		subNegativePreconditions[(*itr).second].push_back(i);
	}
	delete reducedRoot;
	
	
	
	
	
		
};


ADLAction::ADLAction(const ADLAction & toCopy,
		  SatisfactionNode * reducedRoot,
		  EffectNode * effNode) : Action(toCopy.constants, toCopy.constantDomains, toCopy.parameterCount, toCopy.parameterTypes), subActionCount(0), subActions(0), staticInf(toCopy.staticInf) {
	
	//cout << "Making ADL Sub Action " << endl;
	//parameterCount = toCopy.parameterCount;
	//parameterTypes = toCopy.parameterTypes;
	parameterDomains = toCopy.parameterDomains;
	ownParameterDomains = toCopy.ownParameterDomains;
	jointParameterDomainConstraints = toCopy.jointParameterDomainConstraints;
	
	borrowedParameterDomains = true;
	isRootAction = false;

	costs = false;
	
	definedName = toCopy.name();
	ownName = true;
	
	definedName += "-sub";

	bool allMerged = false;	
	while (!allMerged) {
		SatisfactionNode* newSatRoot;
		EffectNode* newEffRoot;
	
		MergeConditions mg(&newSatRoot, &newEffRoot, reducedRoot, effNode);
	
		reducedRoot = newSatRoot;
		effNode = newEffRoot;
	
		allMerged = mg.allMerged;	
	};
	
	rootEffectNode = effNode;
	
	
	
	list<int> dependencies;
	list<int> satisfactionCounts;
	list<pair<int, int> > internalSatisfactionCounts;
	list<pair<AbstractPredicate, int> > preconditionsList;
	list<pair<AbstractPredicate, int> > negativePreconditionsList;
	
	list<bool> essentialContainerList;
	list<bool> disjunctiveContainerList;
	list<bool> essentialPreconditionsList;
	list<bool> essentialNegativePreconditionsList;
	
	
	int parentDIndex = -1;
	pair<int, int>* parentDCounters = 0;
	int nextDIndex = 0;		

	
	
	
	
	reducedRoot->flatten(dependencies, satisfactionCounts, internalSatisfactionCounts, parentDIndex, parentDCounters, nextDIndex, true, preconditionsList, negativePreconditionsList, essentialContainerList, disjunctiveContainerList, essentialPreconditionsList, essentialNegativePreconditionsList, true);
	
	preconditionCount = preconditionsList.size();
	negativePreconditionCount = negativePreconditionsList.size();
	
	preconditions = new AbstractPredicate[preconditionCount];
	preconditionsUpdate = new int[preconditionCount];
	essentialPrecondition = new bool[preconditionCount];
	
	negativePreconditions = new AbstractPredicate[negativePreconditionCount];
	negativePreconditionsUpdate = new int[negativePreconditionCount];
	essentialNegativePrecondition = new bool[negativePreconditionCount];
	
	containerCount = dependencies.size();
	
	subContainers = new list<int>[containerCount];
	subPreconditions = new list<int>[containerCount];
	subNegativePreconditions = new list<int>[containerCount];
	
	containersUpdate = new int[containerCount];
	essentialContainer = new bool[containerCount];
	disjunctiveContainer = new bool[containerCount];
	defaultSatisfactionCounters = new int[containerCount];
	
	int i = 0;
	for (	list<int>::iterator itr = satisfactionCounts.begin();
		itr != satisfactionCounts.end();
		itr++, i++) {
	
		defaultSatisfactionCounters[i] = *itr;
	}
	i = 0;
	list<bool>::iterator sItr = essentialContainerList.begin();
	list<bool>::iterator dcItr = disjunctiveContainerList.begin();
	for (	list<int>::iterator itr = dependencies.begin();
		itr != dependencies.end();
		++sItr, ++dcItr, ++itr, ++i) {
	
		containersUpdate[i] = *itr;
		essentialContainer[i] = *sItr;
		disjunctiveContainer[i] = *dcItr;
		if (*itr != -1) subContainers[*itr].push_back(i);
	}
	i = 0;
	
	sItr = essentialPreconditionsList.begin();
	
	for ( 	list<pair<AbstractPredicate, int> >::iterator itr = preconditionsList.begin();
		itr != preconditionsList.end();
		sItr++, itr++, i++) {
		
		preconditions[i] = (*itr).first;
		preconditionsUpdate[i] = (*itr).second;
		essentialPrecondition[i] = (*sItr);
		subPreconditions[(*itr).second].push_back(i);
		
	}
	i = 0;
	sItr = essentialNegativePreconditionsList.begin();
	for ( 	list<pair<AbstractPredicate, int> >::iterator itr = negativePreconditionsList.begin();
		itr != negativePreconditionsList.end();
		sItr++, itr++, i++) {
		
		negativePreconditions[i] = (*itr).first;
		negativePreconditionsUpdate[i] = (*itr).second;
		essentialNegativePrecondition[i] = (*sItr);
		subNegativePreconditions[(*itr).second].push_back(i);
	}
	
	//cout << "Created sub action; preconditions:" << endl;
	//reducedRoot->writeDot(cout);
	
	//cout << "Effects:" << endl;
	//rootEffectNode->writeDot(cout);
	
	//cout << containerCount << " containers" << endl;
	//cout << "Default at root = " << defaultSatisfactionCounters[0] << endl;
	
	
	
	delete reducedRoot;
	
		
};


void ADLAction::createEffectsArrays(	const Entity** & argAssignments,
					PredicateInstance** & destAddEffectArray,
				 	int & destAddEffectCount,
				 	PredicateInstance** & destDeleteEffectArray,
				 	int & destDeleteEffectCount) const {
					
	
	GroundEffectsVisitor geVis(argAssignments, staticInf);
	
	geVis.groundEffects(rootEffectNode);
	
	destAddEffectCount = geVis.getAddEffects().size();
	destAddEffectArray = new PredicateInstance*[destAddEffectCount];
	
	int i = 0;
	for (PredicateInstanceList::iterator itr = geVis.getAddEffects().begin();
		itr != geVis.getAddEffects().end();
		itr++, i++) {
	
		destAddEffectArray[i] = (*itr);
		destAddEffectArray[i]->incrUsage();
			
	}
	
	destDeleteEffectCount = geVis.getDeleteEffects().size();
	destDeleteEffectArray = new PredicateInstance*[destDeleteEffectCount];
	
	i = 0;
	for (PredicateInstanceList::iterator itr = geVis.getDeleteEffects().begin();
		itr != geVis.getDeleteEffects().end();
		itr++, i++) {
	
		destDeleteEffectArray[i] = (*itr);
		destDeleteEffectArray[i]->incrUsage();
			
	}
	
};

void ADLAction::calculateParameterDomains(StaticInformation * staticInf) {

	for (int i = 0; i < parameterCount; i++) {
		if (ownParameterDomains[i]) {
			ownParameterDomains[i] = false;
			delete parameterDomains[i];
			parameterDomains[i] = 0;
		}
	}
	
	for (int i = 0; i < preconditionCount; i++) {
		const Predicate* currPred = preconditions[i].predType;
		if (currPred->isStatic() && isEssentialPrecondition(i)) {
			int currPredArgCount = currPred->getArgumentCount();
			if (staticInf->verbose) {
				cout << "Processing precondition " << i << " of type " << currPred->name() << endl;
			}
			EntitySet** domains = new EntitySet*[currPred->getArgumentCount()];
			
			for (int j = 0; j < currPredArgCount; j++) {
				if (preconditions[i].hardParameters[j] == 0 && preconditions[i].parameters[j] >= 0) {
					domains[j] = new EntitySet();
				}
			}
				  
			State::const_iterator itr = staticInf->StaticInitialState->begin();
			State::const_iterator itrEnd = staticInf->StaticInitialState->end();

			for (; itr != itrEnd; itr++) {
				
				PredicateInstance* curr = (*itr);
				
				if (curr->isOfType(currPred)) {
				
					//cout << *curr << " in initial state is of the right type" << endl;
					
					bool constantsMatch = true;
					for (int j = 0; constantsMatch && j < currPred->getArgumentCount(); j++) {
						if (preconditions[i].hardParameters[j] != 0 || preconditions[i].parameters[j] < 0) {
							Parameter* param =  curr->getParameter(j);
							Entity* toPush = param->boundTo();
							Entity* toMatch;
							if (preconditions[i].hardParameters[j] != 0) {
								toMatch = preconditions[i].hardParameters[j];
							} else {
								toMatch = constants[-1-preconditions[i].parameters[j]];
							}
							constantsMatch = (toMatch == toPush);
							
						}
					
					}

					if (constantsMatch) {
						for (int j = 0; j < currPred->getArgumentCount(); j++) {
						
							if (preconditions[i].hardParameters[j] == 0 && preconditions[i].parameters[j] >= 0) {
								Parameter* param =  curr->getParameter(j);
								Entity* toPush = param->boundTo();
						
								domains[j]->insert(toPush);	
							}
						
						}
					}
				
				}
				
			}
			
			
			 
			for (int j = 0; j < currPred->getArgumentCount(); j++) {
		
				
				if (preconditions[i].hardParameters[j] == 0) {
					int argRef = preconditions[i].parameters[j];
					 
					if (argRef >= 0) {
				 
						if (!ownParameterDomains[argRef]) {
						
							if (staticInf->verbose) {
								cout << "Domain for argument " << argRef << " previously unrestricted" << endl;
							}
							ownParameterDomains[argRef] = true;
							
							parameterDomains[argRef] = domains[j]; 
							if (staticInf->verbose) {
								cout << "Domain for argument " << argRef << " now:" << endl;
								
								for (EntitySet::iterator esi = parameterDomains[argRef]->begin(); esi != parameterDomains[argRef]->end(); esi++) {
									cout << " " << (*esi)->name();
									
								}
								cout << endl;
							}
												
						} else {
							EntitySet* newES = intersection(domains[j], parameterDomains[argRef]);
							
							delete domains[j];
							delete parameterDomains[argRef];
							
							parameterDomains[argRef] = newES;
							
							if (staticInf->verbose) {
								cout << "Domain for argument " << argRef << " now:" << endl;
								
								for (EntitySet::iterator esi = parameterDomains[argRef]->begin(); esi != parameterDomains[argRef]->end(); esi++) {
									cout << " " << (*esi)->name();
									
								}
								cout << endl;
							}
	
							
						}
					}
				}
			}
			 
			delete [] domains;
			
		}
	}
	
	

};

void ADLAction::calculateStaticJointParameterDomainConstraints(StaticInformation * staticInf) {

	if (staticInf->verbose) {
		cout << "Calculating static joint parameter domain constraints" << endl;
	}
	
	//jointParameterDomainConstraints = new hash_map<Entity*, Entity***>*[parameterCount];
	//parameterDomainConstraintSizes = new hash_map<Entity*, int*>*[parameterCount];
	
	for (int i = 0; i < parameterCount; i++) {
		
		hash_map<Entity*, EntitySet**> * eHash = jointParameterDomainConstraints[i];
		
		
		for (EntitySet::iterator j = getParameterDomain(i)->begin(); j != getParameterDomain(i)->end(); j++) {
		
			EntitySet** epArray = new EntitySet*[parameterCount];
						
			Entity* currEnt = *j;
			
			for (int k = 0; k < parameterCount; k++) {
				epArray[k] = 0;
			}
			hash_map<Entity*, EntitySet**>::iterator ae = eHash->find(currEnt);
			if (ae != eHash->end()) {
				EntitySet** oldEP = (*ae).second;
				
				for (int k = 0; k < parameterCount; k++) {
					if (oldEP[k] != 0) delete oldEP[k];
				}
				
				delete [] oldEP;
				
				(*ae).second = epArray;
			} else {
				
				(*eHash)[currEnt] = epArray;	
			}
			
			
		}
		
			
	}
	
	for (int masterParameter = 0; masterParameter < parameterCount; masterParameter++) {
	
		if (ownParameterDomains[masterParameter]) {
	
			if (staticInf->verbose) {
				cout << " Processing parameter " << masterParameter << endl;
			}		

			for (int currPreconditionIndex = 0; currPreconditionIndex < preconditionCount; currPreconditionIndex++) {
				if (preconditions[currPreconditionIndex].predType->isStatic() && isEssentialPrecondition(currPreconditionIndex)) {
					const Predicate* currPrecondition = preconditions[currPreconditionIndex].predType;
					
					for (int i = 0; i < currPrecondition->getArgumentCount(); i++) {
						if (preconditions[currPreconditionIndex].hardParameters[i] == 0 && preconditions[currPreconditionIndex].parameters[i] == masterParameter) {
						
							if (staticInf->verbose) {
								cout << "  Parameter interacts with static precondition " << currPreconditionIndex << " of type " << currPrecondition->name() << endl;
							}
							
							
								 
							for (EntitySet::iterator domainEntryIndex = getParameterDomain(masterParameter)->begin(); domainEntryIndex != getParameterDomain(masterParameter)->end(); domainEntryIndex++) {
								EntitySet** restrictionAsList = new EntitySet*[currPrecondition->getArgumentCount()];
								for (int zeroArray = 0; zeroArray < currPrecondition->getArgumentCount(); zeroArray++) {
									if (zeroArray != i  && preconditions[currPreconditionIndex].hardParameters[zeroArray] == 0 && preconditions[currPreconditionIndex].parameters[zeroArray] >= 0) {
										
										restrictionAsList[zeroArray] = new EntitySet();	
										//cout << "Creating Entity Set at " << restrictionAsList[zeroArray] << endl;
									} else {
										restrictionAsList[zeroArray] = 0;
									}
						
								}
								
								
										
								State::const_iterator stateItr = staticInf->StaticInitialState->begin();
										
										
										
								for (; stateItr != staticInf->StaticInitialState->end(); stateItr++) {
										
									const PredicateInstance * currStaticPred = (*stateItr);
									if (currStaticPred->isOfType(currPrecondition)) {
																				
										bool matches = (currStaticPred->getParameter(i)->boundTo() == (*domainEntryIndex));
												
										for (int matchIndex = 0; matches && matchIndex < currPrecondition->getArgumentCount(); matchIndex++) {
										
											if (preconditions[currPreconditionIndex].hardParameters[matchIndex] != 0) {
												matches = (currStaticPred->getParameter(matchIndex)->boundTo() == preconditions[currPreconditionIndex].hardParameters[matchIndex]);
											} else if (preconditions[currPreconditionIndex].parameters[matchIndex] < 0) {
												matches = (currStaticPred->getParameter(matchIndex)->boundTo() == constants[-1 - preconditions[currPreconditionIndex].parameters[matchIndex]]);
											}	
																				
										}
											
										if (matches) {
										
											for (int matchIndex = 0; matchIndex < currPrecondition->getArgumentCount(); matchIndex++) {
										
												if (matchIndex != i && (preconditions[currPreconditionIndex].hardParameters[matchIndex] == 0) && preconditions[currPreconditionIndex].parameters[matchIndex] >=0) {
											
													restrictionAsList[matchIndex]->insert(currStaticPred->getParameter(matchIndex)->boundTo());
													
												}
										
											}	
										
										}
										
									}
										
								}
								
								for (int matchIndex = 0; matchIndex < currPrecondition->getArgumentCount(); matchIndex++) {
									
							
									if (matchIndex != i && ( preconditions[currPreconditionIndex].hardParameters[matchIndex] == 0) && preconditions[currPreconditionIndex].parameters[matchIndex] >= 0) {
								
										int correspondingParameter = preconditions[currPreconditionIndex].parameters[matchIndex];
									
									
										
										
										hash_map<Entity*, EntitySet**> * mpHash = jointParameterDomainConstraints[masterParameter];
										
										//Entity** oldJPDC = (*mpHash)[argAssignments[i]][matchIndex];
										EntitySet* oldJPDC = (*mpHash)[(*domainEntryIndex)][correspondingParameter];
										if (oldJPDC == 0) {
											//cout << "Null old JPDC, transferring responsibility for entity set at " << restrictionAsList[matchIndex] << endl;
											(*mpHash)[(*domainEntryIndex)][correspondingParameter] = restrictionAsList[matchIndex];
										} else {
											EntitySet* newJPDC = intersection(oldJPDC, restrictionAsList[matchIndex]);
											delete oldJPDC;
											//cout << "Merged, deleting entity set at " << restrictionAsList[matchIndex] << endl;
											delete restrictionAsList[matchIndex];
											(*mpHash)[(*domainEntryIndex)][correspondingParameter] = newJPDC;
										}
										
										
																		
										
									}			
								}
								
								delete [] restrictionAsList;
								
							}
							
							
							
							
						
						};
					}
					
				}
			}
							
		} else {
			if (staticInf->verbose) {
				cout << " Parameter " << masterParameter << " is unrestricted; moving on." << endl;
			}	
		}
		
		if (staticInf->verbose) {
		
			cout << " Done processing parameter " << masterParameter << endl;
		}
	
	}
	
	

};

bool ADLAction::addsOrDeletes(const Predicate * p) const {

	StaticCheckVisitor vis;
	bool found = vis.containsPredicate(p, rootEffectNode);
	if (found) {
//		cout << "Predicate " << p->name() << " is added/deleted by " << name() << endl;
	} else {
//		cout << "Predicate " << p->name() << " is not added/deleted by " << name() << endl;
	}
	return found;

};

list<AbstractPredicate> ADLAction::addCases(const Predicate* p) const {
	AddCheckVisitor vis;
	return vis.containsPredicate(p, rootEffectNode);
}

list<AbstractPredicate> ADLAction::deleteCases(const Predicate* p) const {
	DeleteCheckVisitor vis;
	return vis.containsPredicate(p, rootEffectNode);
}


void ADLAction::reorderPreconditions(StaticInformation * staticInf) {};

void ADLAction::createSubActions() {

	MakeSubActionVisitor msav(this);
	//cout << "Making sub-actions for ADL action at " << this << endl;
	msav.MakeSubActions();

};

bool ADLAction::hasSubActions() const {
	return (subActionCount > 0);
};

int ADLAction::getSubActionCount() const {
	return subActionCount;
};

Action* ADLAction::getSubAction(const int & i) const {

	return subActions[i];

};


ADLAction::~ADLAction() {

	delete [] preconditionsUpdate;
	delete [] negativePreconditionsUpdate;
	
	delete [] essentialPrecondition;
	delete [] essentialNegativePrecondition;
	
	delete [] essentialContainer;
	delete [] disjunctiveContainer;
	delete [] containersUpdate;
	

	delete [] subContainers;
	delete [] subPreconditions;
	delete [] subNegativePreconditions;
	
	delete [] defaultSatisfactionCounters;
	
	if (subActions != 0) {
		for (int i = 0; i < subActionCount; ++i) delete subActions[i];
		delete [] subActions;
	}
	
	
	delete rootEffectNode;
};
