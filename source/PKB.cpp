#include <cstdlib>
#include "PKB.h"
#include <iostream>

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
	if (statementStackTrace->top() != 0) {
		StatementTableStatement* beingFollowed = statementTable->getStatementUsingStatementNumber(statementStackTrace->top());
		currentStatement->setFollows(beingFollowed);
		beingFollowed->setFollowedBy(currentStatement);
	}
	// Sets this statement's Parent relationship using the statement stack trace
	statementStackTrace->pop();
	if (statementStackTrace->size() > 0) {
		StatementTableStatement* parent = statementTable->getStatementUsingStatementNumber(statementStackTrace->top());
		currentStatement->setParent(parent);
		parent->addChild(currentStatement);
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

void PKB::setInstance(PKB* newPkb) {
	instance = newPkb;
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
	VariableTableVariable* leftVariable = variableTable->getVariableUsingName(variable);

	// Used in for-loops for iterations, as there may be multiple right variables
	std::vector<VariableTableVariable*> rightVariables;
	for (unsigned int i = 0; i < size; i++) {
		if (types[i] == Variable) {
			rightVariables.push_back(variableTable->getVariableUsingName(tokens[i]));
		}
	}
	int rightVariablesSize = rightVariables.size();
	std::cout << "11111";
	// Create a new statement for this assign statement, adding the statement number into current procedure
	currentStatement = newStatement();
	std::cout << "2222";
	// Set the type of the statement to be an assignment
	currentStatement->setType(Assign);
	std::cout << "33333";
	// Set expression of this statement
	std::string rightHandSideExpression = "";
	for (unsigned int i = 0; i < size; i++) {
		rightHandSideExpression += tokens[i];
	}
	std::cout << "4444";
	currentStatement->setRightHandSideExpression(rightHandSideExpression);
	std::cout << "5555";
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
		statementToIterateThroughParents = statementTable->getStatementUsingStatementNumber(statementToIterateThroughParents->getParent());

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
			statementForAddingRelationship = statementTable->getStatementUsingStatementNumber(procedureForAddingRelationship->getStatementCall(x));
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
	VariableTableVariable* currentVariable = variableTable->getVariableUsingName(variable);

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
		statementToIterateThroughParents = statementTable->getStatementUsingStatementNumber(statementToIterateThroughParents->getParent());
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
			statementForAddingRelationship = statementTable->getStatementUsingStatementNumber(procedureForAddingRelationship->getStatementCall(x));
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
	VariableTableVariable* currentVariable = variableTable->getVariableUsingName(variable);

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
		statementToIterateThroughParents = statementTable->getStatementUsingStatementNumber(statementToIterateThroughParents->getParent());
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
			statementForAddingRelationship = statementTable->getStatementUsingStatementNumber(procedureForAddingRelationship->getStatementCall(x));
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

std::vector<std::string> PKB::PQLSelect(TNodeType outputType) {
	std::vector<std::string> returnList;
	StatementTableStatement* statement;

	// return all variables
	if (outputType == VariableName) {
		int variableTableSize = variableTable->getNumberOfVariables();
		for (int i = 0; i < variableTableSize; i++) {
			returnList.push_back(variableTable->getVariableUsingVectorIndexNumber(i)->getName());
		}
		return returnList;

	}

	// If outputType is not Assign, If, While, or Call, return all statements
	bool returnAllStatements = true;
	if ((outputType == Assign) || (outputType == If) || (outputType == While) || (outputType == Call)) {
		returnAllStatements = false;
	}

	// Iterate through statements, check if they should be returned
	int statementTableSize = statementTable->getNumberOfStatements();
	for (int i = 0; i < statementTableSize; i++) {
		statement = statementTable->getStatementUsingVectorIndexNumber(i);
		if ((returnAllStatements) || (statement->getType() == outputType)) {
			returnList.push_back(std::to_string(statement->getIndex()));
		}
	}
	return returnList;
}

std::vector<std::string> PKB::PQLUses(std::string input, int argumentPosition, std::string outputType) {
	std::vector<std::string> returnList;
	int size;

	switch (argumentPosition) {
	case 1: { // Check which procedure, assignment, while or statement number uses the variable "input"
		VariableTableVariable* variableToBeChecked = variableTable->getVariableUsingName(input);

		// Simply return procedures first
		if (outputType == "procedure") {
			size = variableToBeChecked->getProceduresUsesSize();
			for (int i = 0; i < size; i++) {
				returnList.push_back(procedureTable->getProcedure(variableToBeChecked->getProceduresUses(i))->getName());
			}
		}
		else {
			// Check which statements are to be returned
			bool returnAllStatements = (outputType == "statement");
			TNodeType typeToReturn;
			if (outputType == "assign") { typeToReturn = Assign; }
			else if (outputType == "while") { typeToReturn = While; }
			else if (outputType == "if") { typeToReturn = If; }
			else if (outputType == "call") { typeToReturn = Call; }
			else { // outputType is unexpected
				returnList.push_back("none");
				return returnList;
			}

			size = variableToBeChecked->getStatementUsesSize();
			for (int i = 0; i < size; i++) {
				StatementTableStatement* statementToBeChecked = statementTable->getStatementUsingStatementNumber(variableToBeChecked->getStatementUses(i));
				if ((returnAllStatements) || (statementToBeChecked->getType() == typeToReturn)) {
					returnList.push_back(std::to_string(statementToBeChecked->getIndex()));
				}
			}
		}
	}
			break;
	case 2: // Check what variables are used by the procedure or statement with the procedure name or statement number "input"
		if (outputType == "procedure") {
			ProcedureTableProcedure* procedure = procedureTable->getProcedure(input);
			size = procedure->getUsesSize();
			for (int i = 0; i < size; i++) {
				returnList.push_back(variableTable->getVariableUsingVariableIndexNumber(procedure->getUses(i))->getName());
			}
		}
		else { /* As this function only checks for procedures and statements (of which there are several types of statements
				  such as while, if, assign, etc., and since statements are specified by statement number as the "input"
				  argument passed anyway, we can assume that as long as the caller is not looking for a procedure, he or
				  she is looking for a statement, since there can only be one statement with a specific statement number. */
			int statementNumber = std::atoi(input.c_str());
			if (statementNumber != 0) { // If 0 is returned by std::atoi, the string inputted is not a number
				StatementTableStatement* statement = statementTable->getStatementUsingStatementNumber(statementNumber);
				for (int i = 0; i < statement->getUsesSize(); i++) {
					returnList.push_back(variableTable->getVariableUsingVariableIndexNumber(statement->getUses(i))->getName());
				}
			}
		}
		break;
	}

	if (returnList.empty()) {
		returnList.push_back("none");
	}
	return returnList;
}

std::vector<std::string> PKB::PQLModifies(std::string input, int argumentPosition, std::string outputType) {
	std::vector<std::string> returnList;
	int size;

	switch (argumentPosition) {
	case 1: { // Check which procedure, assignment, while or statement number modifies the variable "input"
		VariableTableVariable* variableToBeChecked = variableTable->getVariableUsingName(input);

		// Simply return procedures first
		if (outputType == "procedure") {
			size = variableToBeChecked->getProceduresModifiesSize();
			for (int i = 0; i < size; i++) {
				returnList.push_back(procedureTable->getProcedure(variableToBeChecked->getProceduresModifies(i))->getName());
			}
		}
		else {
			// Check which statements are to be returned
			bool returnAllStatements = (outputType == "statement");
			TNodeType typeToReturn;
			if (outputType == "assign") { typeToReturn = Assign; }
			else if (outputType == "while") { typeToReturn = While; }
			else if (outputType == "if") { typeToReturn = If; }
			else if (outputType == "call") { typeToReturn = Call; }
			else { // outputType is unexpected
				returnList.push_back("none");
				return returnList;
			}

			size = variableToBeChecked->getStatementModifiesSize();
			for (int i = 0; i < size; i++) {
				StatementTableStatement* statementToBeChecked = statementTable->getStatementUsingStatementNumber(variableToBeChecked->getStatementModifies(i));
				if ((returnAllStatements) || (statementToBeChecked->getType() == typeToReturn)) {
					returnList.push_back(std::to_string(statementToBeChecked->getIndex()));
				}
			}
		}
	}
			break;
	case 2: // Check what variables are used by the procedure or statement with the procedure name or statement number "input"
		if (outputType == "procedure") {
			ProcedureTableProcedure* procedure = procedureTable->getProcedure(input);
			size = procedure->getModifiesSize();
			for (int i = 0; i < size; i++) {
				returnList.push_back(variableTable->getVariableUsingVariableIndexNumber(procedure->getModifies(i))->getName());
			}
		}
		else { /* As this function only checks for procedures and statements (of which there are several types of statements
			   such as while, if, assign, etc., and since statements are specified by statement number as the "input"
			   argument passed anyway, we can assume that as long as the caller is not looking for a procedure, he or
			   she is looking for a statement, since there can only be one statement with a specific statement number. */
			int statementNumber = std::atoi(input.c_str());
			if (statementNumber != 0) { // If 0 is returned by std::atoi, the string inputted is not a number
				StatementTableStatement* statement = statementTable->getStatementUsingStatementNumber(statementNumber);
				for (int i = 0; i < statement->getModifiesSize(); i++) {
					returnList.push_back(variableTable->getVariableUsingVariableIndexNumber(statement->getModifies(i))->getName());
				}
			}
		}
		break;
	}

	if (returnList.empty()) {
		returnList.push_back("none");
	}
	return returnList;
}

std::vector<std::string> PKB::PQLFollows(int statementNumber, int argumentPosition) {
	std::vector<std::string> returnList;
	StatementTableStatement* statement = statementTable->getStatementUsingStatementNumber(statementNumber);
	if (statement != NULL) { // In case a non-existing statement number was given
		if (argumentPosition == 1) { //check followed by
			if (statement->hasFollowedBy()) {
				returnList.push_back(std::to_string(statement->getFollowedBy()));
			}
		}
		else if (argumentPosition == 2) { //check follows
			if (statement->hasFollows()) {
				returnList.push_back(std::to_string(statement->getFollows()));
			}
		}
	}

	if (returnList.empty()) {
		returnList.push_back("none");
	}
	return returnList;
}

std::vector<std::string> PKB::PQLFollowsStar(int statementNumber, int argumentPosition) {
	std::vector<std::string> returnList;
	StatementTableStatement* statement = statementTable->getStatementUsingStatementNumber(statementNumber);
	if (statement != NULL) {
		int size;
		if (argumentPosition == 1) { //check followedBy*
			size = statement->getFollowedByStarSize();
			for (int i = 0; i < size; i++) {
				returnList.push_back(std::to_string(statement->getFollowedByStar(i)));
			}
		}
		else if (argumentPosition == 2) { //check follows*
			size = statement->getFollowsStarSize();
			for (int i = 0; i < size; i++) {
				returnList.push_back(std::to_string(statement->getFollowsStar(i)));
			}
		}
	}

	if (returnList.empty()) {
		returnList.push_back("none");
	}
	return returnList;
}

std::vector<std::string> PKB::PQLParent(int statementNumber, int argumentPosition) {
	std::vector<std::string> returnList;
	StatementTableStatement* statement = statementTable->getStatementUsingStatementNumber(statementNumber);
	if (argumentPosition == 1) { //find parent
		if (statement->hasParent()) {
			returnList.push_back(std::to_string(statement->getParent()));
		}
	}
	else if (argumentPosition == 2) { //find children
		int size = statement->getChildrenSize();
		for (int i = 0; i < size; i++) {
			returnList.push_back(std::to_string(statement->getChildren(i)));
		}
	}

	if (returnList.empty()) {
		returnList.push_back("none");
	}
	return returnList;
}

std::vector<std::string> PKB::PQLParentStar(int statementNumber, int argumentPosition) {
	std::vector<std::string> returnList;
	StatementTableStatement* statement = statementTable->getStatementUsingStatementNumber(statementNumber);
	int size;
	if (argumentPosition == 1) { //find parent*
		size = statement->getParentStarSize();
		for (int i = 0; i < size; i++) {
			returnList.push_back(std::to_string(statement->getParentStar(i)));
		}
	}
	else if (argumentPosition == 2) { //find children*
		size = statement->getChildrenStarSize();
		for (int i = 0; i < size; i++) {
			returnList.push_back(std::to_string(statement->getChildrenStar(i)));
		}
	}

	if (returnList.empty()) {
		returnList.push_back("none");
	}
	return returnList;
}

std::vector<std::string> PKB::PQLPattern(TNodeType type, Ref left, Ref right) {
	std::vector<std::string> returnList;
	
	// @todo Supposedly to use AST to check this, but since AST is not up yet, let's go with the lazy method
	// @todo I'm really sorry for this. Will make sure this is no longer done like this by iteration 2./

	int size = statementTable->getNumberOfStatements();
	for (int i = 0; i < size; i++) {
		StatementTableStatement* statement = statementTable->getStatementUsingVectorIndexNumber(i);
		if ((statement->getType() == type) &&
				(((type == Assign) &&
				(((left.getType() == "placeholder") || (variableTable->getVariableUsingVariableIndexNumber(statement->getModifies(0))->getName() == left.getName())) &&
				((right.getType() == "placeholder") ||
				((right.getType() == "part_of_expr") && (right.getName() == statement->getRightHandSideExpression().substr(0, right.getName().length())) ||
				((right.getType() == "expr") && (right.getName() == statement->getRightHandSideExpression())))))) ||
				(((type == While) || (type == If)) &&
				((left.getType() == "placeholder") || (statement->getControlVariable() == left.getName()))))) {
			returnList.push_back(std::to_string(statement->getIndex()));
		}
	}

	if (returnList.empty()) {
		returnList.push_back("none");
	}
	return returnList;
}