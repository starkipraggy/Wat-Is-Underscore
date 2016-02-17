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

ProcedureTableProcedure* ProcedureTable::getProcedure(int procedureNumber) {
	int size = getNumberOfProcedures();
	ProcedureTableProcedure* procedure;
	for (int i = 0; i < size; i++) {
		procedure = procedures->at(i);
		if (procedure->getIndex() == procedureNumber) {
			return procedure;
		}
	}
	return NULL;
}

ProcedureTableProcedure* ProcedureTable::addProcedure(std::string procedure) {
	ProcedureTableProcedure* newProcedure = new ProcedureTableProcedure(procedure, getNumberOfProcedures());
	procedures->push_back(newProcedure);
	return newProcedure;
}