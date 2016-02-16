#include "StatementTableStatement.h"

StatementTableStatement::StatementTableStatement(int lineNumber, int index) {
	this->lineNumber = lineNumber;
	this->index = index;
	follows = 0;
	parent = 0;
	modifies = new std::vector<int>();
	uses = new std::vector<int>();
}

StatementTableStatement::~StatementTableStatement() {
	delete modifies;
	delete uses;
}

int StatementTableStatement::getIndex() {
	return index;
}

bool StatementTableStatement::addModifies(int variableIndexNumber) {
	return addIntoVector(variableIndexNumber, modifies);
}

bool StatementTableStatement::addUses(int variableIndexNumber) {
	return addIntoVector(variableIndexNumber, uses);
}