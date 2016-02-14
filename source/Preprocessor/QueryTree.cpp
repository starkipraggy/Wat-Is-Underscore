#include <regex>
#include "QueryTree.h"
#include "StringUlti.h"
#include "DeclarationMap.h"
using namespace std;

string conditions[] = { "Follows","Follows*","Parent","Parent*","Modifies","Uses" };

QueryTree::QueryTree() {}

QueryTree::QueryTree(string statement) {
	string query = getQuery(statement);
	std::pair<std::string, std::string> selectPair = getSelect(query);
	string name = selectPair.first;
	string type = DeclarationMap::Instance()->find(name);
	selectVariable = Variable(name, type);
	
	string declarationQueries = selectPair.second;
	clauses = processClauses(declarationQueries);
	buildTree();
}

Variable QueryTree::getSelect() {
	return selectVariable;
}

std::vector<Clause> QueryTree::getClauses() {
	return clauses;
}

//private method
string QueryTree::getQuery(string statement) {
	if (statement == "") {
		throw "Query is empty";
	}
	else if (statement.find_last_of(";") != string::npos) {
		statement = statement.substr(statement.find_last_of(";") + 1, statement.length());
	}
	//else statement does not have declaration

	return statement;
}

//pair<querySynonym, declarationQueries>
std::pair<std::string, std::string> QueryTree::getSelect(string query) {
	string querySynonym, declarationQueries;

	query = trim(query);

	if (query.find(' ') != string::npos) {
		int position = query.find(' ');
		if (StringToUpper(query.substr(0, position)) != "SELECT") {
			throw "Query must start with Select";
		}
		query = trim(query.substr(position + 1, query.length()));
	}
	else {
		throw "Query cannot be one word";
	}

	if (query.find(' ') != string::npos) {
		int position = query.find(' ');
		querySynonym = query.substr(0, position);

		declarationQueries = trim(query.substr(position + 1, query.length()));
	}
	else {
		throw "Query cannot be two word";
	}

	std::pair<std::string, std::string> myPair(querySynonym, declarationQueries);
	return myPair;
}

std::vector<Clause> QueryTree::processClauses(string declarationQueries) {
	std::vector<Clause> vect(0);

	std::vector<std::string> tokenizedQueries = tokenize(declarationQueries);

	string prevClause = "";
	string prevQuery = "";

	try{
		for (int i = 0; i < tokenizedQueries.size(); i++) {
			if (StringToUpper(tokenizedQueries[i]) == "SUCH" && i + 1 < tokenizedQueries.size()) {
				if (StringToUpper(tokenizedQueries[i + 1]) == "THAT") {
					if (prevClause != "") {
						Clause clause = toClause(prevQuery, prevClause);
						vect.push_back(clause);
						prevQuery = "";
					}
					prevClause = "SUCH THAT";
				}
				else {
					throw "such clause have to follow by that";
				}
				i++;
			}
			else if (StringToUpper(tokenizedQueries[i]) == "PATTERN") {
				if (prevClause != "") {
					Clause clause = toClause(prevQuery, prevClause);
					vect.push_back(clause);
					prevQuery = "";
				}
				prevClause = "PATTERN";

			}
			else {
				prevQuery += tokenizedQueries[i];
			}
		}
		if (StringToUpper(tokenizedQueries[tokenizedQueries.size() - 1]) != "PATTERN") {
			Clause clause = toClause(prevQuery, prevClause);
			vect.push_back(clause);
		}
		else {
			throw "Cannot end query with clause";
		}
	}
	catch (const char* msg) {
		throw msg;
	}

	return vect;
}

Clause QueryTree::toClause(string rawClause, string condition) {
	Clause clause;
	try {
		if (condition == "PATTERN") {
			clause = toPatternClause(rawClause);
		}
		else {
			clause = toSuchThatClause(rawClause);
		}
	}
	catch (const char* msg) {
		throw msg;
	}
	return clause;
}

