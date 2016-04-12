#include "PKB.h"
#include "CFG.h"
#include <iostream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>	
#include <cstdlib>

PKB* PKB::instance = NULL;
CFG& GlobalCFG = CFG::getGlobalCFG();

// To enable the printing of debug lines
const bool IS_DEBUGGING = false;

PKB::PKB() {
	numberOfStatements = 0;
	procedureTable = new ProcedureTable();
	statementTable = new StatementTable();
	variableTable = new VariableTable();
	constantTable = new ConstantTable();
    GlobalCFG.setStmtTable(statementTable);
	currentProcedure = NULL;
	statementStackTrace = new std::stack<int>();
	statementStackTrace->push(0);
}

PKB::~PKB() {
	delete procedureTable;
	delete statementTable;
	delete variableTable;
	delete constantTable;
	delete statementStackTrace;
}

StatementTableStatement* PKB::newStatement() {
	// Increment number of statements, and use the current
	// number of statements as the new statement's index number
	numberOfStatements++;

	// Create a new statement in StatementTable with the index number
	StatementTableStatement* currentStatement = statementTable->addStatement(numberOfStatements);

	// Adds statement number into current procedure
	if (currentProcedure != NULL) {
		currentProcedure->addStatement(currentStatement);
		currentStatement->setProcedureIndexNumber(currentProcedure->getIndex());
	}

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
		if (IS_DEBUGGING) {
			cout << "DEBUG: Modifies(" << procedure->getName() << ", " << variable->getName() << ")" << endl;
		}
		break;
	case Uses:
		procedure->addUses(variableIndex);
		variable->addProcedureUses(procedureIndex);
		if (IS_DEBUGGING) {
			cout << "DEBUG: Uses(" << procedure->getName() << ", " << variable->getName() << ")" << endl;
		}
		break;
	default:
		return false;
	}

	return true;
}

bool PKB::addRelationship(VariableTableVariable* variable, StatementTableStatement* statement, RelationshipType relationship) {
	int variableIndex = variable->getIndex();
	int statementIndex = statement->getStatementNumber();

	switch (relationship) {
	case Modifies:
		statement->addModifies(variableIndex);
		variable->addStatementModifies(statementIndex);
		if (IS_DEBUGGING) {
			cout << "DEBUG: Modifies(" << statementIndex << ", " << variable->getName() << ")" << endl;
		}
		break;
	case Uses:
		statement->addUses(variableIndex);
		variable->addStatementUses(statementIndex);
		if (IS_DEBUGGING) {
			cout << "DEBUG: Uses(" << statementIndex << ", " << variable->getName() << ")" << endl;
		}
	default:
		return false;
	}

	return true;
}

TNodeType PKB::getType(std::string type) {
	if (type == "assign") {
		return Assign;
	}
	if (type == "while") {
		return While;
	}
	if (type == "if") {
		return If;
	}
	if (type == "call") {
		return Call;
	}
	return Undefined;
}

PKB* PKB::getInstance() {
	if (instance == NULL)
		instance = new PKB();
	return instance;
}

void PKB::setInstance(PKB* newPkb) {
	instance = newPkb;
}

ProcedureTable* PKB::getProcedureTable() {
    return procedureTable;
}

StatementTable* PKB::getStatementTable() {
    return statementTable;
}

VariableTable* PKB::getVariableTable() {
    return variableTable;
}

void PKB::ProcedureStart(std::string nameOfProcedure) {
	/* Create an entry for this new procedure in the procedure table, and keep a pointer to it in currentProcedure, 
	   so that statements being inputted can have their statement numbers added under it */
	currentProcedure = procedureTable->getProcedure(nameOfProcedure);
    currentProcedureAST = new AST(nameOfProcedure);
    procedureAST.push_back(currentProcedureAST);

    //indicate to CFG to begin a new procedure
    GlobalCFG.newProcedure();

	// Clear current stack trace
	if (statementStackTrace->top() != 0) {
		statementStackTrace->pop();
		statementStackTrace->push(0);
	}
}

