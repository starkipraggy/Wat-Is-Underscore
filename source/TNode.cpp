#include "TNode.h"
#include "TNode.h"
#include "TNode.h"
#include "TNode.h"
#include <vector>
#include <string>
#include "TNode.h"


TNode::TNode() {
	type = Undefined;
	lineNumber = NULL;
	value = "";
	parentNode = NULL;
	childNodes.clear();
}

TNode::~TNode() {
}

TNode::TNode(TNodeType initType, std::string initValue, int lineNum) {
	type = initType;
    value = initValue;
	lineNumber = lineNum;
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

void TNode::setValue(std::string value) {
	TNode::value = value;
}

std::string TNode::getValue() {
	return value;
}

void TNode::setParent(TNode* parentNode) {
	TNode::parentNode = parentNode;
}

TNode* TNode::getParent() {
	return parentNode;
}

void TNode::addChild(TNode* childNode) {
	childNodes.push_back(childNode);
    childNode->setParent(this);
}

std::vector<TNode* > TNode::getChildNodes() {
	return childNodes;
}

bool TNode::isContainerStmt() {
    return (TNode::getNodeType() == StmtLst ||
        TNode::getNodeType() == While ||
        TNode::getNodeType() == If ||
        TNode::getNodeType() == ProcedureName);
}

bool TNode::isStmtNode() {
    return (TNode::getNodeType() == While ||
        TNode::getNodeType() == If ||
        TNode::getNodeType() == Assign ||
        TNode::getNodeType() == Call);
}