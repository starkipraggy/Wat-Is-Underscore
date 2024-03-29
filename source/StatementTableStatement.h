/*! \class StatementTableStatement
	\brief Used to represent the individual statements in the StatementTable in the PKB.

	This class is used to represent the individual statements that makes up the table of
	statements in the Program Knowledge Base (PKB). Each StatementTableStatement object
	keeps track of individual information such as line number of statement, the index
	given to it, as well as lists of its relationships with other statements and variables.
*/

#pragma once

#include <set>
#include "DataStructureObject.h"
#include "TNode.h"

class StatementTableStatement {
private:
	int statementNumber;												/**< The statement number of this statement */
	TNodeType type;														/**< The type of this statement */
	std::string rightHandSideExpression;								/**< Right hand side expression, specifically for assign statements, for pattern clauses */
	NAME controlVariable;												/**< Control variable, specifically for if and while statements, for pattern clauses.
																 			 Also used for left hand side expression for assign statements, for Affects clauses */
	int procedureIndexNumber;											/**< Index number of the procedure that it belongs to */

	StatementTableStatement** firstCalls;								/**< The pointer to the pointer pointing to the first statement of the procedure
																 			 that this Calls statement calls, or NULL if it is not a Calls statement*/
	StatementTableStatement** lastCalls;								/**< The pointer to the pointer pointing to the last statement of the procedure
																 			 that this Calls statement calls, or NULL if it is not a Calls statement*/
	StatementTableStatement* follows;									/**< The pointer to the statement that this statement follows
																 			 (this statement appears immediately after the one it follows) */
	StatementTableStatement* followedBy;								/**< The statement number of the statement that this statement is followed
																 			 (that statement appears immediately after this statement ) */
	StatementTableStatement* parent;									/**< The pointer to the statement that is parent to this statement */
	std::vector<StatementTableStatement*>* children;					/**< A list of pointers to statements that have this statement as parent */
	std::vector<int>* modifies;											/**< A list of the index numbers of variables that this statement modifies */
	std::vector<int>* uses;												/**< A list of the index numbers of variables that this statement uses */

	std::vector<int>* parentStar;										/**< A list of statement numbers; has its parent, its parent's parent, etc.
																  			 Used for the Parent* relationship */
	std::vector<int>* followsStar;										/**< A list of statement numbers; has the statement it follows, its follow's follow, etc.
																 			 Used for the Follows* relationship */

	std::set<int>* childrenStar;										/**< A list of statement numbers; has its children, its children's children, etc.
																 			 Used for the Parent* relationship */
	bool hasItsChildrenStarChanged;										/**< Boolean control to check if a new childrenStar set should be fetched, or just use the one cached */
	//! Used to set "hasItsChildrenStarChanged" boolean to true, so that it can no longer used cached childrenStar set
	/*!
		Used to set "hasItsChildrenStarChanged" boolean to true, so that it can no longer used cached childrenStar set
	*/
	void childrenStarHasBeingModified();

	std::vector<int>* followedByStar;									/**< A list of statement numbers; has the one following it, the one following the one following it, etc.
																 			 Used for the Follows* relationship */
	bool hasItsFollowedByStarChanged;									/**< Boolean control to check if a new followedByStar vector should be fetched, or just use the one cached */
	//! Used to set "hasItsFollowedByStarChanged" boolean to true, so that it can no longer used cached followedByStar vector
	/*!
		Used to set "hasItsFollowedByStarChanged" boolean to true, so that it can no longer used cached followedByStar vector
	*/
	void followedByStarHasBeingModified();

	std::vector<StatementTableStatement*>* previous;					/**< A list of pointers to statements that comes directly before this statement in the CFG */
	std::vector<StatementTableStatement*>* previousBIP;					/**< A list of pointers to statements that comes directly before this statement in the inter-procedure CFG */
    std::vector<StatementTableStatement*>* previousStar;    			/**< A list of pointers to all statements that comes before this statement in the CFG */
    std::vector<StatementTableStatement*>* previousStarBIP;				/**< A list of pointers to all statements that comes before this statement in the inter-procedure CFG */
    std::vector<StatementTableStatement*>* previousStarBIPCallsOnly;	/**< A list of pointers to all statements that comes before this statement in the inter-procedure CFG, for call statements *ONLY* */
	std::vector<StatementTableStatement*>* next;						/**< A list of pointers to statements that comes directly after this statement in the CFG */
	std::vector<StatementTableStatement*>* nextBIP;						/**< A list of pointers to statements that comes directly after this statement in the inter-procedure CFG */
    std::vector<StatementTableStatement*>* nextStar;					/**< A list of pointers to all statements that comes after this statement in the CFG */
    std::vector<StatementTableStatement*>* nextStarBIP;					/**< A list of pointers to all statements that comes after this statement in the inter-procedure CFG */
	std::vector<StatementTableStatement*>* affectsThis;					/**< A list of pointers to statements that affects this statement */
	std::vector<StatementTableStatement*>* affectedByThis;				/**< A list of pointers to statements that this statement affects */
public:
	static bool bonusImplementation;						/**< Boolean to control whether the Affects functions checks between inter functions */

