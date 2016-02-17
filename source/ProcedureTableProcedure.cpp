#include "ProcedureTableProcedure.h"

ProcedureTableProcedure::ProcedureTableProcedure(std::string name, int index) {
	this->name = name;
	this->index = index;
	statements = new std::vector<int>();

	modifies = new std::vector<int>();
	uses = new std::vector<int>();
	procedureCalls = new std::vector<int>();
	statementCalls = new std::vector<int>();
}

ProcedureTableProcedure::~ProcedureTableProcedure() {
	delete statements;
	
	delete modifies;
	delete uses;
	delete procedureCalls;
	delete statementCalls;
}

int ProcedureTableProcedure::getIndex() {
	return index;
}

int ProcedureTableProcedure::getProcedureCall(int index) {
	return procedureCalls->at(index);
}

int ProcedureTableProcedure::getProcedureCallsSize() {
	return procedureCalls->size();
}

int ProcedureTableProcedure::getStatementCall(int index) {
	return statementCalls->at(index);
}

int ProcedureTableProcedure::getStatementCallsSize() {
	return statementCalls->size();
}

void ProcedureTableProcedure::addStatement(int statement) {
	statements->push_back(statement);
}

bool ProcedureTableProcedure::addModifies(int variableIndexNumber) {
	return addIntoVector(variableIndexNumber, modifies);
}

bool ProcedureTableProcedure::addUses(int variableIndexNumber) {
	return addIntoVector(variableIndexNumber, uses);
}

bool ProcedureTableProcedure::addProcedureCalls(int procedureIndexNumber) {
	return addIntoVector(procedureIndexNumber, procedureCalls);
}

bool ProcedureTableProcedure::addStatementsCalls(int statementIndexNumber) {
	return addIntoVector(statementIndexNumber, statementCalls);

}