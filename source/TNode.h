/*! \class TNode
	\brief A node used for the Abstract Syntax Tree (AST).

	A node used for the Abstract Syntax Tree (AST). Generally used as a superclass for procedures, statements, etc.
*/

#pragma once
#include <vector>
#include <string>

#include "TypeDef.h"

enum TNodeType {
	Undefined,
	Assign,
	StmtLst,
	If,
	While,
	VariableName,
	IntegerValue,
	OperatorPlus,
	Call // For statements that call a procedure
};

class TNode
{
private:
	TNodeType type;  /**< Type of node: assign, stmtLst, if, while */
	int value;  /**< value of the node if integer*/
	char varName;  /**< name of the node if variable*/
	
	//std::string nodeType;	/**< Type of node: assign, stmtLst, if, while */
	int lineNumber;	/**< line number of the statement this node is representing */
	TNode* parentNode;	/**< the parent node of this node */
	std::vector<TNode* > childNodes;	/**< List of children of the node */
public:
	TNode();	/**< Constructor for empty TNode */
	~TNode(); 
	
	//! Constructor for the TNode
	/*!
	This is the constructor used to create a TNode object meant to be inserted into the AST.
	\param nodeType The type of statement that this TNode object is representing has.
	\param lineNumber The line number of the statement that this TNode object is representing.
	*/
	TNode(TNodeType type, int lineNumber);

	void setLineNumber(int lineNumber);	/**< setter for line number of node */
	int getLineNumber();	/**< getter for line number of node */
	void setNodeType(TNodeType type);	/**< setter for the type of the node */
	TNodeType getNodeType();	/**< getter for the type of the node */
	void setValue(int value);  /**< setter for the value of the node if integer */
	int getValue();  /**< getter for the value of the node if integer*/
	void setVariable(char varName);  /**< setter for the name of the node if variable */
	char getVariable();  /**< getter for the name of the node if variable*/
	void setParent(TNode* parent);	/**< setter for setting parent node of the TNode object */
	TNode* getParent();	/**< getter for getting parent node of the TNode object */
	
	//! Adding child node into the list of children for the node
	/*!
	This is used to add a child TNode object into the list of children of a node for determing parent and follows relationship.
	\param child The node that represents the child node.
	*/
	void addChild(TNode* child);

	std::vector<TNode* > getChildNodes();	/**< getter for getting child nodes list of the TNode object */
};