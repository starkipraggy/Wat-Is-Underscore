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

class StatementTableStatement : public DataStructureObject {
private:
	int lineNumber;						/**< The line number of this statement */
	TNodeType type;						/**< The type of this statement */

	StatementTableStatement* follows;	/**< The pointer to the statement that this statement follows
											 (this statement appears immediately after the one it follows) */
	int followedBy;						/**< The statement number of the statement that this statement is followed
											 (that statement appears immediately after this statement ) */
	StatementTableStatement* parent;	/**< The pointer to the statement that is parent to this statement */
	std::vector<int>* children;			/**< A list of the statement numbers of statements that have this statement as parent */
	std::vector<int>* modifies;			/**< A list of the index numbers of variables that this statement modifies */
	std::vector<int>* uses;				/**< A list of the index numbers of variables that this statement uses */

	std::vector<int>* parentStar;		/**< A list of statement numbers; has its parent, its parent's parent, etc.
											 Used for the Parent* relationship */
	std::vector<int>* followsStar;		/**< A list of statement numbers; has the statement it follows, its follow's follow, etc.
											 Used for the Follows* relationship */
public:
	//! Constructor for the StatementTableStatement.
	/*!
		This is the constructor used to create a StatementTableStatement object.
		\param lineNumber The line number of the statement that this StatementTableStatement object is representing.
		\param index The index number of the statement that this VariableTableVariable object is representing has.
	*/
	StatementTableStatement(int lineNumber, int index);

	~StatementTableStatement();

	//! Checks if this is a statement assigned to be the parent of this statement.
	/*!
		Checks if this is a statement assigned to be the parent of this statement.
		\return True if there is, and false if there is not.
	*/
	bool hasParent();

	//! Checks if this is a statement that this statement is following.
	/*!
		Checks if this is a statement that this statement is following.
		\return True if there is, and false if there is not.
	*/
	bool hasFollows();

	//! Getter function for the statement number of the parent of this statement.
	/*!
		Getter function for the statement number of the parent of this statement; use this function to
		retrieve the statement number of the parent of the statement that this object is representing.
		\return Statement number of this statement's parent, or 0 if it does not have any.
	*/
	int getParent();

	//! Setter function for the type of the statement that this object is representing.
	/*!
		Setter function for the type of the statement that this object is representing; use
		this function to assign the type of the statement that this object is representing.
		\param type TNodeType::Undefined, TNodeType::Assign, TNodeType::If, TNodeType::While, or TNodeType::Call
	*/
	void setType(TNodeType type);

	//! Setter function for the statement number of the statement that this statement is following.
	/*!
		Setter function for the statement number of the statement that this statement is following; use this function to
		assign the statement number of the statement that the statement that this object is representing is following.
		\param follows Pointer to the statement that this statement is following, or 0 if it does not have any.
	*/
	void setFollows(StatementTableStatement* follows);

	//! Setter function for the statement number of the statement that is following this statement.
	/*!
		Setter function for the statement number of the statement that is following this statement; use this function to
		assign the statement number of the statement that is following the statement that this object is representing.
		\param followedBy Statement number of the statement that is following this statement, or 0 if it does not have any.
	*/
	void setFollowedBy(int followedBy);

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
		\param child The statement number of the statement that havs this statement as a parent.
	*/
	void addChild(int child);
	
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

};
