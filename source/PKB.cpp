#include <cstdlib>
#include "PKB.h"

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

void PKB::AssignStatement(NAME variable, std::vector<int> intVector, std::vector<char> varVector) {
	// Evaluate the expression based on the rules of SIMPLE
	// and then assign it to the variable
	int sum = 0;
	
	for (int i = 0; i < varVector.size(); i++) {
		// char newVar = varVector[i];
		// newVarValue = Variables.getValue(newVar[i];
		// sum += newVarValue;
	}

	for (int j = 0; j < intVector.size(); j++) {
		sum += intVector[i];
	}

	variable = sum;
}

void PKB::CallStatement(std::string procedure) {

}

void PKB::WhileStart(NAME variable) {
	bool isInLoop = true;
	while (isInLoop) {
		// Manage the statements whether it's more while loop, or assignment
		// Exit loop when whileEnd is invoked
	}
	// Move onto next line outside of the loop
}

void PKB::WhileEnd() {
	// Sends signal to whileStart to finish looping
	// Moves onto next line

}

void PKB::IfStart(NAME variable) {

}

void PKB::ElseStart() {

}

void PKB::IfElseEnd() {

}