	//! Constructor for the StatementTableStatement.
	/*!
		This is the constructor used to create a StatementTableStatement object.
		\param lineNumber The line number of the statement that this StatementTableStatement object is representing.
	*/
	StatementTableStatement(int lineNumber);

	~StatementTableStatement();

	//! Getter function for the statement number of the statement.
	/*!
		Getter function for the statement number of the statement; use this function
		to retrieve the statement number of the statement that this object is representing.
		\return The statement number of the statement that this item is representing has.
	*/
	int getStatementNumber();

	//! Getter function for right hand side expression of the assign statement.
	/*!
		Getter function for the right hand side expression of the assign statement.
		\return Expression, or "" if this is not an assign statement.
	*/
	std::string getRightHandSideExpression();

	//! Getter function for the control variable of the if/while statement.
	/*!
		Getter function for the control variable of the if/while statement.
		\return Control variable, or "" if this is not a if/while statement.
	*/
	NAME getControlVariable();

	//! Getter function for the index number of the procedure that it belongs to.
	/*!
		Getter function for the index number of the procedure that it belongs to.
		\return Index number of the procedure that it belongs to.
	*/
	int getProcedureIndexNumber();

	//! Checks if this is a statement assigned to be the parent of this statement.
	/*!
		Checks if this is a statement assigned to be the parent of this statement.
		\return True if there is, and false if there is not.
	*/
	bool hasParent();

	//! Getter function for the statement number of the parent of this statement.
	/*!
		Getter function for the statement number of the parent of this statement; use this function to
		retrieve the statement number of the parent of the statement that this object is representing.
		\return Statement number of this statement's parent, or 0 if it does not have any.
	*/
	int getParent();

	//! Getter function for pointer to the pointer pointing to the first statement of the procedure that this Calls statement calls
	/*!
		Getter function for pointer to the pointer pointing to the first statement of the procedure that this Calls statement calls;
		use this function to retrieve the pointer to the pointer pointing to the first statement of the procedure that this Calls statement calls.
		\return Pointer to the pointer pointing to the first statement of the procedure that this Calls statement calls, or NULL if it is not a Calls statement
	*/
	StatementTableStatement** getFirstCalls();

	//! Getter function for pointer to the pointer pointing to the last statement of the procedure that this Calls statement calls
	/*!
		Getter function for pointer to the pointer pointing to the last statement of the procedure that this Calls statement calls;
		use this function to retrieve the pointer to the pointer pointing to the last statement of the procedure that this Calls statement calls.
		\return Pointer to the pointer pointing to the last statement of the procedure that this Calls statement calls, or NULL if it is not a Calls statement
	*/
	StatementTableStatement** getLastCalls();

	//! Checks if this is a statement that this statement is following.
	/*!
		Checks if this is a statement that this statement is following.
		\return True if there is, and false if there is not.
	*/
	bool hasFollows();

	//! Checks if there is a statement following this statement.
	/*!
		Checks if there is a statement following this statement.
		\return True if there is, and false is there is not.
	*/
	bool hasFollowedBy();

	//! Setter function for the type of the statement that this object is representing.
	/*!
		Setter function for the type of the statement that this object is representing; use
		this function to assign the type of the statement that this object is representing.
		\param type TNodeType::Undefined, TNodeType::Assign, TNodeType::If, TNodeType::While, or TNodeType::Call
	*/
	void setType(TNodeType type);

	//! Setter function for the right hand side expression of this assign statement.
	/*!
		Setter function for the right hand side expression of this assign statement.
		Please don't call this function if this statement is not an assign statement!
		\param rightHandSideExpression Right hand side expression of this assign statement, ie. "x+y"
	*/
	void setRightHandSideExpression(std::string rightHandSideExpression);
	
	//! Setter function for the control variable of this if/while statement.
	/*!
		Setter function for the control variable of this if/while statement.
		Please don't call this function if this statement is not a if/while statement!
		\param controlVariable The name of the variable that controls this
	*/
	void setControlVariable(NAME controlVariable);

