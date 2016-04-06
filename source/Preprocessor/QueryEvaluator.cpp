#include "QueryEvaluator.h"
#include <windows.h>
using namespace std;
using namespace std::regex_constants;

const regex designEntityRegex("^(procedure|stmtLst|stmt|assign|call|while|if|variable|constant|prog_line|plus|minus|times)$", icase);

PKB* pkb;

std::vector<std::string> QueryEvaluator::process() {
	Ref select = QueryTree::Instance()->getSelect();
	vector<Clause*> clauses = QueryTree::Instance()->getClauses();
	vector<string> queryResult;
	pkb = PKB::getInstance();

	directoryIndex = 0;
	directory = {};
	result = {};
	if (select.getType() != "boolean") {
		queryResult = pkb->PQLSelect(toTNodeType(select.getType()));
		add(queryResult, select.getName());
	}

	if (!clauses.empty()) {
		for (auto& x : clauses) {
			string clause = x->getClause();
			Ref var1 = x->getRefOne();
			Ref var2 = x->getRefTwo();
			if (StringToUpper(clause) == "PATTERN") {
				PatternClause* p = dynamic_cast<PatternClause*>(x);
				Ref assignVar = p->getAssignedVariable();

				unordered_map<string, int>::const_iterator item = directory.find(assignVar.getName());

				if (item == directory.end()) {
					for (vector<vector<string>>::iterator it = result.begin(); it != result.end();) {

						queryResult = pkb->PQLPattern(Assign, var1, var2);
						it = query(queryResult, it, item->second);

					}

					addDirectory(assignVar.getName());
				}
				else {
					for (vector<vector<string>>::iterator it = result.begin(); it != result.end();) {

						queryResult = pkb->PQLPattern(Assign, var1, var2);
						it = query(queryResult, it, item->second);

					}
				}
			}
			else if (StringToUpper(clause) == "WITH") {
				if (regex_match(var1.getType(), designEntityRegex) && regex_match(var2.getType(), designEntityRegex)) {
					unordered_map<string, int>::const_iterator item1 = directory.find(var1.getName());
					unordered_map<string, int>::const_iterator item2 = directory.find(var2.getName());

					if (item1 == directory.end()) {
						add({ var2.getName() }, var1.getName());
						add({ var1.getName() }, var2.getName());
					}
					else {
						remove(item1->second, item2->second);
					}
				}
				else if (regex_match(var1.getType(), designEntityRegex)) {
					unordered_map<string, int>::const_iterator item1 = directory.find(var1.getName());

					if (item1 == directory.end()) {
						add({var2.getName()}, var1.getName());
					}
					else {
						remove(item1->second, var2.getName());
					}
				}
				else if (regex_match(var2.getType(), designEntityRegex)) {
					unordered_map<string, int>::const_iterator item2 = directory.find(var2.getName());

					if (item2 == directory.end()) {
						add({ var1.getName() }, var2.getName());
					}
					else {
						remove(item2->second, var1.getName());
					}
				}
				else {
					if (var1.getName() != var2.getName()) {
						throw "false evaluation";
					}
				}
			}
			else {
				if (regex_match(var1.getType(), designEntityRegex) && regex_match(var2.getType(), designEntityRegex)) {
					unordered_map<string, int>::const_iterator item1 = directory.find(var1.getName());
					unordered_map<string, int>::const_iterator item2 = directory.find(var2.getName());

					if (item1 != directory.end() && item2 != directory.end()) {
						for (vector<vector<string>>::iterator it = result.begin(); it != result.end();) {

							queryResult = queryPKB(clause, it->at(item1->second), 2, var2.getType());
							it = query(queryResult, it, item2->second);

						}
					}
					else if (item1 != directory.end()) {

						tempResult = {};
						for (unsigned int i = 0; i < result.size(); i++) {

							queryResult = queryPKB(clause, result.at(i).at(item1->second), 2, var2.getType());
							tempResult = query(queryResult, i, tempResult);

						}
						result = tempResult;

						addDirectory(var2.getName());

					}
					else if (item2 != directory.end()) {
						
						tempResult = {};
						for (unsigned int i = 0; i < result.size(); i++) {

							queryResult = queryPKB(clause, result.at(i).at(item2->second), 1, var1.getType());
							tempResult = query(queryResult, i, tempResult);

						}
						result = tempResult;

						addDirectory(var1.getName());

					}
					else {//item1 == directory.end() && item2 == directory.end()
						queryResult = pkb->PQLSelect(toTNodeType(var1.getType()));
						add(queryResult, var1.getName());

						for (vector<vector<string>>::iterator it = result.begin(); it != result.end();) {

							queryResult = queryPKB(clause, it->at(item1->second), 2, var2.getType());
							it = query(queryResult, it, item2->second);

						}

						addDirectory(var2.getName());
					}
				}
				else if (regex_match(var1.getType(), designEntityRegex)) {
					processOneSynonym(var1, var2, clause, 1);
				}
				else if (regex_match(var2.getType(), designEntityRegex)) {
					processOneSynonym(var2, var1, clause, 2);
				}
				else {
					if (var1.getType() == "placeholder") {
						queryResult = queryPKB(clause, var2.getName(), 1, select.getType());
						if (queryResult.size() < 1) {
							throw "result false";
						}
					}
					if (var2.getType() == "placeholder") {
						queryResult = queryPKB(clause, var1.getName(), 1, select.getType());
						if (queryResult.size() < 1) {
							throw "result false";
						}
					}
					else {
						string type;
						if (clause == "calls" || clause == "calls*") {
							type = "procedure";
						}
						else {
							type = "stmt";
						}
						queryResult = queryPKB(clause, var2.getName(), 1, type);
						if (find(queryResult.begin(), queryResult.end(), var1.getName()) == queryResult.end()) {
							if (select.getType() == "boolean") {
								return {"false"};
							}
							else {
								return {};
							}
						}
					}
				}
			}
		}
	}
	 
	vector<string> output;

	if (select.getType() != "boolean") {
		for (unsigned int i = 0; i < result.size(); i++) {
			output.push_back(result.at(i).at(0));
		}
	}
	else {
		if (result.size() > 0) {
			output.push_back("true");
		}
		else {
			output.push_back("false");
		}
	}
	return output;
}

