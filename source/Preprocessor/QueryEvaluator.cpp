#include "QueryEvaluator.h"
#include <windows.h>
using namespace std;
using namespace std::regex_constants;

const regex designEntityRegex("^(STMT|ASSIGN|WHILE|VARIABLE|CONSTANT|PROG_LINE)$", icase);

PKB* pkb;

std::vector<std::string> QueryEvaluator::process() {
	Ref select = QueryTree::Instance()->getSelect();
	vector<Clause*> clauses = QueryTree::Instance()->getClauses();
	vector<string> queryResult;
	pkb = PKB::getInstance();

	if (clauses.empty()) {
		addResult(pkb->PQLSelect(toTNodeType(select.getType())));
	}
	else {
		for (auto& x : clauses) {
			string clause = x->getClause();
			Ref var1 = x->getRefOne();
			Ref var2 = x->getRefTwo();
			if (StringToUpper(clause) == "PATTERN") {
				PatternClause* p = dynamic_cast<PatternClause*>(x);
				Ref assignVar = p->getAssignedVariable();
				addResult(pkb->PQLPattern(Assign, var1, var2));
			}
			else {
				if (regex_match(var1.getType(), designEntityRegex)) {
					query(clause, var1, var2, 1);
				}
				else if (regex_match(var2.getType(), designEntityRegex)) {
					query(clause, var2, var1, 2);
				}
				else if (var1.getType() == "placeholder") {
					addResult(queryPKB(clause, var2.getName(), 1, select.getType()));
				}
				else if (var2.getType() == "placeholder") {
					addResult(queryPKB(clause, var1.getName(), 2, select.getType()));
				}
				else {
					queryResult = queryPKB(clause, var2.getName(), 1, select.getType());
					if (find(queryResult.begin(), queryResult.end(), var1.getName()) == queryResult.end()) {
						result = {};
						break;
					}
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
	result = {};
	return newVec;
}

vector<string> QueryEvaluator::getEachResult() {
	vector<string> newVec;
	std::copy(eachResult.begin(), eachResult.end(), std::back_inserter(newVec));
	eachResult = {};
	return newVec;
}

void QueryEvaluator::addResult(vector<string> currResult) {
	if (result.empty()) {
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
		result = {};
		std::copy(newSet.begin(), newSet.end(), std::inserter(result, result.begin()));
	}
}

void QueryEvaluator::query(string clause, Ref source, Ref dest, int position) {
	vector<string> queryResult;
	if (regex_match(dest.getType(), designEntityRegex)) {
		queryResult = pkb->PQLSelect(toTNodeType(dest.getType()));
		for (auto& x : queryResult) {
			accumulate(queryPKB(clause, x, position, source.getType()));
		}
		addResult(getEachResult());
	}
	else {
		addResult(queryPKB(clause, dest.getName(), position, source.getType()));
	}
}

TNodeType QueryEvaluator::toTNodeType(string type) {
	type = StringToUpper(type);
	TNodeType n = Undefined;

	if (type == "STMT") {
		return Undefined; // Just Undefined will do, I will know that it's a statement. - Wei Liang
	}
	else if (type == "ASSIGN") {
		return Assign;
	}
	else if (type == "WHILE") {
		return While;
	}
	else if (type == "VARIABLE") {
		return VariableName;
	}
	else if (type == "CONSTANT") {
		return Const;
	}
	else if (type == "PROG_LINE") {
		return Undefined;  // Just Undefined will do, I will know that it's a statement. - Wei Liang
	}
	else {
		return Undefined;
	}

	return n;
}

vector<string> QueryEvaluator::queryPKB(string clause, string input, int argumentPosition, string outputType) {
	pkb = PKB::getInstance();
	vector<string> output;

	clause = StringToUpper(clause);

	if (clause == "USES") {
		output = pkb->PQLUses(input, argumentPosition, outputType);
	}
	else if (clause == "MODIFIES") {
		output = pkb->PQLModifies(input, argumentPosition, outputType);
	}

	else{
		int value = atoi(input.c_str());
		if (clause == "FOLLOWS") {
			output = pkb->PQLFollows(value, argumentPosition);
		}
		else if (clause == "FOLLOWS*") {
			output = pkb->PQLFollowsStar(value, argumentPosition);
		}
		else if (clause == "PARENT") {
			output = pkb->PQLParent(value, argumentPosition);
		}
		else if (clause == "PARENT*") {
			output = pkb->PQLParentStar(value, argumentPosition);
		}
	}
	return output;
}