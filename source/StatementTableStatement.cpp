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
		std::stack<int> statementNumbersAfterCalls;
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
				std::vector<int> existingVariables = modifyVariables;

				// Check if this statement modifies any modify variables
				std::vector<int> newModifyVariables;
				int existingVariablesSize = existingVariables.size();
				for (int x = 0; x < existingVariablesSize; x++) {
					if (existingVariables[x] != currentStatement->getModifies(0)) {
						newModifyVariables.push_back(existingVariables[x]);
					}
				}

				// Check if this statement uses any modify variables
				for (int x = 0; x < modifyVariablesSize; x++) {
					for (int y = 0; y < useVariablesSize; y++) {
						if (modifyVariables[x] == currentStatement->getUses(y)) {
							if (getProcedureIndexNumber() == currentStatement->getProcedureIndexNumber()) {
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
						std::vector<int> vectorFromTable = statementsAndModifyVariablesToCheck.at(currentStatementNext);
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
						statementsAndModifyVariablesToCheck.at(currentStatementNext) = newVariablesToBeAdded;
					}
					else {
						statementsAndModifyVariablesToCheck.insert({ currentStatementNext, newModifyVariables });
					}
					currentStatementNumber = minimum;
				}
				else {
					if (!statementNumbersAfterCalls.empty()) {
						currentStatementNumber = statementNumbersAfterCalls.top();
						statementNumbersAfterCalls.pop();
					}
					else {
						StatementTableStatement* nextStatement = statementNumbersAndStatements.at(currentStatementNumber + 1);
						if (nextStatement->getProcedureIndexNumber() == currentStatement->getProcedureIndexNumber()) {
							currentStatementNumber++;
						}
						else {
							currentStatementNumber = biggestStatementNumberThatWeKnowOf + 1; // Time to shut this down!
						}
					}
				}
			}
				break;
			case If:
			case While:
			{
				if (currentStatement->getType() == While) {
					if (whileStatementsAndHistoryOfModifiesVariables.count(currentStatement) == 0) {
						std::vector<std::vector<int>> vectorOfVectors;
						vectorOfVectors.push_back(statementsAndModifyVariablesToCheck.at(currentStatement));
						whileStatementsAndHistoryOfModifiesVariables.insert({ currentStatement, vectorOfVectors });
						currentStatementNumber++;
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
								vectorOfVectors.push_back(latestVector);
								whileStatementsAndHistoryOfModifiesVariables.at(currentStatement) = vectorOfVectors;
								int next1 = currentStatement->getNext()->at(0)->getStatementNumber();
								int next2 = currentStatement->getNext()->at(1)->getStatementNumber();
								if (next1 < next2) {
									currentStatementNumber = next1;
								}
								else {
									currentStatementNumber = next2;
								}
								hasPatternRepeat = true;
							}
						}
					}
				}
				else {
					currentStatementNumber++;
				}

				std::vector<StatementTableStatement*>* currentStatementNexts = currentStatement->getNext();
				int currentStatementNextsSize = currentStatementNexts->size();
				for (int i = 0; i < currentStatementNextsSize; i++) {
					StatementTableStatement* currentStatementNext = currentStatementNexts->at(i);
					int currentStatementNextNumber = currentStatementNext->getStatementNumber();
					statementNumbersAndStatements.insert({ currentStatementNextNumber, currentStatementNext });
					if (biggestStatementNumberThatWeKnowOf < currentStatementNextNumber) {
						biggestStatementNumberThatWeKnowOf = currentStatementNextNumber;
					}
					currentStatementNextNumber++;

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
						statementsAndModifyVariablesToCheck.at(currentStatementNext) = newVariablesToBeAdded;
					}
					else {
						statementsAndModifyVariablesToCheck.insert({ currentStatementNext, newModifyVariables });
					}
				}
			}
				break;
			default:
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

					std::vector<int> newModifyVariables = statementsAndModifyVariablesToCheck.at(currentStatement);
					if (statementsAndModifyVariablesToCheck.count(currStatementNextStmt) > 0) {
						std::set<int> newVariables;
						std::vector<int> existingVariables = statementsAndModifyVariablesToCheck.at(currStatementNextStmt);
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
						statementsAndModifyVariablesToCheck.insert({ currStatementNextStmt, newVariablesToBeAdded });
					}
					else {
						statementsAndModifyVariablesToCheck.insert({ currStatementNextStmt, newModifyVariables });
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