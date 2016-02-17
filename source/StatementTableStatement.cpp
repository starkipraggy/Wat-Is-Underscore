#include "StatementTableStatement.h"

StatementTableStatement::StatementTableStatement(int lineNumber, int index) {
	this->lineNumber = lineNumber;
	this->index = index;
	type = Undefined;

	follows = 0;
	parent = 0;
	modifies = new std::vector<int>();
	uses = new std::vector<int>();
}

StatementTableStatement::~StatementTableStatement() {
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

void StatementTableStatement::setParent(int parent) {
	this->parent = parent;
}

bool StatementTableStatement::addModifies(int variableIndexNumber) {
	return addIntoVector(variableIndexNumber, modifies);
}

bool StatementTableStatement::addUses(int variableIndexNumber) {
	return addIntoVector(variableIndexNumber, uses);
}