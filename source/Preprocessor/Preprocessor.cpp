#include "Preprocessor.h"
using namespace std;
using namespace std::regex_constants;

//Lexical rules
const regex integerRegex("^[[:digit:]]+$");
const regex identRegex("^[[:alpha:]]([#[:alnum:]]*$)"); 

//Auxiliary grammer rules
const regex tupleRegex("^<[[:space:]]*[[:alpha:]]([#[:alnum:]])*([[:space:]]*,[[:space:]]*[[:alpha:]]([#[:alnum:]])*)*[[:space:]]*>$");
const regex attrNameRegex("^(procName|varName|value|stmt#)$", icase);
const regex entRefRegex("^(assign|if|while|procedure|placeholder|expr|integer)$", icase);
const regex stmtRefRegex("^(stmtLst|stmt|assign|while|if|prog_line|placeholder|integer)$", icase);
const regex lineRefRegex("^(stmtLst|stmt|assign|call|while|if|prog_line|placeholder|integer)$", icase);
const regex designEntityRegex("^(procedure|stmtLst|stmt|assign|call|while|if|variable|constant|prog_line|plus|minus|times)$", icase);

//Grammer rules for select clause
const regex clauseRegex("such that+|pattern+|with+|and+", icase);
const regex expressionRegex("^\"[[:alpha:]][[:alnum:]]*\"$"); //'"' IDENT '"'

const regex entVarRefRefRegex("^(modifies|uses)$", icase);
const regex stmtVarRefRefRegex("^(modifies|uses)$", icase);
const regex ententRefRefRegex("^(calls|calls\\*)$", icase);
const regex stmtstmtRefRefRegex("^(parent|parent\\*|follows|follows\\*|affects|affects\\*|affectsBip|affectsBip\\*)$", icase);
const regex linelineRefRefRegex("^(next|next\\*|NextBip|NextBip\\*)$", icase);

const regex patternRegex("^(assign|while|if)$", icase);
const regex varRefRegex("^(variable|placeholder|expr)$", icase);
const regex exprRegex("^(placeholder|part_of_expr|expr)$", icase);

const regex placeholderRegex("^_$");
const regex patternExpressionRegex("(^\")[[:space:]]*([[:alpha:]]([[:alnum:]])*|[[:digit:]]+)([[:space:]]*(\\+|\\-|\\*)[[:space:]]*([[:alpha:]]([[:alnum:]])*|[[:digit:]]+))*[[:space:]]*(\"$)");
const regex partOfExpressionRegex("(^_\")[[:space:]]*([[:alpha:]]([[:alnum:]])*|[[:digit:]]+)([[:space:]]*(\\+|\\-|\\*)[[:space:]]*([[:alpha:]]([[:alnum:]])*|[[:digit:]]+))*[[:space:]]*(\"_$)");

//Additional
const regex procNameRegex("^(procedure|call)$", icase);
const regex stmtNoRegex("^(stmtlst|stmt|assign|while|if|prog_line)$", icase);
const regex varNameRegex("^(variable)$", icase);
const regex valueRegex("^(constant)$", icase);