void PKB::ProcedureEnd() {
	// Remove the pointer to currentProcedure
	currentProcedure = NULL;
    currentProcedureAST = NULL;
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
		else if (types[i] == Constant) {
			// Add constants into a constant table
			constantTable->addConstant(std::stoi(tokens[i]));
		}
	}
	int rightVariablesSize = rightVariables.size();

	// Create a new statement for this assign statement, adding the statement number into current procedure
	currentStatement = newStatement();
	// Set the type of the statement to be an assignment
	currentStatement->setType(Assign);
	// Set control variable of this statement
	currentStatement->setControlVariable(leftVariable->getName());
	// Set expression of this statement
	std::string rightHandSideExpression = "";
	for (unsigned int i = 0; i < size; i++) {
		rightHandSideExpression += tokens[i];
	}
	currentStatement->setRightHandSideExpression(rightHandSideExpression);
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
	std::set<int>* proceduresSet = currentProcedure->getIndirectProcedureCallBy();
	std::set<int>::iterator end = proceduresSet->end();
	ProcedureTableProcedure* procedureForAddingRelationship;
	for (std::set<int>::iterator i = proceduresSet->begin(); i != end; i++) {
		procedureForAddingRelationship = procedureTable->getProcedure(*i);
		addRelationship(leftVariable, procedureForAddingRelationship, Modifies);
		for (int x = 0; x < rightVariablesSize; x++) {
			addRelationship(rightVariables[x], procedureForAddingRelationship, Uses);
		}

		// Add the Modifies and Uses relationships into statements that call the above procedures
		int statementCallsSize = procedureForAddingRelationship->getStatementCallBySize();
		StatementTableStatement* statementForAddingRelationship;
		for (int x = 0; x < statementCallsSize; x++) {
			statementForAddingRelationship = statementTable->getStatementUsingStatementNumber(procedureForAddingRelationship->getStatementCallBy(x));
			addRelationship(leftVariable, statementForAddingRelationship, Modifies);
			for (int y = 0; y < rightVariablesSize; y++) {
				addRelationship(rightVariables[y], statementForAddingRelationship, Uses);
			}
		}
	}

	// Add the Modifies and Uses relationships into statements that call this current procedure, and their ancestors
	int tempStatementCallSize = currentProcedure->getStatementCallBySize();
	StatementTableStatement* tempStatement;
	StatementTableStatement* ancestorStatement;
	int ancestorsSize;
	for (int i = 0; i < tempStatementCallSize; i++) {
		tempStatement = statementTable->getStatementUsingStatementNumber(currentProcedure->getStatementCallBy(i));
		addRelationship(leftVariable, tempStatement, Modifies);
		for (int j = 0; j < rightVariablesSize; j++) {
			addRelationship(rightVariables[j], tempStatement, Uses);
		}

		ancestorsSize = tempStatement->getParentStarSize();
		for (int j = 0; j < ancestorsSize; j++) {
			ancestorStatement = statementTable->getStatementUsingStatementNumber(tempStatement->getParentStar(j));
			addRelationship(leftVariable, ancestorStatement, Modifies);
			for (int k = 0; k < rightVariablesSize; k++) {
				addRelationship(rightVariables[k], ancestorStatement, Uses);
			}
		}
	}
    
    //AST
	currentProcedureAST->addAssignTNode(variable, tokens, currentStatement->getStatementNumber());
    GlobalCFG.addStmt();

	return true;
}

