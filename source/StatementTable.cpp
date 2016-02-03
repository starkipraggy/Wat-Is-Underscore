#include "StatementTable.h"

StatementTable::StatementTable() {
	statements = new std::vector<StatementTableStatement*>();
}

StatementTable::~StatementTable() {
	int numberOfStatements = getNumberOfStatements();
	for (int i = 0; i < numberOfStatements; i++) {
		delete statements->at(i);
		statements->at(i) = NULL;
	}
	delete statements;
}

int StatementTable::getNumberOfStatements() {
	return statements->size();
}

void StatementTable::addStatement(int lineNumber) {
	StatementTableStatement* newStatement = new StatementTableStatement(lineNumber, getNumberOfStatements());
	statements->push_back(newStatement);
}