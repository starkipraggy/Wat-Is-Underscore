#include <iostream>
#include <stack>
#include <queue>
#include <unordered_map>
#include "StatementTableStatement.h"
#include "CFG.h"

void StatementTableStatement::childrenStarHasBeingModified() {
	hasItsChildrenStarChanged = true;
}

void StatementTableStatement::followedByStarHasBeingModified() {
	hasItsFollowedByStarChanged = true;
}

StatementTableStatement::StatementTableStatement(int statementNumber) {
	this->statementNumber = statementNumber;
	type = Undefined;
	rightHandSideExpression = "";
	controlVariable = "";
	procedureIndexNumber = 0;

	firstCalls = NULL;
	lastCalls = NULL;
	follows = NULL;
	followedBy = NULL;
	parent = NULL;
	children = new std::vector<StatementTableStatement*>();
	modifies = new std::vector<int>();
	uses = new std::vector<int>();

	parentStar = new std::vector<int>();
	followsStar = new std::vector<int>();

	childrenStar = new std::set<int>();
	hasItsChildrenStarChanged = false;

	followedByStar = new std::vector<int>();
	hasItsFollowedByStarChanged = false;

	previous = NULL;
	previousStar = NULL;
	next = NULL;
	nextStar = NULL;
	affectsThis = NULL;
	affectedByThis = NULL;
}

StatementTableStatement::~StatementTableStatement() {
	delete children;
	delete modifies;
	delete uses;

	delete parentStar;
	delete followsStar;
	delete childrenStar;
	delete followedByStar;

	if (previous != NULL) { delete previous; }
	if (previousStar != NULL) { delete previousStar; }
	if (next != NULL) { delete next; }
	if (nextStar != NULL) { delete nextStar; }
	if (affectsThis != NULL) { delete affectsThis; }
	if (affectedByThis != NULL) { delete affectedByThis; }
}

int StatementTableStatement::getStatementNumber() {
	return statementNumber;
}

std::string StatementTableStatement::getRightHandSideExpression() {
	return rightHandSideExpression;
}

NAME StatementTableStatement::getControlVariable() {
	return controlVariable;
}

int StatementTableStatement::getProcedureIndexNumber() {
	return procedureIndexNumber;
}

bool StatementTableStatement::hasParent() {
	return (parent != NULL);
}

int StatementTableStatement::getParent() {
	return parent->getStatementNumber();
}

StatementTableStatement** StatementTableStatement::getFirstCalls() {
	return firstCalls;
}

StatementTableStatement** StatementTableStatement::getLastCalls() {
	return lastCalls;
}

bool StatementTableStatement::hasFollows() {
	return (follows != NULL);
}

bool StatementTableStatement::hasFollowedBy() {
	return (followedBy != NULL);
}

void StatementTableStatement::setType(TNodeType type) {
	this->type = type;
}

void StatementTableStatement::setRightHandSideExpression(std::string rightHandSideExpression) {
	this->rightHandSideExpression = rightHandSideExpression;
}

void StatementTableStatement::setControlVariable(NAME controlVariable) {
	this->controlVariable = controlVariable;
}

void StatementTableStatement::setProcedureIndexNumber(int procedureIndexNumber) {
	this->procedureIndexNumber = procedureIndexNumber;
}

void StatementTableStatement::setFirstCalls(StatementTableStatement** firstCalls) {
	this->firstCalls = firstCalls;
}

void StatementTableStatement::setLastCalls(StatementTableStatement** lastCalls) {
	this->lastCalls = lastCalls;
}

void StatementTableStatement::setFollows(StatementTableStatement* follows) {
	this->follows = follows;

	followsStar->clear();
	StatementTableStatement* stmt = this;
	while (stmt->hasFollows()) {
		stmt = stmt->follows;
		followsStar->push_back(stmt->getStatementNumber());
		stmt->followedByStarHasBeingModified();
	}
}

void StatementTableStatement::setFollowedBy(StatementTableStatement* followedBy) {
	this->followedBy = followedBy;
}

void StatementTableStatement::setParent(StatementTableStatement* parent) {
	this->parent = parent;

	parentStar->clear();
	StatementTableStatement* stmt = this;
	while (stmt->hasParent()) {
		stmt = stmt->parent;
		parentStar->push_back(stmt->getStatementNumber());
		stmt->childrenStarHasBeingModified();
	}
}

void StatementTableStatement::addChild(StatementTableStatement* child) {
	children->push_back(child);
}

bool StatementTableStatement::addModifies(int variableIndexNumber) {
	return DataStructureObject::addIntoVector(variableIndexNumber, modifies);
}

bool StatementTableStatement::addUses(int variableIndexNumber) {
	return DataStructureObject::addIntoVector(variableIndexNumber, uses);
}

int StatementTableStatement::getFollows() {
	return follows->getStatementNumber();
}

int StatementTableStatement::getFollowedBy() {
	return followedBy->getStatementNumber();
}

TNodeType StatementTableStatement::getType() {
	return type;
}

int StatementTableStatement::getUses(int index) {
	return uses->at(index);
}

int StatementTableStatement::getUsesSize() {
	return uses->size();
}

int StatementTableStatement::getModifies(int index) {
	return modifies->at(index);
}

int StatementTableStatement::getModifiesSize() {
	return modifies->size();
}

int StatementTableStatement::getFollowsStar(int index) {
	return followsStar->at(index);
}

int StatementTableStatement::getFollowsStarSize() {
	return followsStar->size();
}

int StatementTableStatement::getFollowedByStar(int index) {
	if (hasItsFollowedByStarChanged) {
		fetchNewCopyOfFollowedByStar(); // Fetch new copy

		hasItsFollowedByStarChanged = false;
	}

	return (followedByStar->at(index));
}

int StatementTableStatement::getFollowedByStarSize() {
	if (hasItsFollowedByStarChanged) {
		fetchNewCopyOfFollowedByStar(); // Fetch new copy

		hasItsFollowedByStarChanged = false;
	}

	return followedByStar->size();
}

void StatementTableStatement::fetchNewCopyOfFollowedByStar() {
	followedByStar->clear();

	StatementTableStatement* statementFollowing = this;
	while (statementFollowing->hasFollowedBy()) {
		statementFollowing = statementFollowing->followedBy;
		followedByStar->push_back(statementFollowing->getStatementNumber());
	}
}

int StatementTableStatement::getChildren(int index) {
	return children->at(index)->getStatementNumber();
}

int StatementTableStatement::getChildrenSize() {
	return children->size();
}

int StatementTableStatement::getParentStar(int index) {
	return parentStar->at(index);
}

int StatementTableStatement::getParentStarSize() {
	return parentStar->size();
}

int StatementTableStatement::getChildrenStar(int index) {
	if (hasItsChildrenStarChanged) {
		fetchNewCopyOfChildrenStar(); // Fetch new copy

		hasItsChildrenStarChanged = false;
	}

	std::set<int>::iterator it = childrenStar->begin();
	std::advance(it, index);
	return *it;
}

int StatementTableStatement::getChildrenStarSize() {
	if (hasItsChildrenStarChanged) {
		fetchNewCopyOfChildrenStar(); // Fetch new copy

		hasItsChildrenStarChanged = false;
	}

	return childrenStar->size();
}

void StatementTableStatement::fetchNewCopyOfChildrenStar() {
	childrenStar->clear();
	std::stack<StatementTableStatement*> stackForProcessingChildren;

	int childrenSize = getChildrenSize();
	for (int i = 0; i < childrenSize; i++) {
		stackForProcessingChildren.push(children->at(i));
	}

	StatementTableStatement* child;
	while (!stackForProcessingChildren.empty()) {
		child = stackForProcessingChildren.top();
		stackForProcessingChildren.pop();

		if (childrenStar->count(child->getStatementNumber()) == 0) { // Not already in the list, need to add its children into stack for processing
			childrenStar->insert(child->getStatementNumber());

			childrenSize = child->getChildrenSize();
			for (int x = 0; x < childrenSize; x++) {
				stackForProcessingChildren.push(child->children->at(x));
			}
		}
	}
}

std::vector<StatementTableStatement*>* StatementTableStatement::getPrevious() {
    if (previous != NULL) {
        return previous;
    }

    CFG GlobalCFG = CFG::getGlobalCFG();
    std::vector<int> intermediate = GlobalCFG.prevStmt(statementNumber);
    std::vector<StatementTableStatement*>* result = new std::vector<StatementTableStatement*>();
    GlobalCFG.convertIntToStatement(intermediate, *result);
    previous = result;
    return result;
}

std::vector<StatementTableStatement*>* StatementTableStatement::getNext() {
    if (next != NULL) {
        return next;
    }

    CFG GlobalCFG = CFG::getGlobalCFG();
    std::vector<int> intermediate = GlobalCFG.nextStmt(statementNumber);
    std::vector<StatementTableStatement*>* result = new std::vector<StatementTableStatement*>();
    GlobalCFG.convertIntToStatement(intermediate, *result);
    next = result;
    return result;
}

std::vector<StatementTableStatement*>* StatementTableStatement::getPreviousStar() {
    if (previousStar != NULL) {
        return previousStar;
    }

    CFG GlobalCFG = CFG::getGlobalCFG();
    std::vector<int> intermediate = GlobalCFG.prevStmtStar(statementNumber);
    std::vector<StatementTableStatement*>* result = new std::vector<StatementTableStatement*>();
    GlobalCFG.convertIntToStatement(intermediate, *result);
    previousStar = result;
    return result;
}

std::vector<StatementTableStatement*>* StatementTableStatement::getNextStar() {
    if (nextStar != NULL) {
        return nextStar;
    }

    CFG GlobalCFG = CFG::getGlobalCFG();
    std::vector<int> intermediate = GlobalCFG.nextStmtStar(statementNumber);
    std::vector<StatementTableStatement*>* result = new std::vector<StatementTableStatement*>();
    GlobalCFG.convertIntToStatement(intermediate, *result);
    nextStar = result;
    return result;
}

