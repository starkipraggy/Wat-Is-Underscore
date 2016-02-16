#include <string>
#include <regex>
#include "Clause.h"
#include "StringUlti.h"

Clause::Clause() {}

Clause::Clause(string clau, Variable var1, Variable var2) {
	clause = clau;
	variableOne = var1;
	variableTwo = var2;
}

string Clause::getClause() {
	return clause;
}

Variable Clause::getVariableOne() {
	return variableOne;
}

Variable Clause::getVariableTwo() {
	return variableTwo;
}

string Clause::getQuery() {
	return variableOne.toString() + "+" + variableTwo.toString();
}

bool Clause::hasVariable(Variable v) {
	return variableOne.equals(v) || variableTwo.equals(v);
}

bool Clause::hasLinkedVariable(Variable v) {
	bool result;
	if (variableOne.equals(v)) {
		result = std::regex_match(StringToUpper(variableTwo.getType()), std::regex("ASSIGN|STMT|WHILE|VARIABLE|CONSTANT|PROG_LINE|PROCEDURE"));
	}
	else if (variableTwo.equals(v)) {
		result = std::regex_match(StringToUpper(variableOne.getType()), std::regex("ASSIGN|STMT|WHILE|VARIABLE|CONSTANT|PROG_LINE|PROCEDURE"));
	}
	return result;
}

Variable Clause::getLinkedVariable(Variable v) {
	if (variableOne.equals(v)) {
		return variableTwo;
	}
	else {
		return variableOne;
	}
}

bool Clause::hasAtMostOneCommonSynonym(Clause* c) {
	return !((variableOne.equals(c->getVariableOne()) && variableTwo.equals(c->getVariableTwo()))
		|| (variableOne.equals(c->getVariableTwo()) && variableTwo.equals(c->getVariableOne())));
}