/*! \class VariableTable
	\brief Used to represent the table of variables in the Program Knowledge Base (PKB).

	This class is used to represent the tables of variables in Program Knowledge Base (PKB), and
	stores a list of variables, as well as their relationships with other statements.
*/

#pragma once

#include "VariableTableVariable.h"

class VariableTable {
private:
	std::vector<VariableTableVariable*>* variables;	/**< List of variables in the table */
public:
	VariableTable();
	~VariableTable();

	//! Returns the number of variables in the variable table.
	/*!
		Returns the number of variables in the variable table.
		Uses the size() function of the std::vector class.
	*/
	int getNumberOfVariables();

	//! Use this function to add a variable into the variable table.
	/*!
		This function is used to add a variable into the variable table.
		\param variable The name of the variable that is being added.
	*/
	void addVariable(NAME variable);
};