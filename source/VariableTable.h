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

	//! Returns the VariableTableVariable object that has the name of the variable you want from the variable table.
	/*!
		Returns the VariableTableVariable object that has the name of the variable you want from the variable table.
		If no object with such name exists in the table, a new one would be created and returned.
		\param name The name of the variable of which VariableTableVariable object you are looking for.
		\return The VariableTableVariable object with the same name as the variable name passed.
	*/
	VariableTableVariable* getVariableObject(NAME name);

	//! Use this function to add a variable into the variable table.
	/*!
		This function is used to add a variable into the variable table.
		\param variable The name of the variable that is being added.
		\return The newly created VariableTableVariable object.
	*/
	VariableTableVariable* addVariable(NAME variable);
};