#include "StatementTableStatement.h"

StatementTableStatement::StatementTableStatement(int lineNumber, int index) {
	this->lineNumber = lineNumber;
	this->index = index;
	type = Undefined;

	follows = 0;
	followedBy = 0;
	parent = 0;
	children = new std::vector<int>();
	modifies = new std::vector<int>();
	uses = new std::vector<int>();

	parentStar = new std::vector<int>();
	followsStar = new std::vector<int>();
}

StatementTableStatement::~StatementTableStatement() {
	delete children;
	delete modifies;
	delete uses;

	delete parentStar;
	delete followsStar;
}

int StatementTableStatement::getIndex() {
	return index;
}

bool StatementTableStatement::hasParent() {
	return (parent > 0);
}

bool StatementTableStatement::hasFollows() {
	return (follows > 0);
}

int StatementTableStatement::getParent() {
	return parent->getIndex();
}

void StatementTableStatement::setType(TNodeType type) {
	this->type = type;
}

void StatementTableStatement::setFollows(StatementTableStatement* follows) {
	this->follows = follows;

	followsStar->clear();
	StatementTableStatement* stmt = this;
	while (stmt->hasFollows()) {
		stmt = stmt->follows;
		followsStar->push_back(stmt->getIndex());
	}
}

void StatementTableStatement::setFollowedBy(int followedBy) {
	this->followedBy = followedBy;
}

void StatementTableStatement::setParent(StatementTableStatement* parent) {
	this->parent = parent;

	parentStar->clear();
	StatementTableStatement* stmt = this;
	while (stmt->hasParent()) {
		stmt = stmt->parent;
		parentStar->push_back(stmt->getIndex());
	}
}

void StatementTableStatement::addChild(int child) {
	children->push_back(child);
}

bool StatementTableStatement::addModifies(int variableIndexNumber) {
	return addIntoVector(variableIndexNumber, modifies);
}

bool StatementTableStatement::addUses(int variableIndexNumber) {
	return addIntoVector(variableIndexNumber, uses);
}

int StatementTableStatement::getFollows() {
	return follows->getIndex();
}

int StatementTableStatement::getFollowedBy() {
	return followedBy;
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
	return uses->at(index);
}

int StatementTableStatement::getModifiesSize() {
	return uses->size();
}

int StatementTableStatement::getFollowsStar(int index) {
	return followsStar->at(index);
}

int StatementTableStatement::getFollowsStarSize() {
	return followsStar->size();
}

/*int StatementTableStatement::getFollowedByStar(int index) {
	followsStar->at(index);
}

int StatementTableStatement::getFollowedByStarSize() {
	return followsStar->size();
}*/

int StatementTableStatement::getChildren(int index) {
	return children->at(index);
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

/*int StatementTableStatement::getChildrenStar(int index) {
	childrenStar->at(index);
}

int StatementTableStatement::getChildrenStarSize() {
	return childrenStar->size();
}*/