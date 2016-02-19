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

template<typename T>
bool addIntoVector(T item, std::vector<T>* vector);