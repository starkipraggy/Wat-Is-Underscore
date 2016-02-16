#include "ProcedureTableProcedure.h"

ProcedureTableProcedure::ProcedureTableProcedure(std::string name, int index) {
	this->name = name;
	this->index = index;
	statements = new std::vector<int>();

	modifies = new std::vector<int>();
	uses = new std::vector<int>();
}

ProcedureTableProcedure::~ProcedureTableProcedure() {
	delete statements;
	
	delete modifies;
	delete uses;
}

int ProcedureTableProcedure::getIndex() {
	return index;
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