#ifndef PatternClause_H
#define PatternClause_H

#include <string>
#include "Variable.h"
#include "Clause.h"

class PatternClause : public Clause{
public:
	PatternClause(string clau, Variable var1, Variable var2, Variable var3);

	Variable getAssignedVariable();
	string getQuery();
protected:
	Variable assignedVariable;
};

#endif