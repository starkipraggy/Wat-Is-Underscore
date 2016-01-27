/*! \class Factor
	\brief Used to represent a factor in the SIMPLE programming language.

	This class is a child class of the TNode class, for the purpose of being able to be added into
	an Abstract Syntax Tree (AST), and is generally used to represent a factor in the SIMPLE
	programming language. Factors are either names of variables, or values of a constant,
	and are used in expressions.
*/

#pragma once

#include "TNode.h"

class Factor : public TNode {
private:
	bool isFactorAVariable;		/*!< This factor is a variable if this boolean is TRUE, and is
									 a constant value if boolean is FALSE. This thus affects
									 which of its variables are used. May want to split
									 the variable and constant parts apart by creating new
									 classes for each of them and having them inherit this
									 class itself as a parent class. */
	NAME var_name;				/*!< This variable is only used if the isFactorAVariable
									 boolean is set to true. If not, it will be disregarded. */
	INTEGER const_value;		/*!< This variable is only used if the isFactorAVariable
									 boolean is set to false. If not, it will be disregarded. */
public:
	Factor();
	~Factor();
};