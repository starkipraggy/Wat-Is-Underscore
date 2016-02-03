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
public:
	ProcedureTable();
	~ProcedureTable();

	//! Returns the number of procedures in the procedure table.
	/*!
		Returns the number of procedures in the procedure table.
		Uses the size() function of the std::vector class.
	*/
	int getNumberOfProcedures();

	//! Use this function to add a procedure into the procedure table.
	/*!
		This function is used to add a procedure into the procedure table.
		\param procedure The name of the procedure that is being added.
	*/
	void addProcedure(std::string procedure);
};