// Set to true if statements in procedures being called should be traced for Affects
// Set to false if going to simply regard the variables modified by the procedure
bool bonusImplementation = false;

std::vector<StatementTableStatement*>* StatementTableStatement::getAffectsThis() {
	if (affectsThis == NULL) {
		affectsThis = new std::vector<StatementTableStatement*>();
		if (getType() == Assign) {
			// Used by the algorithm to keep track of its progress
			std::set<int> statementNumbersOfStatementsAlreadyChecked;
			std::stack<StatementTableStatement*> statementsToCheck;
			// This stack is only used by the bonus implementation
			std::stack<StatementTableStatement*> statementsFromPreviousProcedures;
			std::unordered_map<StatementTableStatement*, std::vector<int>> statementsAndUseVariablesToCheck;

			// Use to keep track of the current statement being processed by the algorithm
			StatementTableStatement* currentStatementToCheck;
			int currentStatementNumber;

			// Use to obtain the previous node(s) of the current statement
			// being processed to be added into the statementsToCheck stack
			std::vector<StatementTableStatement*>* currentStatementPrevious;
			int currentStatementPreviousSize;
			StatementTableStatement* currentStatementPreviousSpecific;

			// Keep a vector of the indexes of the variables that this statement uses
			std::vector<int> thisStatementUseVariableIndexes;
			int thisStatementUseVariableIndexesSize = getUsesSize();
			for (int i = 0; i < thisStatementUseVariableIndexesSize; i++) {
				thisStatementUseVariableIndexes.push_back(getUses(i));
			}

			// Create a second vector for the previous node(s) to inherit
			std::vector<int> previousStatementUseVariableIndexes;

			// Obtain this statement's previous node(s) and push into statementsToCheck stack
			std::vector<StatementTableStatement*>* thisStatementPrevious = getPrevious();
			int thisStatementPreviousSize = thisStatementPrevious->size();
			for (int i = 0; i < thisStatementPreviousSize; i++) {
				currentStatementToCheck = thisStatementPrevious->at(i);
				statementsToCheck.push(currentStatementToCheck);
				statementsAndUseVariablesToCheck.insert({ currentStatementToCheck, thisStatementUseVariableIndexes });
			}

			while (!statementsToCheck.empty()) {
				// Obtain the next statement for processing from the statementsToCheck stack
				currentStatementToCheck = statementsToCheck.top();
				statementsToCheck.pop();
				currentStatementNumber = currentStatementToCheck->getStatementNumber();

				// Obtain its corresponding use variables to check with
				thisStatementUseVariableIndexes = statementsAndUseVariablesToCheck.at(currentStatementToCheck);
				thisStatementUseVariableIndexesSize = thisStatementUseVariableIndexes.size();
				previousStatementUseVariableIndexes.clear();

				// Make sure we check only assign statements
				if (currentStatementToCheck->getType() == Assign) {
					// Check the list of uses variables and see if there are any 
					// that matches the modify variable that this statement has
					for (int i = 0; i < thisStatementUseVariableIndexesSize; i++) {
						if (currentStatementToCheck->getModifies(0) == thisStatementUseVariableIndexes[i]) {
							affectsThis->push_back(currentStatementToCheck); // Add it into the list to be returned
						}
						else {
							// Variables not used by current statement can be continued to be checked in previous node(s)
							previousStatementUseVariableIndexes.push_back(thisStatementUseVariableIndexes[i]);
						}
					}
				}
				else if ((currentStatementToCheck->getType() == Call) && (!bonusImplementation)) {
					// Check the list of uses variables and see if there are any 
					// that matches the modify variables that this statement has
					for (int i = 0; i < thisStatementUseVariableIndexesSize; i++) {
						if (currentStatementToCheck->getModifies(0) != thisStatementUseVariableIndexes[i]) {
							// Variables not used by current statement can be continued to be checked in previous node(s)
							previousStatementUseVariableIndexes.push_back(thisStatementUseVariableIndexes[i]);
						}
					}
				}
				else {
					// We do not take any variables away if this is not an assign/calls statement
					previousStatementUseVariableIndexes = thisStatementUseVariableIndexes;
				}

				// Continue up the CFG - make sure there is still at least one remaining use variables
				// that we have to check with before adding it into the statementsToCheck stack
				if (previousStatementUseVariableIndexes.size() > 0) {
					if ((currentStatementToCheck->getType() == Call) && (bonusImplementation)) {
						currentStatementPreviousSpecific = *currentStatementToCheck->getLastCalls();
						if (statementNumbersOfStatementsAlreadyChecked.count(currentStatementPreviousSpecific->getStatementNumber()) == 0) {
							statementsToCheck.push(currentStatementPreviousSpecific);
							statementsAndUseVariablesToCheck.insert({ currentStatementPreviousSpecific, previousStatementUseVariableIndexes });
						}
						currentStatementPrevious = currentStatementToCheck->getPrevious();
						currentStatementPreviousSize = currentStatementPrevious->size();
						if (currentStatementPreviousSize > 0) {
							// Call should only have 0~1 Next
							currentStatementPreviousSpecific = currentStatementPrevious->at(0);
							if (statementNumbersOfStatementsAlreadyChecked.count(currentStatementPreviousSpecific->getStatementNumber()) == 0) {
								statementsFromPreviousProcedures.push(currentStatementPreviousSpecific);
							}
						}
					}
					else {
						currentStatementPrevious = currentStatementToCheck->getPrevious();
						currentStatementPreviousSize = currentStatementPrevious->size();
						for (int i = 0; i < currentStatementPreviousSize; i++) {
							currentStatementPreviousSpecific = currentStatementPrevious->at(i);
							if (statementNumbersOfStatementsAlreadyChecked.count(currentStatementPreviousSpecific->getStatementNumber()) == 0) {
								statementsToCheck.push(currentStatementPreviousSpecific);
								statementsAndUseVariablesToCheck.insert({ currentStatementPreviousSpecific, previousStatementUseVariableIndexes });
							}
						}

						// If statement has no next, and there is a previous procedure
						// that called the procedure this statement is in, we try to jump back
						if ((currentStatementPreviousSize == 0) && (!statementsFromPreviousProcedures.empty())) {
							currentStatementPreviousSpecific = statementsFromPreviousProcedures.top();
							statementsToCheck.push(currentStatementPreviousSpecific);
							statementsAndUseVariablesToCheck.insert({ currentStatementPreviousSpecific, previousStatementUseVariableIndexes });
							statementsFromPreviousProcedures.pop();
						}
					}
				}

				statementNumbersOfStatementsAlreadyChecked.insert(currentStatementNumber);
			}
		}
	}
	return affectsThis;
}

std::vector<StatementTableStatement*>* StatementTableStatement::getAffectedByThis() {
	if (affectedByThis == NULL) {
		affectedByThis = new std::vector<StatementTableStatement*>();
		if (getType() == Assign) {
			// Used by the algorithm to keep track of its progress
			std::set<int> statementNumbersOfStatementsAlreadyChecked;
			std::stack<StatementTableStatement*> statementsToCheck;
			// This stack is only used by the bonus implementation
			std::stack<StatementTableStatement*> statementsFromPreviousProcedures;

			// Use to keep track of the current statement being processed by the algorithm
			StatementTableStatement* currentStatementToCheck;
			int currentStatementNumber;
			int currentStatementUsesSize;

			// Use to obtain the next node(s) of the current statement
			// being processed to be added into the statementsToCheck stack
			StatementTableStatement* nextStatement;
			std::vector<StatementTableStatement*>* currentStatementNext;
			int currentStatementNextSize;
			if (getNext()->size() > 0) {
				// Push the next statement; assign statements only have one Next
				statementsToCheck.push(getNext()->at(0));
			}

			// Keep track of the index of the variable that statement modifies,
			// for comparison with the use variables of other statements
			int variableIndex = getModifies(0);

			while (!statementsToCheck.empty()) {
				// Obtain the next statement for processing from the statementsToCheck stack
				currentStatementToCheck = statementsToCheck.top();
				statementsToCheck.pop();
				currentStatementNumber = currentStatementToCheck->getStatementNumber();

				// Make sure we check only assign statements
				if (currentStatementToCheck->getType() == Assign) {
					// Check its uses variables and see if there are any 
					// that matches the modifies variable that this statement has
					currentStatementUsesSize = currentStatementToCheck->getUsesSize();
					for (int i = 0; i < currentStatementUsesSize; i++) {
						if (variableIndex == currentStatementToCheck->getUses(i)) {
							// Add it into the list to be returned
							affectedByThis->push_back(currentStatementToCheck);
							// Break out of for-loop
							i = currentStatementUsesSize;
						}
					}
				}

				currentStatementNext = currentStatementToCheck->getNext();
				currentStatementNextSize = currentStatementNext->size();
				// If it does not modify, we continue down the CFG
				if ((currentStatementToCheck->getType() != Assign) ||
					((currentStatementToCheck->getType() == Assign) && (currentStatementToCheck->getModifies(0) != variableIndex))) {
					if (currentStatementToCheck->getType() == Call) {
						if (bonusImplementation) {

							// Push in information about next into statementsFromPreviousProcedures stack
							for (int i = 0; i < currentStatementNextSize; i++) {
								nextStatement = currentStatementNext->at(i);
								if (statementNumbersOfStatementsAlreadyChecked.count(nextStatement->getStatementNumber()) == 0) {
									statementsFromPreviousProcedures.push(nextStatement);
								}
							}

							// Trace through the statements in the procedure being called
							nextStatement = *currentStatementToCheck->getFirstCalls();
							if (statementNumbersOfStatementsAlreadyChecked.count(nextStatement->getStatementNumber()) == 0) {
								statementsToCheck.push(nextStatement);
							}
						}
						else {
							// Push the next statement (if any) if this procedure does not modify variable
							bool doesModifyVariable = false;
							int currentStatementModifiesSize = currentStatementToCheck->getModifiesSize();
							for (int i = 0; i < currentStatementModifiesSize; i++) {
								if (currentStatementToCheck->getModifies(i) == variableIndex) {
									doesModifyVariable = true;
									i = currentStatementModifiesSize;
								}
							}
							if (!doesModifyVariable) {
								for (int i = 0; i < currentStatementNextSize; i++) {
									nextStatement = currentStatementNext->at(i);
									if (statementNumbersOfStatementsAlreadyChecked.count(nextStatement->getStatementNumber()) == 0) {
										statementsToCheck.push(nextStatement);
									}
								}
								// If no more next, check the statementsFromPreviousProcedures stack,
								// which will only have statements pushed onto it in the bonus implementation
								if ((currentStatementNextSize == 0) && (!statementsFromPreviousProcedures.empty())) {
									statementsToCheck.push(statementsFromPreviousProcedures.top());
									statementsFromPreviousProcedures.pop();
								}
							}
						}
					}
					else {
						for (int i = 0; i < currentStatementNextSize; i++) {
							StatementTableStatement* nextStatement = currentStatementNext->at(i);
							if (statementNumbersOfStatementsAlreadyChecked.count(nextStatement->getStatementNumber()) == 0) {
								statementsToCheck.push(nextStatement);
							}
						}
						// If no more next, check the statementsFromPreviousProcedures stack,
						// which will only have statements pushed onto it in the bonus implementation
						if ((currentStatementNextSize == 0) && (!statementsFromPreviousProcedures.empty())) {
							statementsToCheck.push(statementsFromPreviousProcedures.top());
							statementsFromPreviousProcedures.pop();
						}

						// If this is a while with only 1 next, and parent is a while, its parent should be its second next
						// (stupid fix for Wilson's next)
						StatementTableStatement* currentStatementParent = currentStatementToCheck->parent;
						if ((currentStatementToCheck->getType() == While) && (currentStatementNextSize == 1) && (currentStatementParent != NULL) &&
							(currentStatementParent->getType() == While) && (statementNumbersOfStatementsAlreadyChecked.count(currentStatementParent->getStatementNumber()) == 0)) {
							statementsToCheck.push(currentStatementParent);
						}
					}
				}

				statementNumbersOfStatementsAlreadyChecked.insert(currentStatementNumber);
			}
		}
	}
	return affectedByThis;
}