void PKB::CallStatement(std::string procedure) {
	StatementTableStatement* currentStatement = newStatement();

	// Set the type of the statement to be a call
	currentStatement->setType(Call);

	// Add the procedure you're calling into the Calls relationship of the procedure that this statement belongs to
	ProcedureTableProcedure* procedureBeingCalled = procedureTable->getProcedure(procedure);
	procedureBeingCalled->addStatementsCallBy(currentStatement->getStatementNumber());
	procedureBeingCalled->addProcedureCallBy(currentProcedure);
	currentProcedure->addProcedureCalls(procedureBeingCalled);
	currentStatement->setFirstCalls(procedureBeingCalled->getFirstStatementPointer());
	currentStatement->setLastCalls(procedureBeingCalled->getLastStatementPointer());

	// Retrieve information about this statement's ancestor(s) in order to add relationships later
	int numberOfAncestors = currentStatement->getParentStarSize();
	vector<StatementTableStatement*> tempAncestors;
	for (int i = 0; i < numberOfAncestors; i++) {
		tempAncestors.push_back(statementTable->getStatementUsingStatementNumber(currentStatement->getParentStar(i)));
	}

	// Retrieve information about the procedures that call this current procedure
	std::set<int>* tempIndirectProcedureCallsSet = currentProcedure->getIndirectProcedureCallBy();
	vector<ProcedureTableProcedure*> tempIndirectProcedureCalls;
	std::set<int>::iterator end = tempIndirectProcedureCallsSet->end();
	for (std::set<int>::iterator iter = tempIndirectProcedureCallsSet->begin(); iter != end; iter++) {
		tempIndirectProcedureCalls.push_back(procedureTable->getProcedure(*iter));
	}
	int tempIndirectProcedureCallsSize = tempIndirectProcedureCalls.size();

	// Retrieve information about the statements that call this current procedure
	int tempStatementCallsSize = currentProcedure->getStatementCallBySize();
	vector<StatementTableStatement*> tempStatementCalls;
	for (int i = 0; i < tempStatementCallsSize; i++) {
		tempStatementCalls.push_back(statementTable->getStatementUsingStatementNumber(currentProcedure->getStatementCallBy(i)));
	}

	// Add the relationships for the variables that are modified and used in the procedure being called into this
	// statement, its ancestor(s), its procedure and whatever calls its procedures
	VariableTableVariable* tempVariable;
	int tempSize = procedureBeingCalled->getModifiesSize();
	for (int i = 0; i < tempSize; i++) { // Add Modifies relationships
		tempVariable = variableTable->getVariableUsingVariableIndexNumber(procedureBeingCalled->getModifies(i));

		addRelationship(tempVariable, currentStatement, Modifies);
		for (int j = 0; j < numberOfAncestors; j++) {
			addRelationship(tempVariable, tempAncestors[j], Modifies);
		}
		addRelationship(tempVariable, currentProcedure, Modifies);
		for (int j = 0; j < tempIndirectProcedureCallsSize; j++) {
			addRelationship(tempVariable, tempIndirectProcedureCalls[j], Modifies);
		}
		for (int j = 0; j < tempStatementCallsSize; j++) {
			addRelationship(tempVariable, tempStatementCalls[j], Modifies);
		}
	}
	tempSize = procedureBeingCalled->getUsesSize();
	for (int i = 0; i < tempSize; i++) { // Add Uses relationships
		tempVariable = variableTable->getVariableUsingVariableIndexNumber(procedureBeingCalled->getUses(i));

		addRelationship(tempVariable, currentStatement, Uses);
		for (int j = 0; j < numberOfAncestors; j++) {
			addRelationship(tempVariable, tempAncestors[j], Uses);
		}
		addRelationship(tempVariable, currentProcedure, Uses);
		for (int j = 0; j < tempIndirectProcedureCallsSize; j++) {
			addRelationship(tempVariable, tempIndirectProcedureCalls[j], Uses);
		}
		for (int j = 0; j < tempStatementCallsSize; j++) {
			addRelationship(tempVariable, tempStatementCalls[j], Uses);
		}
	}

    //AST
	currentProcedureAST->addCallTNode(procedure, currentStatement->getStatementNumber());
    GlobalCFG.addStmt();
}