	//! Setter function for the index number of the procedure that it belongs to.
	/*!
		Setter function for the index number of the procedure that it belongs to.
		\param procedureIndexNumber The index number of the procedure that it belongs to
	*/
	void setProcedureIndexNumber(int procedureIndexNumber);

	//! Setter function for the pointer to the pointer pointing to the first statement of the procedure that this Calls statement calls.
	/*!
		Setter function for the pointer to the pointer pointing to the first statement of the procedure that this Calls statement calls;
		use this function to assign the pointer to the pointer pointing to the first statement of the procedure that this Calls statement calls.
		\param follows Pointer to the pointer pointing to the first statement of the procedure that this Calls statement calls, or NULL if it is not a Calls statement.
	*/
	void setFirstCalls(StatementTableStatement** firstCalls);

	//! Setter function for the pointer to the pointer pointing to the last statement of the procedure that this Calls statement calls.
	/*!
		Setter function for the pointer to the pointer pointing to the last statement of the procedure that this Calls statement calls;
		use this function to assign the pointer to the pointer pointing to the last statement of the procedure that this Calls statement calls.
		\param follows Pointer to the pointer pointing to the last statement of the procedure that this Calls statement calls, or NULL if it is not a Calls statement.
	*/
	void setLastCalls(StatementTableStatement** lastCalls);

	//! Setter function for the statement number of the statement that this statement is following.
	/*!
		Setter function for the statement number of the statement that this statement is following; use this function to
		assign the statement number of the statement that the statement that this object is representing is following.
		\param follows Pointer to the statement that this statement is following, or 0 if it does not have any.
	*/
	void setFollows(StatementTableStatement* follows);

	//! Setter function for the pointer to the statement that is following this statement.
	/*!
		Setter function for pointer to the statement that is following this statement; use this function to
		assign the pointer to the statement that is following the statement that this object is representing.
		\param followedBy Pointer to the statement that is following this statement, or NULL if it does not have any.
	*/
	void setFollowedBy(StatementTableStatement* followedBy);

	//! Setter function for the statement number of the parent of this statement.
	/*!
		Setter function for the statement number of the parent of this statement; use this function to
		assign the statement number of the parent of the statement that this object is representing.
		\param parent Pointer to this statement's parent, or 0 if it does not have any.
	*/
	void setParent(StatementTableStatement* parent);

	//! Allows the adding of statements that have this statement as a parent.
	/*!
		This function is used by the SIMPLE parser API to add statements that have this statement as a parent.
		\param child The pointer to the statement that havs this statement as a parent.
	*/
	void addChild(StatementTableStatement* child);
	
	//! Allows the adding of variables that this statement modifies.
	/*!
		This function is used by the SIMPLE parser API to add variables that this statement modifies.
		\param variableIndexNumber The index number of the variable that this statement modifies.
		\return True if this variable is successfully added, and false if this variable is already inside.
	*/
	bool addModifies(int variableIndexNumber);

	//! Allows the adding of variables that this statement uses.
	/*!
		This function is used by the SIMPLE parser API to add variables that this statement uses.
		\param variableIndexNumber The index number of the variable that this statement uses.
		\return True if this variable is successfully added, and false if this variable is already inside.
	*/
	bool addUses(int variableIndexNumber);

	//! Getter function for the statement number of the follows of this statement.
	/*!
		Getter function for the statement number of the follows of this statement; use this function to
		retrieve the statement number of the follows of the statement that this object is representing.
		\return Statement number of this statement's follows, or 0 if it does not have any.
	*/
	int getFollows();

	//! Getter function for the statement number of the followedBy of this statement.
	/*!
		Getter function for the statement number of the followedBy of this statement; use this function to
		retrieve the statement number of the followedBy of the statement that this object is representing.
		\return Statement number of this statement's follows, or 0 if it does not have any.
	*/
	int getFollowedBy();

	//! Getter function for the type of this statement.
	/*!
		Getter function for the type of this statement; use this function to
		retrieve the type of the statement that this object is representing.
		\return the type of the statement as defined by enum TNodeType.
	*/
	TNodeType getType();

	//! Getter function for members of the uses vector.
	/*!
		Getter function for the members of the uses vector; use this function to
		retrieve the individual variable index numbers that is used by this statement.
		As this function requires the passing of vector index number, which is generally
		unavailable outside, this function is recommended to be used only with the iteration
		of the entire vector.
		\param index The index number of the member of the variable inside the vector.
		\return The index number of the variables in the uses vector.
	*/
	int getUses(int index);

