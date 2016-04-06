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
			// @todo Affects(s,5)
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
			if (getNext()->size() > 0) {
				statementsToCheck.push(getNext()->at(0)); // Push the next statement; assign statements only have one Next
			}
			int variableIndex = getModifies(0);

			while (!statementsToCheck.empty()) {
				currentStatementToCheck = statementsToCheck.top();
				statementsToCheck.pop();
				currentStatementNumber = currentStatementToCheck->getStatementNumber();

				// Make sure we check only assign statements
				if (currentStatementToCheck->getType() == Assign) {
					currentStatementUsesSize = currentStatementToCheck->getUsesSize();
					for (int i = 0; i < currentStatementUsesSize; i++) {
						if (variableIndex == currentStatementToCheck->getUses(i)) {
							affectedByThis->push_back(currentStatementToCheck);
							i = currentStatementUsesSize; // Break out of for-loop
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

	// @todo Affects*(5,s)
	if (getType() == Assign) {
		int currentStatementNumber = -1;
		int biggestStatementNumberThatWeKnowOf = -1;
		std::unordered_map<int, StatementTableStatement*> statementNumbersAndStatements;
		std::unordered_map<StatementTableStatement*, std::vector<int>> statementsAndModifyVariablesToCheck;
		std::unordered_map<StatementTableStatement*, std::vector<std::vector<int>>> whileStatementsAndHistoryOfModifiesVariables;
		StatementTableStatement* currentStatement = NULL;
		if (getNext()->size() > 0) {
			currentStatement = getNext()->at(0);
			std::vector<int> modifyVariables;
			modifyVariables.push_back(getModifies(0));
			currentStatementNumber = currentStatement->getStatementNumber();
			statementNumbersAndStatements.insert({ currentStatementNumber, currentStatement });
			biggestStatementNumberThatWeKnowOf = currentStatementNumber;
			statementsAndModifyVariablesToCheck.insert({ currentStatement, modifyVariables });
		}

		while (currentStatement != NULL) {
			switch (currentStatement->getType()) {
			case Assign:
			{
				std::vector<int> modifyVariables = statementsAndModifyVariablesToCheck.at(currentStatement);
				int modifyVariablesSize = modifyVariables.size();
				int useVariablesSize = currentStatement->getUsesSize();

				// Check if this statement uses any modify variables
				for (int x = 0; x < modifyVariablesSize; x++) {
					for (int y = 0; y < useVariablesSize; y++) {
						if (modifyVariables[x] == currentStatement->getUses(y)) {
							affectedByThisStar.push_back(currentStatement);
							// Break out of both loops
							x = modifyVariablesSize;
							y = useVariablesSize;
						}
					}
				}

				// Check if this statement modifies any modify variables
				std::vector<int> newModifyVariables;
				for (int x = 0; x < modifyVariablesSize; x++) {
					if (modifyVariables[x] != currentStatement->getModifies(0)) {
						newModifyVariables.push_back(modifyVariables[x]);
					}
				}

				// Add next information to the information we know
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

					if (statementsAndModifyVariablesToCheck.count(currentStatementNext) > 0) {
						std::set<int> newVariables;
						std::vector<int> existingVariables = statementsAndModifyVariablesToCheck.at(currentStatementNext);
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
						statementsAndModifyVariablesToCheck.insert({ currentStatementNext, newVariablesToBeAdded });
					}
					else {
						statementsAndModifyVariablesToCheck.insert({ currentStatementNext, newModifyVariables });
					}
				}
				currentStatementNumber = minimum;
			}
				break;
			case If:
			case While:
			{
				int minimum = currentStatementNumber + 1;
				std::vector<StatementTableStatement*>* currentStatementNexts = currentStatement->getNext();
				for (int i = 0; i < 2; i++) {
					StatementTableStatement* currentStatementNext = currentStatementNexts->at(i);
					int currentStatementNextNumber = currentStatementNext->getStatementNumber();
					statementNumbersAndStatements.insert({ currentStatementNextNumber, currentStatementNext });
					if (biggestStatementNumberThatWeKnowOf < currentStatementNextNumber) {
						biggestStatementNumberThatWeKnowOf = currentStatementNextNumber;
					}
					if (currentStatementNextNumber < minimum) {
						minimum = currentStatementNextNumber;
					}

					std::vector<int> newModifyVariables = statementsAndModifyVariablesToCheck.at(currentStatement);
					if (statementsAndModifyVariablesToCheck.count(currentStatementNext) > 0) {
						std::set<int> newVariables;
						std::vector<int> existingVariables = statementsAndModifyVariablesToCheck.at(currentStatementNext);
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
						statementsAndModifyVariablesToCheck.insert({ currentStatementNext, newVariablesToBeAdded });
					}
					else {
						statementsAndModifyVariablesToCheck.insert({ currentStatementNext, newModifyVariables });
					}
				}

				if (currentStatement->getType() == While) {
					if (whileStatementsAndHistoryOfModifiesVariables.count(currentStatement) == 0) {
						std::vector<std::vector<int>> vectorOfVectors;
						vectorOfVectors.push_back(statementsAndModifyVariablesToCheck.at(currentStatement));
						whileStatementsAndHistoryOfModifiesVariables.insert({ currentStatement, vectorOfVectors });
						currentStatementNumber = minimum;
					}
					else {
						bool hasPatternRepeat = true;
						std::vector<std::vector<int>> vectorOfVectors = whileStatementsAndHistoryOfModifiesVariables.at(currentStatement);
						int vectorOfVectorsSize = vectorOfVectors.size();
						std::vector<int> latestVector = statementsAndModifyVariablesToCheck.at(currentStatement);
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

							if (hasPatternRepeat) {
								int next1 = currentStatement->getNext()->at(0)->getStatementNumber();
								int next2 = currentStatement->getNext()->at(1)->getStatementNumber();
								if (next1 > next2) {
									currentStatementNumber = next1;
								}
								else {
									currentStatementNumber = next2;
								}
								x = vectorOfVectorsSize; // Break out of for-loop
							}
							else {
								hasPatternRepeat = true;
							}
						}
					}
				}
				else {
					currentStatementNumber = minimum;
				}
			}
				break;
			default:
			{
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

					std::vector<int> newModifyVariables = statementsAndModifyVariablesToCheck.at(currentStatement);
					if (statementsAndModifyVariablesToCheck.count(currentStatementNext) > 0) {
						std::set<int> newVariables;
						std::vector<int> existingVariables = statementsAndModifyVariablesToCheck.at(currentStatementNext);
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
						statementsAndModifyVariablesToCheck.insert({ currentStatementNext, newVariablesToBeAdded });
					}
					else {
						statementsAndModifyVariablesToCheck.insert({ currentStatementNext, newModifyVariables });
					}
				}
				currentStatementNumber = minimum;
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