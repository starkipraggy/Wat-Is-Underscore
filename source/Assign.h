/*! \class Assign
	\brief Used to represent an assignment in the SIMPLE programming language.

	This class is a child class of the Statement class, for the purpose of being able to be added
	into the list of statements container in the StatementList class. In addition, it is also
	a descendant of the TNode class, and thus is able to be added into an Abstract Syntax Tree (AST).
	Generally, it is used to represent an assignment in the SIMPLE programming language. It contains
	the name of the variable which is having a value assigned to it, as well as the expression that
	generally equates to the value that we are trying to assign to the variable.
*/

#pragma once

#include "Statement.h"
#include "Expression.h"

class Assign : public Statement {
private:
	NAME var_name;
	Expression* expr;
public:
	Assign();
	~Assign();
};