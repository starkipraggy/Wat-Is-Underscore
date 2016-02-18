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

	// Sets this statement's Follows relationship using the statement stack trace
	int beingFollowed = statementStackTrace->top();
	currentStatement->setFollows(beingFollowed);
	statementTable->getStatement(beingFollowed)->setFollowedBy(numberOfStatements);

	// Sets this statement's Parent relationship using the statement stack trace
	statementStackTrace->pop();
	if (statementStackTrace->size() > 0) {
		int parent = statementStackTrace->top();
		currentStatement->setParent(parent);
		statementTable->getStatement(parent)->addChild(numberOfStatements);
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

	// Set the type of the statement to be an assignment
	currentStatement->setType(Assign);

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

	// Set the type of the statement to be a call
	currentStatement->setType(Call);

	// Add the procedure you're calling into the Calls relationship of the procedure that this statement belongs to
	ProcedureTableProcedure* procedureBeingCalled = procedureTable->getProcedure(procedure);
	procedureBeingCalled->addStatementsCalls(currentStatement->getIndex());
	procedureBeingCalled->addProcedureCalls(currentProcedure);
}

void PKB::WhileStart(NAME variable) {
	StatementTableStatement* currentStatement = newStatement();
	VariableTableVariable* currentVariable = variableTable->getVariableObject(variable);

	// Set the type of the statement to be a while-loop
	currentStatement->setType(While);

	// Push 0 into statement stack trace to indicate a new level of nesting
	// 0 so the next statement has its Follows set to 0, ie it does not follow any statements
	statementStackTrace->push(0);

	// Adds Uses relationship with the variable being used to control the while-loop
	addRelationship(currentVariable, currentStatement, Uses);

	// Add Uses relationship with parent, parent's parent, etc.
	StatementTableStatement* statementToIterateThroughParents = currentStatement;
	while (statementToIterateThroughParents->hasParent()) {
		statementToIterateThroughParents = statementTable->getStatement(statementToIterateThroughParents->getParent());
		addRelationship(currentVariable, statementToIterateThroughParents, Uses);
	}

	// Add the Uses relationship into procedures that call the procedure, and all other procedures that call those procedures, etc.
	std::set<int>* proceduresSet = currentProcedure->getIndirectProcedureCalls();
	std::set<int>::iterator end = proceduresSet->end();
	ProcedureTableProcedure* procedureForAddingRelationship;
	for (std::set<int>::iterator i = proceduresSet->begin(); i != end; i++) {
		procedureForAddingRelationship = procedureTable->getProcedure(*i);
		addRelationship(currentVariable, procedureForAddingRelationship, Uses);

		// Add the Uses relationship into statements that call the above procedures
		int statementCallsSize = procedureForAddingRelationship->getStatementCallsSize();
		StatementTableStatement* statementForAddingRelationship;
		for (int x = 0; x < statementCallsSize; x++) {
			statementForAddingRelationship = statementTable->getStatement(procedureForAddingRelationship->getStatementCall(x));
			addRelationship(currentVariable, statementForAddingRelationship, Uses);
		}
	}
}

bool PKB::WhileEnd() {
	// Pop from statement stack trace to indicate the end of current level of nesting
	if (statementStackTrace->size() < 2) {
		return false;
	}

	statementStackTrace->pop();

	return true;
}

void PKB::IfStart(NAME variable) {
	StatementTableStatement* currentStatement = newStatement();
	VariableTableVariable* currentVariable = variableTable->getVariableObject(variable);

	// Set the type of the statement to be an if-statement
	currentStatement->setType(If);

	// Push 0 into statement stack trace to indicate a new level of nesting
	// 0 so the next statement has its Follows set to 0, ie it does not follow any statements
	statementStackTrace->push(0);

	// Adds Uses relationship with the variable being used to control the while-loop
	addRelationship(currentVariable, currentStatement, Uses);

	// Add Uses relationship with parent, parent's parent, etc.
	StatementTableStatement* statementToIterateThroughParents = currentStatement;
	while (statementToIterateThroughParents->hasParent()) {
		statementToIterateThroughParents = statementTable->getStatement(statementToIterateThroughParents->getParent());
		addRelationship(currentVariable, statementToIterateThroughParents, Uses);
	}

	// Add the Uses relationship into procedures that call the procedure, and all other procedures that call those procedures, etc.
	std::set<int>* proceduresSet = currentProcedure->getIndirectProcedureCalls();
	std::set<int>::iterator end = proceduresSet->end();
	ProcedureTableProcedure* procedureForAddingRelationship;
	for (std::set<int>::iterator i = proceduresSet->begin(); i != end; i++) {
		procedureForAddingRelationship = procedureTable->getProcedure(*i);
		addRelationship(currentVariable, procedureForAddingRelationship, Uses);

		// Add the Uses relationship into statements that call the above procedures
		int statementCallsSize = procedureForAddingRelationship->getStatementCallsSize();
		StatementTableStatement* statementForAddingRelationship;
		for (int x = 0; x < statementCallsSize; x++) {
			statementForAddingRelationship = statementTable->getStatement(procedureForAddingRelationship->getStatementCall(x));
			addRelationship(currentVariable, statementForAddingRelationship, Uses);
		}
	}
}

