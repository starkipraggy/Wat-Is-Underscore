#include <vector>
#include <string>
#include "AST.h"



AST::AST()
{
}


AST::~AST()
{
}

std::vector<TNode*> AST::getTree() {
	return tree;
}

TNode* AST::findNode(TNode* node) {
	for (unsigned i = 0; i < tree.size(); i++) {
		if (node == tree[i]) {
			return tree[i];
		}
	}
}