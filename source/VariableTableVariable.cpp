#include "VariableTableVariable.h"

VariableTableVariable::VariableTableVariable(NAME name, int index) {
	this->name = name;
	this->index = index;
	statementModifies = new std::vector<int>();
	statementUses = new std::vector<int>();
	procedureModifies = new std::vector<int>();
	procedureUses = new std::vector<int>();
}

VariableTableVariable::~VariableTableVariable() {
	delete statementModifies;
	delete statementUses;
	delete procedureModifies;
	delete procedureUses;
}

NAME VariableTableVariable::getName() {
	return name;
}

bool VariableTableVariable::addStatementModifies(int statementNumber) {
	return addIntoVector(statementNumber, statementModifies);
}

bool VariableTableVariable::addStatementUses(int statementNumber) {
	return addIntoVector(statementNumber, statementUses);
}

bool VariableTableVariable::addProcedureModifies(int procedureNumber) {
	return addIntoVector(procedureNumber, procedureModifies);
}

bool VariableTableVariable::addProcedureUses(int procedureNumber) {
	return addIntoVector(procedureNumber, procedureUses);
}

std::vector<int>* VariableTableVariable::getProcedureUses() {
	return procedureUses;
}

std::vector<int>* VariableTableVariable::getStatementUses() {
	return statementUses;
}

std::vector<int>* VariableTableVariable::getProcedureModifies() {
	return procedureModifies;
}

std::vector<int>* VariableTableVariable::getStatementModifies() {
	return statementModifies;
}