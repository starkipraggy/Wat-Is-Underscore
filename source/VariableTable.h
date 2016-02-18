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

	//! Use this function to add a variable into the variable table.
	/*!
		This function is used to add a variable into the variable table.
		It is set as private to prevent the adding of variables with the same name.
		To add a variable that does not exist in the table, use the getVariableObject() function.
		\param variable The name of the variable that is being added.
		\return The newly created VariableTableVariable object.
	*/
	VariableTableVariable* addVariable(NAME variable);
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
	VariableTableVariable* getVariableUsingName(NAME name);

	//! Returns the VariableTableVariable object that has the index of the variable you want from the variable table.
	/*!
		Returns the VariableTableVariable object that has the index of the variable you want from the variable table.
		\param variableIndex The index of the variable of which VariableTableVariable object you are looking for.
		\return The VariableTableVariable object with the same index as the variable name passed, or NULL if it does not exist.
	*/
	VariableTableVariable* getVariableUsingVariableIndexNumber(int variableIndex);

	//! Returns the VariableTableVariable object that has the index of the variable you want in the variables vector.
	/*!
		Returns the VariableTableVariable object that has the index of the variable you want in the variables vector.
		Passing 0 in gets the first object in the vector, and passing in size-1 gets the last.
		\param vectorIndex The index of the variable in the vector of which VariableTableVariable object you are looking for.
		\return The VariableTableVariable object with the same index as the variable name passed, or NULL if vectorIndex passed
				in is less than 0 or more than size-1.
	*/
	VariableTableVariable* getVariableUsingVectorIndexNumber(int vectorIndex);
};