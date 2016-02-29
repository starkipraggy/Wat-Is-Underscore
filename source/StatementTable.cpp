#include <iostream>
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

StatementTableStatement* StatementTable::getStatementUsingStatementNumber(int statementNumber) {

	int size = getNumberOfStatements();
	StatementTableStatement* statement;
	for (int i = 0; i < size; i++) {
		statement = statements->at(i);
		if (statement->getStatementNumber() == statementNumber) {
			return statement;
		}
	}
	return addStatement(statementNumber);
}

StatementTableStatement* StatementTable::getStatementUsingVectorIndexNumber(int vectorIndexNumber) {
	if (vectorIndexNumber < 0) {
		return NULL;
	}
	if (vectorIndexNumber >= getNumberOfStatements()) {
		return NULL;
	}
	return statements->at(vectorIndexNumber);
}

StatementTableStatement* StatementTable::addStatement(int lineNumber) {
	StatementTableStatement* newStatement = new StatementTableStatement(lineNumber);
	statements->push_back(newStatement);
	return newStatement;
}