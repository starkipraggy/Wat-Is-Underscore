#include <cstdlib>
#include "PKB.h"

PKB* PKB::instance = NULL;

PKB::PKB() {
	numberOfStatements = 0;
	procedureTable = new ProcedureTable();
	statementTable = new StatementTable();
	variableTable = new VariableTable();

	currentProcedure = NULL;
}

PKB::~PKB() {
	delete procedureTable;
	delete statementTable;
	delete variableTable;
}

StatementTableStatement* PKB::newStatement() {
	// Increment number of statements, and use the current
	// number of statements as the new statement's index number
	numberOfStatements++;

	// Adds statement number into current procedure
	if (currentProcedure != NULL) {
		currentProcedure->addStatement(numberOfStatements);
	}

	// Create a new statement in StatementTable with the index number
	StatementTableStatement* currentStatement = statementTable->addStatement(numberOfStatements);



	return currentStatement;
}

PKB* PKB::getInstance() {
	if (instance == NULL)
		instance = new PKB();
	return instance;
}

void PKB::ProcedureStart(std::string nameOfProcedure) {
	/* Create an entry for this new procedure in the procedure table, and keep a pointer to it in currentProcedure, 
	   so that statements being inputted can have their statement numbers added under it */
	currentProcedure = procedureTable->addProcedure(nameOfProcedure);
}

void PKB::ProcedureEnd() {
	// Remove the pointer to currentProcedure
	currentProcedure = NULL;
}

void PKB::AssignStatement(NAME variable, std::vector<std::string> tokens, std::vector<ExpressionTokenType> types) {
	StatementTableStatement* currentStatement = newStatement();

	// Add variable on the left side into the current procedure AND statement as a Modifies(p, v) relationship
	VariableTableVariable* currentVariable = variableTable->getVariableObject(variable);
	currentProcedure->addModifies(currentVariable->getIndex());
	//currentStatement->addModifies(currentVariable->getIndex());
	//@todo Implement addModifies and addUses in StatementTableStatement, just like as it is done in ProcedureTableProcedure


	// @todo Add variables on the right side into the current ProcedureTableProcedure as a Uses(p, v) relationship


	// @todo Add variables on the right side into the current StatementTableStatement as a Uses(p, v) relationship


	// @todo More implementation after I start working on the other two structures (currently still doing 1st one, ProcedureTable)

}

void PKB::CallStatement(std::string procedure) {
	newStatement();

	// @todo More implementation after I start working on the other two structures (currently still doing 1st one, ProcedureTable)

}

void PKB::WhileStart(NAME variable) {
	newStatement();

	// @todo More implementation after I start working on the other two structures (currently still doing 1st one, ProcedureTable)

}

void PKB::WhileEnd() {
	// @todo More implementation after I start working on the other two structures (currently still doing 1st one, ProcedureTable)

}

void PKB::IfStart(NAME variable) {
	newStatement();

	// @todo More implementation after I start working on the other two structures (currently still doing 1st one, ProcedureTable)

}

void PKB::ElseStart() {
	// @todo More implementation after I start working on the other two structures (currently still doing 1st one, ProcedureTable)

}

void PKB::IfElseEnd() {
	// @todo More implementation after I start working on the other two structures (currently still doing 1st one, ProcedureTable)

}