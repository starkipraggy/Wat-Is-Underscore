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

void ProcedureTableProcedure::addStatement(int statement) {
	statements->push_back(statement);
}

bool ProcedureTableProcedure::addModifies(int variableIndexNumber) {
	unsigned int size = modifies->size();
	for (unsigned int i = 0; i < size; i++) {
		if (variableIndexNumber == modifies->at(i)) {
			return false;
		}
	}
	modifies->push_back(variableIndexNumber);
	return true;
}

bool ProcedureTableProcedure::addUses(int variableIndexNumber) {
	unsigned int size = uses->size();
	for (unsigned int i = 0; i < size; i++) {
		if (variableIndexNumber == uses->at(i)) {
			return false;
		}
	}
	uses->push_back(variableIndexNumber);
	return true;
}