void PKB::WhileStart(NAME variable) {
	StatementTableStatement* currentStatement = newStatement();
	VariableTableVariable* currentVariable = variableTable->getVariableUsingName(variable);
	currentStatement->setControlVariable(variable);

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

	// Add the Uses relationship into the current procedure
	addRelationship(currentVariable, currentProcedure, Uses);

	// Add the Uses relationship into procedures that call the procedure, and all other procedures that call those procedures, etc.
	std::set<int>* proceduresSet = currentProcedure->getIndirectProcedureCallBy();
	std::set<int>::iterator end = proceduresSet->end();
	ProcedureTableProcedure* procedureForAddingRelationship;
	for (std::set<int>::iterator i = proceduresSet->begin(); i != end; i++) {
		procedureForAddingRelationship = procedureTable->getProcedure(*i);
		addRelationship(currentVariable, procedureForAddingRelationship, Uses);

		// Add the Uses relationship into statements that call the above procedures
		int statementCallsSize = procedureForAddingRelationship->getStatementCallBySize();
		StatementTableStatement* statementForAddingRelationship;
		for (int x = 0; x < statementCallsSize; x++) {
			statementForAddingRelationship = statementTable->getStatementUsingStatementNumber(procedureForAddingRelationship->getStatementCallBy(x));
			addRelationship(currentVariable, statementForAddingRelationship, Uses);
		}
	}

	// Add the Uses relationships into statements that call this current procedure, and their ancestors
	int tempStatementCallSize = currentProcedure->getStatementCallBySize();
	StatementTableStatement* tempStatement;
	int ancestorsSize;
	for (int i = 0; i < tempStatementCallSize; i++) {
		tempStatement = statementTable->getStatementUsingStatementNumber(currentProcedure->getStatementCallBy(i));
		addRelationship(currentVariable, tempStatement, Uses);
		ancestorsSize = tempStatement->getParentStarSize();
		for (int j = 0; j < ancestorsSize; j++) {
			addRelationship(currentVariable, statementTable->getStatementUsingStatementNumber(tempStatement->getParentStar(j)), Uses);
		}
	}

    //AST
	currentProcedureAST->addWhileTNode(variable, currentStatement->getStatementNumber());
    GlobalCFG.addWhileStmt();
}

bool PKB::WhileEnd() {
	// Pop from statement stack trace to indicate the end of current level of nesting
	if (statementStackTrace->size() < 2) {
		return false;
	}

	currentProcedureAST->addEndOfContainerRelation();
    GlobalCFG.endWhileStmt();
	statementStackTrace->pop();
		
	return true;
}

void PKB::IfStart(NAME variable) {
	StatementTableStatement* currentStatement = newStatement();
	VariableTableVariable* currentVariable = variableTable->getVariableUsingName(variable);
	currentStatement->setControlVariable(variable);

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
	std::set<int>* proceduresSet = currentProcedure->getIndirectProcedureCallBy();
	std::set<int>::iterator end = proceduresSet->end();
	ProcedureTableProcedure* procedureForAddingRelationship;
	for (std::set<int>::iterator i = proceduresSet->begin(); i != end; i++) {
		procedureForAddingRelationship = procedureTable->getProcedure(*i);
		addRelationship(currentVariable, procedureForAddingRelationship, Uses);

		// Add the Uses relationship into statements that call the above procedures
		int statementCallsSize = procedureForAddingRelationship->getStatementCallBySize();
		StatementTableStatement* statementForAddingRelationship;
		for (int x = 0; x < statementCallsSize; x++) {
			statementForAddingRelationship = statementTable->getStatementUsingStatementNumber(procedureForAddingRelationship->getStatementCallBy(x));
			addRelationship(currentVariable, statementForAddingRelationship, Uses);
		}
	}

	// Add the Uses relationships into statements that call this current procedure, and their ancestors
	int tempStatementCallSize = currentProcedure->getStatementCallBySize();
	StatementTableStatement* tempStatement;
	int ancestorsSize;
	for (int i = 0; i < tempStatementCallSize; i++) {
		tempStatement = statementTable->getStatementUsingStatementNumber(currentProcedure->getStatementCallBy(i));
		addRelationship(currentVariable, tempStatement, Uses);
		ancestorsSize = tempStatement->getParentStarSize();
		for (int j = 0; j < ancestorsSize; j++) {
			addRelationship(currentVariable, statementTable->getStatementUsingStatementNumber(tempStatement->getParentStar(j)), Uses);
		}
	}

    //AST
	currentProcedureAST->addIfTNode(variable, currentStatement->getStatementNumber());
    GlobalCFG.addIfStmt();
}

bool PKB::ElseStart() {
	if (statementStackTrace->size() < 2) {
		return false;
	}

	statementStackTrace->pop();
	statementStackTrace->push(0);

    //AST
	currentProcedureAST->addElseRelation();
    GlobalCFG.elseStmt();

	return true;
}

bool PKB::IfElseEnd() {
	if (statementStackTrace->size() < 2) {
		return false;
	}

	currentProcedureAST->addEndOfContainerRelation();
    GlobalCFG.endIfStmt();
	statementStackTrace->pop();

	return true;
}

