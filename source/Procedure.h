/*! \class Procedure
	\brief Used to represent a procedure in the SIMPLE programming language.

	This class is a child class of the TNode class, for the purpose of being able to be added into
	an Abstract Syntax Tree (AST), and is generally used to represent a procedure in the SIMPLE
	programming language.
*/

#pragma once

#include "StatementList.h"

class Procedure : public TNode {
private:
	NAME proc_name;
	StatementList* stmtLst;
public:
	Procedure();
	~Procedure();
};