#include <cstdlib>
#include "PKB.h"

PKB* PKB::instance = NULL;

PKB::PKB() {
	numberOfStatements = 0;
	procedureTable = new ProcedureTable();
	statementTable = new StatementTable();
	variableTable = new VariableTable();

	currentProcedure = NULL;
	statementStackTrace = new std::stack<int>();
	statementStackTrace->push(0);
}

PKB::~PKB() {
	delete procedureTable;
	delete statementTable;
	delete variableTable;
	delete statementStackTrace;
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

	// Sets this statement's Follows and Parent relationship using the statement stack trace
	currentStatement->setFollows(statementStackTrace->top());
	statementStackTrace->pop();
	if (statementStackTrace->size() > 0) {
		currentStatement->setParent(statementStackTrace->top());
	}
	statementStackTrace->push(numberOfStatements);

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
	int currentVariableIndex;
	int parentStatementIndex;

	// Create a new statement for this assign statement, adding the statement number into current procedure
	currentStatement = newStatement();

	// Add variable on the left side into the current procedure AND statement as a Modifies(p, v) relationship
	currentVariable = variableTable->getVariableObject(variable);
	currentVariableIndex = currentVariable->getIndex();
	currentProcedure->addModifies(currentVariableIndex);
	currentVariable->addProcedureModifies(currentProcedure->getIndex());
	currentStatement->addModifies(currentVariableIndex);
	currentVariable->addStatementModifies(currentStatement->getIndex());

	// Add the Modifies relationship into statement's parent, and its parent, and its parent, etc.
	while (currentStatement->hasParent()) {
		parentStatementIndex = currentStatement->getParent();
		currentStatement = statementTable->getStatement(parentStatementIndex);
		currentStatement->addModifies(currentVariableIndex);
		currentVariable->addStatementModifies(parentStatementIndex);
	}

	// @todo Add the Modifies relationship into procedures that call the procedure, and all other procedures that call those procedures, etc., recursively

	// @todo Add the Modifies relationship into statements that call the above procedures

	for (unsigned int i = 0; i < size; i++) {
		if (types[i] == Variable) {
			currentVariable = variableTable->getVariableObject(tokens[i]);
			currentVariableIndex = currentVariable->getIndex();

			// Add variables on the right side into the current procedure AND statement as a Uses(p, v) relationship
			currentProcedure->addUses(currentVariableIndex);
			currentVariable->addProcedureUses(currentProcedure->getIndex());
			currentStatement->addUses(currentVariableIndex);
			currentVariable->addStatementUses(currentStatement->getIndex());

			// Add the Uses relationship into statement's parent, and its parent, and its parent, etc., recursively
			while (currentStatement->hasParent()) {
				parentStatementIndex = currentStatement->getParent();
				currentStatement = statementTable->getStatement(parentStatementIndex);
				currentStatement->addUses(currentVariableIndex);
				currentVariable->addStatementUses(parentStatementIndex);
			}

			// @todo Add the Uses relationship into procedures that call the procedure, and all other procedures that call those procedures, etc., recursively

			// @todo Add the Uses relationship into statements that call the above procedures
		}
	}

	return true;
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