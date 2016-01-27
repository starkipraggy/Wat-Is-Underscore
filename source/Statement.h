/*! \class Statement
	\brief Used to represent a statement in the SIMPLE programming language.

	This class is a child class of the TNode class, for the purpose of being able to be added into
	an Abstract Syntax Tree (AST), and is generally used to represent a statement in the SIMPLE
	programming language. This class is also meant to be a superclass for child classes such as
	"assign" and "while"
*/

#pragma once

#include "TNode.h"

class Statement : public TNode {
private:
public:
	Statement();
	~Statement();
};