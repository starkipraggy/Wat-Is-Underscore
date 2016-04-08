#include "QueryEvaluator.h"
#include <windows.h>
using namespace std;
using namespace std::regex_constants;

const regex designEntityRegex("^(procedure|stmtLst|stmt|assign|call|while|if|variable|constant|prog_line|plus|minus|times)$", icase);
const regex nameRegex("^[[:alpha:]]([[:alnum:]]*$)");

PKB* pkb;

vector<vector<string>> QueryEvaluator::process() {
	vector<Ref> select = QueryTree::Instance()->getSelect();
	vector<Clause*> clauses = QueryTree::Instance()->getClauses();
	vector<string> queryResult;
	pkb = PKB::getInstance();

	isTrueStatement = false;
	directoryIndex = 0;
	directory = {};
	result = {};
	/*if (select.getType() != "boolean") {
		vector<string> eachTemp;
		queryResult = pkb->PQLSelect(toTNodeType(select.getType()));
		add(queryResult, select.getName());
	}*/

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
					queryResult = pkb->PQLPattern(toTNodeType(assignVar.getType()), var1, var2);
					add(queryResult, assignVar.getName());
				}
				else {
					queryResult = pkb->PQLPattern(toTNodeType(assignVar.getType()), var1, var2);
					for (vector<vector<string>>::iterator it = result.begin(); it != result.end();) {

						it = query(queryResult, it, item->second);

					}
				}
			}
			else if (StringToUpper(clause) == "WITH") {
				if (regex_match(var1.getType(), designEntityRegex) && regex_match(var2.getType(), designEntityRegex)) {
					unordered_map<string, int>::const_iterator item1 = directory.find(var1.getName());
					unordered_map<string, int>::const_iterator item2 = directory.find(var2.getName());
					

					if (item1 == directory.end()) {
						queryResult = pkb->PQLSelect(toTNodeType(var1.getType()));
						add(queryResult, var1.getName());

						queryResult = pkb->PQLSelect(toTNodeType(var2.getType()));
						add(queryResult, var2.getName());
					}
						
					remove(item1->second, item2->second);
				}
				else if (regex_match(var1.getType(), designEntityRegex)) {
					unordered_map<string, int>::const_iterator item1 = directory.find(var1.getName());
					checkValid(var2.getName(), var1.getType());

					if (item1 == directory.end()) {
						add({ var2.getName() }, var1.getName());
					}
					else {
						remove(item1->second, var2.getName());
					}
				}
				else if (regex_match(var2.getType(), designEntityRegex)) {
					unordered_map<string, int>::const_iterator item2 = directory.find(var2.getName());
					checkValid(var1.getName(), var2.getType());

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
					else {
						isTrueStatement = true;
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
							tempResult = add(queryResult, i, tempResult);

						}
						result = tempResult;

						addDirectory(var2.getName());

					}
					else if (item2 != directory.end()) {
						
						tempResult = {};
						for (unsigned int i = 0; i < result.size(); i++) {

							queryResult = queryPKB(clause, result.at(i).at(item2->second), 1, var1.getType());
							tempResult = add(queryResult, i, tempResult);

						}
						result = tempResult;

						addDirectory(var1.getName());

					}
					else {//item1 == directory.end() && item2 == directory.end()
						queryResult = pkb->PQLSelect(toTNodeType(var1.getType()));
						add(queryResult, var1.getName());

						item1 = directory.find(var1.getName());
						tempResult = {};
						for (unsigned int i = 0; i < result.size(); i++) {

							queryResult = queryPKB(clause, result.at(i).at(item1->second), 2, var2.getType());
							tempResult = add(queryResult, i, tempResult);

						}
						result = tempResult;

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
					string type;
					if (clause == "calls" || clause == "calls*") {
						type = "procedure";
					}
					else {
						type = "stmt";
					}

					if (var1.getType() == "placeholder") {
						queryResult = queryPKB(clause, var2.getName(), 1, type);
						if (queryResult.size() < 1) {
							throw "result false";
						}
					}
					if (var2.getType() == "placeholder") {
						queryResult = queryPKB(clause, var1.getName(), 1, type);
						if (queryResult.size() < 1) {
							throw "result false";
						}
					}
					else {
						queryResult = queryPKB(clause, var2.getName(), 1, type);
						if (find(queryResult.begin(), queryResult.end(), var1.getName()) == queryResult.end()) {
							if (select.at(0).getType() == "boolean") {
								return{ {"false"} };
							}
							else {
								return{ {} };
							}
						}
						else {
							isTrueStatement = true;
						}
					}
				}
			}
		}
	}
	 
	vector<vector<string>> output;

	if (select.at(0).getType() != "boolean") {
		unordered_map<string, int>::const_iterator item;
		vector<string> eachOutput;
		vector<int> tempPos;

		for (unsigned int i = 0; i < select.size(); i++) {
			item = directory.find(select.at(i).getName());
			if (item == directory.end()) {
				queryResult = pkb->PQLSelect(toTNodeType(select.at(i).getType()));
				add(queryResult, select.at(i).getName());
			}
			item = directory.find(select.at(i).getName());
			tempPos.push_back(item->second);
		}
		for (unsigned int i = 0; i < result.size(); i++) {
			eachOutput = {};
			for (unsigned int j = 0; j < tempPos.size(); j++) {
				eachOutput.push_back(result.at(i).at(tempPos.at(j)));
			}
			output.push_back(eachOutput);
		}
	}
	else {
		if (result.size() > 0 || isTrueStatement) {
			output = { {"true"} };
		}
		else {
			output = { {"false"} };
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
				tempResult = add(queryResult, i, tempResult);

			}
			result = tempResult;

		}
		else {
			queryResult = queryPKB(clause, des.getName(), pos, source.getType());
			add(queryResult, source.getName());

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

void QueryEvaluator::checkValid(string input, string type) {
	vector<string> queryResult = pkb->PQLSelect(toTNodeType(type));
	if (find(queryResult.begin(), queryResult.end(), input) == queryResult.end()) {
		throw "invalid input";
	}
}

void QueryEvaluator::add(vector<string> queryResult, string name) {
	vector<vector<string>> temp;
	vector<string> eachTemp;

	if (result.empty()) {
		for (unsigned int i = 0; i < queryResult.size(); i++) {
			eachTemp = {};
			eachTemp.push_back(queryResult.at(i));
			temp.push_back(eachTemp);
		}
	}
	else {
		for (unsigned int i = 0; i < result.size(); i++) {
			for (unsigned int j = 0; j < queryResult.size(); j++) {
				eachTemp = {};
				eachTemp.assign(result.at(i).begin(), result.at(i).end());
				eachTemp.push_back(queryResult.at(j));
				temp.push_back(eachTemp);
			}
		}
	}

	addDirectory(name);
	result = temp;
}

vector<vector<string>> QueryEvaluator::add(vector<string> queryResult, int i, vector<vector<string>> temp) {
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
		if (regex_match(input, nameRegex) && argumentPosition == 2) {
			output = pkb->PQLUses(input, argumentPosition, "procedure");
		}
		else {
			output = pkb->PQLUses(input, argumentPosition, outputType);
		}
	}
	else if (clause == "MODIFIES") {
		if (regex_match(input, nameRegex) && argumentPosition == 2) {
			output = pkb->PQLModifies(input, argumentPosition, "procedure");
		}
		else {
			output = pkb->PQLModifies(input, argumentPosition, outputType);
		}
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