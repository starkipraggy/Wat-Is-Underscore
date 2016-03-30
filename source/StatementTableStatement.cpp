#include <iostream>
#include <stack>
#include "StatementTableStatement.h"

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
	next = NULL;
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
	if (next != NULL) { delete next; }
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

// @note for Wilson
/* For the two functions that comes after this, named getPrevious() and getNext(),
   you will find a if-statement with an empty body, other than a // @todo note.
   Please kindly initialize the respective std::vector<int>*, previous and next,
   inside the body of those if-statements. Likewise, if you find that a
   std::vector<int>* may not suit your requirements (eg you may find a
   std::set<int>* better, or a std::vector<StatementTableStatement*>* better),
   feel free to change it as you like. Let me and Alan know immediately if you
   have any questions. Leaving this to you, thanks a lot! ;D

   PS: If you need access to your CFG object, may I suggest a static CFG* pointer
   in this StatementTableStatement class, passed in during the creation of this
   object? Either that or make your CFG a Singleton, Iunno, up to you to implement lols. */

std::vector<int>* StatementTableStatement::getPrevious() {
	if (previous == NULL) {
		// @todo
	}
	return previous;
}

std::vector<int>* StatementTableStatement::getNext() {
	if (next != NULL) {
		// @todo
	}
	return next;
}

// @note for Wilson
/* For the following two functions that come after this, getPreviousStar() and
   getNextStar(), we are not allowed to store them in a table, even after computing
   their contents (retarded, I know, but whatever rofl). Right now, we have two ways
   of implementing this:
   1. Make use of the CFG, traverse through the entire thing until you reach the end
   2. Use the Previous and Next tables respectively of other StatementTableStatement
      objects recursively
   If you'd like to do this the second way then maybe we'd need to change our container
   to store StatementTableStatement* pointers instead of just statement number
   integers, for ease of recursion (since we don't have access to where those objects
   are if we only store their statement numbers), so it seems to me like the first
   way may be more intuitive, but which, of course, requires more effort on your part,
   as you are the one who came up with the CFG. Anyway, I leave it fully up to you
   how you would like to implement it. Again, let me and Alan know if there is any
   questions you have or things you need, etc. Thanks a lot!

   Yours sincerely, Wei Liang

   P.S.: Sorry for the tl;dr
   P.P.S.: Go fix the damn signed/unsigned mismatch warnings in your AST.cpp, lines
		   210 and 244 LOL. vector.size() returns an unsigned integer, so you'd need
		   another unsigned integer to do comparison with
*/

std::vector<int>* StatementTableStatement::getPreviousStar() {
	// @todo
	return NULL;
}

std::vector<int>* StatementTableStatement::getNextStar() {
	// @todo
	return NULL;
}

std::vector<int>* StatementTableStatement::getAffectsThis() {
	if (affectsThis != NULL) {
		// @todo by Wei Liang
	}
	return affectsThis;
}

std::vector<int>* StatementTableStatement::getAffectedByThis() {
	if (affectedByThis != NULL) {
		// @todo by Wei Liang
	}
	return affectedByThis;
}