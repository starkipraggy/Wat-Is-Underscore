#include "CFG.h"

CFGNode::CFGNode() {
    leftLmt = 0;
    rightLmt = 0;
    type = Unused;
    useChd1 = true;
    child1 = NULL;
    child2 = NULL;
}

CFGNode::CFGNode(CFGNodeType ty, int sm) {
    type = ty;
    leftLmt = sm;
    rightLmt = sm;
}

CFGNode::~CFGNode(){
}

void CFGNode::addChild(CFGNode * node){
    if (useChd1) {
        child1 = node;
    } else {
        child2 = node;
    }
    node->parent.push_back(this);
}

CFG::CFG(){
    currNode = new CFGNode();
    graph.push_back(currNode);
}

CFG::~CFG(){
}

std::vector<CFGNode*> CFG::getGraph(){
    return graph;
}

CFGNode* CFG::CFGNodeByStmtNum(int stmtNum){
    try {
        return stmtFinder.at(stmtNum);
    }catch (const std::out_of_range& oor) {
        return nullptr;
    }
}

void CFG::addStmt(){
    if (currNode->type == Unused) {
        currNode->type = Normal;
        currNode->leftLmt = stmtCount;
        currNode->rightLmt = stmtCount;
    } else {
        currNode->rightLmt++;
    }
    stmtFinder[stmtCount] = currNode;
    stmtCount++;
}

void CFG::addWhileStmt(){
    if (currNode->type == Unused) {
        currNode->type = WhileNode;
        currNode->leftLmt = stmtCount;
        currNode->rightLmt = stmtCount;
    } else {
        CFGNode* newWhile = new CFGNode(WhileNode, stmtCount);
        currNode->addChild(newWhile);
        currNode = newWhile;
        graph.push_back(currNode);
    }
    containerStk.push(currNode);
    stmtFinder[stmtCount] = currNode;
    currNode->addChild(new CFGNode());
    currNode = currNode->child1;
    graph.push_back(currNode);
    stmtCount++;
}

void CFG::addIfStmt(){
    if (currNode->type == Unused) {
        currNode->type = IfElse;
        currNode->leftLmt = stmtCount;
        currNode->rightLmt = stmtCount;
    } else {
        CFGNode* newIf = new CFGNode(IfElse, stmtCount);
        currNode->addChild(newIf);
        currNode = newIf;
        graph.push_back(currNode);
    }
    containerStk.push(currNode);
    stmtFinder[stmtCount] = currNode;
    graph.push_back(currNode);
    currNode->addChild(new CFGNode());
    currNode = currNode->child1;
    graph.push_back(currNode);
    stmtCount++;
}

void CFG::endWhileStmt(){
    containerStk.top()->addChild(new CFGNode());
    currNode = containerStk.top()->child2;
    containerStk.pop();
}

void CFG::endIfStmt(){
    CFGNode* lastIf = containerStk.top();
    CFGNode* combined = new CFGNode();
    lastIf->child1->addChild(combined);
    lastIf->child2->addChild(combined);
    containerStk.pop();
    currNode = combined;
}

void CFG::elseStmt(){
    containerStk.top()->addChild(new CFGNode());
    currNode = containerStk.top()->child2;
}

std::vector<int> CFG::nextToLeft(int stmtNum){
    std::vector<int> result;
    CFGNode* thisNode = stmtFinder[stmtNum];
    if (stmtNum > thisNode->leftLmt) {
        result.push_back(stmtNum - 1);
    } else if (stmtNum == 1) {
        result.push_back(0);
    } else {
        for (int i = 0; i < thisNode->parent.size(); i++) {
            result.push_back(thisNode->parent[i]->rightLmt);
        }
    }
    return result;
}

std::vector<int> CFG::nextToRight(int stmtNum){
    std::vector<int> result;
    CFGNode* thisNode = stmtFinder[stmtNum];
    if (stmtNum < thisNode->rightLmt) {
        result.push_back(stmtNum - 1);
    } else if (stmtNum == 1) {
        result.push_back(0);
    } else {
        result.push_back(thisNode->child1->leftLmt);
        result.push_back(thisNode->child2->leftLmt);
    }
    return result;
}

std::vector<int> CFG::nextStarToLeft(int stmtNum){
    std::vector<int> result;
    std::stack<CFGNode*> toSearch;
    CFGNode* thisNode = stmtFinder[stmtNum];
    for (int i = thisNode->leftLmt; i < stmtNum; i++) {
        result.push_back(i);
    }
    for (int i = 0; i < thisNode->parent.size(); i++) {
        toSearch.push(thisNode->parent[i]);
    }
    while (!toSearch.empty()) {
        thisNode = toSearch.top();
        toSearch.pop();

        for (int i = 0; i < thisNode->parent.size(); i++) {
            if (thisNode->leftLmt > thisNode->parent[i]->rightLmt) {
                toSearch.push(thisNode->parent[i]);
            }
        }
        
        for (int i = thisNode->leftLmt; i <= thisNode->rightLmt; i++) {
            result.push_back(i);
        }
    }
    return result;
}

std::vector<int> CFG::nextStarToRight(int stmtNum){
    std::vector<int> result;
    std::stack<CFGNode*> toSearch;
    CFGNode* thisNode = stmtFinder[stmtNum];
    for (int i = stmtNum+1; i <= thisNode->rightLmt; i++) {
        result.push_back(i);
    }
    if (thisNode->child1->type != Unused){
        toSearch.push(thisNode->child1);
    }
    if (thisNode->child2->type != Unused) {
        toSearch.push(thisNode->child2);
    }
    while (!toSearch.empty()) {
        thisNode = toSearch.top();
        toSearch.pop();

        if (thisNode->child1->type != Unused && thisNode->rightLmt < thisNode->child1->leftLmt) {
            toSearch.push(thisNode->child1);
        }
        if (thisNode->child1->type != Unused && thisNode->rightLmt < thisNode->child2->leftLmt) {
            toSearch.push(thisNode->child2);
        }

        for (int i = thisNode->leftLmt; i <= thisNode->rightLmt; i++) {
            result.push_back(i);
        }
    }
    return result;
}

