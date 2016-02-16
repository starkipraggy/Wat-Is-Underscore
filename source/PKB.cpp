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

bool PKB::AssignStatement(NAME variable, std::vector<std::string> tokens, std::vector<ExpressionTokenType> types) {
	// Ensure that the sizes of "tokens" and "types" are the same
	unsigned int size = tokens.size();
	if (size != types.size()) {
		return false;
	}

	StatementTableStatement* currentStatement;
	VariableTableVariable* currentVariable;

	// Create a new statement for this assign statement, adding the statement number into current procedure
	currentStatement = newStatement();

	// Add variable on the left side into the current procedure AND statement as a Modifies(p, v) relationship
	currentVariable = variableTable->getVariableObject(variable);
	currentProcedure->addModifies(currentVariable->getIndex());
	currentVariable->addProcedureModifies(currentProcedure->getIndex());
	currentStatement->addModifies(currentVariable->getIndex());
	currentVariable->addStatementModifies(currentStatement->getIndex());

	// @todo Add the Modifies relationship into statement's parent, and its parent, and its parent, etc., recursively

	// @todo Add the Modifies relationship into procedures that call the procedure, and all other procedures that call those procedures, etc., recursively

	// @todo Add the Modifies relationship into statements that call the above procedures

	// Add variables on the right side into the current procedure AND statement as a Uses(p, v) relationship
	for (unsigned int i = 0; i < size; i++) {
		if (types[i] == Variable) {
			currentVariable = variableTable->getVariableObject(tokens[i]);
			currentProcedure->addUses(currentVariable->getIndex());
			currentVariable->addProcedureUses(currentProcedure->getIndex());
			currentStatement->addUses(currentVariable->getIndex());
			currentVariable->addStatementUses(currentStatement->getIndex());
		}
	}

	// @todo Add the Uses relationship into statement's parent, and its parent, and its parent, etc., recursively

	// @todo Add the Uses relationship into procedures that call the procedure, and all other procedures that call those procedures, etc., recursively

	// @todo Add the Uses relationship into statements that call the above procedures

	// @todo Implement being able to find the statement it follows (this statement appears immediately after the one it follows)

	// @todo Implement being able to find the parent of this statement
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