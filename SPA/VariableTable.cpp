#include "VariableTable.h"

VariableTable::VariableTable() {
	variables = new std::vector<VariableTableVariable*>();
}

VariableTable::~VariableTable() {
	int numberOfVariables = getNumberOfVariables();
	for (int i = 0; i < numberOfVariables; i++) {
		delete variables->at(i);
		variables->at(i) = NULL;
	}
	delete variables;
}

int VariableTable::getNumberOfVariables() {
	return variables->size();
}

void VariableTable::addVariable(NAME variable) {
	VariableTableVariable* newVariable = new VariableTableVariable(variable, getNumberOfVariables());
	variables->push_back(newVariable);
}