Clause QueryTree::toSuchThatClause(string rawClause) {
	Clause clause;

	try{
		rawClause = trim(rawClause);
		int openBracket = rawClause.find("(");
		if (openBracket == string::npos) {
			throw "clause have no open bracket";
		}

		string condition = rawClause.substr(0, openBracket);
		bool check = false;
		for (int i = 0; i < 6; i++) {
			if (conditions[i] == condition) {
				check = true;
			}
		}
		if (check = false) {
			throw "not a valid clause";
		}

		string remaining = rawClause.substr(openBracket + 1, string::npos);
		remaining = trim(remaining);

		int semicolonAfterBracket = remaining.find(",");
		if (semicolonAfterBracket == string::npos) {
			throw "clause only have one parameter";
		}

		int closeBracket = remaining.find(")");
		if (closeBracket != remaining.size() - 1) {
			string error = "clause '" + rawClause + "' have no close bracket";
			throw error;
		}

		string firstVariable = remaining.substr(0, semicolonAfterBracket);
		string secondVariable = remaining.substr(semicolonAfterBracket + 1, closeBracket - semicolonAfterBracket - 1);
		firstVariable = trim(firstVariable);
		secondVariable = trim(secondVariable);
		string firstType = Variable::findSuchThatType(firstVariable);
		string secondType = Variable::findSuchThatType(secondVariable);
		Variable var1 = Variable(firstVariable, firstType);
		Variable var2 = Variable(secondVariable, secondType);

		condition = StringToUpper(condition);
		if (condition == "USES" || condition == "USES*" || condition == "MODIFIES" || condition == "MODIFIES*") {
			clause = toAffectVariable(condition, var1, var2);
		}
		else if (condition == "FOLLOWS" || condition == "FOLLOWS*" || condition == "PARENT" || condition == "PARENT*") {
			clause = toDefaultClause(condition, var1, var2);
		}
		else {
			throw "not a valid clause";
		}
	}
	catch (const char* msg) {
		throw msg;
	}
	return clause;
}

Clause QueryTree::toPatternClause(string rawClause) {
	rawClause = trim(rawClause);
	int openBracket = rawClause.find("(");
	if (openBracket == string::npos) {
		throw "clause have no open bracket";
	}
	string assignedVariable = rawClause.substr(0, openBracket);
	assignedVariable = trim(assignedVariable);
	string assignedType = DeclarationMap::Instance()->find(assignedVariable);
	Variable assignedVar = Variable(assignedVariable, assignedType);

	string remaining = rawClause.substr(openBracket + 1, string::npos);
	remaining = trim(remaining);

	int semicolonAfterBracket = remaining.find(",");
	if (semicolonAfterBracket == string::npos) {
		throw "clause only have one parameter";
	}

	int closeBracket = remaining.find(")");
	if (closeBracket != remaining.size() - 1) {
		throw "clause have no close bracket";
	}

	string firstVariable = remaining.substr(0, semicolonAfterBracket);
	string secondVariable = remaining.substr(semicolonAfterBracket + 1, closeBracket - semicolonAfterBracket - 1);
	firstVariable = trim(firstVariable);
	secondVariable = trim(secondVariable);
	string firstType = Variable::findPatternType(firstVariable);
	string secondType = Variable::findPatternType(secondVariable);
	Variable var1 = Variable(firstVariable, firstType);
	Variable var2 = Variable(secondVariable, secondType);

	Clause clause = PatternClause("PATTERN", var1, var2, assignedVar);

	return clause;
}

Clause QueryTree::toAffectVariable(string clauseType, Variable first, Variable second) {
	string firstType = first.getType();
	string secondType = second.getType();
	Clause clause;

	if ((firstType == "assign" || firstType == "while" || firstType == "procedure" || isProcedureCallStatement(firstType))
		&& secondType == "variable") {
		clause = Clause(clauseType, first, second);
	}
	else {
		throw "invalid clause";
	}
	return clause;
}

Clause QueryTree::toDefaultClause(string clauseType, Variable first, Variable second) {
	string firstType = first.getType();
	string secondType = second.getType();
	Clause clause;

	clause = Clause(clauseType, first, second);

	return clause;
}

bool QueryTree::isProcedureCallStatement(string statement) {
	statement = trim(statement);
	statement = StringToUpper(statement);
	
	if (statement.find("CALL ") == 0) {
		string subStatement = statement.substr(statement.find(" "), string::npos);
		if (statement.find(" ")) {
			throw "invalid call statement";
		}
		return true;
	}
	return false;
}

void QueryTree::buildTree() {
	std::list<Clause> tree;
	std::list<Clause> remaining;
	std::list<Clause> temp(clauses.begin(), clauses.end());
	std::list<Variable> newLink;
	newLink.push_back(selectVariable);
	Variable currentVariable;

	while (!newLink.empty()) {
		currentVariable = newLink.front();
		newLink.pop_front();
		for (auto& x : temp) {
			if (x.hasVariable(currentVariable)) {
				tree.push_back(x);
				if (x.hasLinkedVariable(currentVariable)) {
					newLink.push_back(x.getLinkedVariable(currentVariable));
				}
			}
			else {
				remaining.push_back(x);
			}
		}
		temp.assign(remaining.begin(), remaining.end());
	}
}