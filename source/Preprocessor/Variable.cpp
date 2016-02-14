#include <string>
#include <regex>
#include "StringUlti.h"
#include "DeclarationMap.h"
#include "Variable.h"

Variable::Variable() {}

Variable::Variable(string nam, string typ) {
	name = nam;
	type = typ;
}

string Variable::getName() {
	return name;
}

string Variable::getType() {
	return type;
}

string Variable::toString() {
	return "<" + name + "," + type + ">";
}

bool Variable::equals(Variable v) {
	return name == v.getName() && type == v.getType();
}

string Variable::findSuchThatType(string raw) {
	string result;
	regex r("[[:digit:]]+");

	if (isExpr(raw)) {
		//name = raw.substr(1, raw.length() - 2);
		result = "expr";
	}
	else if (isName(raw)) {
		try {
			result = DeclarationMap::Instance()->find(raw);
		}
		catch (const char* msg) {
			throw msg;
		}
	}
	else if (regex_match(raw, r)) {
		result = "const_value";
	}
	else {
		throw "invalid variable " + raw;
	}
	return result;
}

string Variable::findPatternType(string raw) {
	string result;
	if (raw == "_") {
		result = "pattern_any";
	}
	else if (raw.find("_\"") == 0 && raw.find_last_of("\"_") == raw.length() - 2) {
		result = "pattern_anyExpr";
	}
	else if (isName(raw)) {
		try {
			result = DeclarationMap::Instance()->find(raw);
		}
		catch (const char* msg) {
			throw msg;
		}
	}
	else if (isExpr(raw)) {
		result = "expr";
	}
	else {
		throw "invalid pattern " + raw;
	}
	return result;
}

bool Variable::isName(string raw) {
	char tab2[1024];
	strcpy_s(tab2, raw.c_str());
	return isalpha(tab2[0]);
}

bool Variable::isExpr(string raw) {
	return raw.find("\"") == 0 && raw.find_last_of("\"") == raw.length() - 1;
}