std::vector<StatementTableStatement*> StatementTableStatement::getAffectsThisStar() {
	std::vector<StatementTableStatement*> affectsThisStar;
	if (getType() == Assign) {
		// Used by the algorithm to keep track of its progress
		int currentStatementNumber = -1;
		int smallestStatementNumberThatWeKnowOf = -1;
		std::stack<int> statementNumbersBeforeCalls;
		std::unordered_map<int, StatementTableStatement*> statementNumbersAndStatements;
		std::unordered_map<int, std::vector<int>> statementsAndUsesVariablesToCheck;
		std::unordered_map<StatementTableStatement*, std::vector<std::vector<int>>> whileStatementsAndHistoryOfUsesVariables;
		StatementTableStatement* currentStatement = NULL;
		
		// We input information of this node's Previous node(s) into
		// our data structures and prepare for processing by the algorithm
		std::vector<StatementTableStatement*>* thisStatementPrevious = getPrevious();
		int thisStatementPreviousSize = thisStatementPrevious->size();
		StatementTableStatement* thisPreviousWithLargerStatementNumber = NULL;
		for (int i = 0; i < thisStatementPreviousSize; i++) {
			currentStatement = thisStatementPrevious->at(i);
			currentStatementNumber = currentStatement->getStatementNumber();
			if ((thisPreviousWithLargerStatementNumber == NULL) || (currentStatementNumber > thisPreviousWithLargerStatementNumber->getStatementNumber())) {
				thisPreviousWithLargerStatementNumber = currentStatement;
			}
			std::vector<int> useVariables;
			int useVariablesSize = getUsesSize();
			for (int x = 0; x < useVariablesSize; x++) {
				useVariables.push_back(getUses(x));
			}
			statementNumbersAndStatements.insert({ currentStatementNumber, currentStatement });
			if ((smallestStatementNumberThatWeKnowOf == -1) || (smallestStatementNumberThatWeKnowOf > currentStatementNumber)) {
				smallestStatementNumberThatWeKnowOf = currentStatementNumber;
			}
			statementsAndUsesVariablesToCheck.insert({ currentStatementNumber, useVariables });
		}
		currentStatement = thisPreviousWithLargerStatementNumber;

		while (currentStatement != NULL) {

			// Execution varies depending on the type of statement
			switch (currentStatement->getType()) {
			case Assign:
			{
				// Prepare the uses variables of the current statement to be compared
				std::vector<int> usesVariables = statementsAndUsesVariablesToCheck.at(currentStatementNumber);
				int usesVariablesSize = usesVariables.size();
				int modifyVariableIndex = currentStatement->getModifies(0);
				// Make a copy to modify and use for the further next nodes, if applicable
				std::vector<int> newUsesVariables;

				bool isAddedIntoList = false;
				// Check if this statement modifies any uses variables - if it does, remove it from the list
				for (int i = 0; i < usesVariablesSize; i++) {
					if ((isAddedIntoList) || (modifyVariableIndex != usesVariables[i])) {
						// Check if adding duplicate
						bool isDuplicate = false;
						int newUsesVariablesSize = newUsesVariables.size();
						for (int x = 0; x < newUsesVariablesSize; x++) {
							if (newUsesVariables[x] == usesVariables[i]) {
								isDuplicate = true;
							}
						}
						if (!isDuplicate) {
							newUsesVariables.push_back(usesVariables[i]);
						}
					}
					else {
						isAddedIntoList = true;
						if (getProcedureIndexNumber() == currentStatement->getProcedureIndexNumber()) {
							// Found a statement affected by this statement, add it into the list to be returned
							affectsThisStar.push_back(currentStatement);

							// NOTE: Copy and paste the three lines below and bring it out of this if-statement if 
							// variables used by statements in a procedure cannot affect statements in another procedure
							// Add in variables to affect the next statements
							int currStmtUsesSize = currentStatement->getUsesSize();
							for (int x = 0; x < currStmtUsesSize; x++) {
								newUsesVariables.push_back(currentStatement->getUses(x));
							}
						}

						// NOTE: Code goes here if variables used by statements in
						// a procedure can affect statements in another procedure
					}
				}

				// Add information regarding the previous node(s) to the data structures for our algorithm's processing
				int maximum = currentStatementNumber - 1;
				std::vector<StatementTableStatement*>* currentStatementPreviouses = currentStatement->getPrevious();
				int currentStatementPreviousesSize = currentStatementPreviouses->size();
				for (int i = 0; i < currentStatementPreviousesSize; i++) {
					StatementTableStatement* currentStatementPrevious = currentStatementPreviouses->at(i);
					int currentStatementPreviousNumber = currentStatementPrevious->getStatementNumber();
					statementNumbersAndStatements.insert({ currentStatementPreviousNumber, currentStatementPrevious });
					if (smallestStatementNumberThatWeKnowOf > currentStatementPreviousNumber) {
						smallestStatementNumberThatWeKnowOf = currentStatementPreviousNumber;
					}
					if (currentStatementPreviousNumber > maximum) {
						maximum = currentStatementPreviousNumber;
					}

					if (statementsAndUsesVariablesToCheck.count(currentStatementPreviousNumber) > 0) {
						// If information about the variables to be checked for this previous statement exists,
						// update said information to include all variables
						std::set<int> newVariables;
						std::vector<int> vectorFromTable = statementsAndUsesVariablesToCheck.at(currentStatementPreviousNumber);
						int vectorFromTableSize = vectorFromTable.size();
						int newUsesVariablesSize = newUsesVariables.size();
						for (int i = 0; i < vectorFromTableSize; i++) {
							newVariables.insert(vectorFromTable[i]);
						}
						for (int i = 0; i < newUsesVariablesSize; i++) {
							newVariables.insert(newUsesVariables[i]);
						}
						std::vector<int> newVariablesToBeAdded;
						std::set<int>::iterator end = newVariables.end();
						for (std::set<int>::iterator it = newVariables.begin(); it != end; it++) {
							newVariablesToBeAdded.push_back(*it);
						}
						statementsAndUsesVariablesToCheck.at(currentStatementPreviousNumber) = newVariablesToBeAdded;
					}
					else {
						statementsAndUsesVariablesToCheck.insert({ currentStatementPreviousNumber, newUsesVariables });
					}
					currentStatementNumber = maximum;
				}

				if (currentStatementPreviouses->size() == 0) {
					// Statement does not have a previous; we need to figure
					// out which statement would the algorithm process next
					if (!statementNumbersBeforeCalls.empty()) {
						// Jump back to the statement before the Call statement that 
						// called the procedure that the current statement is in
						int nextStatementNumber = statementNumbersBeforeCalls.top();
						statementNumbersBeforeCalls.pop();

						// Populate that statement with information it has obtained so far
						if (statementsAndUsesVariablesToCheck.count(nextStatementNumber) > 0) {
							// If information about the variables to be checked for this next statement exists,
							// update said information to include all variables
							std::set<int> newVariables;
							std::vector<int> vectorFromTable = statementsAndUsesVariablesToCheck.at(nextStatementNumber);
							int vectorFromTableSize = vectorFromTable.size();
							int newUsesVariablesSize = newUsesVariables.size();
							for (int i = 0; i < vectorFromTableSize; i++) {
								newVariables.insert(vectorFromTable[i]);
							}
							for (int i = 0; i < newUsesVariablesSize; i++) {
								newVariables.insert(newUsesVariables[i]);
							}
							std::vector<int> newVariablesToBeAdded;
							std::set<int>::iterator end = newVariables.end();
							for (std::set<int>::iterator it = newVariables.begin(); it != end; it++) {
								newVariablesToBeAdded.push_back(*it);
							}
							statementsAndUsesVariablesToCheck.at(nextStatementNumber) = newVariablesToBeAdded;
						}
						else {
							statementsAndUsesVariablesToCheck.insert({ nextStatementNumber, newUsesVariables });
						}

						currentStatementNumber = nextStatementNumber;
					}
					else {
						// Statement is not in a procedure that was called by another procedure
						if (statementNumbersAndStatements.count(currentStatementNumber - 1) == 0) {
							// No records of the statement with the statement number -1 of this current statement
							// (it either does not exist or in inaccessible in the CFG); time to terminate this
							currentStatementNumber = smallestStatementNumberThatWeKnowOf - 1;
						}
						else {
							// Records of statement with statement number -1 of this current statement exists (it exists 
							// AND is accessible in the CFG); determine if it belongs to the same procedure as this one
							StatementTableStatement* nextStatement = statementNumbersAndStatements.at(currentStatementNumber - 1);
							if (nextStatement->getProcedureIndexNumber() == currentStatement->getProcedureIndexNumber()) {
								// Check the next statement out (this is due to the branching caused by if-else)
								currentStatementNumber--;
							}
							else {
								// It does not belong to the same procedure as the current statement. No point traversing it
								// again, as it most likely would have been traversed before, since for information about it
								// to be in our data structures, its procedure has to be called by a Calls statement, which
								// would immediately deem it to be the next statement to process right after itself
								currentStatementNumber = smallestStatementNumberThatWeKnowOf - 1; // Time to shut this down!
							}
						}
					}
				}
			}
			break;
			case If:
			case While:
			{
				// Extract information about the current statement's Previous node(s) and
				// add them into the data structures for this algorithm to process later
				std::vector<StatementTableStatement*>* currentStatementPreviouses = currentStatement->getPrevious();
				int currentStatementPreviousesSize = currentStatementPreviouses->size();
				for (int i = 0; i < currentStatementPreviousesSize; i++) {
					StatementTableStatement* currentStatementPrevious = currentStatementPreviouses->at(i);
					int currentStatementPreviousNumber = currentStatementPrevious->getStatementNumber();
					statementNumbersAndStatements.insert({ currentStatementPreviousNumber, currentStatementPrevious });
					if (smallestStatementNumberThatWeKnowOf > currentStatementPreviousNumber) {
						smallestStatementNumberThatWeKnowOf = currentStatementPreviousNumber;
					}

					if (statementsAndUsesVariablesToCheck.count(currentStatementPreviousNumber) > 0) {
						if (statementsAndUsesVariablesToCheck.count(currentStatementNumber) > 0) {
							// If information about the variables to be checked for this previous statement exists,
							// update said information to include all variables
							std::set<int> newVariables;
							std::vector<int> newUsesVariables = statementsAndUsesVariablesToCheck.at(currentStatementNumber);
							std::vector<int> existingVariables = statementsAndUsesVariablesToCheck.at(currentStatementPreviousNumber);
							int existingVariablesSize = existingVariables.size();
							int newUsesVariablesSize = newUsesVariables.size();
							for (int i = 0; i < existingVariablesSize; i++) {
								newVariables.insert(existingVariables[i]);
							}
							for (int i = 0; i < newUsesVariablesSize; i++) {
								newVariables.insert(newUsesVariables[i]);
							}
							std::vector<int> newVariablesToBeAdded;
							std::set<int>::iterator end = newVariables.end();
							for (std::set<int>::iterator it = newVariables.begin(); it != end; it++) {
								newVariablesToBeAdded.push_back(*it);
							}
							statementsAndUsesVariablesToCheck.at(currentStatementPreviousNumber) = newVariablesToBeAdded;
						}
					}
					else {
						if (statementsAndUsesVariablesToCheck.count(currentStatementNumber) > 0) {
							statementsAndUsesVariablesToCheck.insert({ currentStatementPreviousNumber, statementsAndUsesVariablesToCheck.at(currentStatementNumber) });
						}
						else {
							// I do not think that this would ever be true, but let's just add this just to be sure
							std::vector<int> newEmptyVariables;
							statementsAndUsesVariablesToCheck.insert({ currentStatementPreviousNumber, newEmptyVariables });
						}
					}
				}

				bool isLooping = false;

				// While-loops are slightly more complicated and require more care to prevent infinite loops
				if (currentStatement->getType() == While) {
					if (whileStatementsAndHistoryOfUsesVariables.count(currentStatement) == 0) {
						// It is the first time that this while-loop is being processed by the algorithm. Store
						// its current uses variables to check with into its history of uses variables, so 
						// that we can check and compare with later to see if it is time to break out of this loop
						std::vector<std::vector<int>> vectorOfVectors;
						vectorOfVectors.push_back(statementsAndUsesVariablesToCheck.at(currentStatementNumber));
						whileStatementsAndHistoryOfUsesVariables.insert({ currentStatement, vectorOfVectors });
						isLooping = true;
					}
					else {
						// This while-loop has been processed by the algorithm before. Check its current uses
						// variables with its history of uses variables to see whether it has been repeated before
						bool hasPatternRepeat = true;
						isLooping = true;
						std::vector<std::vector<int>> vectorOfVectors = whileStatementsAndHistoryOfUsesVariables.at(currentStatement);
						int vectorOfVectorsSize = vectorOfVectors.size();
						std::vector<int> latestVector = statementsAndUsesVariablesToCheck.at(currentStatementNumber);
						int latestVectorSize = latestVector.size();
						for (int x = 0; x < vectorOfVectorsSize; x++) {
							if (vectorOfVectors[x].size() == latestVectorSize) {
								for (int y = 0; y < latestVectorSize; y++) {
									if (vectorOfVectors[x][y] != latestVector[y]) {
										hasPatternRepeat = false;
										y = latestVectorSize; // Break out of for-loop
									}
								}
							}
							else {
								hasPatternRepeat = false;
							}

							// Execution differs on whether this current history of uses 
							// variables matches the list of uses variables we have currently
							if (hasPatternRepeat) {
								isLooping = false;
								x = vectorOfVectorsSize; // Break out of for-loop
							}
							else {
								// Reset for verification with next history
								hasPatternRepeat = true;
							}
						}

						// Current list of modify variables does not match any lists in history
						if (isLooping) {
							// Add current list into history for reference later
							vectorOfVectors.push_back(latestVector);
							whileStatementsAndHistoryOfUsesVariables.at(currentStatement) = vectorOfVectors;
						}
					}
				}

				if (isLooping) {
					// If while with 1 previous, jump to that previous
					// If while with 2 previous, jump to the previous with bigger statement number
					if (currentStatementPreviousesSize == 1) {
						currentStatementNumber = currentStatementPreviouses->at(0)->getStatementNumber();
					}
					else {
						int previous1StatementNumber = currentStatementPreviouses->at(0)->getStatementNumber();
						int previous2StatementNumber = currentStatementPreviouses->at(1)->getStatementNumber();
						currentStatementNumber = (previous1StatementNumber > previous2StatementNumber) ? previous1StatementNumber : previous2StatementNumber;
					}
				}
				else {
					// If while with 2 previous, jump to the previous with smaller statement number
					// If while with 1 previous or If with no previous, jump out of current procedure
					// If If with 2 previous, jump to the previous with bigger statement number
					if (currentStatementPreviousesSize == 2) {
						if (currentStatement->getType() == While) { // While with 2 previous
							int previous1StatementNumber = currentStatementPreviouses->at(0)->getStatementNumber();
							int previous2StatementNumber = currentStatementPreviouses->at(1)->getStatementNumber();
							currentStatementNumber = (previous1StatementNumber < previous2StatementNumber) ? previous1StatementNumber : previous2StatementNumber;
						}
						else { // If with 2 previous
							int previous1StatementNumber = currentStatementPreviouses->at(0)->getStatementNumber();
							int previous2StatementNumber = currentStatementPreviouses->at(1)->getStatementNumber();
							currentStatementNumber = (previous1StatementNumber > previous2StatementNumber) ? previous1StatementNumber : previous2StatementNumber;
						}
					}
					else { // Jump out of current procedure
						if (!statementNumbersBeforeCalls.empty()) {
							// Jump back to the statement before the Call statement that 
							// called the procedure that the current statement is in
							int nextStatementNumber = statementNumbersBeforeCalls.top();
							statementNumbersBeforeCalls.pop();

							// Populate that statement with information it has obtained so far
							if (statementsAndUsesVariablesToCheck.count(nextStatementNumber) > 0) {
								// If information about the variables to be checked for this next statement exists,
								// update said information to include all variables
								std::set<int> newVariables;
								std::vector<int> vectorFromTable = statementsAndUsesVariablesToCheck.at(nextStatementNumber);
								int vectorFromTableSize = vectorFromTable.size();
								std::vector<int> newUsesVariables = statementsAndUsesVariablesToCheck.at(currentStatementNumber);
								int newUsesVariablesSize = newUsesVariables.size();
								for (int i = 0; i < vectorFromTableSize; i++) {
									newVariables.insert(vectorFromTable[i]);
								}
								for (int i = 0; i < newUsesVariablesSize; i++) {
									newVariables.insert(newUsesVariables[i]);
								}
								std::vector<int> newVariablesToBeAdded;
								std::set<int>::iterator end = newVariables.end();
								for (std::set<int>::iterator it = newVariables.begin(); it != end; it++) {
									newVariablesToBeAdded.push_back(*it);
								}
								statementsAndUsesVariablesToCheck.at(nextStatementNumber) = newVariablesToBeAdded;
							}
							else {
								statementsAndUsesVariablesToCheck.insert({ nextStatementNumber, statementsAndUsesVariablesToCheck.at(currentStatementNumber) });
							}

							currentStatementNumber = nextStatementNumber;
						}
						else {
							// Statement is not in a procedure that was called by another procedure
							if (statementNumbersAndStatements.count(currentStatementNumber - 1) == 0) {
								// No records of the statement with the statement number -1 of this current statement
								// (it either does not exist or in inaccessible in the CFG); time to terminate this
								currentStatementNumber = smallestStatementNumberThatWeKnowOf - 1;
							}
							else {
								// Records of statement with statement number -1 of this current statement exists (it exists 
								// AND is accessible in the CFG); determine if it belongs to the same procedure as this one
								StatementTableStatement* nextStatement = statementNumbersAndStatements.at(currentStatementNumber - 1);
								if (nextStatement->getProcedureIndexNumber() == currentStatement->getProcedureIndexNumber()) {
									// Check the next statement out (this is due to the branching caused by if-else)
									currentStatementNumber--;
								}
								else {
									// It does not belong to the same procedure as the current statement. No point traversing it
									// again, as it most likely would have been traversed before, since for information about it
									// to be in our data structures, its procedure has to be called by a Calls statement, which
									// would immediately deem it to be the next statement to process right after itself
									currentStatementNumber = smallestStatementNumberThatWeKnowOf - 1; // Time to shut this down!
								}
							}
						}
					}
				}
			}
			break;
			case Call:
			{
				if (bonusImplementation) {
					// Add information about its previous statement in the same procedure
					std::vector<StatementTableStatement*>* currStatementPreviousStmts = currentStatement->getPrevious();
					int currStatementPreviousStmtsSize = currStatementPreviousStmts->size();
					StatementTableStatement* currStatementPreviousStmt;
					for (int i = 0; i < currStatementPreviousStmtsSize; i++) {
						currStatementPreviousStmt = currStatementPreviousStmts->at(i);
						int currStatementPreviousStmtNumber = currStatementPreviousStmt->getStatementNumber();
						statementNumbersAndStatements.insert({ currStatementPreviousStmtNumber, currStatementPreviousStmt });
						if (smallestStatementNumberThatWeKnowOf > currStatementPreviousStmtNumber) {
							smallestStatementNumberThatWeKnowOf = currStatementPreviousStmtNumber;
						}

						std::vector<int> newUsesVariables = statementsAndUsesVariablesToCheck.at(currentStatementNumber);
						if (statementsAndUsesVariablesToCheck.count(currStatementPreviousStmtNumber) > 0) {
							// If information about the variables to be checked for this next statement exists,
							// update said information to include all variables
							std::set<int> newVariables;
							std::vector<int> existingVariables = statementsAndUsesVariablesToCheck.at(currStatementPreviousStmtNumber);
							int existingVariablesSize = existingVariables.size();
							int newUsesVariablesSize = newUsesVariables.size();
							for (int i = 0; i < existingVariablesSize; i++) {
								newVariables.insert(existingVariables[i]);
							}
							for (int i = 0; i < newUsesVariablesSize; i++) {
								newVariables.insert(newUsesVariables[i]);
							}
							std::vector<int> newVariablesToBeAdded;
							std::set<int>::iterator end = newVariables.end();
							for (std::set<int>::iterator it = newVariables.begin(); it != end; it++) {
								newVariablesToBeAdded.push_back(*it);
							}
							statementsAndUsesVariablesToCheck.insert({ currStatementPreviousStmtNumber, newVariablesToBeAdded });
						}
						else {
							statementsAndUsesVariablesToCheck.insert({ currStatementPreviousStmtNumber, newUsesVariables });
						}
					}

					// Add information about the last statement of the procedure that it calls
					bool hasGotPrevious = false;
					StatementTableStatement** pointerToPointer = currentStatement->getLastCalls();
					if (pointerToPointer != NULL) {
						StatementTableStatement* currentStatementPrevious = *pointerToPointer;
						if (currentStatementPrevious != NULL) {
							int currentStatementPreviousNumber = currentStatementPrevious->getStatementNumber();
							statementNumbersAndStatements.insert({ currentStatementPreviousNumber, currentStatementPrevious });
							if (smallestStatementNumberThatWeKnowOf > currentStatementPreviousNumber) {
								smallestStatementNumberThatWeKnowOf = currentStatementPreviousNumber;
							}

							std::vector<int> newUsesVariables = statementsAndUsesVariablesToCheck.at(currentStatementNumber);
							if (statementsAndUsesVariablesToCheck.count(currentStatementPreviousNumber) > 0) {
								// If information about the variables to be checked for this next statement exists,
								// update said information to include all variables
								std::set<int> newVariables;
								std::vector<int> existingVariables = statementsAndUsesVariablesToCheck.at(currentStatementPreviousNumber);
								int existingVariablesSize = existingVariables.size();
								int newUsesVariablesSize = newUsesVariables.size();
								for (int i = 0; i < existingVariablesSize; i++) {
									newVariables.insert(existingVariables[i]);
								}
								for (int i = 0; i < newUsesVariablesSize; i++) {
									newVariables.insert(newUsesVariables[i]);
								}
								std::vector<int> newVariablesToBeAdded;
								std::set<int>::iterator end = newVariables.end();
								for (std::set<int>::iterator it = newVariables.begin(); it != end; it++) {
									newVariablesToBeAdded.push_back(*it);
								}
								statementsAndUsesVariablesToCheck.insert({ currentStatementPreviousNumber, newVariablesToBeAdded });
							}
							else {
								statementsAndUsesVariablesToCheck.insert({ currentStatementPreviousNumber, newUsesVariables });
							}

							// Have algorithm jump to the last statement of the procedure it calls
							currentStatementNumber = currentStatementPreviousNumber;
							switch (currStatementPreviousStmtsSize) {
							case 1: // If one previous, push it into directly onto statementNumbersBeforeCalls stack
								statementNumbersBeforeCalls.push(currStatementPreviousStmts->at(0)->getStatementNumber());
								break;
							case 2: // If two previous, push the one with a greater statement number onto stack
							{
								int previous1StatementNumber = currStatementPreviousStmts->at(0)->getStatementNumber();
								int previous2StatementNumber = currStatementPreviousStmts->at(1)->getStatementNumber();
								statementNumbersBeforeCalls.push((previous1StatementNumber > previous2StatementNumber) ? previous1StatementNumber : previous2StatementNumber);
							}
							break;
							}
							hasGotPrevious = true;
						}
					}

					if (!hasGotPrevious) {
						if (!statementNumbersBeforeCalls.empty()) {
							currentStatementNumber = statementNumbersBeforeCalls.top();
							statementNumbersBeforeCalls.pop();
						}
						else {
							currentStatementNumber--;
						}
					}
				}
				else {
					if (currentStatement->getPrevious()->size() == 0) {
						currentStatementNumber = smallestStatementNumberThatWeKnowOf - 1;
					}
					else {
						// Prepare the uses variables of the procedure being called to be compared
						std::vector<int> usesVariables = statementsAndUsesVariablesToCheck.at(currentStatementNumber);
						int usesVariablesSize = usesVariables.size();
						int modifyVariablesSize = currentStatement->getModifiesSize();
						// Make a copy to modify and use for the further next nodes, if applicable
						std::vector<int> newUsesVariables;

						// Check if this statement modifies any uses variables - if it does, remove it from the list
						for (int i = 0; i < usesVariablesSize; i++) {
							bool shouldAdd = true;
							for (int j = 0; j < modifyVariablesSize; j++) {
								if (currentStatement->getModifies(j) == usesVariables[i]) {
									shouldAdd = false;
									j = modifyVariablesSize;
								}
							}
							if (shouldAdd) {
								newUsesVariables.push_back(usesVariables[i]);
							}
						}

						// NOTE: Uncomment the three lines below if variables used by
						// statements in a procedure can affect statements in another procedure
						/*
						int currStmtUsesSize = currentStatement->getUsesSize();
						for (int x = 0; x < currStmtUsesSize; x++) {
							newUsesVariables.push_back(currentStatement->getUses(x));
						}
						*/

						if (newUsesVariables.size() > 0) {
							// Add information regarding the previous node(s) to the data structures for our algorithm's processing
							int maximum = currentStatementNumber - 1;
							std::vector<StatementTableStatement*>* currentStatementPreviouses = currentStatement->getPrevious();
							int currentStatementPreviousesSize = currentStatementPreviouses->size();
							for (int i = 0; i < currentStatementPreviousesSize; i++) {
								StatementTableStatement* currentStatementPrevious = currentStatementPreviouses->at(i);
								int currentStatementPreviousNumber = currentStatementPrevious->getStatementNumber();
								statementNumbersAndStatements.insert({ currentStatementPreviousNumber, currentStatementPrevious });
								if (smallestStatementNumberThatWeKnowOf > currentStatementPreviousNumber) {
									smallestStatementNumberThatWeKnowOf = currentStatementPreviousNumber;
								}
								if (currentStatementPreviousNumber > maximum) {
									maximum = currentStatementPreviousNumber;
								}

								if (statementsAndUsesVariablesToCheck.count(currentStatementPreviousNumber) > 0) {
									// If information about the variables to be checked for this previous statement exists,
									// update said information to include all variables
									std::set<int> newVariables;
									std::vector<int> vectorFromTable = statementsAndUsesVariablesToCheck.at(currentStatementPreviousNumber);
									int vectorFromTableSize = vectorFromTable.size();
									int newUsesVariablesSize = newUsesVariables.size();
									for (int i = 0; i < vectorFromTableSize; i++) {
										newVariables.insert(vectorFromTable[i]);
									}
									for (int i = 0; i < newUsesVariablesSize; i++) {
										newVariables.insert(newUsesVariables[i]);
									}
									std::vector<int> newVariablesToBeAdded;
									std::set<int>::iterator end = newVariables.end();
									for (std::set<int>::iterator it = newVariables.begin(); it != end; it++) {
										newVariablesToBeAdded.push_back(*it);
									}
									statementsAndUsesVariablesToCheck.at(currentStatementPreviousNumber) = newVariablesToBeAdded;
								}
								else {
									statementsAndUsesVariablesToCheck.insert({ currentStatementPreviousNumber, newUsesVariables });
								}
								currentStatementNumber = maximum;
							}
						}
					}
				}
			}
			break;
			}

			// Move on to the previous statement in sequence of statement numbers
			if (statementNumbersAndStatements.count(currentStatementNumber) > 0) {
				currentStatement = statementNumbersAndStatements.at(currentStatementNumber);
			}
			else {
				bool hasFoundNext = false;
				for (int runningOutOfVariableNamesForForLoops = currentStatementNumber;
						runningOutOfVariableNamesForForLoops >= smallestStatementNumberThatWeKnowOf;
						runningOutOfVariableNamesForForLoops--) {
					if (statementNumbersAndStatements.count(runningOutOfVariableNamesForForLoops) > 0) {
						currentStatementNumber = runningOutOfVariableNamesForForLoops;
						currentStatement = statementNumbersAndStatements.at(currentStatementNumber);
						hasFoundNext = true;
						// Break out of for-loop
						runningOutOfVariableNamesForForLoops = smallestStatementNumberThatWeKnowOf - 1;
					}
				}
				if (!hasFoundNext) {
					currentStatement = NULL;
				}
			}
		}
	}
	return affectsThisStar;
}

