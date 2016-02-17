#include "QueryEvaluator.h"
using namespace std;
using namespace std::regex_constants;

const regex designEntityRegex("^(STMT|ASSIGN|WHILE|VARIABLE|CONSTANT|PROG_LINE)$", icase);

std::vector<std::string> QueryEvaluator::process() {
	Variable select = QueryTree::Instance()->getSelect();
	vector<Clause*> clauses = QueryTree::Instance()->getClauses();
	vector<string> queryResult;

	if (clauses.empty()) {
		//addResult(getSelect(select.getType()));
	}
	else {
		for (auto& x : clauses) {
			string clause = x->getClause();
			Variable var1 = x->getVariableOne();
			Variable var2 = x->getVariableTwo();
			if (regex_match(var1.getType(), designEntityRegex)) {
				query(clause, var1, var2, 1);
			}
			else if (regex_match(var2.getType(), designEntityRegex)) {
				query(clause, var2, var1, 2);
			}
			else if(var1.getType() == "placeholder"){
				//addResult(getClause(clause, var2, 1, select.getType()));
			}
			else if (var2.getType() == "placeholder") {
				//addResult(getClause(clause, var1, 2, select.getType()));
			}
			else {
				//queryResult = getClause(clause, var2, 1, var1.getType());
				if (find(queryResult.begin(), queryResult.end(), var1) == queryResult.end()) {
					result = {};
					break;
				}
			}
		}
	}
	return getResult();
}

void QueryEvaluator::accumulate(vector<string> currResult) {
	for (auto& x : currResult) {
		eachResult.insert(x);
	}
}

vector<string> QueryEvaluator::getResult() {
	vector<string> newVec;
	std::copy(result.begin(), result.end(), std::back_inserter(newVec));
	eachResult = {};
	return newVec;
}

vector<string> QueryEvaluator::getEachResult() {
	vector<string> newVec;
	std::copy(eachResult.begin(), eachResult.end(), std::back_inserter(newVec));
	eachResult = {};
	return newVec;
}

void QueryEvaluator::addResult(vector<string> currResult) {
	if (result.empty) {
		std::copy(currResult.begin(), currResult.end(), std::inserter(result, result.begin()));
	}
	else {
		unordered_set<string> newSet;

		for (auto& x : currResult) {
			std::unordered_set<std::string>::const_iterator got = result.find(x);

			if (got != result.end()) {
				newSet.insert(*got);
			}
		}

		std::copy(newSet.begin(), newSet.end(), std::inserter(result, result.begin()));
	}
}

void QueryEvaluator::query(string clause, Variable source, Variable dest, int position) {
	vector<string> queryResult;

	if (regex_match(dest.getType(), designEntityRegex)) {
		//queryResult = getSelect(dest.getType());
		for (auto& x : queryResult) {
			//accumulate(getClause(clause, x, position, source.getType()));
		}
		addResult(getEachResult());
	}
	else {
		//addResult(getClause(clause, dest, position, source.getType()));
	}
}