#pragma once

#include <vector>
#include "TNode.h"

class AST
{
private:
	std::vector<TNode*> tree;
public:
	AST();
	~AST();
	std::vector<TNode*> getTree();
	TNode* findNode(TNode* node);
	void addToTree(TNode* node);
	void makeChild(TNode* parent, TNode* child);
	std::vector<TNode*> getChildren(TNode * node);
	TNode * getParent(TNode * node);
	void makeParent(TNode* child, TNode* parent);
};