bool PKB::ElseStart() {
	if (statementStackTrace->size() < 2) {
		return false;
	}

	statementStackTrace->pop();
	statementStackTrace->push(0);

	return true;
}

bool PKB::IfElseEnd() {
	if (statementStackTrace->size() < 2) {
		return false;
	}

	statementStackTrace->pop();

	return true;

}

std::vector<int>* PKB::QueryPKBWithoutPatternUsesMods(std::string queryType, std::string input, int argumentPosition, std::string outputType) {
	if (queryType == "uses") {
		if (argumentPosition == 1) { //check which procedure, assignment or statement number uses a variable
			if (outputType == "procedure") { //
				VariableTableVariable* variableToBeChecked = variableTable->getVariableObject(input);
				std::vector<int>* procedureUses = variableToBeChecked->getProcedureUses();
				return procedureUses;
			}
			else if (outputType == "statement") {
				VariableTableVariable* variableToBeChecked = variableTable->getVariableObject(input);
				std::vector<int>* statementUses = variableToBeChecked->getStatementUses();
				return statementUses;
			}
			else { //assignment uses
				VariableTableVariable* variableToBeChecked = variableTable->getVariableObject(input);

			}
		}
		else { //check what variables are used by a procedure or statement
			if (outputType == "procedure") {
				ProcedureTableProcedure* procedure = procedureTable->getProcedure(input);
				std::vector<int>* usesList = procedure->getUses();
				return usesList;
			}
			else if (outputType == "statement") {
				int inputInteger = atoi(input.c_str());
				StatementTableStatement* statement = statementTable->getStatement(inputInteger);
				std::vector<int>* usesList = statement->getUses();
				return usesList;
			}
			else {

			}
		}
	}
	else {
		if (argumentPosition == 1) { //check which procedure, assignment or statement number modifies a variable
			if (outputType == "procedure") { //
				VariableTableVariable* variableToBeChecked = variableTable->getVariableObject(input);
				std::vector<int>* procedureModifies = variableToBeChecked->getProcedureModifies();
				return procedureModifies;
			}
			else if (outputType == "statement") {
				VariableTableVariable* variableToBeChecked = variableTable->getVariableObject(input);
				std::vector<int>* statementModifies = variableToBeChecked->getStatementModifies();
				return statementModifies;
			}
			else {

			}
		}
		else { //check what variables are modified by a procedure or statement
			if (outputType == "procedure") {
				ProcedureTableProcedure* procedure = procedureTable->getProcedure(input);
				std::vector<int>* modifiesList = procedure->getModifies();
				return modifiesList;
			}
			else if (outputType == "statement") {
				int inputInteger = atoi(input.c_str());
				StatementTableStatement* statement = statementTable->getStatement(inputInteger);
				std::vector<int>* modifiesList = statement->getModifies();
				return modifiesList;
			}
			else {

			}
		}
	}
}

std::vector<int>* PKB::QueryPKBWithoutPatternFollowsParent(std::string queryType, int input, int argumentPosition, std::string outputType) {
	if (queryType == "follows") {
		if (argumentPosition == 1) { //check statements that has follows(s, input)

		}
		else { //check statements that has follows(input, s)
			StatementTableStatement* statement = statementTable->getStatement(input);
			int follows = statement->getFollows();
			std::vector<int>* returnList = new std::vector<int>();
			returnList->push_back(follows);
			return returnList;
		}
	}
	else if (queryType == "follows*") {
		if (argumentPosition == 1) { //check statements that has follows*(s, input)

		}
		else { //check statements that has follows*(input, s)

		}
	}
	else if (queryType == "parent") {
		if (argumentPosition == 1) { //check statements that has parent(s, input)

		}
		else { //check statements that has parent(input, s)
			StatementTableStatement* statement = statementTable->getStatement(input);
			int parent = statement->getParent();
			std::vector<int>* returnList = new std::vector<int>();
			returnList->push_back(parent);
			return returnList;
		}
	}
	else { //parent* relationship
		if (argumentPosition == 1) { //check statements that has parent*(s, input)

		}
		else { //check statements that has parent*(input, s)

		}
	}
}

std::vector<int>* PKB::QueryPKBPattern(NAME leftVariable, std::string rightExpression, bool isUnderscored) {
	// @todo Wait for Alan and Chun How's confirmation
	return NULL;
}