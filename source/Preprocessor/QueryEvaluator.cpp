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

	directoryIndex = 0;
	result = {};
	add(select);

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

					pair<string, int> newPair(assignVar.getName(), directoryIndex);
					directory.insert(newPair);
					directoryIndex++;
				}
				else {
					for (vector<vector<string>>::iterator it = result.begin(); it != result.end();) {

						queryResult = pkb->PQLPattern(Assign, var1, var2);
						it = query(queryResult, it, item->second);

					}
				}
			}
			else {
				if (regex_match(var1.getType(), designEntityRegex) && regex_match(var2.getType(), designEntityRegex)) {
					unordered_map<string, int>::const_iterator item1 = directory.find(var1.getName());
					unordered_map<string, int>::const_iterator item2 = directory.find(var2.getName());

					if (item1 == directory.end() && item2 == directory.end()) {
						for (vector<vector<string>>::iterator it = result.begin(); it != result.end();) {

							int col = item2->second;
							queryResult = queryPKB(clause, it->at(col), 2, var2.getType());
							it = query(queryResult, it, col);

						}
					}
					else if (item1 == directory.end()) {

						tempResult = {};
						for (int i = 0; i < result.size(); i++) {

							queryResult = queryPKB(clause, result.at(i).at(item2->second), 1, var1.getType());
							tempResult = query(queryResult, i, tempResult);

						}
						result = tempResult;

						pair<string, int> newPair(var1.getName(), directoryIndex);
						directory.insert(newPair);
						directoryIndex++;

					}
					else if (item2 == directory.end()) {
						
						tempResult = {};
						for (int i = 0; i < result.size(); i++) {

							queryResult = queryPKB(clause, result.at(i).at(item1->second), 2, var2.getType());
							tempResult = query(queryResult, i, tempResult);

						}
						result = tempResult;

						pair<string, int> newPair(var2.getName(), directoryIndex);
						directory.insert(newPair);
						directoryIndex++;
					}
					else {//item1 != directory.end() && item2 != directory.end()
						add(var1);

						for (vector<vector<string>>::iterator it = result.begin(); it != result.end();) {

							int col = item2->second;
							queryResult = queryPKB(clause, it->at(col), 2, var2.getType());
							it = query(queryResult, it, col);

						}
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
					if (var1.getType() == "placeholder") {
						queryResult = queryPKB(clause, var1.getName(), 1, select.getType());
						if (queryResult.size() < 1) {
							throw "result false";
						}
					}
					else {
						queryResult = queryPKB(clause, var2.getName(), 1, select.getType());
						if (find(queryResult.begin(), queryResult.end(), var1.getName()) == queryResult.end()) {
							throw "result false";
						}
					}
				}
			}
		}
	}
	 
	vector<string> output;
	for (int i = 0; i < result.size(); i++) {
		output.push_back(result.at(i).at(0));
	}
	return output;
}

void QueryEvaluator::processOneSynonym(Ref source, Ref des, string clause, int pos) {
	vector<string> queryResult;
	unordered_map<string, int>::const_iterator item = directory.find(source.getName());
	if (item == directory.end()) {

		if (des.getType() == "placeholder") {
			tempResult = {};
			for (int i = 0; i < result.size(); i++) {

				queryResult = queryPKB(clause, result.at(i).at(item->second), pos, source.getType());
				tempResult = query(queryResult, i, tempResult);

			}
			result = tempResult;

		}
		else {
			queryResult = queryPKB(clause, des.getName(), pos, source.getType());
			tempResult = {};
			for (int i = 0; i < result.size(); i++) {

				tempResult = query(queryResult, i, tempResult);

			}
			result = tempResult;

		}

		pair<string, int> newPair(source.getName(), directoryIndex);
		directory.insert(newPair);
		directoryIndex++;
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

void QueryEvaluator::add(Ref ref) {
	vector<string> queryResult, temp, newTemp;

	queryResult = pkb->PQLSelect(toTNodeType(ref.getType()));

	//if (result.size() == 0) {
		for (int i = 0; i < queryResult.size(); i++) {
			temp = {};
			temp.push_back(queryResult.at(i));
			result.push_back(temp);
		}
	/*}
	else {
		for (int i = 0; i < result.size(); i++) {
			temp = result.at(i);
			for (int j = 0; j < queryResult.size(); j++) {
				newTemp = temp;
				newTemp.push_back(queryResult.at(j));
				result.push_back(temp);
			}
		}
	}*/

	pair<string, int> newPair(ref.getName(), directoryIndex);
	directory.insert(newPair);
	directoryIndex++;
}

vector<vector<string>> QueryEvaluator::query(vector<string> queryResult, int i, vector<vector<string>> temp) {
	vector<string> eachTemp;

	for (int j = 0; j < queryResult.size(); j++) {
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
	else if (clause == "CALLS") {
		if (argumentPosition == 1) {
			output = pkb->PQLCalls(input, true);
		}
		else { //argumentPosition == 2
			output = pkb->PQLCalledBy(input, true);
		}
	}
	else if (clause == "CALLS*") {
		if (argumentPosition == 1) {
			output = pkb->PQLCalls(input, false);
		}
		else { //argumentPosition == 2
			output = pkb->PQLCalledBy(input, false);
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