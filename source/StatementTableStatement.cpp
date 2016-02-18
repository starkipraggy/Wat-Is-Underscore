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
}

StatementTableStatement::~StatementTableStatement() {
	delete children;
	delete modifies;
	delete uses;
}

bool StatementTableStatement::hasParent() {
	return (parent > 0);
}

int StatementTableStatement::getParent() {
	return parent;
}

void StatementTableStatement::setType(TNodeType type) {
	this->type = type;
}

void StatementTableStatement::setFollows(int follows) {
	this->follows = follows;
}

void StatementTableStatement::setFollowedBy(int followedBy) {
	this->followedBy = followedBy;
}

void StatementTableStatement::setParent(int parent) {
	this->parent = parent;
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
	return follows;
}

TNodeType StatementTableStatement::getType() {
	return type;
}