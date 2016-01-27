/*! \class Expression
	\brief Used to represent an expression in the SIMPLE programming language.

	This class is a child class of the TNode class, for the purpose of being able to be added into
	an Abstract Syntax Tree (AST), and is generally used to represent an expression in the SIMPLE
	programming language. An expression consists of either simply a factor (which is the value of
	either a variable or a value of a constant), or another expression, an operator, and a factor.
*/

#pragma once

#include "Factor.h"

class Expression : public TNode {
private:
	bool isSimplyAFactor;
	Expression* expr;		/*!< Would not be taken into consideration if isSimplyAFactor be TRUE
								 and thus should simply be set to NULL should such conditions be met */
	OPERATOR op;			/*!< Only '+' available for now in Iteration 1.
								 Would not be taken into consideration if isSimplyAFactor be TRUE
								 and thus should simply be set to NULL should such conditions be met */
	Factor* factor;
public:
	Expression();
	~Expression();
};