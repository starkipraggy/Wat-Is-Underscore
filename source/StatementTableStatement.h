/*! \class StatementTableStatement
	\brief Used to represent the individual statements in the StatementTable in the PKB.

	This class is used to represent the individual statements that makes up the table of
	statements in the Program Knowledge Base (PKB). Each StatementTableStatement object
	keeps track of individual information such as line number of statement, the index
	given to it, as well as lists of its relationships with other statements and variables.
*/

#pragma once

#include <vector>

class StatementTableStatement {
private:
	int lineNumber;				/**< The line number of this statement */
	int index;					/**< The index number given to the statement */

	int follows;				/**< The statement number of the statement that this statement follows */
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
};
