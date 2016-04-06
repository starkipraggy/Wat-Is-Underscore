#pragma once

#include <stack>
#include <unordered_map>

//forward declaration
class StatementTable;
class StatementTableStatement;

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
 * The type of CFGNode defaults to Unused if not containing any statements.
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

/**Singleton class that holds CFGs for all procedures.
 * The CFG class is a globally accessible singleton that contains all the CFGNodes in use
 * for a particular SIMPLE program -- that is to say, all the CFGNodes in a given run.
 * The singleton is retrieved using the static getGlobalCFG() method. While it is possible
 * to create your own instance of this class, only do so for testing purposes. For production
 * code, please stick to using the singleton copy. As only the PKB and its dependents require
 * the use of this class, as much as possible, modify PKB code instead. This class also
 * provides a utility method to retrieve StatementTableStatements given integers.
 * The class provides two different ways to access CFGNodes:
 * - Retrieve the entire class via getGraph()
 * - Retrieve the node given the statement number with CFGNodeByStmtNum()
 * General flow for use of the CFG is the same as that of the PKB. A new procedure
 * requires explicit declaration via newProcedure(). If statements require an 
 * addIfStmt() call, an elseStmt() call, and an endIfStmt() call to be complete.
 * Likewise, while statements require the addWhileStmt() call and the endWhileStmt() call.
 * Assign statements and Call statements are treated as generic and are added with
 * an addStmt() call.
 * The class does no validation on its method calls, so tread with caution.
 */

class CFG {
public:
    CFG();          /**< Default constructor */
    ~CFG();         /**< Default destructor  */

    /**Retrieves the CFG singleton instance.
     * This method administrates the reference to the CFG singleton.
     * It will automatically handle the creation of the singleton if
     * not already created.
     */
    static CFG& getGlobalCFG();

    /**Retrieves all CFGNodes in the CFG.
     * This method retrieves all CFGNodes across procedures in the
     * current CFG object in a vector. CFGNodes are placed in a top-down
     * order. For branching, left branches of if statements and the
     * recurring branch of while statements go first.
     */
    std::vector<CFGNode*> getGraph();

    /**Retrieves a CFGNode given a statement number.
     * This method will, given a statement number, find the relevant
     * CFGNode and return it. Time complexity of this algorithm is O(1).
     */
    CFGNode* CFGNodeByStmtNum(int);

    /**Sets the StatementTable reference the CFG will search in.
     * This method will set the StatementTable that the CFG will reference
     * when building the StatementTableStatement vector to be returned 
     * in convertIntToStatement(). If the PKB is created, it will supply
     * the correct StatementTable to the CFG automatically.
     */
    void setStmtTable(StatementTable*);

    /**Tells the CFG to begin a new Procedure.
     * As the CFG does not begin with any nodes, this method must be called
     * before any other operations are called, otherwise an error will be
     * thrown.
     */
    void newProcedure();
    /**Tells the CFG to add a statement to the current active CFGNode.
     * This method simply increments the statements held by the currently
     * CFGNode by 1. It will crash the program if called on an empty CFG.
     */
    void addStmt();
    /**Tells the CFG to begin a new While statement.
     * Creates the necessary CFGNodes and links them, ready to have statements
     * added to them.
     */
    void addWhileStmt();    
    /**Tells the CFG to begin a new If statement.
     * Creates the root If statement node and the first node of the left branch.
     */
    void addIfStmt();
    /**Tells the CFG to end the While statement.
     * Closes the loop of the While statement and adds a CFGNode that starts
     * after the control statement of the while loop
     */
    void endWhileStmt();
    /**Tells the CFG to end the If statement.
     * Traverses both branches to the end, then links those CFGNodes to a new
     * CFGNode, ready to be added to.
     */
    void endIfStmt();
    /**Tells the CFG to begin an Else section.
     * Creates a CFGNode corresponding to the right branch and adds it to the graph.
     */
    void elseStmt();

    /**Retreives the statement number of the next statement in the graph.
     * Typical performance is O(1).
     */
    std::vector<int> nextStmt(int);
    /**Retrieves the statement number of the previous statement in the graph.
     * Typical performance is O(1).
     */
    std::vector<int> prevStmt(int);
    /**Retrieves the statement numbers of all statements coming after the given one.
     * This method uses an unordered map to store traversed branching nodes. As
     * typical retrieval in a map takes O(1), it is not expected to impact the runtime.
     * Actual runtime is bounded by O(n) where n is the number of CFGNodes in the graph.
     */
    std::vector<int> nextStmtStar(int);
    /**Retrieves the statement numbers of all statements coming before the given one.
     * This method employs 2 seperate stacks to store traversed branching nodes. Retrieval
     * time is O(1), however it comes at the cost of far more convoluted logic. Actual runtime
     * is however still bounded by O(n) where n is the number of existing CFGNodes.
     */
    std::vector<int> prevStmtStar(int);
    /**Converts a integer vector to a StatementTableStatement vector.
     * This method simply searches for StatementTableStatements in the StatementTable
     * object pointer contained in the CFG and places them in the provided StatementTableStatement
     * vector reference. Do not call this method before providing the CFG with a StatementTable
     * pointer. The PKB does handle the provision however, so you may use it freely
     * after the PKB is initialised.
     */
    void convertIntToStatement(std::vector<int>, std::vector<StatementTableStatement*>&);

private:
    std::vector<CFGNode*> graph;                    /**< Vector containing all CFGNodes */
    /**Stack containing container statements
     * This stack keep track of container statements during the building of the CFG graph
     * for later reference. CFGNodes are added on creation, and popped when fully resolved.
     */
    std::stack<CFGNode*> containerStk;
    /**Stack containing the first left child of if statements
     * This stack tracks the left children of unresolved if statements. Once an if statement
     * is resolved, the corresponding child will be popped.
     */
    std::stack<CFGNode*> if1stchildStk;
    CFGNode* currNode;          /**< The pointer to the current CFGNode being processed */
    int stmtCount = 1;  /**< The statement number of the next statement to be processed */
    
    /**Map to all CFGNodes, by statement number.
     * This map stores keypairs of key statement number and value CFGNode.
     * Accessing the map using a given statement number will return you the CFGNode
     * that the statement is contained in.
     */
    std::unordered_map<int,CFGNode*> stmtFinder;
    /**Pointer to the StatementTable object to be searched
     * The PKB will supply the correct reference if initialised. Otherwise,
     * if you wish to use the int to StatementTableStatement functionality,
     * you must initialise this with the StatementTable reference to be used.
     */
    StatementTable* stmtObjTable; 
};