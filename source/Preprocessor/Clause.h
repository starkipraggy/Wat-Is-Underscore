#ifndef Clause_H
#define Clause_H

#include <string>
#include "Variable.h"
using namespace std;

class Clause {
public:
	Clause();
	Clause::Clause(string clau, Variable var1, Variable var2);

	string getClause();
	Variable getVariableOne();
	Variable getVariableTwo();
	virtual string getQuery();
	bool hasVariable(Variable v);
	bool hasLinkedVariable(Variable v);
	Variable getLinkedVariable(Variable v);
	bool hasAtMostOneCommonSynonym(Clause* c);
protected:
	string clause;
	Variable variableOne;
	Variable variableTwo;
};

#endif