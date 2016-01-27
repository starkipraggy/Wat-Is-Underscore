/*! \class While
	\brief Used to represent a while-loop in the SIMPLE programming language.

	This class is a child class of the Statement class, for the purpose of being able to be added
	into the list of statements container in the StatementList class. In addition, it is also
	a descendant of the TNode class, and thus is able to be added into an Abstract Syntax Tree (AST).
	Generally, it is used to represent a while-loop in the SIMPLE programming language. It contains
	the name of a variable which value we will check to determine if the loop continues or terminates,
	as well as a list of statements for the while-loop to execute while continuing.
*/

#pragma once

#include "StatementList.h"

class While : public Statement {
private:
	NAME var_name;
	StatementList* stmtLst;
public:
	While();
	~While();
};