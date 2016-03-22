#pragma once

#include <vector>
#include "TNode.h"
#include <stack>
#include "SimpleParser.h"

//! Class that represents an AST for a single procedure.
/** 
 * The AST class represents an AST for a single given procedure. All statement root nodes are stored in a vector,
 * and the first node in the vector is guaranteed to be the root node. The name of the procedure
 * will be stored as the value in the root node. All container statements will contain at least one StmtLst child,
 * which will hold the statement nodes that are the direct children of that statement. As such, to retrieve Parent 
 * or Parent* relationships, use the getParent/getChildren methods provided by the class. None of the methods in the
 * class perform any input validation, so the onus is on the caller to perform any validation.
 */
class AST
{
private:
	std::vector<TNode*> tree;	/**< List of nodes that make up the AST */

    //! Function to add a node in the tree
    /*!
    This is the function used to add a TNode object into the AST.
    \param node The type of statement that this TNode object is representing has.

    Uses the push_back(object) function of the std::vector class.
    */
    void addToTree(TNode* node);
public:
	AST();
    AST(std::string name);
	~AST();

	std::vector<TNode*> getTree();	/**< Getter function for the AST object */

	//! Function to find a node in the tree
	/*!
	This is the function used to find a specific TNode object in the AST.
	\param node The type of statement that this TNode object is representing has.
	
	Returns the node if it is found.
	Returns a empty node if it is not found.
	*/
	TNode* findNode(TNode* node);

    /**Retrieves the index of a node in the tree
     * This function retrieves the index of a TNode object given its pointer.
     * \param The TNode to be found
     * \return The index if found, otherwise -1.
     */
    int getNodeIndex(TNode* node);

    //! Retrieves all statements that are the child of a statement node.
    /** 
     * This function retrieves all children from all subordinate StmtLst nodes.
     * This function is not fail-safe.
     * \param node The parent node to be searched.
     * \return A vector containing statement TNodes that are a child of this statement node.
     */
    std::vector<TNode*> getChildren(TNode * node);	/**< getter for getting the child node list of the node*/

    //! Retrieves the parent of a statement node.
    /**
    * This function simply retrieves the parent statement node of the statement node.
    * \param node The child node to be searched.
    * \return A single TNode that is a statement node parent of this statement node.
    */
    TNode * getParent(TNode * node);

    //! Retrieves the statement node last added to the tree.
    /**
    * This function returns a pointer to the statement node that was last added to the tree.
    * \return TNode pointer to the last statement node added to the tree.
    */
    TNode * getLastAddedNode();

	//! Function to create the parent and children links between two nodes in the tree
	/*!
	This is the function used to create the parent and children links between two nodes in the tree.
	\param child The child node.
	\param parent The parent node.
	*/
	void makeParent(TNode* child, TNode* parent);

    //! Function to create the parent and children links between two nodes in the tree.
    /*!
    This is the function used to create the parent and children links between two nodes in the tree.
    \param parent The parent node.
    \param child The child node.
    */
    void makeChild(TNode* parent, TNode* child);

    //! Adds a new statement node to the tree.
    /**
    * This function creates a new statement node to be added to the tree, then finds a relevant parent to
    * add the new node to. It then returns a pointer to the node added to the tree.
    * \return pointer to the node representing the new statement.
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
    TNode * processAssignmentStmt(std::vector<std::string>& tokens);

    //! Function that attempts to find expression subtree in AST
    /** 
     * This function will attempt to check for a given expression subtree inside the main tree
     * by recursing over every single relevant statement node as provided in the type parameter.
     * Refer to the method findSubtreeInTree for more details.
     * \param type The type of statement node to be matched.
     * \param subtree A pointer to the root node of the subtree to be found.
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
     * \param a vector of tokens representing an RHS expression
     * \return the root node of the mini-AST
     */
    static TNode * constructExpressionTree(std::vector<std::string>& tokens);

    //! Constructs a mini-AST given a string representation of a right-hand side expression.
    /** 
     * This function simply serves as a wrapper for the main function which takes in a vector
     * of tokens, hence the overload. Like its base function, it is not fail-safe and will implode
     * given an invalid RHS expression. Expect the runtime to cost an additional O(n) for tokenisation.
     * \param a string representation of an RHS expression
     * \return the root node of the mini-AST
     */
    static TNode* constructExpressionTree(std::string expression);

    //! Constructs a vector of tokenised TNodes from an string.
    /** 
     * This function takes a string representation of an expression, formats it into a vector
     * TNodes and dumps it into the provided array reference.
     * \param a string representation of an expression
     */
    static void convertExpressionToTNodes(std::string expression, std::vector<TNode*> &resVector);

    //! Compares two trees given the root node.
    /** 
     * This function compares two trees by brute force recursion. It checks the value and type of
     * every node recursively. As soon as there is a non-match, false is returned (fail-fast behaviour).
     * \param root node of first tree to be compared.
     * \param root node of second tree to be compared.
     * \return true if all the nodes match, false otherwise.
     */
    static bool compareTrees(TNode* tree1, TNode* tree2);

    //! Checks if a subtree exists inside a given tree
    /**
     * This function will attempt to find if a subtree exists in a given tree by recursing over
     * every single node and checking if the node has a structure identical to the subtree.
     * \param subtree A TNode pointer to the root node of the subtree to be found.
     * \param tree A TNode pointer to the root node of the tree to be searched.
     * \return true if the subtree is found, false otherwise.
     */
    static bool findSubtreeInTree(TNode* subtree, TNode* tree);
};