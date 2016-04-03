#pragma once

#include <unordered_map>
#include <stack>

enum CFGNodeType {
    Unused,
    Normal,
    WhileNode,
    IfElse
};

/**Class that represents a single node in the CFG
 * An instance of this class represents a contiguous block of statements that follow each other.
 * Each CFGNode contains a leftLmt and a rightLmt that represents the range of the statements that
 * it contains, inclusive of the indexes returned.
 * The type of CFGNode can also be differentiated based on the number of parent/child relations.
 * Normal: 1 parent, 1 child
 * WhileNode: 2 parent, 2 children
 * IfNode: 1 parent, 2 children
 * with exceptions (first normal node after if/else split has 2 parents)
 */

class CFGNode {
public:
    CFGNode(); /**< Vanilla constructor*/

    /**Constructor for CFGNode with CFGNodeType and statement number argument
     * This function creates a new CFGNode given a CFGNodeType and a statement number.
     * Both leftLmt and rightLmt will be set to the statement number argument
     */
    CFGNode(CFGNodeType, int);
    ~CFGNode(); /**< Vanilla destructor */

    CFGNodeType getType(); /**< Retrieves the type of the CFGNode */
    void setType(CFGNodeType); /**< Sets the type of the CFGNode */
    int getLeftLmt(); /** Retrieves the statement number of the first statement contained in this CFGNode*/
    int getRightLmt(); /** Retrieves the statement number of the last statement contained in this CFGNode*/
    std::vector<CFGNode*> getParents(); /**Returns a vector containing pointers to each parent of this CFGNode*/

    /**Retrieves first child if any. 
     * This function will check if this CFGNode has an actual first child, and if there is,
     * returns it.
     */
    CFGNode* getChd1();
    /**Retrieves first child if any.
     * This function will check if this CFGNode has an actual second child, and if there is,
     * returns it.
     */
    CFGNode* getChd2();

    /**Adds a child to the CFGNode.
     * This function takes in a CFGNode pointer, and makes the attached CFGNode a child.
     */
    void addChild(CFGNode*);
    
    /**Assigns a new type and new limit values to this CFG
     * Primarily used for assigning a NodeType and value to a CFGNode.
     * Both limit values will be set to the provided statement number.
     */
    void initialise(CFGNodeType, int);

    /**Increments the right limit of the CFGNode.
     * Calling this function means that you are increasing the number of statements in the node by 1.
     */
    void incrementRightLmt();
private:
    int leftLmt; /**< The statement number of the first statement contained in this CFGNode, inclusive*/
    int rightLmt; /**< The statement number of the last statement contained in this CFGNode, inclusive*/
    CFGNodeType type; /**< The type of this CFGNode */
    CFGNode* child1; /**< The first child of this CFGNode*/
    CFGNode* child2; /**< The second child of this CFGNode*/
    std::vector<CFGNode*> parent; /** vector containing all the parents of this CFGNode*/
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

    std::vector<int> nextStmt(int);
    std::vector<int> prevStmt(int);
    std::vector<int> nextStmtStar(int);
    std::vector<int> prevStmtStar(int);

private:
    std::vector<CFGNode*> graph;
    std::stack<CFGNode*> containerStk;
    CFGNode* currNode;
    int stmtCount = 1;
    bool combine = false;
    bool ifelse = false;
    bool endIf = false;
    std::unordered_map<int,CFGNode*> stmtFinder;
};