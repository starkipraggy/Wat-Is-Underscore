#ifndef ProcessQuery_H
#define ProcessQuery_H

#include <string>
#include <vector>
#include <regex>
#include <list>

#include "Clause.h"
#include "PatternClause.h"
#include "StringUlti.h"
using namespace std;

class QueryTree {
public:
	static QueryTree* Instance();

	Variable getSelect();
	void setSelect(Variable v);
	void addClause(Clause* c);
	std::vector<Clause*> getClauses();
	void newTree();
	void buildTree();
	bool v1Validation();
	
private:
	static QueryTree* m_pInstance;
	Variable selectVariable;
	std::vector<Clause*> clauses;

	QueryTree();
	QueryTree(QueryTree const&) {};
	QueryTree& operator=(QueryTree const&) {};
};

#endif