void QueryEvaluator::processOneSynonym(Ref source, Ref des, string clause, int pos) {
	vector<string> queryResult;
	unordered_map<string, int>::const_iterator item = directory.find(source.getName());
	if (item == directory.end()) {

		if (des.getType() == "placeholder") {
			tempResult = {};
			for (unsigned int i = 0; i < result.size(); i++) {

				queryResult = queryPKB(clause, result.at(i).at(item->second), pos, source.getType());
				tempResult = query(queryResult, i, tempResult);

			}
			result = tempResult;

		}
		else {
			queryResult = queryPKB(clause, des.getName(), pos, source.getType());
			tempResult = {};
			for (unsigned int i = 0; i < result.size(); i++) {

				tempResult = query(queryResult, i, tempResult);

			}
			result = tempResult;

		}

		addDirectory(source.getName());

	}
	else {
		if (des.getType() == "placeholder") {
			for (vector<vector<string>>::iterator it = result.begin(); it != result.end();) {

				queryResult = queryPKB(clause, it->at(item->second), pos, source.getType());
				it = query(queryResult, it, item->second);

			}
		}
		else {
			queryResult = queryPKB(clause, des.getName(), pos, source.getType());

			for (vector<vector<string>>::iterator it = result.begin(); it != result.end();) {

				it = query(queryResult, it, item->second);

			}
		}
	}
}

void QueryEvaluator::add(vector<string> queryResult, string name) {
	vector<string> temp, newTemp;

	for (unsigned int i = 0; i < queryResult.size(); i++) {
		temp = {};
		temp.push_back(queryResult.at(i));
		result.push_back(temp);
	}

	addDirectory(name);

}

void QueryEvaluator::remove(int pos1, int pos2) {
	for (vector<vector<string>>::iterator it = result.begin(); it != result.end();) {

		if (it->at(pos1) != it->at(pos2)) {
			it = result.erase(it);
		}
		else {
			++it;
		}

	}
}

