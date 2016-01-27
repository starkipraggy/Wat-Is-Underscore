/*! \class StatementList
	\brief Used to represent a list of statements in the SIMPLE programming language.

	This class is a child class of the TNode class, for the purpose of being able to be added into
	an Abstract Syntax Tree (AST), and is generally used to represent a list of statements in the
	SIMPLE programming language. The statements is stored using the std::vector container.
*/

#pragma once

#include "Statement.h"
#include <vector>

class StatementList : public TNode {
private:
	std::vector<Statement*> stmtList;
public:
	StatementList();
	~StatementList();
};