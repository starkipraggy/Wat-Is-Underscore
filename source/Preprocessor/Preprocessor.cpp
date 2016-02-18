#include "Preprocessor.h"
using namespace std;
using namespace std::regex_constants;

string conditions[] = { "Follows","Follows*","Parent","Parent*","Modifies","Uses" };

const regex identRegex("(^[[:alpha:]])([[:alnum:]]+|#+)*$");
const regex integerRegex("[[:digit:]]+");
const regex placeholderRegex("^_$");
const regex expressionRegex("(^\"[[:alpha:]])([[:alnum:]]+|#+)*\"$");
const regex partOfExpressionRegex("(^_\"[[:alpha:]])([[:alnum:]]+|#+)*\"_$");

const regex designEntityRegex("^(STMT|ASSIGN|WHILE|VARIABLE|CONSTANT|PROG_LINE)$",icase);

void Preprocessor::process(string statement) {
	setMap(statement);
	
	setQueryTree(statement);

}

void Preprocessor::setMap(string statement) {
	declarationMap = {};

	vector<string> declarations = tokenize(statement, ";");
	declarations.pop_back();

	for (auto& x : declarations) {
		try {
			setDeclaration(x);
		}
		catch (const char* msg) {
			throw msg;
		}
	}
}

//private

//declaration:<entity> <synoymn> - user input
//pair:<synoymn> <entity> - for the purpose of map
void Preprocessor::setDeclaration(string line) {
	if (line.find(" ") != string::npos) {
		int position = line.find(" ");

		string first = line.substr(0, position);
		if (!std::regex_match(first, designEntityRegex)) {
			throw "Invalid entity";
		}

		string second = line.substr(position + 1, string::npos);
		second = trim(second);
		vector<string> variables = tokenize(second, ",");
		for (auto& x : variables) {
			if (std::regex_match(x, identRegex)) {
				std::pair<std::string, std::string> newPair(x, first);
				if (declarationMap.find(x) != declarationMap.end()) {
					throw "cannot have synonym with the same name";
				}
				declarationMap.insert(newPair);
			}
			else {
				throw "not a valid synonym";
			}
		}
	}
}

void Preprocessor::setQueryTree(string statement) {
	QueryTree::Instance()->newTree();
	string query = getQuery(statement);
	std::pair<std::string, std::string> selectPair = getSelect(query);
	string name = selectPair.first;
	string type = declarationMap.find(name)->second;
	QueryTree::Instance()->setSelect(Ref(name, type));

	string declarationQueries = selectPair.second;
	if (declarationQueries != "") {
		processClauses(declarationQueries);

		//QueryTree::Instance()->buildTree();
		QueryTree::Instance()->v1Validation();
	}
}

string Preprocessor::getQuery(string statement) {
	if (statement == "") {
		throw "Query is empty";
	}
	else if (statement.find_last_of(";") != string::npos) {
		statement = statement.substr(statement.find_last_of(";") + 1, string::npos);
	}
	//else statement does not have declaration

	return statement;
}

//pair<querySynonym, declarationQueries>
std::pair<std::string, std::string> Preprocessor::getSelect(string query) {
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
	else {//no clause
		querySynonym = query;
		declarationQueries = "";
	}

	std::pair<std::string, std::string> myPair(querySynonym, declarationQueries);
	return myPair;
}

void Preprocessor::processClauses(string declarationQueries) {

	std::vector<std::string> tokenizedQueries = tokenize(declarationQueries);

	string prevClause = "";
	string prevQuery = "";

	try {
		for (unsigned int i = 0; i < tokenizedQueries.size(); i++) {
			if (StringToUpper(tokenizedQueries[i]) == "SUCH" && i + 1 < tokenizedQueries.size()) {
				if (StringToUpper(tokenizedQueries[i + 1]) == "THAT") {
					if (prevClause != "") {
						addClause(prevQuery, prevClause);
						prevQuery = "";
					}
					prevClause = "SUCH THAT";
				}
				else {
					throw "the such that clause have to be use together";
				}
				i++;
			}
			else if (StringToUpper(tokenizedQueries[i]) == "PATTERN") {
				if (prevClause != "") {
					addClause(prevQuery, prevClause);
					prevQuery = "";
				}
				prevClause = "PATTERN";

			}
			else {
				prevQuery += tokenizedQueries[i];
			}
		}
		if (StringToUpper(tokenizedQueries[tokenizedQueries.size() - 1]) != "PATTERN") {
			addClause(prevQuery, prevClause);
		}
		else {
			throw "Cannot end query with clause";
		}
	}
	catch (const char* msg) {
		throw msg;
	}
}

