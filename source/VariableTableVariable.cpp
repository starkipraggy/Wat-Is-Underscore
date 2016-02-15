#include "VariableTableVariable.h"

VariableTableVariable::VariableTableVariable(NAME name, int index) {
	this->name = name;
	this->index = index;
	modifies = new std::vector<int>();
	uses = new std::vector<int>();
}

VariableTableVariable::~VariableTableVariable() {
	delete modifies;
	delete uses;
}

NAME VariableTableVariable::getName() {
	return name;
}

int VariableTableVariable::getIndex() {
	return index;
}