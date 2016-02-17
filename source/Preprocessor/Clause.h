#ifndef Clause_H
#define Clause_H

#include <string>
#include <regex>

#include "Variable.h"
#include "Clause.h"
#include "StringUlti.h"
using namespace std;

class Clause {
public:
	Clause();
	Clause::Clause(string clau, Variable var1, Variable var2);

	string getClause();
	Variable getVariableOne();
	Variable getVariableTwo();
	virtual string getQuery();
	virtual bool equals(Clause* c);
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