#pragma once

#include <unordered_map>
#include <stack>

enum CFGNodeType {
    Unused,
    Normal,
    WhileNode,
    IfElse
};

class CFGNode {
public:
    CFGNode();
    CFGNode(CFGNodeType, int);
    ~CFGNode();

    void addChild(CFGNode*);

    int leftLmt;
    int rightLmt;
    CFGNodeType type;
    bool useChd1;
    CFGNode* child1;
    CFGNode* child2;
    std::vector<CFGNode*> parent;
};

class CFG {
public:
    CFG();
    ~CFG();

    std::vector<CFGNode*> getGraph();
    CFGNode* CFGNodeByStmtNum(int);

    void addStmt();
    void addWhileStmt();
    void addIfStmt();
    void endWhileStmt();
    void endIfStmt();
    void elseStmt();

    std::vector<int> nextToLeft(int);
    std::vector<int> nextToRight(int);
    std::vector<int> nextStarToLeft(int);
    std::vector<int> nextStarToRight(int);

private:
    std::vector<CFGNode*> graph;
    std::stack<CFGNode*> containerStk;
    CFGNode* currNode;
    int stmtCount = 1;
    bool combine = false;
    bool ifelse = false;
    bool endIf = false;
    //todo: use map to optimise
    std::unordered_map<int,CFGNode*> stmtFinder;
};