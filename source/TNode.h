/*! \class TNode
	\brief A node used for the Abstract Syntax Tree (AST).

	A node used for the Abstract Syntax Tree (AST). Generally used as a superclass for procedures, statements, etc.
*/

#pragma once
#include <vector>
#include <string>

#include "TypeDef.h"

class TNode
{
private:
	std::string nodeType;
	int lineNumber;
	TNode* parentNode;
	std::vector<TNode* > childNodes;
public:
	TNode();
	~TNode(); 
	TNode(std::string nodeType, int lineNumber);
	void setLineNumber(int lineNumber);
	void setNodeType(std::string nodeType);
	int getLineNumber();
	std::string getNodeType();
	void setParent(TNode* parent);
	TNode* getParent();
	void addChild(TNode* child);
	std::vector<TNode* > getChildNodes();
};