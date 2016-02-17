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

bool PKB::addRelationship(VariableTableVariable* variable, ProcedureTableProcedure* procedure, RelationshipType relationship) {
	int variableIndex = variable->getIndex();
	int procedureIndex = procedure->getIndex();

	switch (relationship) {
	case Modifies:
		procedure->addModifies(variableIndex);
		variable->addProcedureModifies(procedureIndex);
		break;
	case Uses:
		procedure->addUses(variableIndex);
		variable->addProcedureUses(procedureIndex);
		break;
	default:
		return false;
	}

	return true;
}

bool PKB::addRelationship(VariableTableVariable* variable, StatementTableStatement* statement, RelationshipType relationship) {
	int variableIndex = variable->getIndex();
	int statementIndex = statement->getIndex();

	switch (relationship) {
	case Modifies:
		statement->addModifies(variableIndex);
		variable->addStatementModifies(statementIndex);
		break;
	case Uses:
		statement->addUses(variableIndex);
		variable->addStatementUses(statementIndex);
	default:
		return false;
	}

	return true;
}

PKB* PKB::getInstance() {
	if (instance == NULL)
		instance = new PKB();
	return instance;
}

void PKB::ProcedureStart(std::string nameOfProcedure) {
	/* Create an entry for this new procedure in the procedure table, and keep a pointer to it in currentProcedure, 
	   so that statements being inputted can have their statement numbers added under it */
	currentProcedure = procedureTable->getProcedure(nameOfProcedure);
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

	// Fixed throughout the entire function as there is only one
	VariableTableVariable* leftVariable = variableTable->getVariableObject(variable); 

	// Used in for-loops for iterations, as there may be multiple right variables
	std::vector<VariableTableVariable*> rightVariables;
	for (unsigned int i = 0; i < size; i++) {
		if (types[i] == Variable) {
			rightVariables.push_back(variableTable->getVariableObject(tokens[i]));
		}
	}
	int rightVariablesSize = rightVariables.size();

	// Create a new statement for this assign statement, adding the statement number into current procedure
	currentStatement = newStatement();

	// Add variable on the left side into the current procedure AND statement as a Modifies(p, v) relationship
	addRelationship(leftVariable, currentProcedure, Modifies);
	addRelationship(leftVariable, currentStatement, Modifies);

	// Add variables on the right side into the current procedure AND statement as a Uses(p, v) relationship
	for (int i = 0; i < rightVariablesSize; i++) {
		addRelationship(rightVariables[i], currentProcedure, Uses);
		addRelationship(rightVariables[i], currentStatement, Uses);
	}

	StatementTableStatement* statementToIterateThroughParents = currentStatement;
	while (statementToIterateThroughParents->hasParent()) {
		statementToIterateThroughParents = statementTable->getStatement(statementToIterateThroughParents->getParent());

		// Add the Modifies relationship into statement's parent, and its parent, and its parent, etc.
		addRelationship(leftVariable, statementToIterateThroughParents, Modifies);

		// Add the Uses relationship into statement's parent, and its parent, and its parent, etc.
		for (int i = 0; i < rightVariablesSize; i++) {
			addRelationship(rightVariables[i], statementToIterateThroughParents, Uses);
		}
	}

	// Add the Modifies and Uses relationships into procedures that call the procedure, and all other procedures that call those procedures, etc.
	std::set<int>* proceduresSet = currentProcedure->getIndirectProcedureCalls();
	std::set<int>::iterator end = proceduresSet->end();
	ProcedureTableProcedure* procedureForAddingRelationship;
	for (std::set<int>::iterator i = proceduresSet->begin(); i != end; i++) {
		procedureForAddingRelationship = procedureTable->getProcedure(*i);
		addRelationship(leftVariable, procedureForAddingRelationship, Modifies);
		for (int x = 0; x < rightVariablesSize; x++) {
			addRelationship(rightVariables[x], procedureForAddingRelationship, Uses);
		}

		// Add the Modifies and Uses relationships into statements that call the above procedures
		int statementCallsSize = procedureForAddingRelationship->getStatementCallsSize();
		StatementTableStatement* statementForAddingRelationship;
		for (int x = 0; x < statementCallsSize; x++) {
			statementForAddingRelationship = statementTable->getStatement(procedureForAddingRelationship->getStatementCall(x));
			addRelationship(leftVariable, statementForAddingRelationship, Modifies);
			for (int y = 0; y < rightVariablesSize; y++) {
				addRelationship(rightVariables[y], statementForAddingRelationship, Uses);
			}
		}
	}

	return true;
}

void PKB::CallStatement(std::string procedure) {
	StatementTableStatement* currentStatement = newStatement();

	// @todo Add the procedure you're calling into the Calls relationship of the procedure that this statement belongs to
	ProcedureTableProcedure* procedureBeingCalled = procedureTable->getProcedure(procedure);
	procedureBeingCalled->addStatementsCalls(currentStatement->getIndex());
	procedureBeingCalled->addProcedureCalls(currentProcedure);
}

void PKB::WhileStart(NAME variable) {
	StatementTableStatement* currentStatement = newStatement();

	// Push 0 into statement stack trace to indicate a new level of nesting
	// 0 so the next statement has its Follows set to 0, ie it does not follow any statements
	statementStackTrace->push(0);

	// Adds Uses relationship with the variable being used to control the while-loop
	VariableTableVariable* currentVariable = variableTable->getVariableObject(variable);
	currentStatement->addUses(currentVariable->getIndex());
	currentVariable->addStatementUses(currentStatement->getIndex());

	// @todo Add Uses relationship with parent, parent's parent, etc.

	// @todo Add the Uses relationship into procedures that call the procedure, and all other procedures that call those procedures, etc.

	// @todo Add the Uses relationship into statements that call the above procedures
}

bool PKB::WhileEnd() {
	// Pop from statement stack trace to indicate the end of current level of nesting
	if (statementStackTrace->empty()) {
		return false;
	}

	statementStackTrace->pop();

	return true;
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