	//! Getter function for the size of the uses vector.
	/*!
		Getter function for the size of the uses vector; use this function to
		retrieve the number of variables that this statement uses.
		\return The number of variables that this statement uses.
	*/
	int getUsesSize();

	//! Getter function for members of the modifies vector.
	/*!
		Getter function for the members of the modifies vector; use this function to
		retrieve the individual variable index numbers that is modified by this statement.
		As this function requires the passing of vector index number, which is generally
		unavailable outside, this function is recommended to be used only with the iteration
		of the entire vector.
		\param index The index number of the member of the variable inside the vector.
		\return The index number of the variables in the modifies vector.
	*/
	int getModifies(int index);

	//! Getter function for the size of the modifies vector.
	/*!
		Getter function for the size of the modifies vector; use this function to
		retrieve the number of variables that this statement modifies.
		\return The number of variables that this statement modifies.
	*/
	int getModifiesSize();

	//! Getter function for members of the followsStar vector.
	/*!
		Getter function for the members of the followsStar vector; use this function to
		retrieve the individual statement numbers that follows* this statement.
		As this function requires the passing of vector index number, which is generally
		unavailable outside, this function is recommended to be used only with the iteration
		of the entire vector.
		\param index The index number of the member of the statement inside the vector.
		\return The statement number of the statement in the followsStar vector.
	*/
	int getFollowsStar(int index);

	//! Getter function for the size of the followsStar vector.
	/*!
		Getter function for the size of the followsStar vector; use this function to
		retrieve the number of statements that follows* this statement.
		\return The number of statements that follows* this statement.
	*/
	int getFollowsStarSize();

	//! Getter function for members of the followedByStar vector.
	/*!
		Getter function for the members of the followedByStar vector; use this function to
		retrieve the individual statement numbers that have this statement as Follows*.
		As this function requires the passing of vector index number, which is generally
		unavailable outside, this function is recommended to be used only with the iteration
		of the entire vector.
		\param index The index number of the member of the statement inside the vector.
		\return The statement number of the statement in the childrenStar vector.
	*/
	int getFollowedByStar(int index);

	//! Getter function for the size of the followedByStar vector.
	/*!
		Getter function for the size of the followedByStar vector; use this function to
		retrieve the number of statements that are Follows* of this statement.
		\return The number of statements that are Follows* of this statement.
	*/
	int getFollowedByStarSize();

	//! This function is called when the followedByStar vector is needed but is outdated.
	/*!
		This function is called when the followedByStar vector is needed but is outdated.
		A new vector would be generated.
	*/
	void fetchNewCopyOfFollowedByStar();

	//! Getter function for members of the children vector.
	/*!
		Getter function for the members of the children vector; use this function to
		retrieve the individual statement numbers that are children of this statement.
		As this function requires the passing of vector index number, which is generally
		unavailable outside, this function is recommended to be used only with the iteration
		of the entire vector.
		\param index The index number of the member of the statement inside the vector.
		\return The statement number of the statement in the children vector.
	*/
	int getChildren(int index);

	//! Getter function for the size of the children vector.
	/*!
		Getter function for the size of the children vector; use this function to
		retrieve the number of statements that are children of this statement.
		\return The number of statements that are children of this statement.
	*/
	int getChildrenSize();

	//! Getter function for members of the parentStar vector.
	/*!
		Getter function for the members of the parentStar vector; use this function to
		retrieve the individual statement numbers that are parent* of this statement.
		As this function requires the passing of vector index number, which is generally
		unavailable outside, this function is recommended to be used only with the iteration
		of the entire vector.
		\param index The index number of the member of the statement inside the vector.
		\return The statement number of the statement in the parentStar vector.
	*/
	int getParentStar(int index);

	//! Getter function for the size of the parentStar vector.
	/*!
		Getter function for the size of the parentStar vector; use this function to
		retrieve the number of statements that are parent* of this statement.
		\return The number of statements that are parent* of this statement.
	*/
	int getParentStarSize();

	//! Getter function for members of the childrenStar vector.
	/*!
		Getter function for the members of the childrenStar vector; use this function to
		retrieve the individual statement numbers that have this statement as parent*.
		As this function requires the passing of vector index number, which is generally
		unavailable outside, this function is recommended to be used only with the iteration
		of the entire vector.
		\param index The index number of the member of the statement inside the vector.
		\return The statement number of the statement in the childrenStar vector.
	*/
	int getChildrenStar(int index);

