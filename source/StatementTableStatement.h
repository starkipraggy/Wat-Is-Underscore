/*! \class StatementTableStatement
	\brief Used to represent the individual statements in the StatementTable in the PKB.

	This class is used to represent the individual statements that makes up the table of
	statements in the Program Knowledge Base (PKB). Each StatementTableStatement object
	keeps track of individual information such as line number of statement, the index
	given to it, as well as lists of its relationships with other statements and variables.
*/

#pragma once

#include "DataStructureFunctions.h"

class StatementTableStatement {
private:
	int lineNumber;				/**< The line number of this statement */
	int index;					/**< The index number given to the statement */

	int follows;				/**< The statement number of the statement that this statement follows
									 (this statement appears immediately after the one it follows) */
	int parent;					/**< The statement number of the statement that is parent to this statement */
	std::vector<int>* modifies;	/**< A list of the index numbers of variables that this statement modifies */
	std::vector<int>* uses;		/**< A list of the index numbers of variables that this statement uses */
public:
	//! Constructor for the StatementTableStatement.
	/*!
		This is the constructor used to create a StatementTableStatement object.
		\param lineNumber The line number of the statement that this StatementTableStatement object is representing.
		\param index The index number of the statement that this VariableTableVariable object is representing has.
	*/
	StatementTableStatement(int lineNumber, int index);

	~StatementTableStatement();

	//! Getter function for the index number of the statement.
	/*!
		Getter function for the statement number of the statement; use this function to
		retrieve the statement number of the statement that this object is reprensenting.
		\return The statement number of the statement that this object is representing has.
	*/
	int getIndex();

	//! Checks if this is a statement assigned to be the parent of this statement.
	/*!
		Checks if this is a statement assigned to be the parent of this statement.
		\return True if there is, and false if there is not.
	*/
	bool hasParent();

	//! Getter function for the statement number of the parent of this statement.
	/*!
		Getter function for the statement number of the parent of this statement; use this function to
		retrieve the statement number of the parent of the statement that this object is reprensenting.
		\return Statement number of this statement's parent, or 0 if it does not have any.
	*/
	int getParent();

	//! Setter function for the statement number of the statement that this statement is following.
	/*!
		Setter function for the statement number of the statement that this statement is following; use this function to
		assign the statement number of the statement that the statement that this object is reprensenting is following.
		\param follows Statement number of the statement that this statement is following, or 0 if it does not have any.
	*/
	void setFollows(int follows);

	//! Setter function for the statement number of the parent of this statement.
	/*!
		Setter function for the statement number of the parent of this statement; use this function to
		assign the statement number of the parent of the statement that this object is reprensenting.
		\param parent Statement number of this statement's parent, or 0 if it does not have any.
	*/
	void setParent(int parent);
	
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
};
