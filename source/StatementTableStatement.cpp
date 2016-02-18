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

std::vector<int>* StatementTableStatement::getUses() {
	return uses;
}

std::vector<int>* StatementTableStatement::getModifies() {
	return modifies;
}

int StatementTableStatement::getFollows() {
	return follows->getIndex();
}

TNodeType StatementTableStatement::getType() {
	return type;
}