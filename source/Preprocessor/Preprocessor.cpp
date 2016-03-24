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
const regex attrNameRegex("^(PROCNAME|VARNAME|VALUE|STMT#)$", icase);
const regex stmtDesignEntityRegex("^(STMT|ASSIGN|WHILE|CONSTANT|PROG_LINE)$", icase);
const regex entDesignEntityRegex("^(VARIABLE)$", icase);

const regex clauseRegex("such that+|pattern+|with+|and+", icase);
const regex booleanRegex("^(BOOLEAN)$", icase);

void Preprocessor::process(string statement) {
	initialize(statement);
	setMap();
	setSelect();
	setQueryTree();

	//QueryTree::Instance()->v1Validation();

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
	string name, type;

	if (regex_match(selectPart, booleanRegex)) {
		name = "";
		type = "boolean";
	}
	else {
		name = selectPart;
		type = declarationMap.find(name)->second;
	}

	QueryTree::Instance()->setSelect(Ref(name, type));
}

void Preprocessor::setQueryTree() {

	if (clausesPart != "") {
		processClauses(clausesPart);

	}
}

void Preprocessor::processClauses(string declarationQueries) {

	string eachToken, clause;
	int submatches[] = { -1, 0 };
	std::regex_token_iterator<std::string::iterator> rit(declarationQueries.begin(), declarationQueries.end(), clauseRegex, submatches);
	std::regex_token_iterator<std::string::iterator> rend;

	int i = -1;
	while (rit != rend) {
		eachToken = trim(rit->str());
		if (i == -1) {
			if (eachToken != "") {
				throw "should not have condition before clause";
			}
		}
		else{
			if (i % 2 == 0) { //clause
				if (StringToUpper(eachToken) != "AND") {
					clause = eachToken;
				}
			}
			else { //condition
				addClause(eachToken, clause);
			}
		}
		++rit;
		i++;
	}

	if (i % 2 != 0) {
		throw "query cannot end with clause";
	}
}

void Preprocessor::addClause(string rawClause, string condition) {
	rawClause = trim(rawClause);
	try {
		if (StringToUpper(condition) == "PATTERN") {
			addPatternClause(rawClause);
		}
		else if (StringToUpper(condition) == "WITH") {
			addWithClause(rawClause);
		}
		else{
			addSuchThatClause(rawClause);
		}
	}
	catch (const char* msg) {
		throw msg;
	}
}

void Preprocessor::addSuchThatClause(string rawClause) {

	try {
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

void Preprocessor::addWithClause(string rawClause) {
	int equalPos = rawClause.find("=");
	if (equalPos == string::npos) {
		throw "attrCompare does not have '='";
	}

	string firstRef = trim(rawClause.substr(0, equalPos));
	string secondRef = trim(rawClause.substr(equalPos + 1));

	Ref var1 = createWithRef(trim(firstRef));
	Ref var2 = createWithRef(trim(secondRef));
	QueryTree::Instance()->addClause(new Clause("WITH", var1, var2));
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

Ref Preprocessor::createWithRef(string name) {
	Ref ref;
	
	if (name.find(".") != string::npos) {
		string type;

		int position = name.find(".");
		string synonym = trim(name.substr(0, position));
		string attrName = trim(name.substr(position + 1));
		try {
			type = declarationMap.find(synonym)->second;
		}
		catch (const char* msg) {
			throw msg;
		}

		if (!regex_match(attrName, attrNameRegex)) {
			throw "wrong attrName";
		}

		type = type + "-" + attrName;
		ref = Ref(synonym, type);
	}
	else if (regex_match(name, identRegex)) {
		try {
			string type = declarationMap.find(name)->second;
			if (type == "prog_line") {
				ref = Ref(name, type);
			}
			else {
				throw "with ref need to be prog_line";
			}
		}
		catch (const char* msg) {
			throw msg;
		}
	}
	else if (regex_match(name = removeSpace(name), expressionRegex)) {
		ref = Ref(name.substr(1, name.length() - 2), "expr");
	}
	else if (regex_match(name, integerRegex)) {
		ref = Ref(name, "integer");
	}
	else {
		throw "invalid with ref";
	}

	//check if same type

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