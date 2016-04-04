#include "CFG.h"
#include "StatementTable.h"

CFGNode::CFGNode() {
    leftLmt = 0;
    rightLmt = 0;
    type = Unused;
    child1 = NULL;
    child2 = NULL;
}

CFGNode::CFGNode(CFGNodeType ty, int sm) {
    type = ty;
    leftLmt = sm;
    rightLmt = sm;
    child1 = NULL;
    child2 = NULL;
}

CFGNode::~CFGNode(){
}

CFGNodeType CFGNode::getType(){
    return type;
}

int CFGNode::getLeftLmt() {
    return leftLmt;
}

int CFGNode::getRightLmt(){
    return rightLmt;
}

std::vector<CFGNode*> CFGNode::getParents(){
    return parent;
}

CFGNode * CFGNode::getChd1(){
    return child1;
}

CFGNode * CFGNode::getChd2(){
    return child2;
}

void CFGNode::addChild(CFGNode * node){
    if (child1 == NULL || child1->getType() == Unused) {
        child1 = node;
    } else {
        child2 = node;
    }
    node->parent.push_back(this);
}

void CFGNode::initialise(CFGNodeType newType, int stmtNum){
    type = newType;
    leftLmt = stmtNum;
    rightLmt = stmtNum;
}

void CFGNode::incrementRightLmt(){
    rightLmt++;
}

CFG::CFG(){
}

CFG::~CFG(){
}

