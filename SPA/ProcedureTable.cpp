#include "ProcedureTable.h"

ProcedureTable::ProcedureTable() {
	procedures = new std::vector<ProcedureTableProcedure*>();
}

ProcedureTable::~ProcedureTable() {
	int numberOfProcedures = getNumberOfProcedures();
	for (int i = 0; i < numberOfProcedures; i++) {
		delete procedures->at(i);
		procedures->at(i) = NULL;
	}
	delete procedures;
}

int ProcedureTable::getNumberOfProcedures() {
	return procedures->size();
}

void ProcedureTable::addProcedure(std::string procedure) {
	ProcedureTableProcedure* newProcedure = new ProcedureTableProcedure(procedure, getNumberOfProcedures());
	procedures->push_back(newProcedure);
}