std::vector<StatementTableStatement*> StatementTableStatement::getAffectedByThisStar() {
	std::vector<StatementTableStatement*> affectedByThisStar;
	if (getType() == Assign) {
		// Used by the algorithm to keep track of its progress
		int currentStatementNumber = -1;
		int biggestStatementNumberThatWeKnowOf = -1;
		std::stack<int> statementNumbersAfterCalls;
		std::unordered_map<int, StatementTableStatement*> statementNumbersAndStatements;
		std::unordered_map<int, std::vector<int>> statementsAndModifyVariablesToCheck;
		std::unordered_map<StatementTableStatement*, std::vector<std::vector<int>>> whileStatementsAndHistoryOfModifiesVariables;
		StatementTableStatement* currentStatement = NULL;

		// As assign statements have only at most one Next, we input its information, if any,
		// into our data structures and prepare for processing by the algorithm
		if (getNext()->size() > 0) {
			currentStatement = getNext()->at(0);
			std::vector<int> modifyVariables;
			modifyVariables.push_back(getModifies(0));
			currentStatementNumber = currentStatement->getStatementNumber();
			statementNumbersAndStatements.insert({ currentStatementNumber, currentStatement });
			biggestStatementNumberThatWeKnowOf = currentStatementNumber;
			statementsAndModifyVariablesToCheck.insert({ currentStatementNumber, modifyVariables });
		}

		while (currentStatement != NULL) {
			std::cout << "statement = " << currentStatementNumber << ", modify = ";
			std::vector<int> mod = statementsAndModifyVariablesToCheck.at(currentStatementNumber);
			for (unsigned int i = 0; i < mod.size(); i++) {
				std::cout << mod[i] << " ";
			}
			std::cout << std::endl;

			// Execution varies depending on the type of statement
			switch (currentStatement->getType()) {
			case Assign:
			{
				// Prepare the modify variables of the current statement to be compared
				std::vector<int> modifyVariables = statementsAndModifyVariablesToCheck.at(currentStatementNumber);
				int modifyVariablesSize = modifyVariables.size();
				int useVariablesSize = currentStatement->getUsesSize();
				// Make a copy to modify and use for the further next nodes, if applicable
				std::vector<int> newModifyVariables;

				// Check if this statement modifies any modify variables
				for (int x = 0; x < modifyVariablesSize; x++) {
					if (modifyVariables[x] != currentStatement->getModifies(0)) {
						newModifyVariables.push_back(modifyVariables[x]);
					}
				}

				// Check if this statement uses any modify variables
				for (int x = 0; x < modifyVariablesSize; x++) {
					for (int y = 0; y < useVariablesSize; y++) {
						if (modifyVariables[x] == currentStatement->getUses(y)) {
							if (getProcedureIndexNumber() == currentStatement->getProcedureIndexNumber()) {
								// Make sure we are not entering a duplicate
								int affectedByThisStarSize = affectedByThisStar.size();
								bool isDuplicate = false;
								for (int i = 0; i < affectedByThisStarSize; i++) {
									if (affectedByThisStar[i] == currentStatement) {
										isDuplicate = true;
									}
								}
								if (!isDuplicate) {
									// Found a statement affected by this statement, add it into the list to be returned)
									affectedByThisStar.push_back(currentStatement);
								}
								// NOTE: Copy and paste the three lines below and bring it out of this if-statement if 
								// variables used by statements in a procedure cannot affect statements in another procedure
								// Add in variables to affect the next statements
								newModifyVariables.push_back(currentStatement->getModifies(0));
							}
							// NOTE: Code goes here if variables used by statements in
							// a procedure can affect statements in another procedure

							// Break out of both loops
							x = modifyVariablesSize;
							y = useVariablesSize;
						}
					}
				}

				// Add information regarding the next node(s) to the data structures for our algorithm's processing
				int minimum = currentStatementNumber + 1;
				std::vector<StatementTableStatement*>* currentStatementNexts = currentStatement->getNext();
				if (currentStatementNexts->size() > 0) {
					StatementTableStatement* currentStatementNext = currentStatementNexts->at(0);
					int currentStatementNextNumber = currentStatementNext->getStatementNumber();
					statementNumbersAndStatements.insert({ currentStatementNextNumber, currentStatementNext });
					if (biggestStatementNumberThatWeKnowOf < currentStatementNextNumber) {
						biggestStatementNumberThatWeKnowOf = currentStatementNextNumber;
					}
					if (currentStatementNextNumber < minimum) {
						minimum = currentStatementNextNumber;
					}

					if (statementsAndModifyVariablesToCheck.count(currentStatementNextNumber) > 0) {
						// If information about the variables to be checked for this next statement exists,
						// update said information to include all variables
						std::set<int> newVariables;
						std::vector<int> vectorFromTable = statementsAndModifyVariablesToCheck.at(currentStatementNextNumber);
						int vectorFromTableSize = vectorFromTable.size();
						int newModifyVariablesSize = newModifyVariables.size();
						for (int i = 0; i < vectorFromTableSize; i++) {
							newVariables.insert(vectorFromTable[i]);
						}
						for (int i = 0; i < newModifyVariablesSize; i++) {
							newVariables.insert(newModifyVariables[i]);
						}
						std::vector<int> newVariablesToBeAdded;
						std::set<int>::iterator end = newVariables.end();
						for (std::set<int>::iterator it = newVariables.begin(); it != end; it++) {
							newVariablesToBeAdded.push_back(*it);
						}
						statementsAndModifyVariablesToCheck.at(currentStatementNextNumber) = newVariablesToBeAdded;
					}
					else {
						statementsAndModifyVariablesToCheck.insert({ currentStatementNextNumber, newModifyVariables });
					}
					currentStatementNumber = minimum;
				}
				else {
					// Statement does not have a next; we need to figure
					// out which statement would the algorithm process next
					if (!statementNumbersAfterCalls.empty()) {
						// Jump back to the statement after the Call statement that 
						// called the procedure that the current statement is in
						int nextStatementNumber = statementNumbersAfterCalls.top();
						statementNumbersAfterCalls.pop();

						// Populate that statement with information it has obtained so far
						if (statementsAndModifyVariablesToCheck.count(nextStatementNumber) > 0) {
							// If information about the variables to be checked for this next statement exists,
							// update said information to include all variables
							std::set<int> newVariables;
							std::vector<int> vectorFromTable = statementsAndModifyVariablesToCheck.at(nextStatementNumber);
							int vectorFromTableSize = vectorFromTable.size();
							int newModifyVariablesSize = newModifyVariables.size();
							for (int i = 0; i < vectorFromTableSize; i++) {
								newVariables.insert(vectorFromTable[i]);
							}
							for (int i = 0; i < newModifyVariablesSize; i++) {
								newVariables.insert(newModifyVariables[i]);
							}
							std::vector<int> newVariablesToBeAdded;
							std::set<int>::iterator end = newVariables.end();
							for (std::set<int>::iterator it = newVariables.begin(); it != end; it++) {
								newVariablesToBeAdded.push_back(*it);
							}
							statementsAndModifyVariablesToCheck.at(nextStatementNumber) = newVariablesToBeAdded;
						}
						else {
							statementsAndModifyVariablesToCheck.insert({ nextStatementNumber, newModifyVariables });
						}

						currentStatementNumber = nextStatementNumber;
					}
					else {
						// Statement is not in a procedure that was called by another procedure
						if (statementNumbersAndStatements.count(currentStatementNumber + 1) == 0) {
							// No records of the statement with the statement number +1 of this current statement
							// (it either does not exist or in inaccessible in the CFG); time to terminate this
							currentStatementNumber = biggestStatementNumberThatWeKnowOf + 1;
						}
						else {
							// Records of statement with statement number +1 of this current statement exists (it exists 
							// AND is accessible in the CFG); determine if it belongs to the same procedure as this one
							StatementTableStatement* nextStatement = statementNumbersAndStatements.at(currentStatementNumber + 1);
							if (nextStatement->getProcedureIndexNumber() == currentStatement->getProcedureIndexNumber()) {
								// Check the next statement out (this is due to the branching caused by if-else)
								currentStatementNumber++;
							}
							else {
								// It does not belong to the same procedure as the current statement. No point traversing it
								// again, as it most likely would have been traversed before, since for information about it
								// to be in our data structures, its procedure has to be called by a Calls statement, which
								// would immediately deem it to be the next statement to process right after itself
								currentStatementNumber = biggestStatementNumberThatWeKnowOf + 1; // Time to shut this down!
							}
						}
					}
				}
			}
				break;
			case If:
			case While:
			{
				// Extract information about the current statement's Next nodes and
				// add them into the data structures for this algorithm to process later
				std::vector<StatementTableStatement*>* currentStatementNexts = currentStatement->getNext();
				int currentStatementNextsSize = currentStatementNexts->size();
				for (int i = 0; i < currentStatementNextsSize; i++) {
					StatementTableStatement* currentStatementNext = currentStatementNexts->at(i);
					int currentStatementNextNumber = currentStatementNext->getStatementNumber();
					statementNumbersAndStatements.insert({ currentStatementNextNumber, currentStatementNext });
					if (biggestStatementNumberThatWeKnowOf < currentStatementNextNumber) {
						biggestStatementNumberThatWeKnowOf = currentStatementNextNumber;
					}

					if (statementsAndModifyVariablesToCheck.count(currentStatementNextNumber) > 0) {
						if (statementsAndModifyVariablesToCheck.count(currentStatementNumber) > 0) {
							// If information about the variables to be checked for this next statement exists,
							// update said information to include all variables
							std::set<int> newVariables;
							std::vector<int> newModifyVariables = statementsAndModifyVariablesToCheck.at(currentStatementNumber);
							std::vector<int> existingVariables = statementsAndModifyVariablesToCheck.at(currentStatementNextNumber);
							int existingVariablesSize = existingVariables.size();
							int newModifyVariablesSize = newModifyVariables.size();
							for (int i = 0; i < existingVariablesSize; i++) {
								newVariables.insert(existingVariables[i]);
							}
							for (int i = 0; i < newModifyVariablesSize; i++) {
								newVariables.insert(newModifyVariables[i]);
							}
							std::vector<int> newVariablesToBeAdded;
							std::set<int>::iterator end = newVariables.end();
							for (std::set<int>::iterator it = newVariables.begin(); it != end; it++) {
								newVariablesToBeAdded.push_back(*it);
							}
							statementsAndModifyVariablesToCheck.at(currentStatementNextNumber) = newVariablesToBeAdded;
						}
					}
					else {
						if (statementsAndModifyVariablesToCheck.count(currentStatementNumber) > 0) {
							statementsAndModifyVariablesToCheck.insert({ currentStatementNextNumber, statementsAndModifyVariablesToCheck.at(currentStatementNumber) });
						}
						else {
							// I do not think that this would ever be true, but let's just add this just to be sure
							std::vector<int> newEmptyVariables;
							statementsAndModifyVariablesToCheck.insert({ currentStatementNextNumber, newEmptyVariables });
						}
					}
				}

				// While-loops are slightly more complicated and require more care to prevent infinite loops
				if (currentStatement->getType() == While) {
					if (whileStatementsAndHistoryOfModifiesVariables.count(currentStatement) == 0) {
						// It is the first time that this while-loop is being processed by the algorithm. Store
						// its current modify variables to check with into its history of modify variables, so 
						// that we can check and compare with later to see if it is time to break out of this loop
						std::vector<std::vector<int>> vectorOfVectors;
						vectorOfVectors.push_back(statementsAndModifyVariablesToCheck.at(currentStatementNumber));
						whileStatementsAndHistoryOfModifiesVariables.insert({ currentStatement, vectorOfVectors });
						currentStatementNumber++;
					}
					else {
						// This while-loop has been processed by the algorithm before. Check its current modify
						// variables with its history of modify variables to see whether it has been repeated before
						bool hasPatternRepeat = true;
						bool isTerminatingForLoop = false;
						std::vector<std::vector<int>> vectorOfVectors = whileStatementsAndHistoryOfModifiesVariables.at(currentStatement);
						int vectorOfVectorsSize = vectorOfVectors.size();
						std::vector<int> latestVector = statementsAndModifyVariablesToCheck.at(currentStatementNumber);
						int latestVectorSize = latestVector.size();
						for (int x = 0; x < vectorOfVectorsSize; x++) {
							if (vectorOfVectors[x].size() == latestVectorSize) {
								for (int y = 0; y < latestVectorSize; y++) {
									if (vectorOfVectors[x][y] != latestVector[y]) {
										hasPatternRepeat = false;
										y = latestVectorSize; // Break out of for-loop
									}
								}
							}
							else {
								hasPatternRepeat = false;
							}

							// Execution differs on whether this current history of modify 
							// variables matches the list of modify variables we have currently
							if (hasPatternRepeat) {
								// If our current list matches with this list in history, we break out of the for-loop (as no 
								// point checking the ones after this), and execute the statement with a larger statement
								// number among its two Next statements next (the statement with the smaller statement number
								// is the first statement in the statement list of this while loop [ie. that Next statement
								// has this While statement as parent]) in order to break out of the for-loop
								if (currentStatementNexts->size() > 1) {
									int next1 = currentStatementNexts->at(0)->getStatementNumber();
									int next2 = currentStatementNexts->at(1)->getStatementNumber();
									currentStatementNumber = (next1 > next2) ? next1 : next2;
								}
								else {
									// Check if its parent is a while loop. If it is, exit back to parent
									// (Stupid fix since Wilson's Next is not giving me the answers I need, but oh well)
									StatementTableStatement* currentStatementParent = currentStatement->parent;
									if ((currentStatementParent != NULL) && (currentStatementParent->getType() == While)) {
										int currentStatementParentNumber = currentStatementParent->getStatementNumber();
										// Check if information regarding its parent exists
										if (statementNumbersAndStatements.count(currentStatementParentNumber) == 0) {
											statementNumbersAndStatements.insert({ currentStatementParentNumber, currentStatementParent });
										}
										if (statementsAndModifyVariablesToCheck.count(currentStatementParentNumber) > 0) {
											if (statementsAndModifyVariablesToCheck.count(currentStatementNumber) > 0) {
												// If information about the variables to be checked for this next statement exists,
												// update said information to include all variables
												std::set<int> newVariables;
												std::vector<int> newModifyVariables = statementsAndModifyVariablesToCheck.at(currentStatementNumber);
												std::vector<int> existingVariables = statementsAndModifyVariablesToCheck.at(currentStatementParentNumber);
												int existingVariablesSize = existingVariables.size();
												int newModifyVariablesSize = newModifyVariables.size();
												for (int i = 0; i < existingVariablesSize; i++) {
													newVariables.insert(existingVariables[i]);
												}
												for (int i = 0; i < newModifyVariablesSize; i++) {
													newVariables.insert(newModifyVariables[i]);
												}
												std::vector<int> newVariablesToBeAdded;
												std::set<int>::iterator end = newVariables.end();
												for (std::set<int>::iterator it = newVariables.begin(); it != end; it++) {
													newVariablesToBeAdded.push_back(*it);
												}
												statementsAndModifyVariablesToCheck.at(currentStatementParentNumber) = newVariablesToBeAdded;
											}
										}
										else {
											if (statementsAndModifyVariablesToCheck.count(currentStatementNumber) > 0) {
												statementsAndModifyVariablesToCheck.insert({ currentStatementParentNumber, statementsAndModifyVariablesToCheck.at(currentStatementNumber) });
											}
											else {
												// I do not think that this would ever be true, but let's just add this just to be sure
												std::vector<int> newEmptyVariables;
												statementsAndModifyVariablesToCheck.insert({ currentStatementParentNumber, newEmptyVariables });
											}
										}

										currentStatementNumber = currentStatementParentNumber;
									}
									else if (!statementNumbersAfterCalls.empty()) {
										// Jump back to the statement after the Call statement that 
										// called the procedure that the current statement is in
										int nextStatementNumber = statementNumbersAfterCalls.top();
										statementNumbersAfterCalls.pop();

										std::vector<int> newModifyVariables = statementsAndModifyVariablesToCheck.at(currentStatementNumber);
										// Populate that statement with information it has obtained so far
										if (statementsAndModifyVariablesToCheck.count(nextStatementNumber) > 0) {
											// If information about the variables to be checked for this next statement exists,
											// update said information to include all variables
											std::set<int> newVariables;
											std::vector<int> vectorFromTable = statementsAndModifyVariablesToCheck.at(nextStatementNumber);
											int vectorFromTableSize = vectorFromTable.size();
											int newModifyVariablesSize = newModifyVariables.size();
											for (int i = 0; i < vectorFromTableSize; i++) {
												newVariables.insert(vectorFromTable[i]);
											}
											for (int i = 0; i < newModifyVariablesSize; i++) {
												newVariables.insert(newModifyVariables[i]);
											}
											std::vector<int> newVariablesToBeAdded;
											std::set<int>::iterator end = newVariables.end();
											for (std::set<int>::iterator it = newVariables.begin(); it != end; it++) {
												newVariablesToBeAdded.push_back(*it);
											}
											statementsAndModifyVariablesToCheck.at(nextStatementNumber) = newVariablesToBeAdded;
										}
										else {
											statementsAndModifyVariablesToCheck.insert({ nextStatementNumber, newModifyVariables });
										}

										currentStatementNumber = nextStatementNumber;
									}
									else {
										// If it is not, shut this thing down?
										currentStatementNumber = biggestStatementNumberThatWeKnowOf + 1;
									}
								}
								x = vectorOfVectorsSize; // Break out of for-loop
								isTerminatingForLoop = true;
							}
							else {
								// Reset for verification with next history
								hasPatternRepeat = true;
							}
						}

						// Current list of modify variables does not match any lists in history
						if (!isTerminatingForLoop) {
							// Add current list into history for reference later
							vectorOfVectors.push_back(latestVector);
							whileStatementsAndHistoryOfModifiesVariables.at(currentStatement) = vectorOfVectors;
							// Have algorithm traverse into the Next statement with the smaller statement number (ie. the
							// first statement in the statement list of this while-loop) to continue looping through it
							if (currentStatementNexts->size() > 1) {
								int next1 = currentStatement->getNext()->at(0)->getStatementNumber();
								int next2 = currentStatement->getNext()->at(1)->getStatementNumber();
								currentStatementNumber = (next1 < next2) ? next1 : next2;
							}
							else {
								// Surely a while-loop has AT LEAST one Next, right?
								currentStatementNumber = currentStatementNexts->at(0)->getStatementNumber();
							}
						}
					}
				}
				else {
					// If-else statements are easy to handle; just proceed onto the statement with 
					// statement number +1, which is the first statement of the if statement list
					currentStatementNumber++;
				}
			}
				break;
			case Call:
			{
				if (bonusImplementation) {
					// Add information about its next statement in the same procedure
					std::vector<StatementTableStatement*>* currStatementNextStmts = currentStatement->getNext();
					int currStatementNextStmtsSize = currStatementNextStmts->size();
					StatementTableStatement* currStatementNextStmt;
					for (int i = 0; i < currStatementNextStmtsSize; i++) {
						currStatementNextStmt = currStatementNextStmts->at(i);
						int currStatementNextNumber = currStatementNextStmt->getStatementNumber();
						statementNumbersAndStatements.insert({ currStatementNextNumber, currStatementNextStmt });
						if (biggestStatementNumberThatWeKnowOf < currStatementNextNumber) {
							biggestStatementNumberThatWeKnowOf = currStatementNextNumber;
						}

						std::vector<int> newModifyVariables = statementsAndModifyVariablesToCheck.at(currentStatementNumber);
						if (statementsAndModifyVariablesToCheck.count(currStatementNextNumber) > 0) {
							// If information about the variables to be checked for this next statement exists,
							// update said information to include all variables
							std::set<int> newVariables;
							std::vector<int> existingVariables = statementsAndModifyVariablesToCheck.at(currStatementNextNumber);
							int existingVariablesSize = existingVariables.size();
							int newModifyVariablesSize = newModifyVariables.size();
							for (int i = 0; i < existingVariablesSize; i++) {
								newVariables.insert(existingVariables[i]);
							}
							for (int i = 0; i < newModifyVariablesSize; i++) {
								newVariables.insert(newModifyVariables[i]);
							}
							std::vector<int> newVariablesToBeAdded;
							std::set<int>::iterator end = newVariables.end();
							for (std::set<int>::iterator it = newVariables.begin(); it != end; it++) {
								newVariablesToBeAdded.push_back(*it);
							}
							statementsAndModifyVariablesToCheck.insert({ currStatementNextNumber, newVariablesToBeAdded });
						}
						else {
							statementsAndModifyVariablesToCheck.insert({ currStatementNextNumber, newModifyVariables });
						}
					}

					// Add information about the first statement of the procedure that it calls
					bool hasGotNext = false;
					StatementTableStatement** pointerToPointer = currentStatement->getFirstCalls();
					if (pointerToPointer != NULL) {
						StatementTableStatement* currentStatementNext = *pointerToPointer;
						if (currentStatementNext != NULL) {
							int currentStatementNextNumber = currentStatementNext->getStatementNumber();
							statementNumbersAndStatements.insert({ currentStatementNextNumber, currentStatementNext });
							if (biggestStatementNumberThatWeKnowOf < currentStatementNextNumber) {
								biggestStatementNumberThatWeKnowOf = currentStatementNextNumber;
							}

							std::vector<int> newModifyVariables = statementsAndModifyVariablesToCheck.at(currentStatementNumber);
							if (statementsAndModifyVariablesToCheck.count(currentStatementNextNumber) > 0) {
								// If information about the variables to be checked for this next statement exists,
								// update said information to include all variables
								std::set<int> newVariables;
								std::vector<int> existingVariables = statementsAndModifyVariablesToCheck.at(currentStatementNextNumber);
								int existingVariablesSize = existingVariables.size();
								int newModifyVariablesSize = newModifyVariables.size();
								for (int i = 0; i < existingVariablesSize; i++) {
									newVariables.insert(existingVariables[i]);
								}
								for (int i = 0; i < newModifyVariablesSize; i++) {
									newVariables.insert(newModifyVariables[i]);
								}
								std::vector<int> newVariablesToBeAdded;
								std::set<int>::iterator end = newVariables.end();
								for (std::set<int>::iterator it = newVariables.begin(); it != end; it++) {
									newVariablesToBeAdded.push_back(*it);
								}
								statementsAndModifyVariablesToCheck.insert({ currentStatementNextNumber, newVariablesToBeAdded });
							}
							else {
								statementsAndModifyVariablesToCheck.insert({ currentStatementNextNumber, newModifyVariables });
							}

							// Have algorithm jump to the first statement of the procedure it calls
							currentStatementNumber = currentStatementNextNumber;
							if (currStatementNextStmtsSize > 0) {
								statementNumbersAfterCalls.push(currStatementNextStmts->at(0)->getStatementNumber());
							}
							hasGotNext = true;
						}
					}

					if (!hasGotNext) {
						if (!statementNumbersAfterCalls.empty()) {
							currentStatementNumber = statementNumbersAfterCalls.top();
							statementNumbersAfterCalls.pop();
						}
						else {
							currentStatementNumber++;
						}
					}
				}
				else {
					// Prepare the modify variables of the current statement to be compared
					std::vector<int> modifyVariables = statementsAndModifyVariablesToCheck.at(currentStatementNumber);
					int modifyVariablesSize = modifyVariables.size();
					int currentStatementModifyVariablesSize = currentStatement->getModifiesSize();
					// Make a copy to modify and use for the further next nodes, if applicable
					std::vector<int> newModifyVariables;

					// Check if this statement modifies any modify variables
					for (int x = 0; x < modifyVariablesSize; x++) {
						bool shouldAdd = true;
						for (int y = 0; y < currentStatementModifyVariablesSize; y++) {
							if (modifyVariables[x] == currentStatement->getModifies(y)) {
								shouldAdd = false;
								y = currentStatementModifyVariablesSize;
							}
						}
						if (shouldAdd) {
							newModifyVariables.push_back(modifyVariables[x]);
						}
					}

					// NOTE: Uncomment the three lines below if variables modified by 
					// statements in a procedure can affect statements in another procedure
					/*
					for (int i = 0; i < currentStatementModifyVariablesSize; i++) {
						newModifyVariables.push_back(currentStatement->getModifies(i));
					}
					*/

					// Add information regarding the next node(s) to the data structures for our algorithm's processing
					int minimum = currentStatementNumber + 1;
					std::vector<StatementTableStatement*>* currentStatementNexts = currentStatement->getNext();
					if ((newModifyVariables.size() > 0) && (currentStatementNexts->size() > 0)) {
						StatementTableStatement* currentStatementNext = currentStatementNexts->at(0);
						int currentStatementNextNumber = currentStatementNext->getStatementNumber();
						statementNumbersAndStatements.insert({ currentStatementNextNumber, currentStatementNext });
						if (biggestStatementNumberThatWeKnowOf < currentStatementNextNumber) {
							biggestStatementNumberThatWeKnowOf = currentStatementNextNumber;
						}
						if (currentStatementNextNumber < minimum) {
							minimum = currentStatementNextNumber;
						}

						if (statementsAndModifyVariablesToCheck.count(currentStatementNextNumber) > 0) {
							// If information about the variables to be checked for this next statement exists,
							// update said information to include all variables
							std::set<int> newVariables;
							std::vector<int> vectorFromTable = statementsAndModifyVariablesToCheck.at(currentStatementNextNumber);
							int vectorFromTableSize = vectorFromTable.size();
							int newModifyVariablesSize = newModifyVariables.size();
							for (int i = 0; i < vectorFromTableSize; i++) {
								newVariables.insert(vectorFromTable[i]);
							}
							for (int i = 0; i < newModifyVariablesSize; i++) {
								newVariables.insert(newModifyVariables[i]);
							}
							std::vector<int> newVariablesToBeAdded;
							std::set<int>::iterator end = newVariables.end();
							for (std::set<int>::iterator it = newVariables.begin(); it != end; it++) {
								newVariablesToBeAdded.push_back(*it);
							}
							statementsAndModifyVariablesToCheck.at(currentStatementNextNumber) = newVariablesToBeAdded;
						}
						else {
							statementsAndModifyVariablesToCheck.insert({ currentStatementNextNumber, newModifyVariables });
						}
						currentStatementNumber = minimum;
					}
					else {
						currentStatementNumber = biggestStatementNumberThatWeKnowOf + 1;
					}
				}
			}
				break;
			}

			// Move on to the next statement in sequence of statement numbers
			if (statementNumbersAndStatements.count(currentStatementNumber) > 0) {
				currentStatement = statementNumbersAndStatements.at(currentStatementNumber);
			}
			else {
				bool hasFoundNext = false;
				for (int i = currentStatementNumber; i <= biggestStatementNumberThatWeKnowOf; i++) {
					if (statementNumbersAndStatements.count(i) > 0) {
						currentStatementNumber = i;
						currentStatement = statementNumbersAndStatements.at(currentStatementNumber);
						hasFoundNext = true;
						// Break out of for-loop
						i = biggestStatementNumberThatWeKnowOf + 1;
					}
				}
				if (!hasFoundNext) {
					currentStatement = NULL;
				}
			}
		}
	}

	return affectedByThisStar;
}