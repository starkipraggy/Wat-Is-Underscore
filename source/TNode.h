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
    ProcedureName,
	Assign,
	StmtLst,
	If,
	While,
	VariableName,
	IntegerValue,
	OperatorPlus,
    OperatorMinus,
    OperatorTimes,
    OperatorEquals,
    LeftParenthesis,
    RightParenthesis,
	Call, // For statements that call a procedure
	//Stmt, // Guys, why is Stmt here? If, While, Assign and Call can be a Stmt
	Const,
	//Prog_line // Guys, why is Stmt here? If, While, Assign and Call can be a Stmt
};

class TNode
{
private:
	TNodeType type;  /**< Type of node: assign, stmtLst, if, while */
    std::string value;  /**< value of the node. It should make sense with respect to the type of the node.*/
    int lineNumber;	/**< line number of the statement this node is representing */
	TNode* parentNode;	/**< the parent node of this node */
	std::vector<TNode* > childNodes;	/**< List of children of the node */
public:
	TNode();	/**< Constructor for empty TNode */
	~TNode(); 
	
	//! Constructor for the TNode with type, value and line number
	/*!
	This is the constructor used to create a TNode object meant to be inserted into the AST.
	\param type The type of statement that this TNode object is representing has.
	\param lineNumber The line number of the statement that this TNode object is representing.
	*/
	TNode(TNodeType type, std::string initValue="", int lineNumber=0);

	void setLineNumber(int lineNumber);	/**< setter for line number of node */
	int getLineNumber();	/**< getter for line number of node */
	void setNodeType(TNodeType type);	/**< setter for the type of the node */
	TNodeType getNodeType();	/**< getter for the type of the node */
    void setValue(std::string value);  /**< setter for the value of the node */
	std::string getValue();  /**< getter for the value of the node*/
	void setParent(TNode* parent);	/**< setter for setting parent node of the TNode object */
	TNode* getParent();	/**< getter for getting parent node of the TNode object */
	
	//! Adding child node into the list of children for the node
	/*!
	This is used to add a child TNode object into the list of children of a node for determining parent and follows relationship.
	\param child The node that represents the child node.
	*/
	void addChild(TNode* child);

	std::vector<TNode*> getChildNodes();	/**< getter for getting child nodes list of the TNode object */

    /** Checks if a node is a container statement
     * Given a TNode, checks if the node is a container statement and returns the appropriate boolean
     * \param node The node to be checked
     * \return true if the node is a container statement and false if not.
     */
    static bool isContainerStmt(TNode & node);
    
    /** Checks if a node is a statement
    * Given a TNode, checks if the node is a statement and returns the appropriate boolean
    * \param node The node to be checked
    * \return true if the node is a statement and false if not.
    */
    static bool isStmtNode(TNode & node);
};