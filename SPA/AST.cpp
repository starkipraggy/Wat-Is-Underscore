#include <vector>
#include <string>
#include "AST.h"
#include "Tnode.h"



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
	return new TNode();
}

void AST::addToTree(TNode* node) {
	tree.push_back(node);
}

void AST::makeChild(TNode* parent, TNode* child) {
	for (unsigned i = 0; i < tree.size(); i++) {
		if (parent == tree[i]) {
			tree[i]->addChild(child);
			child->setParent(tree[i]);
		}
	}
}

void AST::makeParent(TNode* child, TNode* parent) {
	for (unsigned i = 0; i < tree.size(); i++) {
		if (child == tree[i]) {
			tree[i]->setParent(parent);
			parent->addChild(tree[i]);
		}
	}
}

std::vector<TNode* > AST::getChildren(TNode* node) {
	TNode* parentNode = findNode(node);
	return node->getChildNodes();
}

TNode* AST::getParent(TNode* node) {
	TNode* childNode = findNode(node);
	return node->getParent();
}