void Preprocessor::process(string statement) {
	try {
		initialize(statement);
		setMap();
		setSelect();
		setQueryTree();
	}
	catch (const char* msg) {
		throw msg;
	}

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
		if (statement.find('>') != string::npos) {
			int position = statement.find('>');
			selectPart = statement.substr(0, position + 1);
			clausesPart = trim(statement.substr(position + 2));
		}
		else {
			int position = statement.find(' ');
			selectPart = statement.substr(0, position);
			clausesPart = trim(statement.substr(position + 1));
		}
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
				unordered_map<string, string>::const_iterator got = declarationMap.find(x);
				if (got != declarationMap.end()) {
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
	Ref ref;
	vector<string> eachSelect;
	vector<Ref> select;

	if (regex_match(selectPart, tupleRegex)) {
		selectPart = selectPart.substr(1, selectPart.length() - 2);

		eachSelect = tokenize(selectPart, ",");
	}
	else {
		eachSelect = {selectPart};
	}

	for (unsigned int i = 0; i < eachSelect.size(); i++) {
		selectPart = trim(eachSelect.at(i));
		if (StringToUpper(selectPart) == "BOOLEAN") {
			ref = Ref("", "boolean");
		}
		else if (selectPart.find(".") != string::npos) {
			ref = createAttrRef(selectPart);
		}
		else {
			string type;

			try {
				type = getDeclarationType(selectPart);
			}
			catch (const char* msg) {
				throw msg;
			}

			ref = Ref(selectPart, type);
		}
		select.push_back(ref);
	}
	QueryTree::Instance()->setSelect(select);
}

void Preprocessor::setQueryTree() {

	if (clausesPart != "") {
		processClauses(clausesPart);

	}
	QueryTree::Instance()->buildTree();
}

void Preprocessor::processClauses(string declarationQueries) {

	string eachToken, clause;
	int submatches[] = { -1, 0 };
	std::regex_token_iterator<std::string::iterator> rit(declarationQueries.begin(), declarationQueries.end(), clauseRegex, submatches);
	std::regex_token_iterator<std::string::iterator> rend;
	int i = -1;

	try{
		while (rit != rend) {
			eachToken = trim(rit->str());
			if (i == -1) {
				if (eachToken != "") {
					throw "should not have condition before clause";
				}
			}
			else {
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
	}
	catch (const char* msg) {
		throw msg;
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
		condition = trim(condition);
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
		string var1Type = var1.getType();
		string var2Type = var2.getType();
		
		if (regex_match(condition, entVarRefRefRegex) || regex_match(condition, stmtVarRefRefRegex)) {
			if (!((regex_match(var1Type, entRefRegex) || regex_match(var1Type, stmtRefRegex))
				&& regex_match(var2Type, varRefRegex))) {
				throw "invalid entVarRefRef or stmtVarRefRef";
			}
			if (var1.getType() == "placeholder" && var2.getType() == "expr") {
				throw "ambiguity";
			}
		}
		else if (regex_match(condition, ententRefRefRegex)) {
			if (!(regex_match(var1Type, entRefRegex) && regex_match(var2Type, entRefRegex))) {
				throw "invalid entEntRefRef";
			}
		}
		else if (regex_match(condition, stmtstmtRefRefRegex)) {
			if (!(regex_match(var1Type, stmtRefRegex) && regex_match(var2Type, stmtRefRegex))) {
				throw "invalid stmtstmtRefRef";
			}
		}
		else if (regex_match(condition, linelineRefRefRegex)) {
			if (!(regex_match(var1Type, lineRefRegex) && regex_match(var2Type, lineRefRegex))) {
				throw "invalid lineLineRefRef";
			}
		}
		else {
			throw "not a valid clause";
		}

		QueryTree::Instance()->addClause(new Clause(condition, var1, var2));
		
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
	string assignedType;
	
	try {
		assignedType = getDeclarationType(assignedVariable);
	}
	catch (const char* msg) {
		throw msg;
	}

	if (!regex_match(assignedType, patternRegex)) {
		throw "wrong assigned type";
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
	if (assignedType == "if") {
		if (removeSpace(secondVariable) == "_,_") {
			secondVariable = "_";
		}
		else {
			throw "if pattern has no 3rd clause";
		}
	}
	Ref var2 = createPatternRef(secondVariable);

	if (regex_match(var1.getType(), varRefRegex) && regex_match(var2.getType(), exprRegex)) {
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
		string type;
		
		try {
			type = getDeclarationType(name);
		}
		catch (const char* msg) {
			throw msg;
		}

		result = Ref(name, type);
	}
	else if (regex_match(name, expressionRegex)) {
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
		string type;
		try {
			type = getDeclarationType(name);
		}
		catch (const char* msg) {
			throw msg;
		}

		ref = Ref(name, type);
	}
	else {
		throw "invalid pattern";
	}
	return ref;
}

Ref Preprocessor::createWithRef(string name) {
	Ref ref;
	
	if (name.find(".") != string::npos) {
		ref = createAttrRef(name);
	}
	else if (regex_match(name, identRegex)) {
		try {
			string type = getDeclarationType(name);
			ref = Ref(name, type);
		}
		catch (const char* msg) {
			throw msg;
		}
	}
	else if (regex_match(name, expressionRegex)) {
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

Ref Preprocessor::createAttrRef(string name) {
	string type;

	int position = name.find(".");
	string synonym = trim(name.substr(0, position));
	string attrName = trim(name.substr(position + 1));
	attrName = StringToUpper(attrName);
	try {
		type = getDeclarationType(synonym);
	}
	catch (const char* msg) {
		throw msg;
	}

	if (regex_match(type, procNameRegex)) {
		if (attrName != "PROCNAME") {
			throw "wrong procedure type";
		}
	}
	else if (regex_match(type, stmtNoRegex)) {
		if (attrName != "STMT#") {
			throw "wrong stmt type";
		}
	}
	else if (regex_match(type, varNameRegex)) {
		if (attrName != "VARNAME") {
			throw "wrong variable type";
		}
	}
	else if (regex_match(type, valueRegex)) {
		if (attrName != "VALUE") {
			throw "wrong constant type";
		}
	}
	else {
		throw "synonym should not have type";
	}

	return Ref(synonym, type);
}

string Preprocessor::getDeclarationType(string name) {
	unordered_map<string, string>::const_iterator got = declarationMap.find(name);
	if (got == declarationMap.end()) {
		throw "invalid declaration";
	}
	return got->second;
}