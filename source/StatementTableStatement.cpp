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

	follows = 0;
	followedBy = 0;
	parent = 0;
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

bool StatementTableStatement::hasParent() {
	return (parent != NULL);
}

int StatementTableStatement::getParent() {
	return parent->getStatementNumber();
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
			// Multiple uses variables here
			int numberOfUsesVariables = getUsesSize();

			std::set<int> statementNumbersOfWhilesAlreadyChecked;
			std::unordered_map<StatementTableStatement*, bool*> statementsAndVariables;
			std::queue<StatementTableStatement*> statementsToCheck;
			StatementTableStatement* currentStatementToCheck;
			bool* currentBooleans;
			StatementTableStatement* previousStatement1;
			StatementTableStatement* previousStatement2;
			statementsToCheck.push(this);
			currentBooleans = new bool[numberOfUsesVariables];
			for (int i = 0; i < numberOfUsesVariables; i++) {
				currentBooleans[i] = true;
			}
			statementsAndVariables.insert({this, currentBooleans});

			while (!statementsToCheck.empty()) {
				currentStatementToCheck = statementsToCheck.front();
				statementsToCheck.pop();
				currentBooleans = statementsAndVariables.at(currentStatementToCheck);

				// Make sure we check only assign statements, and don't check self - check their modify
				if ((currentStatementToCheck->getType() == Assign) && (currentStatementToCheck != this)) {
					for (int i = 0; i < numberOfUsesVariables; i++) {
						if ((currentBooleans[i]) && (currentStatementToCheck->getModifies(0) == getUses(i))) {
							affectsThis->push_back(currentStatementToCheck);
							i = numberOfUsesVariables; // Break out of for-loop
						}
					}
				}

				// Check what variables are we left with that can go up
				if ((currentStatementToCheck->getType() == Assign) && (currentStatementToCheck != this)) {
					for (int i = 0; i < numberOfUsesVariables; i++) {
						if (currentStatementToCheck->getModifies(0) == getUses(i)) {
							currentBooleans[i] = false;
						}
					}
				}

				// If assign statement, only able to continue up if at least one boolean is true
				bool canContinue = false;
				if (currentStatementToCheck->getType() == Assign) {
					for (int i = 0; i < numberOfUsesVariables; i++) {
						canContinue = canContinue || currentBooleans[i];
					}
				}
				else {
					canContinue = true;
				}

				// Continue up the CFG
				if (canContinue) {
					std::vector<StatementTableStatement*>* currentStatementPrevious = currentStatementToCheck->getPrevious();
					switch (currentStatementPrevious->size()) {
					case 1:
						previousStatement1 = currentStatementToCheck->getPrevious()->at(0);

						bool* newBooleans;
						if (statementsAndVariables.count(previousStatement1) == 1) {
							newBooleans = statementsAndVariables.at(previousStatement1);
							for (int i = 0; i < numberOfUsesVariables; i++) {
								newBooleans[i] = newBooleans[i] || currentBooleans[i];
							}
						}
						else {
							newBooleans = new bool[numberOfUsesVariables];
							for (int i = 0; i < numberOfUsesVariables; i++) {
								newBooleans[i] = currentBooleans[i];
							}
							statementsAndVariables.insert({ previousStatement1 , newBooleans });
						}
						statementsToCheck.push(previousStatement1);
						break;
					case 2:
						previousStatement1 = currentStatementToCheck->getPrevious()->at(0);
						previousStatement2 = currentStatementToCheck->getPrevious()->at(1);

						// if this is a while-statement, add the cyclic node into the queue first!
						if (currentStatementToCheck->getType() == While) {
							int currentChildrenSize = currentStatementToCheck->getChildrenSize();
							for (int i = 0; i < currentChildrenSize; i++) {
								int previousStatement2StatementNumber = previousStatement2->getStatementNumber();
								if (previousStatement2StatementNumber == currentStatementToCheck->getChildren(i)) {
									// This is the cyclic node, make sure we add it into the queue first
									StatementTableStatement* temporaryForSwapping = previousStatement2;
									previousStatement2 = previousStatement1;
									previousStatement1 = temporaryForSwapping;
									i = currentChildrenSize; // Break out of loop safely
								}
							}
						}

						for (int x = 0; x < 2; x++) {
							StatementTableStatement* tempStatement = (x == 0) ? previousStatement1 : previousStatement2;
							bool* newBooleans;
							if (statementsAndVariables.count(tempStatement) == 1) {
								newBooleans = statementsAndVariables.at(tempStatement);
								for (int i = 0; i < numberOfUsesVariables; i++) {
									newBooleans[i] = newBooleans[i] || currentBooleans[i];
								}
							}
							else {
								newBooleans = new bool[numberOfUsesVariables];
								for (int i = 0; i < numberOfUsesVariables; i++) {
									newBooleans[i] = currentBooleans[i];
								}
								statementsAndVariables.insert({ tempStatement , newBooleans });
							}
							statementsToCheck.push(tempStatement);
						}
						break;
					}
				}
			}
			// According to http://www.cplusplus.com/reference/unordered_map/unordered_map/clear/,
			// the destructors of the elements inside the container are called, so I assume
			// I don't have to do so manually. Let me know otherwise
			statementsAndVariables.clear();
		}
	}
	return affectsThis;
}