void Preprocessor::addClause(string rawClause, string condition) {
	try {
		if (condition == "PATTERN") {
			addPatternClause(rawClause);
		}
		else {
			addSuchThatClause(rawClause);
		}
	}
	catch (const char* msg) {
		throw msg;
	}
}

void Preprocessor::addSuchThatClause(string rawClause) {

	try {
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
			string error = "clause have no close bracket";
			throw error;
		}

		string firstVariable = remaining.substr(0, semicolonAfterBracket);
		string secondVariable = remaining.substr(semicolonAfterBracket + 1, closeBracket - semicolonAfterBracket - 1);
		firstVariable = trim(firstVariable);
		secondVariable = trim(secondVariable);
		string firstType = findSuchThatType(firstVariable);
		string secondType = findSuchThatType(secondVariable);
		Ref var1 = Ref(firstVariable, firstType);
		Ref var2 = Ref(secondVariable, secondType);
		
		if (!isStmtRef(var1)) {
			throw "invalid var1";
		}

		condition = StringToUpper(condition);
		if (condition == "USES" || condition == "MODIFIES") {
			if (isEntRef(var2)) {
				QueryTree::Instance()->addClause(new Clause(condition, var1, var2));
			}
			else {
				throw "invalid var2";
			}
		}
		else if (condition == "FOLLOWS" || condition == "FOLLOWS*" || condition == "PARENT" || condition == "PARENT*") {
			if (isStmtRef(var2)) {
				QueryTree::Instance()->addClause(new Clause(condition, var1, var2));
			}
			else {
				throw "invalid var2";
			}
		}
		else {
			throw "not a valid clause";
		}
	}
	catch (const char* msg) {
		throw msg;
	}
}

void Preprocessor::addPatternClause(string rawClause) {
	rawClause = trim(rawClause);
	int openBracket = rawClause.find("(");
	if (openBracket == string::npos) {
		throw "clause have no open bracket";
	}
	string assignedVariable = rawClause.substr(0, openBracket);
	assignedVariable = trim(assignedVariable);
	string assignedType = declarationMap.find(assignedVariable)->second;
	if (assignedType != "assign") {
		throw "syn-assign must be of type assign";
	}
	Ref assignedVar = Ref(assignedVariable, assignedType);

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
	string firstType = findPatternType(firstVariable);
	string secondType = findPatternType(secondVariable);
	Ref var1 = Ref(firstVariable, firstType);
	Ref var2 = Ref(secondVariable, secondType);

	if (isEntRef(var1) && isExprSpec(var2)) {
		QueryTree::Instance()->addClause(new PatternClause("PATTERN", var1, var2, assignedVar));
	}
	else {
		throw "pattern has wrong arguement ref or expr";
	}
}

string Preprocessor::findSuchThatType(string raw) {
	string result;

	if (regex_match(raw, expressionRegex)) {
		result = "expr";
	}
	else if (regex_match(raw, integerRegex)) {
		result = "integer";
	}
	else if (regex_match(raw, placeholderRegex)) {
		result = "placeholder";
	}
	else if (regex_match(raw, identRegex)) {
		try {
			result = declarationMap.find(raw)->second;
		}
		catch (const char* msg) {
			throw msg;
		}
	}
	else {
		throw "invalid variable";
	}
	return result;
}

string Preprocessor::findPatternType(string raw) {
	string result;
	if (regex_match(raw, placeholderRegex)) {
		result = "placeholder";
	}
	else if (regex_match(raw, partOfExpressionRegex)) {
		result = "part_of_expr";
	}
	else if (regex_match(raw, expressionRegex)) {
		result = "expr";
	}
	else if (regex_match(raw, identRegex)) {
		try {
			result = declarationMap.find(raw)->second;
		}
		catch (const char* msg) {
			throw msg;
		}
	}
	else {
		throw "invalid pattern";
	}
	return result;
}

bool Preprocessor::isStmtRef(Ref v) {
	string type = v.getType();
	
	return (type == "placeholder" || type == "integer" ||
		regex_match(type, designEntityRegex));
}

bool Preprocessor::isEntRef(Ref v) {
	string type = v.getType();

	return (type == "placeholder" || type == "expr" ||
		regex_match(type, designEntityRegex));
}

bool Preprocessor::isExprSpec(Ref v) {
	string type = v.getType();

	return (type == "placeholder" || type == "part_of_expr");
}