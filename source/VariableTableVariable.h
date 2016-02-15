/*! \class VariableTableVariable
	\brief Used to represent the individual variables in the VariableTable in the PKB.

	This class is used to represent the individual variables that makes up the table of
	variables in the Program Knowledge Base (PKB). Each VariableTableVariable object
	keeps track of individual information such as name of variable, the index given to
	it, as well as lists of its relationships with statements.
*/

#pragma once

#include "TypeDef.h"
#include <vector>

class VariableTableVariable {
private:
	NAME name;					/**< The name used to refer to the variable */
	int index;					/**< The index number given to the variable */

	std::vector<int>* modifies;	/**< A list of the statement numbers of statements that modifies this variable */
	std::vector<int>* uses;		/**< A list of the statement numbers of statements that uses this variable */
public:
	//! Constructor for the VariableTableVariable.
	/*!
		This is the constructor used to create a VariableTableVariable object.
		\param name The name of the variable that this VariableTableVariable object is representing has.
		\param index The index number of the variable that this VariableTableVariable object is representing has.
	*/
	VariableTableVariable(NAME name, int index);

	~VariableTableVariable();

	//! Getter function for the name of the variable.
	/*!
		Getter function for the name of the variable; use this function to retrieve the name of the variable that this object is reprensenting.
		\return The name of the variable that this object is representing has.
	*/
	NAME getName();

	//! Getter function for the index number of the variable.
	/*!
		Getter function for the index number of the variable; use this function to retrieve the index number of the variable that this object is reprensenting.
		\return The index number of the variable that this object is representing has.
	*/
	int getIndex();
};