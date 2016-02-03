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