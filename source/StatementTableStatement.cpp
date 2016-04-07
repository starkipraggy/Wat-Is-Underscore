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

	calls = NULL;
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

StatementTableStatement** StatementTableStatement::getCalls() {
	return calls;
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

void StatementTableStatement::setCalls(StatementTableStatement** calls) {
	this->calls = calls;
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

std::vector<StatementTableStatement*>* StatementTableStatement::getAffectsThis() {
	if (affectsThis == NULL) {
		affectsThis = new std::vector<StatementTableStatement*>();
		if (getType() == Assign) {
			// Used by the algorithm to keep track of its progress
			std::set<int> statementNumbersOfStatementsAlreadyChecked;
			std::stack<StatementTableStatement*> statementsToCheck;
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
				previousStatementUseVariableIndexes.clear();

				// Make sure we check only assign statements
				if (currentStatementToCheck->getType() == Assign) {
					// Check its modify variables and see if there are any 
					// that matches the uses variables that this statement has
					for (int i = 0; i < thisStatementUseVariableIndexesSize; i++) {
						if (currentStatementToCheck->getModifies(i) == thisStatementUseVariableIndexes[i]) {
							affectsThis->push_back(currentStatementToCheck); // Add it into the list to be returned
						}
						else {
							// Variables not used by current statement can be continued to be checked in previous node(s)
							previousStatementUseVariableIndexes.push_back(thisStatementUseVariableIndexes[i]);
						}
					}
				}
				else {
					// We do not take any variables away if this is not an assign statement
					previousStatementUseVariableIndexes = thisStatementUseVariableIndexes;
				}

				// Continue up the CFG - make sure there is still at least one remaining use variables
				// that we have to check with before adding it into the statementsToCheck stack
				if (previousStatementUseVariableIndexes.size() > 0) {
					currentStatementPrevious = currentStatementToCheck->getPrevious();
					currentStatementPreviousSize = currentStatementPrevious->size();
					for (int i = 0; i < currentStatementPreviousSize; i++) {
						currentStatementPreviousSpecific = currentStatementPrevious->at(i);
						if (statementNumbersOfStatementsAlreadyChecked.count(currentStatementPreviousSpecific->getStatementNumber()) == 0) {
							statementsToCheck.push(currentStatementPreviousSpecific);
							statementsAndUseVariablesToCheck.insert({ currentStatementPreviousSpecific, previousStatementUseVariableIndexes });
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

			// Use to keep track of the current statement being processed by the algorithm
			StatementTableStatement* currentStatementToCheck;
			int currentStatementNumber;
			int currentStatementUsesSize;

			// Use to obtain the next node(s) of the current statement
			// being processed to be added into the statementsToCheck stack
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

				// If it does not modify, we continue down the CFG
				if ((currentStatementToCheck->getType() != Assign) || (currentStatementToCheck->getModifies(0) != variableIndex)) {
					currentStatementNext = currentStatementToCheck->getNext();
					currentStatementNextSize = currentStatementNext->size();
					for (int i = 0; i < currentStatementNextSize; i++) {
						StatementTableStatement* nextStatement = currentStatementNext->at(i);
						if (statementNumbersOfStatementsAlreadyChecked.count(nextStatement->getStatementNumber()) == 0) {
							statementsToCheck.push(nextStatement);
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
		// @todo Affects*(s,5)
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
								// Found a statement affected by this statement, add it into the list to be returned
								affectedByThisStar.push_back(currentStatement);
							}
							// Add in variables to affect the next statements
							newModifyVariables.push_back(currentStatement->getModifies(0));
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
						// @todo What if One calls Two which only calls Three? I cannot jump back more than one procedure currently
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
								int next1 = currentStatement->getNext()->at(0)->getStatementNumber();
								int next2 = currentStatement->getNext()->at(1)->getStatementNumber();
								if (next1 > next2) {
									currentStatementNumber = next1;
								}
								else {
									currentStatementNumber = next2;
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
							int next1 = currentStatement->getNext()->at(0)->getStatementNumber();
							int next2 = currentStatement->getNext()->at(1)->getStatementNumber();
							if (next1 < next2) {
								currentStatementNumber = next1;
							}
							else {
								currentStatementNumber = next2;
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
				StatementTableStatement** pointerToPointer = currentStatement->getCalls();
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
						if (currentStatement->getNext()->size() > 0) {
							statementNumbersAfterCalls.push(currentStatement->getNext()->at(0)->getStatementNumber());
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
				break;
			}

			// Move on to the next statement in sequence of statement numbers
			if (statementNumbersAndStatements.count(currentStatementNumber) > 0) {
				currentStatement = statementNumbersAndStatements.at(currentStatementNumber);
			}
			else {
				bool hasFoundNext = false;
				for (int runningOutOfVariableNamesForForLoops = currentStatementNumber;
						runningOutOfVariableNamesForForLoops <= biggestStatementNumberThatWeKnowOf;
						runningOutOfVariableNamesForForLoops++) {
					if (statementNumbersAndStatements.count(runningOutOfVariableNamesForForLoops) > 0) {
						currentStatementNumber = runningOutOfVariableNamesForForLoops;
						currentStatement = statementNumbersAndStatements.at(currentStatementNumber);
						hasFoundNext = true;
						// Break out of for-loop
						runningOutOfVariableNamesForForLoops = biggestStatementNumberThatWeKnowOf;
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