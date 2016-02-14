#include <vector>
#include <string>
#include "TNode.h"


TNode::TNode() {
	TNode::nodeType = "";
	TNode::lineNumber = 0;
}

TNode::~TNode() {
}

TNode::TNode(std::string nodeType, int lineNumber) {
	TNode::nodeType = nodeType;
	TNode::lineNumber = lineNumber;
	parentNode = new TNode();
}

void TNode::setLineNumber(int lineNumber) {
	TNode::lineNumber = lineNumber;
}

void TNode::setNodeType(std::string nodeType) {
	TNode::nodeType = nodeType;
}

int TNode::getLineNumber() {
	return lineNumber;
}

std::string TNode::getNodeType() {
	return nodeType;
}

void TNode::setParent(TNode* parentNode) {
	parentNode = parentNode;
}

TNode* TNode::getParent() {
	return parentNode;
}

void TNode::addChild(TNode* childNode) {
	childNodes.push_back(childNode);
}

std::vector<TNode* > TNode::getChildNodes() {
	return childNodes;
}