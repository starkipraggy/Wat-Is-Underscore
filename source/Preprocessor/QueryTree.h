#ifndef ProcessQuery_H
#define ProcessQuery_H

#include <string>
#include <vector>
#include "Clause.h"
#include "PatternClause.h"
using namespace std;

class QueryTree {
public:
	static QueryTree* Instance();

	void setQueryTree(string statement);
	Variable getSelect();
	std::vector<Clause*> getClauses();
	
private:
	static QueryTree* m_pInstance;
	Variable selectVariable;
	std::vector<Clause*> clauses;

	QueryTree();
	QueryTree(QueryTree const&) {};
	QueryTree& operator=(QueryTree const&) {};

	string getQuery(string statement);
	std::pair<std::string, std::string> getSelect(string query);
	void processClauses(string declarationQueries);
	Clause* toClause(string rawClause, string condition);
	Clause* toSuchThatClause(string rawClause);
	Clause* toPatternClause(string rawClause);
	Clause* toAffectVariable(string clauseType, Variable first, Variable second);
	Clause* toDefaultClause(string clauseType, Variable first, Variable second);
	bool isProcedureCallStatement(string statement);
	void buildTree();
	bool v1Validation();
};

#endif