CFG& CFG::getGlobalCFG(){
    // TODO: insert return statement here
    static CFG GlobalCFG;
    return GlobalCFG;
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

void CFG::setStmtTable(StatementTable * table){
    stmtObjTable = table;
}

void CFG::newProcedure() {
    currNode = new CFGNode();
    graph.push_back(currNode);
}

void CFG::addStmt(){
    if (currNode->getType() == Unused) {
        currNode->initialise(Normal, stmtCount);
    } else {
        currNode->incrementRightLmt();
    }
    stmtFinder[stmtCount] = currNode;
    stmtCount++;
}

void CFG::addWhileStmt(){
    if (currNode->getType() == Unused) {
        currNode->initialise(WhileNode, stmtCount);
    } else {
        CFGNode* newWhile = new CFGNode(WhileNode, stmtCount);
        currNode->addChild(newWhile);
        currNode = newWhile;
        graph.push_back(currNode);
    }
    containerStk.push(currNode);
    stmtFinder[stmtCount] = currNode;
    currNode->addChild(new CFGNode());
    currNode = currNode->getChd1();
    graph.push_back(currNode);
    //currNode->addChild(containerStk.top());
    stmtCount++;
}

void CFG::addIfStmt(){
    if (currNode->getType() == Unused) {
        currNode->initialise(IfElse, stmtCount);
    } else {
        CFGNode* newIf = new CFGNode(IfElse, stmtCount);
        currNode->addChild(newIf);
        currNode = newIf;
        graph.push_back(currNode);
    }
    containerStk.push(currNode);
    stmtFinder[stmtCount] = currNode;
    currNode->addChild(new CFGNode());
    currNode = currNode->getChd1();
    graph.push_back(currNode);
    stmtCount++;
}

void CFG::endWhileStmt(){
    currNode->addChild(containerStk.top());
    containerStk.top()->addChild(new CFGNode());
    currNode = containerStk.top()->getChd2();
    graph.push_back(currNode);
    containerStk.pop();
}

void CFG::endIfStmt(){
    CFGNode* lastIf = containerStk.top();
    CFGNode* combined = new CFGNode();
    lastIf->getChd1()->addChild(combined);
    lastIf->getChd2()->addChild(combined);
    containerStk.pop();
    currNode = combined;
    graph.push_back(currNode);
}

void CFG::elseStmt(){
    containerStk.top()->addChild(new CFGNode());
    currNode = containerStk.top()->getChd2();
    graph.push_back(currNode);
}

std::vector<int> CFG::prevStmt(int stmtNum){
    std::vector<int> result;
    CFGNode* thisNode = stmtFinder[stmtNum];
    if (stmtNum > thisNode->getLeftLmt()) {
        result.push_back(stmtNum - 1);
    } else {
        for (size_t i = 0; i < thisNode->getParents().size(); i++) {
            result.push_back(thisNode->getParents()[i]->getRightLmt());
        }
    }
    return result;
}

std::vector<int> CFG::nextStmt(int stmtNum){
    std::vector<int> result;
    CFGNode* thisNode = stmtFinder[stmtNum];
    if (stmtNum < thisNode->getRightLmt()) {
        result.push_back(stmtNum + 1);
    } else {
        if (thisNode->getChd1() != NULL && thisNode->getChd1()->getType() != Unused){
            result.push_back(thisNode->getChd1()->getLeftLmt());
        }
        if (thisNode->getChd2() != NULL && thisNode->getChd2()->getType() != Unused) {
            result.push_back(thisNode->getChd2()->getLeftLmt());
        }
    }
    return result;
}

std::vector<int> CFG::prevStmtStar(int stmtNum){
    std::vector<int> result;
    std::stack<CFGNode*> toSearch;
    std::stack<CFGNode*> whileStack;
    CFGNode* thisNode = stmtFinder[stmtNum];
    for (int i = thisNode->getLeftLmt(); i < stmtNum; i++) {
        result.push_back(i);
    }
    for (size_t i = 0; i < thisNode->getParents().size(); i++) {
        toSearch.push(thisNode->getParents()[i]);
        if (thisNode->getParents()[i]->getType() == WhileNode) {
            whileStack.push(thisNode->getParents()[i]);
        }
    }
    while (!toSearch.empty()) {
        thisNode = toSearch.top();
        toSearch.pop();

        for (size_t i = 0; i < thisNode->getParents().size(); i++) {
            if (thisNode->getParents()[i] != whileStack.top()) {
                toSearch.push(thisNode->getParents()[i]);

                if (thisNode->getParents()[i]->getType() == WhileNode) {
                    whileStack.push(thisNode->getParents()[i]);
                }
            } else {
                whileStack.pop();
            }
        }
        
        for (int i = thisNode->getLeftLmt(); i <= thisNode->getRightLmt(); i++) {
            result.push_back(i);
        }
    }
    return result;
}

void CFG::convertIntToStatement(std::vector<int> input, std::vector<StatementTableStatement*>& result){
    for (size_t i = 0; i < input.size(); i++) {
        result.push_back(stmtObjTable->getStatementUsingStatementNumber(input[i]));
    }
}

std::vector<int> CFG::nextStmtStar(int stmtNum){
    std::vector<int> result;
    std::stack<CFGNode*> toSearch;
    CFGNode* thisNode = stmtFinder[stmtNum];
    for (int i = stmtNum+1; i <= thisNode->getRightLmt(); i++) {
        result.push_back(i);
    }
    if (thisNode->getChd1() != NULL && thisNode->getChd1()->getType() != Unused){
        toSearch.push(thisNode->getChd1());
    }
    if (thisNode->getChd2() != NULL && thisNode->getChd2()->getType() != Unused) {
        toSearch.push(thisNode->getChd2());
    }
    while (!toSearch.empty()) {
        thisNode = toSearch.top();
        toSearch.pop();

        if ((thisNode->getChd1() != NULL && thisNode->getChd1()->getType() != Unused) 
            && thisNode->getRightLmt() < thisNode->getChd1()->getLeftLmt()) {
            toSearch.push(thisNode->getChd1());
        }
        if ((thisNode->getChd2() != NULL && thisNode->getChd2()->getType() != Unused) 
            && thisNode->getRightLmt() < thisNode->getChd2()->getLeftLmt()) {
            toSearch.push(thisNode->getChd2());
        }

        for (int i = thisNode->getLeftLmt(); i <= thisNode->getRightLmt(); i++) {
            result.push_back(i);
        }
    }
    return result;
}

