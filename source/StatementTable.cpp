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

StatementTableStatement* StatementTable::getStatement(int statementNumber) {
	int size = getNumberOfStatements();
	StatementTableStatement* statement;
	for (int i = 0; i < size; i++) {
		statement = statements->at(i);
		if (statement->getIndex() == statementNumber) {
			return statement;
		}
	}
	return NULL;
}

StatementTableStatement* StatementTable::addStatement(int lineNumber) {
	StatementTableStatement* newStatement = new StatementTableStatement(lineNumber, getNumberOfStatements());
	statements->push_back(newStatement);
	return newStatement;
}