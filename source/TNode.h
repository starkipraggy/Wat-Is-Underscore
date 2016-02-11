/*! \class TNode
	\brief A node used for the Abstract Syntax Tree (AST).

	A node used for the Abstract Syntax Tree (AST). Generally used as a superclass for procedures, statements, etc.
*/

#pragma once

#include "TypeDef.h"

class TNode
{
private:
	std::string nodeType;
	int lineNumber;
public:
	TNode();
	~TNode();
	TNode(std::string nodeType, int lineNumber);
	void setLineNumber(int lineNumber);
	void setNodeType(std::string nodeType);
	int getLineNumber();
	std::string getNodeType();
};