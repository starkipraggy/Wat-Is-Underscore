#include "ProcedureTableProcedure.h"

ProcedureTableProcedure::ProcedureTableProcedure(std::string name, int index) {
	this->name = name;
	this->index = index;
	statements = new std::vector<int>();

	modifies = new std::vector<int>();
	uses = new std::vector<int>();
	procedureCallBy = new std::vector<ProcedureTableProcedure*>();
	statementCallBy = new std::vector<int>();

	indirectProcedureCallBy = new std::set<int>();
	isIndirectProcedureCallByModified = false;
}

ProcedureTableProcedure::~ProcedureTableProcedure() {
	delete statements;
	
	delete modifies;
	delete uses;
	delete procedureCallBy;
	delete statementCallBy;

	delete indirectProcedureCallBy;
}

std::string ProcedureTableProcedure::getName() {
	return name;
}

int ProcedureTableProcedure::getIndex() {
	return index;
}

ProcedureTableProcedure* ProcedureTableProcedure::getProcedureCallBy(int index) {
	return procedureCallBy->at(index);
}

int ProcedureTableProcedure::getProcedureCallBySize() {
	return procedureCallBy->size();
}

#include <iostream>

std::set<int>* ProcedureTableProcedure::getIndirectProcedureCallBy() {
	if (isIndirectProcedureCallByModified) {
		std::set<int>* secondarySet;
		ProcedureTableProcedure* procedureCall;
		int procedureCallIndex;

		// Iterate through the procedures that call this procedure
		int size = getProcedureCallBySize();
		for (int i = 0; i < size; i++) {
			procedureCall = getProcedureCallBy(i);
			procedureCallIndex = procedureCall->getIndex();

			// Check if you need to add this procedure's indirect procedure calls list to your own
			if ((indirectProcedureCallBy->count(procedureCallIndex) == 0) || (procedureCall->isIndirectProcedureCallByModified)) {
				// Add the procedures that directly or indirectly call that procedure into the list as well
				secondarySet = procedureCall->getIndirectProcedureCallBy();
				int sizeOfSecondSet = secondarySet->size();
				std::set<int>::iterator end = secondarySet->end();
				for (std::set<int>::iterator x = secondarySet->begin(); x != end; x++) {
					indirectProcedureCallBy->insert(*x);
				}
			}

			indirectProcedureCallBy->insert(procedureCallIndex);
		}

		isIndirectProcedureCallByModified = false;
	} 

	return indirectProcedureCallBy;
}

int ProcedureTableProcedure::getStatementCallBy(int index) {
	return statementCallBy->at(index);
}

int ProcedureTableProcedure::getStatementCallBySize() {
	return statementCallBy->size();
}

void ProcedureTableProcedure::addStatement(int statement) {
	statements->push_back(statement);
}

bool ProcedureTableProcedure::addModifies(int variableIndexNumber) {
	return DataStructureObject::addIntoVector(variableIndexNumber, modifies);
}

bool ProcedureTableProcedure::addUses(int variableIndexNumber) {
	return DataStructureObject::addIntoVector(variableIndexNumber, uses);
}

bool ProcedureTableProcedure::addProcedureCallBy(ProcedureTableProcedure* procedure) {
	isIndirectProcedureCallByModified = true;
	return DataStructureObject::addIntoVector(procedure, procedureCallBy);
}

bool ProcedureTableProcedure::addStatementsCallBy(int statementIndexNumber) {
	return DataStructureObject::addIntoVector(statementIndexNumber, statementCallBy);
}

int ProcedureTableProcedure::getUses(int index) {
	return uses->at(index);
}

int ProcedureTableProcedure::getUsesSize() {
	return uses->size();
}

int ProcedureTableProcedure::getModifies(int index) {
	return modifies->at(index);
}

int ProcedureTableProcedure::getModifiesSize() {
	return modifies->size();
}