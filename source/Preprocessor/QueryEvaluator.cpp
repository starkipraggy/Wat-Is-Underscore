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
	vector<Clause*> boolClauses = QueryTree::Instance()->getBoolClauses();
	vector<string> selectResult, queryResult;
	pkb = PKB::getInstance();

	isTrueStatement = false;
	directoryIndex = 0;
	directory = {};
	result = {};

	if (!boolClauses.empty()) {
		for (auto& x : boolClauses) {
			string clause = x->getClause();
			Ref var1 = x->getRefOne();
			Ref var2 = x->getRefTwo();

			if (StringToUpper(clause) == "PATTERN") {
				PatternClause* p = dynamic_cast<PatternClause*>(x);
				Ref assignVar = p->getAssignedVariable();

				if (regex_match(var1.getType(), designEntityRegex)) {
					selectResult = pkb->PQLSelect(toTNodeType(var1.getType()));
					for (unsigned int i = 0; i < selectResult.size(); i++) {
						queryResult = pkb->PQLPattern(toTNodeType(assignVar.getType()), Ref(selectResult.at(i), var1.getType()), var2);
						if (!queryResult.empty()) {
							isTrueStatement = true;
							break;
						}
					}

				}
				else {
					queryResult = pkb->PQLPattern(toTNodeType(assignVar.getType()), var1, var2);
					if (!queryResult.empty()) {
						isTrueStatement = true;
					}
				}
			}
			else if (StringToUpper(clause) == "WITH") {
				if (regex_match(var1.getType(), designEntityRegex) && regex_match(var2.getType(), designEntityRegex)) {
					vector<string> select1Result, select2Result;

					if (var1.getType() != var2.getType()) {
						select1Result = pkb->PQLSelect(toTNodeType(var1.getType()));
						select2Result = pkb->PQLSelect(toTNodeType(var2.getType()));

						for (unsigned int i = 0; i < select1Result.size(); i++) {
							for (unsigned int j = 0; j < select2Result.size(); j++) {
								if (select1Result.at(i) == select2Result.at(j)) {
									isTrueStatement = true;
									break;
								}
							}
						}
					}
				}
				else if (regex_match(var1.getType(), designEntityRegex) || regex_match(var2.getType(), designEntityRegex)) {
					Ref synonym, nonSynonym;

					if (regex_match(var1.getType(), designEntityRegex)) {
						synonym = var1;
						nonSynonym = var2;
					}
					else {
						synonym = var2;
						nonSynonym = var1;
					}

					selectResult = pkb->PQLSelect(toTNodeType(synonym.getType()));

					for (unsigned int i = 0; i < selectResult.size(); i++) {
						if (selectResult.at(i) == nonSynonym.getName()) {
							isTrueStatement = true;
							break;
						}
					}
				}
				else {
					if (var1.getName() == var2.getName()) {
						isTrueStatement = true;
					}
				}
			}
			else { //such that
				if (regex_match(var1.getType(), designEntityRegex) && regex_match(var2.getType(), designEntityRegex)) {
					selectResult = pkb->PQLSelect(toTNodeType(var1.getType()));
					for (unsigned int i = 0; i < selectResult.size(); i++) {
						queryResult = queryPKB(clause, selectResult.at(i), 2, var2.getType());
						if (!queryResult.empty()) {
							isTrueStatement = true;
							break;
						}
					}
				}
				else if (regex_match(var1.getType(), designEntityRegex)) {
					if (var2.getType() == "placeholder") {
						selectResult = pkb->PQLSelect(toTNodeType(var1.getType()));
						for (unsigned int i = 0; i < selectResult.size(); i++) {
							queryResult = queryPKB(clause, selectResult.at(i), 2, var1.getType());
							if (!queryResult.empty()) {
								isTrueStatement = true;
								break;
							}
						}
					}
					else {
						queryResult = queryPKB(clause, var2.getName(), 1, var1.getType());
						if (!queryResult.empty()) {
							isTrueStatement = true;
						}
					}
				}
				else if(regex_match(var2.getType(), designEntityRegex)) {
					if (var1.getType() == "placeholder") {
						selectResult = pkb->PQLSelect(toTNodeType(var2.getType()));
						for (unsigned int i = 0; i < selectResult.size(); i++) {
							queryResult = queryPKB(clause, selectResult.at(i), 1, var2.getType());
							if (!queryResult.empty()) {
								isTrueStatement = true;
								break;
							}
						}
					}
					else {
						queryResult = queryPKB(clause, var1.getName(), 2, var2.getType());
						if (!queryResult.empty()) {
							isTrueStatement = true;
						}
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
					queryResult = queryPKB(clause, var1.getName(), 2, type);
					if (find(queryResult.begin(), queryResult.end(), var2.getName()) != queryResult.end()) {
						isTrueStatement = true;
					}
				}
			}

			if (!isTrueStatement) {
				if (StringToUpper(select.at(0).getType()) == "BOOLEAN") {
					return{ { "false" } };
				}
				else {
					throw "false";
				}
			}
		}
	}

	if (!clauses.empty()) {
		for (auto& x : clauses) {
			string clause = x->getClause();
			Ref var1 = x->getRefOne();
			Ref var2 = x->getRefTwo();
			if (StringToUpper(clause) == "PATTERN") {
				PatternClause* p = dynamic_cast<PatternClause*>(x);
				Ref assignVar = p->getAssignedVariable();

				unordered_map<string, int>::const_iterator assign = directory.find(assignVar.getName());

				if (regex_match(var1.getType(), designEntityRegex)) {
					unordered_map<string, int>::const_iterator item1 = directory.find(var1.getName());

					if (assign != directory.end() && item1 != directory.end()) {
						for (vector<vector<string>>::iterator it = result.begin(); it != result.end();) {
							queryResult = pkb->PQLPattern(toTNodeType(assignVar.getType()), Ref(it->at(item1->second), var1.getType()), var2);
							it = query(queryResult, it, assign->second);
						}

						for (vector<vector<string>>::iterator it = result.begin(); it != result.end();) {
							queryResult = pkb->PQLModifies(it->at(assign->second), 2, "variable");
							it = query(queryResult, it, item1->second);
						}

					}
					else if (assign != directory.end()) { //item1 == directory.end()
						for (vector<vector<string>>::iterator it = result.begin(); it != result.end();) {
							queryResult = pkb->PQLPattern(toTNodeType(assignVar.getType()), Ref(it->at(item1->second), var1.getType()), var2);
							it = query(queryResult, it, assign->second);
						}

						tempResult = {};
						for (unsigned int i = 0; i < result.size(); i++) {
							queryResult = pkb->PQLModifies(result.at(i).at(assign->second), 2, "variable");
							add(queryResult, i, tempResult);
						}
						result = tempResult;
						addDirectory(var1.getName());
					}
					else if(item1 != directory.end()){ //assign == directory.end()
						tempResult = {};
						for (unsigned int i = 0; i < result.size(); i++) {
							queryResult = pkb->PQLPattern(toTNodeType(assignVar.getType()), Ref(result.at(i).at(item1->second), var1.getType()), var2);
							add(queryResult, i, tempResult);
						}
						result = tempResult;
						addDirectory(assignVar.getName());
					}
					else{ //assign == directory.end() && item1 == directory.end()
						vector<vector<string>> temp;
						vector<string> tempOne, tempTwo, eachTemp, selectResult;

						selectResult = pkb->PQLSelect(toTNodeType(var1.getType()));

						for (unsigned int i = 0; i < selectResult.size(); i++) {

							queryResult = pkb->PQLPattern(toTNodeType(assignVar.getType()), Ref(selectResult.at(i), var1.getType()), var2);


							for (unsigned int j = 0; j < queryResult.size(); j++) {
								tempOne.push_back(selectResult.at(i));
								tempTwo.push_back(queryResult.at(j));
							}
						}

						if (result.empty()) {
							for (unsigned int i = 0; i < tempOne.size(); i++) {
								eachTemp = {};
								eachTemp.push_back(tempOne.at(i));
								eachTemp.push_back(tempTwo.at(i));
								result.push_back(eachTemp);
							}
						}
						else {
							for (unsigned int i = 0; i < result.size(); i++) {
								for (unsigned int j = 0; j < tempOne.size(); j++) {
									eachTemp = {};
									eachTemp.assign(result.at(i).begin(), result.at(i).end());
									eachTemp.push_back(tempOne.at(j));
									eachTemp.push_back(tempTwo.at(j));
									temp.push_back(eachTemp);
								}
							}
							result = temp;
						}
						addDirectory(var1.getName());
						addDirectory(assignVar.getName());
					}

				}
				else {
					if (assign == directory.end()) {
						queryResult = pkb->PQLPattern(toTNodeType(assignVar.getType()), var1, var2);
						add(queryResult, assignVar.getName());
					}
					else {
						queryResult = pkb->PQLPattern(toTNodeType(assignVar.getType()), var1, var2);
						for (vector<vector<string>>::iterator it = result.begin(); it != result.end();) {

							it = query(queryResult, it, assign->second);

						}
					}
				}
			}
			else if (StringToUpper(clause) == "WITH") {
				if (regex_match(var1.getType(), designEntityRegex) && regex_match(var2.getType(), designEntityRegex)) {
					unordered_map<string, int>::const_iterator item1 = directory.find(var1.getName());
					unordered_map<string, int>::const_iterator item2 = directory.find(var2.getName());
					
					if (item1 == directory.end() && item2 == directory.end()) {
						//default add item2
						queryResult = pkb->PQLSelect(toTNodeType(var2.getType()));
						add(queryResult, var2.getName());
						item2 = directory.find(var2.getName());
					}

					if (item1 == directory.end() || item2 == directory.end()) {
						string name, type;
						int pos;
						if (item1 == directory.end()) {
							name = var1.getName();
							type = var1.getType();
							pos = item2->second;
						}
						else { //item2 == directory.end();
							name = var2.getName();
							type = var2.getType();
							pos = item1->second;
						}

						for (vector<vector<string>>::iterator it = result.begin(); it != result.end();) {

							queryResult = pkb->PQLSelect(toTNodeType(type));
							it = query(queryResult, it, pos);

						}

						string target;
						for (unsigned int i = 0; i < result.size(); i++) {
							target = result.at(i).at(pos);
							result.at(i).push_back(target);
						}
						addDirectory(name);
						
					}
					else { //item1 != directory.end() && item2 != directory.end()
						remove(item1->second, item2->second);
					}
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
						if (var1.equals(var2)) {
							vector<string> temp;

							queryResult = pkb->PQLSelect(toTNodeType(var1.getType()));

							for (vector<string>::iterator it = queryResult.begin(); it != queryResult.end();) {

								temp = queryPKB(clause, *it, 2, var2.getType());
								if (temp.empty()) {
									it = queryResult.erase(it);
								}
								else {
									if (find(temp.begin(), temp.end(), *it) == temp.end()) {
										it = queryResult.erase(it);
									}
									else {
										++it;
									}
								}
							}

							add(queryResult, var1.getName());
						}
						else {
							vector<vector<string>> temp;
							vector<string> tempOne, tempTwo, eachTemp, selectResult;

							selectResult = pkb->PQLSelect(toTNodeType(var1.getType()));

							for (unsigned int i = 0; i < selectResult.size(); i++) {

								queryResult = queryPKB(clause, selectResult.at(i), 2, var2.getType());
								
								for (unsigned int j = 0; j < queryResult.size(); j++) {
									tempOne.push_back(selectResult.at(i));
									tempTwo.push_back(queryResult.at(j));
								}
							}
							
							if (result.empty()) {
								for (unsigned int i = 0; i < tempOne.size(); i++) {
									eachTemp = {};
									eachTemp.push_back(tempOne.at(i));
									eachTemp.push_back(tempTwo.at(i));
									result.push_back(eachTemp);
								}
							}
							else {
								for (unsigned int i = 0; i < result.size(); i++) {
									for (unsigned int j = 0; j < tempOne.size(); j++) {
										eachTemp = {};
										eachTemp.assign(result.at(i).begin(), result.at(i).end());
										eachTemp.push_back(tempOne.at(j));
										eachTemp.push_back(tempTwo.at(j));
										temp.push_back(eachTemp);
									}
								}
								result = temp;
							}
							addDirectory(var1.getName());
							addDirectory(var2.getName());
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
	 
	vector<vector<string>> output = {};

	if (select.at(0).getType() != "boolean") {
		unordered_map<string, int>::const_iterator item;
		vector<string> eachOutput;
		vector<int> tempPos = {};

		for (unsigned int i = 0; i < select.size(); i++) {
			item = directory.find(select.at(i).getName());
			if (item == directory.end()) {
				queryResult = pkb->PQLSelect(toTNodeType(select.at(i).getType()));
				add(queryResult, select.at(i).getName());
				item = directory.find(select.at(i).getName());
			}
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

	if (des.getType() == "placeholder") {
		vector<string> temp;

		if (pos == 1) {
			pos = 2;
		}
		else { //pos == 2
			pos = 1;
		}

		queryResult = pkb->PQLSelect(toTNodeType(source.getType()));

		for (vector<string>::iterator it = queryResult.begin(); it != queryResult.end();) {

			temp = queryPKB(clause, *it, pos, source.getType());
			if (temp.empty()) {
				it = queryResult.erase(it);
			}
			else {
				++it;
			}

		}

		add(queryResult, source.getName());

	}

	if (item == directory.end()) {

		if (des.getType() == "placeholder") {

			add(queryResult, source.getName());

		}
		else {
			queryResult = queryPKB(clause, des.getName(), pos, source.getType());
			add(queryResult, source.getName());

		}

	}
	else {
		if (des.getType() == "placeholder") {

			for (vector<vector<string>>::iterator it = result.begin(); it != result.end();) {

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
			temp = add(queryResult, i, temp);
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
		return StmtLst;
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
		else if (clause == "NEXTBIP") {
			if (argumentPosition == 1) {
				output = pkb->PQLPrevious(value, true, true);
			}
			else { //argumentPosition == 2
				output = pkb->PQLNext(value, true, true);
			}
		}
		else if (clause == "NEXTBIP*") {
			if (argumentPosition == 1) {
				output = pkb->PQLPrevious(value, false, true);
			}
			else { //argumentPosition == 2
				output = pkb->PQLNext(value, false, true);
			}
		}
		else if (clause == "AFFECTSBIP") {
			if (argumentPosition == 1) {
				output = pkb->PQLAffectsThis(value, true, true);
			}
			else { //argumentPosition == 2
				output = pkb->PQLAffectedByThis(value, true, true);
			}
		}
		else if (clause == "AFFECTSBIP*") {
			if (argumentPosition == 1) {
				output = pkb->PQLAffectsThis(value, false, true);
			}
			else { //argumentPosition == 2
				output = pkb->PQLAffectedByThis(value, false, true);
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