std::vector<StatementTableStatement*>* StatementTableStatement::getAffectedByThis() {
	if (affectedByThis == NULL) {
		affectedByThis = new std::vector<StatementTableStatement*>();
		if (getType() == Assign) {
			std::set<int> statementNumbersOfStatementsAlreadyChecked;
			std::stack<StatementTableStatement*> statementsToCheck;
			StatementTableStatement* currentStatementToCheck;
			int currentStatementNumber;
			int currentStatementUsesSize;
			std::vector<StatementTableStatement*>* currentStatementNext;
			int currentStatementNextSize;
			statementsToCheck.push(getNext()->at(0)); // Push the next statement; assign statements only have one Next

			int variableIndex = getModifies(0);

			while (!statementsToCheck.empty()) {
				currentStatementToCheck = statementsToCheck.top();
				statementsToCheck.pop();
				currentStatementNumber = currentStatementToCheck->getStatementNumber();

				// Make sure we check only assign statements
				if (currentStatementToCheck->getType() == Assign) {
					// This statement is not checked; check it
					if (statementNumbersOfStatementsAlreadyChecked.count(currentStatementNumber) == 0) {
						currentStatementUsesSize = currentStatementToCheck->getUsesSize();
						for (int i = 0; i < currentStatementUsesSize; i++) {
							if (variableIndex == currentStatementToCheck->getUses(i)) {
								affectedByThis->push_back(currentStatementToCheck);
								i = currentStatementUsesSize; // Break out of for-loop
							}
						}
					}
				}

				// If it does not modify, we continue down the CFG
				if ((currentStatementToCheck->getType() != Assign) || (currentStatementToCheck->getModifies(0) != variableIndex)) {
					currentStatementNext = currentStatementToCheck->getNext();
					currentStatementNextSize = currentStatementNext->size();
					for (int i = 0; i < currentStatementNextSize; i++) {
						statementsToCheck.push(currentStatementNext->at(i));
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
	std::set<StatementTableStatement*> statementsChecked;
	std::stack<StatementTableStatement*> statementsToCheck;
	statementsToCheck.push(this);
	StatementTableStatement* currentStatementToCheck;
	std::vector<StatementTableStatement*>* currentStatementAffectsThis;
	int currentStatementAffectsThisSize;
	StatementTableStatement* currentCurrentStatementAffectsThis;

	while (!statementsToCheck.empty()) {
		currentStatementToCheck = statementsToCheck.top();
		statementsToCheck.pop();

		if (statementsChecked.count(currentStatementToCheck) == 0) {
			currentStatementAffectsThis = currentStatementToCheck->getAffectsThis();
			currentStatementAffectsThisSize = currentStatementAffectsThis->size();
			for (int i = 0; i < currentStatementAffectsThisSize; i++) {
				currentCurrentStatementAffectsThis = currentStatementAffectsThis->at(i);
				affectsThisStar.push_back(currentCurrentStatementAffectsThis);
				statementsToCheck.push(currentCurrentStatementAffectsThis);
			}
			statementsChecked.insert(currentStatementToCheck);
		}
	}
	return affectsThisStar;
}

std::vector<StatementTableStatement*> StatementTableStatement::getAffectedByThisStar() {
	std::vector<StatementTableStatement*> affectedByThisStar;
	std::set<StatementTableStatement*> statementsChecked;
	std::stack<StatementTableStatement*> statementsToCheck;
	statementsToCheck.push(this);
	StatementTableStatement* currentStatementToCheck;
	std::vector<StatementTableStatement*>* currentStatementAffectedByThis;
	int currentStatementAffectedByThisSize;
	StatementTableStatement* currentCurrentStatementAffectedByThis;

	while (!statementsToCheck.empty()) {
		currentStatementToCheck = statementsToCheck.top();
		statementsToCheck.pop();

		if (statementsChecked.count(currentStatementToCheck) == 0) {
			currentStatementAffectedByThis = currentStatementToCheck->getAffectedByThis();
			currentStatementAffectedByThisSize = currentStatementAffectedByThis->size();
			for (int i = 0; i < currentStatementAffectedByThisSize; i++) {
				currentCurrentStatementAffectedByThis = currentStatementAffectedByThis->at(i);
				affectedByThisStar.push_back(currentCurrentStatementAffectedByThis);
				statementsToCheck.push(currentCurrentStatementAffectedByThis);
			}
			statementsChecked.insert(currentStatementToCheck);
		}
	}

	return affectedByThisStar;
}