void QueryEvaluator::remove(int pos, string input) {
	for (vector<vector<string>>::iterator it = result.begin(); it != result.end();) {

		if (it->at(pos) != input) {
			it = result.erase(it);
		}
		else {
			++it;
		}

	}
}

vector<vector<string>> QueryEvaluator::query(vector<string> queryResult, int i, vector<vector<string>> temp) {
	vector<string> eachTemp;

	for (unsigned int j = 0; j < queryResult.size(); j++) {
		eachTemp = {};
		eachTemp.assign(result.at(i).begin(), result.at(i).end());
		eachTemp.push_back(queryResult.at(j));
		temp.push_back(eachTemp);
	}

	return temp;
}

vector<vector<string>>::iterator QueryEvaluator::query(vector<string> queryResult, vector<vector<string>>::iterator it, int col) {

	if (std::find(queryResult.begin(), queryResult.end(), it->at(col)) == queryResult.end()) {
		it = result.erase(it);
	}
	else {
		++it;
	}

	return it;
}

TNodeType QueryEvaluator::toTNodeType(string type) {
	type = StringToUpper(type);
	TNodeType n = Undefined;

	if (type == "PROCEDURE") {
		return ProcedureName;
	}
	else if (type == "STMTLST") {
		return Undefined;
	}
	else if (type == "STMT") {
		return Undefined; // Just Undefined will do, I will know that it's a statement. - Wei Liang
	}
	else if (type == "ASSIGN") {
		return Assign;
	}
	else if (type == "CALL") {
		return Call;
	}
	else if (type == "WHILE") {
		return While;
	}
	else if (type == "IF") {
		return If;
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
	else if (type == "PLUS") {
		return OperatorPlus;
	}
	else if (type == "MINUS") {
		return OperatorMinus;
	}
	else if (type == "TIMES") {
		return OperatorTimes;
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
	else if (clause == "CALLS") {
		if (argumentPosition == 1) {
			output = pkb->PQLCalledBy(input, true);
		}
		else { //argumentPosition == 2
			output = pkb->PQLCalls(input, true);
		}
	}
	else if (clause == "CALLS*") {
		if (argumentPosition == 1) {
			output = pkb->PQLCalledBy(input, false);
		}
		else { //argumentPosition == 2
			output = pkb->PQLCalls(input, false);
		}
	}
	else{
		int value = atoi(input.c_str());
		if (clause == "FOLLOWS") {
			output = pkb->PQLFollows(value, argumentPosition, outputType);
		}
		else if (clause == "FOLLOWS*") {
			output = pkb->PQLFollowsStar(value, argumentPosition, outputType);
		}
		else if (clause == "PARENT") {
			output = pkb->PQLParent(value, argumentPosition, outputType);
		}
		else if (clause == "PARENT*") {
			output = pkb->PQLParentStar(value, argumentPosition, outputType);
		}
		else if (clause == "NEXT") {
			if (argumentPosition == 1) {
				output = pkb->PQLPrevious(value, true);
			}
			else { //argumentPosition == 2
				output = pkb->PQLNext(value, true);
			}
		}
		else if (clause == "NEXT*") {
			if (argumentPosition == 1) {
				output = pkb->PQLPrevious(value, false);
			}
			else { //argumentPosition == 2
				output = pkb->PQLNext(value, false);
			}
		}
		else if (clause == "AFFECTS") {
			if (argumentPosition == 1) {
				output = pkb->PQLAffectsThis(value, true);
			}
			else { //argumentPosition == 2
				output = pkb->PQLAffectedByThis(value, true);
			}
		}
		else if (clause == "AFFECTS*") {
			if (argumentPosition == 1) {
				output = pkb->PQLAffectsThis(value, false);
			}
			else { //argumentPosition == 2
				output = pkb->PQLAffectedByThis(value, false);
			}
		}
	}

	return output;
}

void QueryEvaluator::addDirectory(string name) {
	pair<string, int> newPair(name, directoryIndex);
	directory.insert(newPair);
	directoryIndex++;
}