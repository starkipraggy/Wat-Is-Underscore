#include "ProcedureTableProcedure.h"

ProcedureTableProcedure::ProcedureTableProcedure(std::string name, int index) {
	this->name = name;
	this->index = index;
	modifies = new std::vector<int>();
	uses = new std::vector<int>();
}

ProcedureTableProcedure::~ProcedureTableProcedure() {
	delete modifies;
	delete uses;
}