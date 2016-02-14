#ifndef ProcessQuery_H
#define ProcessQuery_H

#include <string>
#include <vector>
#include "Clause.h"
#include "PatternClause.h"
using namespace std;

class QueryTree {
public:
	QueryTree(void);
	QueryTree(string statement);

	Variable getSelect();
	std::vector<Clause> getClauses();
	
private:
	Variable selectVariable;
	std::vector<Clause> clauses;

	string getQuery(string statement);
	std::pair<std::string, std::string> getSelect(string query);
	std::vector<Clause> processClauses(string declarationQueries);
	Clause toClause(string rawClause, string condition);
	Clause toSuchThatClause(string rawClause);
	Clause toPatternClause(string rawClause);
	Clause toAffectVariable(string clauseType, Variable first, Variable second);
	Clause toDefaultClause(string clauseType, Variable first, Variable second);
	bool isProcedureCallStatement(string statement);
	void buildTree();
};

#endif