std::vector<std::string> PKB::PQLSelect(TNodeType outputType) {
	std::vector<std::string> returnList;
	StatementTableStatement* statement;

	// return all procedures
	if (outputType == ProcedureName) {
		int procedureTableSize = procedureTable->getNumberOfProcedures();
		for (int i = 0; i < procedureTableSize; i++) {
			returnList.push_back(procedureTable->getProcedure(i)->getName());
		}
		return returnList;
	}

	// return all variables
	if (outputType == VariableName) {
		int variableTableSize = variableTable->getNumberOfVariables();
		for (int i = 0; i < variableTableSize; i++) {
			returnList.push_back(variableTable->getVariableUsingVectorIndexNumber(i)->getName());
		}
		return returnList;
	}

	// return all constants
	if (outputType == Const) {
		unsigned int constantTableSize = constantTable->getNumberOfConstants();
		for (unsigned int i = 0; i < constantTableSize; i++) {
			returnList.push_back(std::to_string(constantTable->getConstant(i)));
		}
		return returnList;
		
	}

	// If outputType is not Assign, If, While, or Call, return all statements
	bool returnAllStatements = true;
	if ((outputType == Assign) || (outputType == If) || (outputType == While) || (outputType == Call) || (outputType == StmtLst)) {
		returnAllStatements = false;
	}

	// Iterate through statements, check if they should be returned
	int statementTableSize = statementTable->getNumberOfStatements();
	for (int i = 0; i < statementTableSize; i++) {
		statement = statementTable->getStatementUsingVectorIndexNumber(i);
		if ((returnAllStatements) || (statement->getType() == outputType) || ((outputType == StmtLst) && (!statement->hasFollows()))) {
			returnList.push_back(std::to_string(statement->getStatementNumber()));
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
		else { // Check which statements are to be returned
			bool returnAllStatements = (outputType == "stmt");
			TNodeType typeToReturn = Undefined;
			if (!returnAllStatements) {
				typeToReturn = getType(outputType);
			}

			if ((returnAllStatements) || (typeToReturn != Undefined)) {
				size = variableToBeChecked->getStatementUsesSize();
				for (int i = 0; i < size; i++) {
					StatementTableStatement* statementToBeChecked = statementTable->getStatementUsingStatementNumber(variableToBeChecked->getStatementUses(i));
					if ((returnAllStatements) || (statementToBeChecked->getType() == typeToReturn)) {
						returnList.push_back(std::to_string(statementToBeChecked->getStatementNumber()));
					}
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
			bool returnAllStatements = (outputType == "stmt");
			TNodeType typeToReturn = Undefined;
			if (!returnAllStatements) {
				typeToReturn = getType(outputType);
			}

			if ((returnAllStatements) || (typeToReturn != Undefined)) {
				size = variableToBeChecked->getStatementModifiesSize();
				for (int i = 0; i < size; i++) {
					StatementTableStatement* statementToBeChecked = statementTable->getStatementUsingStatementNumber(variableToBeChecked->getStatementModifies(i));
					if ((returnAllStatements) || (statementToBeChecked->getType() == typeToReturn)) {
						returnList.push_back(std::to_string(statementToBeChecked->getStatementNumber()));
					}
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
	return returnList;
}

std::vector<std::string> PKB::PQLFollows(int statementNumber, int argumentPosition, std::string outputType) {
	std::vector<std::string> returnList;
	StatementTableStatement* statement = statementTable->getStatementUsingStatementNumber(statementNumber);
	int statementReturnIndex;

	bool returnAllStatements = (outputType == "stmt");
	TNodeType typeToReturn = Undefined;
	if (!returnAllStatements) {
		typeToReturn = getType(outputType);
	}

	// In case a non-existing statement number was given
	if ((statement != NULL) && ((returnAllStatements) || (typeToReturn != Undefined))) {
		if (argumentPosition == 1) { //check follows
			if (statement->hasFollows()) {
				statementReturnIndex = statement->getFollows();
				if ((returnAllStatements) || (statementTable->getStatementUsingStatementNumber(statementReturnIndex)->getType() == typeToReturn)) {
					returnList.push_back(std::to_string(statementReturnIndex));
				}
			}
		}
		else if (argumentPosition == 2) { //check followed by
			if (statement->hasFollowedBy()) {
				statementReturnIndex = statement->getFollowedBy();
				if ((returnAllStatements) || (statementTable->getStatementUsingStatementNumber(statementReturnIndex)->getType() == typeToReturn)) {
					returnList.push_back(std::to_string(statementReturnIndex));
				}
			}
		}
	}
	return returnList;
}

std::vector<std::string> PKB::PQLFollowsStar(int statementNumber, int argumentPosition, std::string outputType) {
	std::vector<std::string> returnList;
	StatementTableStatement* statement = statementTable->getStatementUsingStatementNumber(statementNumber);
	int statementReturnIndex;

	bool returnAllStatements = (outputType == "stmt");
	TNodeType typeToReturn = Undefined;
	if (!returnAllStatements) {
		typeToReturn = getType(outputType);
	}

	if ((statement != NULL) && ((returnAllStatements) || (typeToReturn != Undefined))) {
		int size;
		if (argumentPosition == 1) { //check follows*
			size = statement->getFollowsStarSize();
			for (int i = 0; i < size; i++) {
				statementReturnIndex = statement->getFollowsStar(i);
				if ((returnAllStatements) || (statementTable->getStatementUsingStatementNumber(statementReturnIndex)->getType() == typeToReturn)) {
					returnList.push_back(std::to_string(statementReturnIndex));
				}
			}
		}
		else if (argumentPosition == 2) { //check followedBy*
			size = statement->getFollowedByStarSize();
			for (int i = 0; i < size; i++) {
				statementReturnIndex = statement->getFollowedByStar(i);
				if ((returnAllStatements) || (statementTable->getStatementUsingStatementNumber(statementReturnIndex)->getType() == typeToReturn)) {
					returnList.push_back(std::to_string(statementReturnIndex));
				}
			}
		}
	}
	return returnList;
}

std::vector<std::string> PKB::PQLParent(int statementNumber, int argumentPosition, std::string outputType) {
	std::vector<std::string> returnList;
	StatementTableStatement* statement = statementTable->getStatementUsingStatementNumber(statementNumber);
	int statementReturnIndex;

	bool returnAllStatements = (outputType == "stmt");
	TNodeType typeToReturn = Undefined;
	if (!returnAllStatements) {
		typeToReturn = getType(outputType);
	}

	if ((returnAllStatements) || (typeToReturn != Undefined)) {
		if (argumentPosition == 1) { //find parent
			if (statement->hasParent()) {
				statementReturnIndex = statement->getParent();
				if ((returnAllStatements) || (statementTable->getStatementUsingStatementNumber(statementReturnIndex)->getType() == typeToReturn)) {
					returnList.push_back(std::to_string(statementReturnIndex));
				}
			}
		}
		else if (argumentPosition == 2) { //find children
			int size = statement->getChildrenSize();
			for (int i = 0; i < size; i++) {
				statementReturnIndex = statement->getChildren(i);
				if ((returnAllStatements) || (statementTable->getStatementUsingStatementNumber(statementReturnIndex)->getType() == typeToReturn)) {
					returnList.push_back(std::to_string(statementReturnIndex));
				}
			}
		}
	}
	return returnList;
}

std::vector<std::string> PKB::PQLParentStar(int statementNumber, int argumentPosition, std::string outputType) {
	std::vector<std::string> returnList;
	StatementTableStatement* statement = statementTable->getStatementUsingStatementNumber(statementNumber);
	int size;
	int statementReturnIndex;

	bool returnAllStatements = (outputType == "stmt");
	TNodeType typeToReturn = Undefined;
	if (!returnAllStatements) {
		typeToReturn = getType(outputType);
	}

	if ((returnAllStatements) || (typeToReturn != Undefined)) {
		if (argumentPosition == 1) { //find parent*
			size = statement->getParentStarSize();
			for (int i = 0; i < size; i++) {
				statementReturnIndex = statement->getParentStar(i);
				if ((returnAllStatements) || (statementTable->getStatementUsingStatementNumber(statementReturnIndex)->getType() == typeToReturn)) {
					returnList.push_back(std::to_string(statementReturnIndex));
				}
			}
		}
		else if (argumentPosition == 2) { //find children*
			size = statement->getChildrenStarSize();
			for (int i = 0; i < size; i++) {
				statementReturnIndex = statement->getChildrenStar(i);
				if ((returnAllStatements) || (statementTable->getStatementUsingStatementNumber(statementReturnIndex)->getType() == typeToReturn)) {
					returnList.push_back(std::to_string(statementReturnIndex));
				}
			}
		}
	}
	return returnList;
}


// trim from start
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}


std::vector<std::string> PKB::PQLPattern(TNodeType type, Ref left, Ref right) {
    std::vector<std::string> returnList;

    switch (type) {
    case Assign:
        for (size_t proc = 0; proc < procedureAST.size(); proc++) {
            for (size_t stmt = 0; stmt < procedureAST[proc]->getTree().size(); stmt++) {
                if (procedureAST[proc]->getTree()[stmt]->getNodeType() == type) {
                    if (left.getType() == "placeholder" || 
                        left.getName() == procedureAST[proc]->getTree()[stmt]->getChildNodes()[0]->getChildNodes()[0]->getValue()) {
                        if (right.getType() == "placeholder") {
                            returnList.push_back(std::to_string(procedureAST[proc]->getTree()[stmt]->getLineNumber()));
                        } else {
                            TNode* rightTree = AST::constructExpressionTree(right.getName());
                            if (right.getType() == "expr") {
                                if (AST::compareTrees(rightTree,
                                    procedureAST[proc]->getTree()[stmt]->getChildNodes()[0]->getChildNodes()[1])) {
                                    returnList.push_back(std::to_string(procedureAST[proc]->getTree()[stmt]->getLineNumber()));
                                }
                            } else if (right.getType() == "part_of_expr") {
                                if (AST::findSubtreeInTree(rightTree, procedureAST[proc]->getTree()[stmt]->getChildNodes()[0]->getChildNodes()[1])) {
                                    returnList.push_back(std::to_string(procedureAST[proc]->getTree()[stmt]->getLineNumber()));
                                }
                            }
                        }
                    }
                }
            }
        }
        break;
    case While:
    case If:
        for (size_t proc = 0; proc < procedureAST.size(); proc++) {
            for (size_t stmt = 0; stmt < procedureAST[proc]->getTree().size(); stmt++) {
                if (procedureAST[proc]->getTree()[stmt]->getNodeType() == type) {
                    if (left.getType() == "placeholder" ||
                        left.getName() == procedureAST[proc]->getTree()[stmt]->getChildNodes()[0]->getValue()) {
                        returnList.push_back(std::to_string(procedureAST[proc]->getTree()[stmt]->getLineNumber()));
                    }
                }
            }
        }
        break;
    default:
        break;
    }
	
	return returnList;
}

std::vector<std::string> PKB::PQLCalls(std::string procedureName, bool isDirectCalls) {
	std::vector<std::string> returnList;
	ProcedureTableProcedure* currentProcedure = procedureTable->getProcedure(procedureName);

	if (isDirectCalls) {
		int size = currentProcedure->getProcedureCallsSize();
		for (int i = 0; i < size; i++) {
			returnList.push_back(currentProcedure->getProcedureCalls(i)->getName());
		}
	}
	else {
		std::set<int>* proceduresSet = currentProcedure->getIndirectProcedureCalls();
		std::set<int>::iterator end = proceduresSet->end();
		for (std::set<int>::iterator iter = proceduresSet->begin(); iter != end; iter++) {
			returnList.push_back(procedureTable->getProcedure(*iter)->getName());
		}
	}
	return returnList;
}

std::vector<std::string> PKB::PQLCalledBy(std::string procedureName, bool isDirectCalls) {
	std::vector<std::string> returnList;
	ProcedureTableProcedure* currentProcedure = procedureTable->getProcedure(procedureName);

	if (isDirectCalls) {
		int size = currentProcedure->getProcedureCallBySize();
		for (int i = 0; i < size; i++) {
			returnList.push_back(currentProcedure->getProcedureCallBy(i)->getName());
		}
	}
	else {
		std::set<int>* proceduresSet = currentProcedure->getIndirectProcedureCallBy();
		std::set<int>::iterator end = proceduresSet->end();
		for (std::set<int>::iterator iter = proceduresSet->begin(); iter != end; iter++) {
			returnList.push_back(procedureTable->getProcedure(*iter)->getName());
		}
	}
	return returnList;
}

std::vector<std::string> PKB::PQLPrevious(int statementNumber, bool isDirect, bool isBip) {
	std::vector<std::string> returnList;
	StatementTableStatement* currentStatement = statementTable->getStatementUsingStatementNumber(statementNumber);
	int size;

    if (isDirect) {
        std::vector<StatementTableStatement*>* previousList = currentStatement->getPrevious(isBip);
        size = previousList->size();
        for (int i = 0; i < size; i++) {
            returnList.push_back(std::to_string(previousList->at(i)->getStatementNumber()));
        }
    } else {
        std::vector<StatementTableStatement*>* previousStarList = currentStatement->getPreviousStar(isBip);
        size = previousStarList->size();
        for (int i = 0; i < size; i++) {
            returnList.push_back(std::to_string(previousStarList->at(i)->getStatementNumber()));
        }
    }
	return returnList;
}

std::vector<std::string> PKB::PQLNext(int statementNumber, bool isDirect, bool isBip) {
	std::vector<std::string> returnList;
	StatementTableStatement* currentStatement = statementTable->getStatementUsingStatementNumber(statementNumber);
	int size;

    if (isDirect) {
        std::vector<StatementTableStatement*>* nextList = currentStatement->getNext(isBip);
        size = nextList->size();
        for (int i = 0; i < size; i++) {
            returnList.push_back(std::to_string(nextList->at(i)->getStatementNumber()));
        }
    } else {
        std::vector<StatementTableStatement*>* nextStarList = currentStatement->getNextStar(isBip);
        size = nextStarList->size();
        for (int i = 0; i < size; i++) {
            returnList.push_back(std::to_string(nextStarList->at(i)->getStatementNumber()));
        }
    }
	return returnList;
}

std::vector<std::string> PKB::PQLAffectsThis(int statementNumber, bool isDirect, bool isBip) {
	std::vector<std::string> returnList;
	StatementTableStatement* currentStatement = statementTable->getStatementUsingStatementNumber(statementNumber);
	int size;

	StatementTableStatement::bonusImplementation = isBip;

	if (isDirect) {
		std::vector<StatementTableStatement*>* affectsThisList = currentStatement->getAffectsThis();
		size = affectsThisList->size();
		for (int i = 0; i < size; i++) {
			returnList.push_back(std::to_string(affectsThisList->at(i)->getStatementNumber()));
		}
	}
	else {
		std::vector<StatementTableStatement*> affectsThisStarList = currentStatement->getAffectsThisStar();
		size = affectsThisStarList.size();
		for (int i = 0; i < size; i++) {
			returnList.push_back(std::to_string(affectsThisStarList[i]->getStatementNumber()));
		}
	}

	return returnList;
}

std::vector<std::string> PKB::PQLAffectedByThis(int statementNumber, bool isDirect, bool isBip) {
	std::vector<std::string> returnList;
	StatementTableStatement* currentStatement = statementTable->getStatementUsingStatementNumber(statementNumber);
	int size;

	StatementTableStatement::bonusImplementation = isBip;

	if (isDirect) {
		std::vector<StatementTableStatement*>* affectedByThisList = currentStatement->getAffectedByThis();
		size = affectedByThisList->size();
		for (int i = 0; i < size; i++) {
			returnList.push_back(std::to_string(affectedByThisList->at(i)->getStatementNumber()));
		}
	}
	else {
		std::vector<StatementTableStatement*> affectedByThisStarList = currentStatement->getAffectedByThisStar();
		size = affectedByThisStarList.size();
		for (int i = 0; i < size; i++) {
			returnList.push_back(std::to_string(affectedByThisStarList[i]->getStatementNumber()));
		}
	}
	return returnList;
}

std::vector<std::string> PKB::PQLSibling(int statementNumber) {
	std::vector<std::string> returnList;

	std::vector<int> statementsInInteger = statementTable->getStatementUsingStatementNumber(statementNumber)->getSiblings();
	int statementsInIntegerSize = statementsInInteger.size();

	for (int i = 0; i < statementsInIntegerSize; i++) {
		returnList.push_back(std::to_string(statementsInInteger[i]));
	}

	return returnList;
}