/*! \file DataStructureFunctions.h
	\brief Contains functions that faciliate the usage of data structures like vectors.

	This header file contains function definitions of functions that faciliate the usage
	of data structures like vectors, and would tend to be used by classes that have to
	handle said data structures, like ProcedureTableProcedure, and StatementTableStatement.
*/

#include <vector>

//! Private function to control the adding of items into vectors.
/*!
	Private function called by functions such as addModifies() and addUses(). Generally ensures to check
	whether the item already exists inside of the vector to ensure no duplication.
	\param item The item being added into the vector.
	\param vector The vector in which the item is being added into.
	\return True if the items is successfully added, and false if the item is already inside.
*/
bool addIntoVector(int item, std::vector<int>* vector);