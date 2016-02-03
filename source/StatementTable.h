/*! \class StatementTable
	\brief Used to represent the table of statement in the Program Knowledge Base (PKB).

	This class is used to represent the tables of statements in Program Knowledge Base (PKB), and
	stores a list of statements, as well as their relationships with other statements.
*/

#pragma once

#include "StatementTableStatement.h"

class StatementTable {
private:
	std::vector<StatementTableStatement*>* statements;	/**< List of statements in the table */
public:
	StatementTable();
	~StatementTable();

	//! Returns the number of statements in the statement table.
	/*!
		Returns the number of statements in the statement table.
		Uses the size() function of the std::vector class.
	*/
	int getNumberOfStatements();

	//! Use this function to add a statement into the statement table.
	/*!
		This function is used to add a statement into the statement table.
		\param lineNumber The oline number of the statement that is being added.
	*/
	void addStatement(int lineNumber);
};