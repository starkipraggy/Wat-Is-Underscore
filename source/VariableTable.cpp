#include "VariableTable.h"

VariableTableVariable* VariableTable::addVariable(NAME variable) {
	VariableTableVariable* newVariable = new VariableTableVariable(variable, getNumberOfVariables());
	variables->push_back(newVariable);
	return newVariable;
}

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

VariableTableVariable* VariableTable::getVariableUsingName(NAME name) {
	int size = getNumberOfVariables();
	VariableTableVariable* currentVariable;

	for (int i = 0; i < size; i++) {
		currentVariable = variables->at(i);
		if (currentVariable->getName() == name) {
			return currentVariable;
		}
	}

	return addVariable(name);
}

VariableTableVariable* VariableTable::getVariableUsingVariableIndexNumber(int index) {
	int size = getNumberOfVariables();
	VariableTableVariable* currentVariable;

	for (int i = 0; i < size; i++) {
		currentVariable = variables->at(i);
		if (currentVariable->getIndex() == index) {
			return currentVariable;
		}
	}

	return NULL;
}

VariableTableVariable* VariableTable::getVariableUsingVectorIndexNumber(int vectorIndex) {
	if (vectorIndex < 0) {
		return NULL;
	}
	if (vectorIndex >= getNumberOfVariables()) {
		return NULL;
	}
	return variables->at(vectorIndex);
}