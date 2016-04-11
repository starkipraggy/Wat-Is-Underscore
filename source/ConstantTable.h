/*! \class ConstantTable
	\brief Used to represent the table of constants in the Program Knowledge Base (PKB).

	This class is used to represent the tables of constants in Program Knowledge Base (PKB), and
	stores a list of constants.
*/

#pragma once

#include <vector>

class ConstantTable {
private:
	std::vector<int>* constants;	/**< List of constants in the table */
public:
	ConstantTable();
	~ConstantTable();

	//! Use this function to add a constant into the constant table.
	/*!
		This function is used to add a constant into the constant table.
		It checks if said constant already exists in the table, to prevent
		duplicates from being added.
		\param constant The value of the constant that is being added.
		\return True if the constant is successfully added, and false if the constant already exists in the table.
	*/
	bool addConstant(int constant);

	//! Returns the number of constants in the constant table.
	/*!
		Returns the number of constants in the constant table.
		Uses the size() function of the std::vector class.
		\return The number of constants in the constant table.
	*/
	unsigned int getNumberOfConstants();

	//! Given a specific index number of a constant in the vector of constants, return the value of the constant in that index in the ProcedureTable.
	/*!
		Given a specific index number of a constant in the vector of constants,
		return the value of the constant in that index in the ProcedureTable.
		\param index The index number of the constant that you would like to retrieve.
		\return The value of the constant.
	*/
	int getConstant(int index);
};