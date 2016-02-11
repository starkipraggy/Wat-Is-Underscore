#include <vector>
#include <string>
#include "TNode.h"


TNode::TNode() {
	TNode::nodeType = "";
	TNode::lineNumber = -1;
}

TNode::~TNode() {
}

TNode::TNode(std::string nodeType, int lineNumber) {
	TNode::nodeType = nodeType;
	TNode::lineNumber = lineNumber;
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