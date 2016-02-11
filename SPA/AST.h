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
};

