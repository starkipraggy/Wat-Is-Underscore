/*! \class ProcedureTable
	\brief Used to represent the table of procedures in the Program Knowledge Base (PKB).

	This class is used to represent the tables of procedures in Program Knowledge Base (PKB), and
	stores a list of procedures, as well as their relationships with statements and variables.
*/

#pragma once

#include "ProcedureTableProcedure.h"

class ProcedureTable {
private:
	std::vector<ProcedureTableProcedure*>* procedures;	/**< List of procedures in the table */

	//! Use this function to add a procedure into the procedure table.
	/*!
		This function is used to add a procedure into the procedure table.
		It is set as private to prevent the adding of variables with the same name.
		To add a variable that does not exist in the table, use the getVariableObject() function.
		\param procedure The name of the procedure that is being added.
		\return The pointer to the ProcedureTableProcedure object that was just added.
	*/
	ProcedureTableProcedure* addProcedure(std::string procedure);
public:
	ProcedureTable();
	~ProcedureTable();

	//! Returns the number of procedures in the procedure table.
	/*!
		Returns the number of procedures in the procedure table.
		Uses the size() function of the std::vector class.
		\return The number of procedures in the proedure table.
	*/
	int getNumberOfProcedures();

	//! Given a specific index number of a procedure, look for the object representing that procedure in the ProcedureTable.
	/*!
		Given a specific index number of a procedure, look for the object representing
		that procedure in the ProcedureTable, and returns it.
		\param procedureIndex The index number of the procedure that you would like to retrieve.
		\return The ProcedureTableProcedure object that has the index number passed in, or NULL if
		this procedure cannot be found in the ProcedureTable.
	*/
	ProcedureTableProcedure* getProcedure(int procedureNumber);

	//! Given the name of a procedure, look for the object representing that procedure in the ProcedureTable.
	/*!
		Given the name of a procedure, look for the object representing that procedure in the ProcedureTable, and returns it.
		If no object with such name exists in the table, a new one would be created and returned.
		\param procedureName The name of the procedure that you would like to retrieve.
		\return The ProcedureTableProcedure object with the same name as the procedure name passed in.
	*/
	ProcedureTableProcedure* getProcedure(std::string procedureName);
};