	//! Getter function for the size of the childrenStar vector.
	/*!
		Getter function for the size of the parentStar vector; use this function to
		retrieve the number of statements that are parent* of this statement.
		\return The number of statements that are parent* of this statement.
	*/
	int getChildrenStarSize();

	//! This function is called when the childrenStar vector is needed but is outdated.
	/*!
		This function is called when the childrenStar vector is needed but is outdated.
		A new vector would be generated.
	*/
	void fetchNewCopyOfChildrenStar();

	//! Getter function for the list of pointers to statements that has this statement after directly in the CFG
	/*!
		Getter function for list of pointers to statements that has this statement after directly in the CFG;
		use this function to retrieve a vector of pointers to statements s which Next(s, this) is true
		\param isBip Set this to true to include inter-procedures, or false to exclude
		\return List of pointers to statements that this statement has this statement after directlyin the CFG
	*/
	std::vector<StatementTableStatement*>* getPrevious(bool isBip = false);

	//! Getter function for the list of pointers to statements that comes after this statement directly in the CFG
	/*!
		Getter function for list of pointers to statements that comes after this statement directly in the CFG;
		use this function to retrieve a vector of pointers to statements s which Next(this, s) is true
		\param isBip Default is false. Set this to true to include inter-procedures
		\return List of pointers to statements that comes after this statement directly in the CFG
	*/
	std::vector<StatementTableStatement*>* getNext(bool isBip = false);

	//! Getter function for the list of pointers to statements that has this statement after directly or indirectly in the CFG
	/*!
		Getter function for list of pointers to statements that has this statement after directly or indirectly in the CFG;
		use this function to retrieve a vector of pointers to statements s which Next*(s, this) is true
		\param isBip Default is false. Set this to true to include inter-procedures
		\return List of pointers to statements that this statement has this statement after directly or indirectly in the CFG
	*/
	std::vector<StatementTableStatement*>* getPreviousStar(bool isBip);

	//! Getter function for the list of pointers to statements that comes after this statement directly or indirectly in the CFG
	/*!
		Getter function for list of pointers to statements that comes after this statement directly or indirectly in the CFG;
		use this function to retrieve a vector of pointers to statements s which Next*(this, s) is true
		\param isBip Set this to true to include inter-procedures, or false to exclude
		\return List of pointers to statements that comes after this statement directly or indirectly in the CFG
	*/
	std::vector<StatementTableStatement*>* getNextStar(bool isBip);

	//! Getter function for the list of pointers to statements that directly affects this statement
	/*!
		Getter function for list of pointers to statements that directly affects this statement;
		use this function to retrieve a vector of pointers to statements s which Affects(s, this) is true
		\return List of pointers to statements that directly affects this statement
	*/
	std::vector<StatementTableStatement*>* getAffectsThis();

	//! Getter function for the list of pointers to statements that this statement directly affects
	/*!
		Getter function for list of pointers to statements that this statement directly affects;
		use this function to retrieve a vector of pointers to statements s which Affects(this, s) is true
		\return List of pointers to statements that this statement directly affects
	*/
	std::vector<StatementTableStatement*>* getAffectedByThis();

	//! Getter function for the list of pointers to statements that directly or indirectly affects this statement
	/*!
		Getter function for list of pointers to statements that directly or indirectly affects this statement;
		use this function to retrieve a vector of pointers to statements s which Affects*(s, this) is true
		\return List of pointers to statements that directly or indirectly affects this statement
	*/
	std::vector<StatementTableStatement*> getAffectsThisStar();

	//! Getter function for the list of pointers to statements that this statement directly or indirectly affects
	/*!
		Getter function for list of pointers to statements that this statement directly or indirectly affects;
		use this function to retrieve a vector of pointers to statements s which Affects*(this, s) is true
		\return List of pointers to statements that this statement directly or indirectly affects
	*/
	std::vector<StatementTableStatement*> getAffectedByThisStar();

	//! Getter function for statement numbers of statements that has this statement as sibling
	/*!
		Getter function for statement numbers of statements that has this statement as sibling; use this function 
		to retrieve a vector of statement numbers of statements s which Sibling(this, s)/Sibling(s, this) is true
		\return List of statement numbers of statements that has this statement as sibling
	*/
	std::vector<int> getSiblings();

	//! Setter function for pointers to statements that has this statement after directly in the CFG
	/*!
		Setter function for pointers to statements that has this statement after directly in the CFG;
		use this function to set pointers to statements s which NextBIP(s, this) is true.
		\param callStatement Pointer to statements that has this statement after directly in the CFG
	*/
	void addPreviousBIP(StatementTableStatement* callStatement);
};