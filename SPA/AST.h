#pragma once

#include <vector>
#include "TNode.h"

class AST
{
private:
	std::vector<TNode*> tree;	/**< List of nodes that make up the AST */
public:
	AST();
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

	//! Function to add a node in the tree
	/*!
	This is the function used to add a TNode object into the AST.
	\param node The type of statement that this TNode object is representing has.

	Uses the Uses the push_back(object) function of the std::vector class.
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

};

