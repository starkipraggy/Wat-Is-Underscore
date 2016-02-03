#include "PKB.h"

#include <cstdlib>

PKB* PKB::instance = NULL;

PKB::PKB() {

}

PKB::~PKB() {

}

PKB* PKB::getInstance() {
	if (instance == NULL)
		instance = new PKB();
	return instance;
}

void PKB::ProcedureStart(std::string nameOfProcedure) {

}

void PKB::ProcedureEnd() {

}

void PKB::AssignStatement(NAME variable, std::string expression) {

}

void PKB::CallStatement(std::string procedure) {

}

void PKB::WhileStart(NAME variable) {
	
}

void PKB::WhileEnd() {

}

void PKB::IfStart(NAME variable) {

}

void PKB::ElseStart() {

}

void PKB::IfElseEnd() {

}