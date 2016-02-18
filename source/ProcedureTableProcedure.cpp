#include "ProcedureTableProcedure.h"

ProcedureTableProcedure::ProcedureTableProcedure(std::string name, int index) {
	this->name = name;
	this->index = index;
	statements = new std::vector<int>();

	modifies = new std::vector<int>();
	uses = new std::vector<int>();
	procedureCalls = new std::vector<ProcedureTableProcedure*>();
	statementCalls = new std::vector<int>();

	indirectProcedureCalls = new std::set<int>();
	isIndirectProcedureCallsModified = false;
}

ProcedureTableProcedure::~ProcedureTableProcedure() {
	delete statements;
	
	delete modifies;
	delete uses;
	delete procedureCalls;
	delete statementCalls;

	delete indirectProcedureCalls;
}

std::string ProcedureTableProcedure::getName() {
	return name;
}

ProcedureTableProcedure* ProcedureTableProcedure::getProcedureCall(int index) {
	return procedureCalls->at(index);
}

int ProcedureTableProcedure::getProcedureCallsSize() {
	return procedureCalls->size();
}

std::set<int>* ProcedureTableProcedure::getIndirectProcedureCalls() {
	if (isIndirectProcedureCallsModified) {
		std::set<int>* secondarySet;
		ProcedureTableProcedure* procedureCall;
		int procedureCallIndex;

		// Iterate through the procedures that call this procedure
		int size = getProcedureCallsSize();
		for (int i = 0; i < size; i++) {
			procedureCall = getProcedureCall(i);
			procedureCallIndex = procedureCall->getIndex();

			// Check if you need to add this procedure's indirect procedure calls list to your own
			if ((indirectProcedureCalls->count(procedureCallIndex) == 0) || (procedureCall->isIndirectProcedureCallsModified)) {
				// Add the procedures that directly or indirectly call that procedure into the list as well
				secondarySet = procedureCall->getIndirectProcedureCalls();
				int sizeOfSecondSet = secondarySet->size();
				std::set<int>::iterator end = secondarySet->end();
				for (std::set<int>::iterator x = secondarySet->begin(); x != end; x++) {
					indirectProcedureCalls->insert(*x);
				}
			}

			indirectProcedureCalls->insert(procedureCallIndex);
		}

		isIndirectProcedureCallsModified = false;
	} 

	return indirectProcedureCalls;
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

bool ProcedureTableProcedure::addProcedureCalls(ProcedureTableProcedure* procedure) {
	return addIntoVector(procedure, procedureCalls);
	isIndirectProcedureCallsModified = true;
}

bool ProcedureTableProcedure::addStatementsCalls(int statementIndexNumber) {
	return addIntoVector(statementIndexNumber, statementCalls);
}

int ProcedureTableProcedure::getUses(int index) {
	uses->at(index);
}

int ProcedureTableProcedure::getUsesSize() {
	return uses->size();
}