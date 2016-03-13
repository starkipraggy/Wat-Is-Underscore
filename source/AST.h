#pragma once

#include <vector>
#include "TNode.h"
#include <stack>
#include "SimpleParser.h"

/** Class that represents an AST for a single procedure.
 * 
 * The AST class represents an AST for a single given procedure. The nodes are stored in a vector,
 * and the first node in the vector is guaranteed to always be the root node. The name of the procedure
 * will be stored as the value in the root node.
 */
class AST
{
private:
	std::vector<TNode*> tree;	/**< List of nodes that make up the AST */
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
    int getNodeIndex(TNode * node);

	//! Function to add a node in the tree
	/*!
	This is the function used to add a TNode object into the AST.
	\param node The type of statement that this TNode object is representing has.

	Uses the push_back(object) function of the std::vector class.
	*/
	void addToTree(TNode* node);

	//! Function to create the parent and children links between two nodes in the tree
	/*!
	This is the function used to create the parent and children links between two nodes in the tree.
	\param parent The parent node.
	\param child The child node.

	The functions only creates the links if the parent TNode is found in the tree.
	*/
	void makeChild(TNode* parent, TNode* child);


	std::vector<TNode*> getChildren(TNode * node);	/**< getter for getting the child node list of the node*/

	//! Function to create the parent and children links between two nodes in the tree
	/*!
	This is the function used to create the parent and children links between two nodes in the tree.
	\param child The child node.
	\param parent The parent node.

	The functions only creates the links if the child TNode is found in the tree.
	*/
	void makeParent(TNode* child, TNode* parent);

	TNode * getParent(TNode * node);	/**< getter for getting the parent node of the node*/
    //! Function to create the parent and children

    /** Constructs a mini-AST given a tokenised representation of a right-hand side expression.
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

    /** Constructs a mini-AST given a string representation of a right-hand side expression.
     * This function simply serves as a wrapper for the main function which takes in a vector
     * of tokens, hence the overload. Like its base function, it is not fail-safe and will implode
     * given an invalid RHS expression. Expect the runtime to cost an additional O(n) for tokenisation.
     * \param a string representation of an RHS expression
     * \return the root node of the mini-AST
     */
    static TNode* constructExpressionTree(std::string expression);
    static void convertExpressionToTNodes(std::string expression, std::vector<TNode*> &resVector);

    /** Compares two trees given the root node.
     * This function compares two trees by brute force recursion. It checks the value and type of
     * every node recursively. As soon as there is a non-match, false is returned.
     * \param root node of first tree to be compared.
     * \param root node of second tree to be compared.
     * \return true if all the nodes match, false otherwise.
     */
    static bool compareTrees(TNode* tree1, TNode* tree2);
};

