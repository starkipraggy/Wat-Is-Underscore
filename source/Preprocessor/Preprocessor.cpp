#include "Preprocessor.h"
using namespace std;
using namespace std::regex_constants;

string conditions[] = { "Follows","Follows*","Parent","Parent*","Modifies","Uses" };

const regex identRegex("(^[[:alpha:]])([[:alnum:]]+|#+)*$");
const regex integerRegex("[[:digit:]]+");
const regex placeholderRegex("^_$");
const regex expressionRegex("(^\"[[:alpha:]])([[:alnum:]]*|#*)*\"$");
const regex patternExpressionRegex("(^\")[[:space:]]*([[:alpha:]]([[:alnum:]]|#)*|[[:digit:]]+)[[:space:]]*(\\+[[:space:]]*([[:alpha:]]([[:alnum:]]|#)*|[[:digit:]]+))*[[:space:]]*(\"$)");
const regex partOfExpressionRegex("(^_\")[[:space:]]*([[:alpha:]]([[:alnum:]]|#)*|[[:digit:]]+)[[:space:]]*(\\+[[:space:]]*([[:alpha:]]([[:alnum:]]|#)*|[[:digit:]]+))*[[:space:]]*(\"_$)");


const regex designEntityRegex("^(STMT|ASSIGN|WHILE|VARIABLE|CONSTANT|PROG_LINE)$",icase);
const regex stmtDesignEntityRegex("^(STMT|ASSIGN|WHILE|CONSTANT|PROG_LINE)$", icase);
const regex entDesignEntityRegex("^(VARIABLE)$", icase);

void Preprocessor::process(string statement) {
	initialize(statement);
	setMap();
	setSelect();
	setQueryTree();

	QueryTree::Instance()->v1Validation();

}

void Preprocessor::initialize(string statement) {

	statement = trim(statement);
	QueryTree::Instance()->newTree();

	if (statement == "") {
		throw "Query is empty";
	}

	//set declarationPart
	if (statement.find_last_of(';') != string::npos) {
		int position = statement.find_last_of(';');
		declarationPart = statement.substr(0, position);
		statement = trim(statement.substr(position + 1));
	}
	else {
		declarationPart = "";
	}

	//validate select
	if (statement.find(' ') != string::npos) {
		int position = statement.find(' ');
		if (StringToUpper(statement.substr(0, position)) != "SELECT") {
			throw "Query must start with Select";
		}
		statement = trim(statement.substr(position + 1));
	}
	else {
		throw "Query cannot be one word";
	}

	//set selectPart
	if (statement.find(' ') != string::npos) {
		int position = statement.find(' ');
		selectPart = statement.substr(0, position);
		clausesPart = trim(statement.substr(position + 1));
	}
	else {
		selectPart = statement;
		clausesPart = "";
	}
}

void Preprocessor::setMap() {
	declarationMap = {};

	vector<string> declarations = tokenize(declarationPart, ";");

	for (auto& x : declarations) {
		try {
			setDeclaration(x);
		}
		catch (const char* msg) {
			throw msg;
		}
	}
}

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

void Preprocessor::setSelect() {
	string name = selectPart;
	string type = declarationMap.find(name)->second;
	QueryTree::Instance()->setSelect(Ref(name, type));
}

void Preprocessor::setQueryTree() {

	if (clausesPart != "") {
		processClauses(clausesPart);

	}
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
		Ref var1 = createSuchThatRef(firstVariable);
		Ref var2 = createSuchThatRef(secondVariable);
		
		if (var1.equals(var2) && regex_match(var1.getType(), designEntityRegex)) {
			throw "cannot same synonym ref";
		}

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
	Ref var1 = createPatternRef(firstVariable);
	Ref var2 = createPatternRef(secondVariable);

	if (isEntRef(var1) && isExprSpec(var2)) {
		QueryTree::Instance()->addClause(new PatternClause("PATTERN", var1, var2, assignedVar));
	}
	else {
		throw "pattern has wrong arguement ref or expr";
	}
}

Ref Preprocessor::createSuchThatRef(string name) {
	Ref result;

	if (regex_match(name, integerRegex)) {
		result = Ref(name, "integer");
	}
	else if (regex_match(name, placeholderRegex)) {
		result = Ref(name, "placeholder");
	}
	else if (regex_match(name, identRegex)) {
		try {
			result = Ref(name, declarationMap.find(name)->second);
		}
		catch (const char* msg) {
			throw msg;
		}
	}
	else if (regex_match(name = removeSpace(name), expressionRegex)) {
		result = Ref(name.substr(1, name.length() - 2), "expr");
	}
	else {
		throw "invalid variable";
	}
	return result;
}

Ref Preprocessor::createPatternRef(string name) {
	Ref ref;
	if (regex_match(name, placeholderRegex)) {
		ref = Ref(name, "placeholder");
	}
	else if (regex_match(name, partOfExpressionRegex)) {
		ref = Ref(name.substr(2, name.length() - 4), "part_of_expr");
	}
	else if (regex_match(name, expressionRegex)) {
		ref = Ref(name.substr(1, name.length() - 2), "expr");
	}
	else if (regex_match(name, patternExpressionRegex)) {
		ref = Ref(name.substr(1, name.length() - 2), "expr");
	}
	else if (regex_match(name, identRegex)) {
		try {
			ref = Ref(name, declarationMap.find(name)->second);
		}
		catch (const char* msg) {
			throw msg;
		}
	}
	else {
		throw "invalid pattern";
	}
	return ref;
}

bool Preprocessor::isStmtRef(Ref v) {
	string type = v.getType();
	
	return (type == "placeholder" || type == "integer" ||
		regex_match(type, stmtDesignEntityRegex));
}

bool Preprocessor::isEntRef(Ref v) {
	string type = v.getType();

	return (type == "placeholder" || type == "expr" ||
		regex_match(type, entDesignEntityRegex));
}

bool Preprocessor::isExprSpec(Ref v) {
	string type = v.getType();

	return (type == "placeholder" || type == "part_of_expr" || type == "expr");
}