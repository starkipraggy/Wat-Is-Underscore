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

int VariableTableVariable::getIndex() {
	return index;
}

bool VariableTableVariable::addStatementModifies(int statementNumber) {
	return DataStructureObject::addIntoVector(statementNumber, statementModifies);
}

bool VariableTableVariable::addStatementUses(int statementNumber) {
	return DataStructureObject::addIntoVector(statementNumber, statementUses);
}

bool VariableTableVariable::addProcedureModifies(int procedureNumber) {
	return DataStructureObject::addIntoVector(procedureNumber, procedureModifies);
}

bool VariableTableVariable::addProcedureUses(int procedureNumber) {
	return DataStructureObject::addIntoVector(procedureNumber, procedureUses);
}

int VariableTableVariable::getProceduresUses(int index) {
	return procedureUses->at(index);
}

int VariableTableVariable::getProceduresUsesSize() {
	return procedureUses->size();
}

int VariableTableVariable::getProceduresModifies(int index) {
	return procedureModifies->at(index);
}

int VariableTableVariable::getProceduresModifiesSize() {
	return procedureModifies->size();
}

int VariableTableVariable::getStatementUses(int index) {
	return statementUses->at(index);
}

int VariableTableVariable::getStatementUsesSize() {
	return statementUses->size();
}

int VariableTableVariable::getStatementModifies(int index) {
	return statementModifies->at(index);
}

int VariableTableVariable::getStatementModifiesSize() {
	return statementModifies->size();
}
