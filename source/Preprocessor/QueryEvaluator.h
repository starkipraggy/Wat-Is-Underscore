#ifndef Source_H
#define Source_H

#include <vector>
#include <string>
#include <regex>
#include <set>
#include <unordered_set>
#include "Clause.h"
#include "PatternClause.h"
#include "QueryTree.h"

#include "StringUlti.h"

class QueryEvaluator {
public:
	std::vector<string> process();

private:
	unordered_set<string> result;
	set<string> eachResult;

	vector<string> getResult();
	vector<string> getEachResult();
	void accumulate(vector<string> currResult);
	void addResult(vector<string> currResult);
	void query(string clause, Variable source, Variable dest, int position);
};
#endif