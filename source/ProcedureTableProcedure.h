/*! \class ProcedureTableProcedure
	\brief Used to represent the individual procedures in the ProcedureTable in the PKB.

	This class is used to represent the individual procedures that makes up the table of
	procedures in the Program Knowledge Base (PKB). Each ProcedureTableProcedure object
	keeps track of individual information such as name of procedure, the index given to
	it, as well as lists of its relationships with other statements and variables.
*/

#pragma once

#include <vector>
#include <string>
#include "TypeDef.h"

class ProcedureTableProcedure {
private:
	std::string name;			/**< The name of this procedure */
	int index;					/**< The index number given to the procedure */
	std::vector<int>* statements;	/**< The index numbers of the statements that belongs to this procedure */

	std::vector<int>* modifies;	/**< A list of the index numbers of variables that this procedure modifies */
	std::vector<int>* uses;		/**< A list of the index numbers of variables that this procedure uses */
public:
	//! Constructor for the ProcedureTableProcedure.
	/*!
		This is the constructor used to create a ProcedureTableProcedure object.
		\param name The name of the procedure that this ProcedureTableProcedure object is representing has.
		\param index The index number of the procedure that this ProcedureTableProcedure object is representing has.
	*/
	ProcedureTableProcedure(std::string name, int index);

	~ProcedureTableProcedure();

	//! Allows the adding of index numbers of additional statements that belongs to this procedure.
	/*!
		This function is used by the SIMPLE parser API to add index numbers of additional statements that belongs to this procedure.
		\param statement The index number of the statement that belongs to this procedure.
	*/
	void addStatement(int statement);
	
	//! Allows the adding of variables that this procedure modifies.
	/*!
		This function is used by the SIMPLE parser API to add variables that this procedure modifies.
		\param variableIndexNumber The index number of the variable that this procedure modifies.
		\return True if this variable is successfully added, and false if this variable is already inside.
	*/
	bool addModifies(int variableIndexNumber);

	//! Allows the adding of variables that this procedure uses.
	/*!
		This function is used by the SIMPLE parser API to add variables that this procedure uses.
		\param variableIndexNumber The index number of the variable that this procedure uses.
		\return True if this variable is successfully added, and false if this variable is already inside.
	*/
	bool addUses(int variableIndexNumber);
};
