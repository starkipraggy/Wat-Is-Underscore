#include <vector>
#include <string>
#include "TNode.h"


TNode::TNode() {
	type = Undefined;
	lineNumber = NULL;
	value = NULL;
	varName = NULL;
	parentNode = NULL;
	childNodes.clear();
}

TNode::~TNode() {
}

TNode::TNode(TNodeType type, int lineNumber) {
	TNode* node = new TNode;
	node -> type = type;
	node -> lineNumber = lineNumber;
	//parentNode = new TNode();
}

void TNode::setLineNumber(int lineNumber) {
	TNode::lineNumber = lineNumber;
}

int TNode::getLineNumber() {
	return lineNumber;
}

void TNode::setNodeType(TNodeType type) {
	TNode::type = type;
}

TNodeType TNode::getNodeType() {
	return type;
}

void TNode::setValue(int value) {
	TNode::value = value;
}

int TNode::getValue() {
	return value;
}

void TNode::setVariable(char varName) {
	TNode::varName = varName;
}

char TNode::getVariable() {
	return varName;
}

void TNode::setParent(TNode* parentNode) {
	TNode::parentNode = parentNode;
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