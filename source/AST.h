#pragma once

#include <vector>
#include "TNode.h"
#include <stack>
#include "SimpleParser.h"

//! Class that represents an AST for a single procedure.
/** 
 * The AST class represents an AST for a single given procedure. All statement root TNodes are stored in a vector,
 * and the first TNode in the vector is guaranteed to be the root TNode. The name of the procedure
 * will be stored as the value in the root TNode. All container statements will contain at least one StmtLst child,
 * which will hold the statement TNodes that are the direct children of that statement. As such, to retrieve Parent 
 * or Parent* relationships, use the getParent/getChildren methods provided by the class. None of the methods in the
 * class perform any input validation, so the onus is on the caller to perform any validation.
 */
class AST
{
private:
	std::vector<TNode*> tree;	/**< List of TNodes that make up the AST */
	bool endOfContainerStmt = false;    /**< Used to determine the end of a container statement */
	int numberOfEndContainerCalls = 0;	/**< Used to determine the number of end statement calls */

    //! Function to add a TNode in the tree
    /*!
    This is the function used to add a TNode object into the AST.
    \param TNode The type of statement that this TNode object is representing has.

    Uses the push_back(object) function of the std::vector class.
    */
    void addToTree(TNode* node);
public:
	AST();
    
    //! Constructor with Procedure Name.
    /**
     * Constructor to create a new AST object with the procedure name provided.
     * \param name The name of the procedure.
     */
    AST(std::string name);
	~AST();

    //! Getter function for the vector of statement TNodes in the tree.
    /**
     * This function returns the vector containing all the statement TNodes in the tree.
     * The root TNode, which is the procedure name, will be in the 0th index.
     * The TNode corresponding to any statement n can be found by accessing the nth index.
     */
	std::vector<TNode*> getTree();

	//! Function to find a TNode in the tree
	/*!
	This is the function used to find a specific TNode object in the AST.
	\param node The type of statement that this TNode object is representing has.
	\return Returns the TNode if it is found, else returns an empty TNode.
	*/
	TNode* findNode(TNode* node);

    /**Retrieves the index of a TNode in the tree
     * This function retrieves the index of a TNode object given its pointer.
     * \param node The TNode to be found
     * \return The index if found, otherwise -1.
     */
    int getNodeIndex(TNode* node);

    //! Retrieves all statements that are the child of a statement TNode.
    /** 
     * This function retrieves all children from all subordinate StmtLst TNodes.
     * This function is not fail-safe.
     * \param node The parent TNode to be searched.
     * \return A vector containing statement TNodes that are a child of this statement TNode.
     */
    std::vector<TNode*> getChildren(TNode * node);

    //! Retrieves the parent of a statement TNode.
    /**
    * This function simply retrieves the parent statement TNode of the statement TNode.
    * \param node The child TNode to be searched.
    * \return A single TNode that is a statement TNode parent of this statement TNode.
    */
    TNode * getParent(TNode * node);

    //! Retrieves the statement TNode last added to the tree.
    /**
    * This function returns a pointer to the statement TNode that was last added to the tree.
    * \return TNode pointer to the last statement TNode added to the tree.
    */
    TNode * getLastAddedNode();

	//! Function to create the parent and children links between two TNodes in the tree
	/*!
	This is the function used to create the parent and children links between two TNodes in the tree.
	\param child The child TNode.
	\param parent The parent TNode.
	*/
	void makeParent(TNode* child, TNode* parent);

    //! Function to create the parent and children links between two TNodes in the tree.
    /*!
    This is the function used to create the parent and children links between two TNodes in the tree.
    \param parent The parent TNode.
    \param child The child TNode.
    */
    void makeChild(TNode* parent, TNode* child);

    //! Adds a new statement TNode to the tree.
    /**
    * This function creates a new statement TNode to be added to the tree, then finds a relevant parent to
    * add the new TNode to. It then returns a pointer to the TNode added to the tree.
    * \param type The type of TNode to be added. Defaults to Undefined.
    * \param value The value that the TNode should hold. Defaults to an empty string.
    * \param stmtNum The statement number of the TNode. Defaults to 0.
    * \return pointer to the TNode representing the new statement.
    */
    TNode * appendNewStmtNode(TNodeType = Undefined, std::string = "", int = 0);

    
    //! Function that processes a vector representation of an assignment statement into a TNode.
    /** 
     * This function automates the processing of a vector of string tokens representing an
     * assignment statement into a TNode with the relevant subtree built.
     * This function is not fail-safe.
     * \params tokens The vector containing the token representation of the statement.
     * \return A TNode pointer representing the TNode of the assignment statement.
     */
    TNode * processAssignmentStmt(NAME variable, std::vector<std::string>& tokens);

