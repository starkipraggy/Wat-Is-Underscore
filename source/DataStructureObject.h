/*! \class DataStructureObject
	\brief Used to represent the individual statements in the StatementTable in the PKB.

	As the data structures used in the Program Knowledge Base (PKB) share similar
	information, such as the Modifies and Uses relationships, this class is written
	as a parent class for these data structures to inherit to prevent the rewriting
	of similar code. Currently, the StatementTableStatement, VariableTableVariable,
	and ProcedureTableProcedure classes inherit from this class	as a parent class.
	This class is an abstract class, and thus objects cannot be created from it.
*/

#pragma once

#include <vector>

class DataStructureObject {
protected:
	int index;	/**< The index number assigned to the item this object is representing */

	//! Private function to control the adding of items into vectors.
	/*!
		Private function called by functions such as addModifies() and addUses(). Generally ensures to check
		whether the item already exists inside of the vector to ensure no duplication.
		\param item The item being added into the vector.
		\param vector The vector in which the item is being added into.
		\return True if the items is successfully added, and false if the item is already inside.
	*/
	template<typename T>
	static bool addIntoVector(T item, std::vector<T>* vector);
public:
	//! Getter function for the index number of the item.
	/*!
		Getter function for the index number of the item; use this function
		to retrieve the index number of the item that this object is reprensenting.
		\return The index number of the variable that this item is representing has.
	*/
	int getIndex();

	// @todo make this an abstract class
};