    //! Function that attempts to find expression subtree in AST
    /** 
     * This function will attempt to check for a given expression subtree inside the main tree
     * by recursing over every single relevant statement TNode as provided in the type parameter.
     * Refer to the method findSubtreeInTree for more details.
     * \param type The type of statement TNode to be matched.
     * \param subtree A pointer to the root TNode of the subtree to be found.
     * \return true if the subtree is found, false otherwise.
     */
    bool findSubtreeInAST(TNodeType type, TNode* subtree);

    //! Constructs a mini-AST given a tokenised representation of a right-hand side expression.
    /** 
     * This function constructs an expression tree given a tokenised expression in a vector.
     * It implements Djikstra's Shunting-Yard algorithm for AST given SIMPLE language constraints.
     * For more details on how it is implemented, visit the source code.
     * This function is NOT fail-safe and will implode if given an invalid RHS expression.
     * Order of growth is O(n) where n is the number of tokens in the expression. 
     * Actual runtime is about 1.5n.
     * \param tokens A vector of tokens representing an RHS expression
     * \return The root TNode of the mini-AST
     */
    static TNode * constructExpressionTree(std::vector<std::string>& tokens);

    //! Constructs a mini-AST given a string representation of a right-hand side expression.
    /** 
     * This function simply serves as a wrapper for the main function which takes in a vector
     * of tokens, hence the overload. Like its base function, it is not fail-safe and will implode
     * given an invalid RHS expression. Expect the runtime to cost an additional O(n) for tokenisation.
     * \param expression a string representation of an RHS expression
     * \return The root TNode of the mini-AST
     */
    static TNode* constructExpressionTree(std::string expression);

    //! Constructs a vector of tokenised TNodes from an string.
    /** 
     * This function takes a string representation of an expression, formats it into a vector
     * of TNodes and dumps it into the provided array reference.
     * \param expression A string representation of an expression
     * \param resVector The vector to place the TNodes in.
     */
    static void convertExpressionToTNodes(std::string expression, std::vector<TNode*> &resVector);

    //! Compares two trees given the root TNode.
    /** 
     * This function compares two trees by brute force recursion. It checks the value and type of
     * every TNode recursively. As soon as there is a non-match, false is returned (fail-fast behaviour).
     * \param root TNode of first tree to be compared.
     * \param root TNode of second tree to be compared.
     * \return true if all the TNodes match, false otherwise.
     */
    static bool compareTrees(TNode* tree1, TNode* tree2);

    //! Checks if a subtree exists inside a given tree
    /**
     * This function will attempt to find if a subtree exists in a given tree by recursing over
     * every single TNode and checking if the TNode has a structure identical to the subtree.
     * \param subtree A TNode pointer to the root TNode of the subtree to be found.
     * \param tree A TNode pointer to the root TNode of the tree to be searched.
     * \return true if the subtree is found, false otherwise.
     */
    static bool findSubtreeInTree(TNode* subtree, TNode* tree);

	//! Adds an assign statement TNode into the AST.
	/**
	* This function adds the given assignment statement TNode into the AST.
	* \param tokens A vector of tokens representing the assignment statement.
	* \param statementNumber The statement number of the statement to be added into the AST.
    * \return A pointer to the generated TNode.
	*/
	TNode* addAssignTNode(NAME variable, std::vector<std::string> tokens, int statementNumber);

	//! Adds an call statement TNode into the AST.
	/**
	* This function adds the call statement into the AST.
	* \param callNode A TNodeType representing the TNodeType Call.
	* \param procName The name of the procedure being called.
	* \param statementNumber The statement number of the statement to be added into the AST.    
    * \return A pointer to the generated TNode.
	*/
	TNode* addCallTNode(std::string procName, int statementNumber);

	//! Adds an while statement TNode into the AST.
	/**
	* This function adds the while statement TNode into the AST.
	* \param variableNameNode A TNodeType representing the TNodeType VariableName.
	* \param variable The name of the control variable of the loop.
	* \param statementNumber The statement number of the statement to be added into the AST.    
    * \return A pointer to the generated TNode.
	*/
	TNode* addWhileTNode(NAME variable, int statementNumber);

	//! Adds an if statement TNode into the AST.
	/**
	* This function adds the if statement TNode into the AST.
	* \param variableNameNode A TNodeType representing the TNodeType VariableName.
	* \param variable The name of the control variable of the if statement.
	* \param stmtLstNode A TNodeType representing the TNodeType StmtLst.
    * \return A pointer to the generated TNode.
	*/
	TNode* addIfTNode(NAME variable, int statementNumber);

	//! Creates an Else relation on the last container statement TNode.
	/**
	* This function will prep the parent statement TNode of the last statement TNode created
    * for entering statements on the else StmtLst. As the description states, this works on
    * all container statements and there is no validation, so exercise caution.
	*/
	void addElseRelation();

	//! Setter for determining the end of a container statement.
	/**
	* This function will set the endOfContainerStmt bool to true so that the end of a
	* container statement can be determined
	*/
	void addEndOfContainerRelation();
};