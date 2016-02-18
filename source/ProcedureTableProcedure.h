/*! \class ProcedureTableProcedure
	\brief Used to represent the individual procedures in the ProcedureTable in the PKB.

	This class is used to represent the individual procedures that makes up the table of
	procedures in the Program Knowledge Base (PKB). Each ProcedureTableProcedure object
	keeps track of individual information such as name of procedure, the index given to
	it, as well as lists of its relationships with other statements and variables.
*/

#pragma once

#include <string>
#include <set>
#include "TypeDef.h"
#include "DataStructureObject.h"

class ProcedureTableProcedure : public DataStructureObject {
private:
	std::string name;										/**< The name of this procedure */
	std::vector<int>* statements;							/**< The index numbers of the statements that belongs to this procedure */

	std::vector<int>* modifies;								/**< A list of the index numbers of variables that this procedure modifies */
	std::vector<int>* uses;									/**< A list of the index numbers of variables that this procedure uses */
	std::vector<ProcedureTableProcedure*>* procedureCalls;	/**< A list of pointers to procedures that call this procedure */
	std::vector<int>* statementCalls;						/**< A list of the index numbers of statements that call this procedure */

	std::set<int>* indirectProcedureCalls;					/**< This set is used as a cache for the getIndirectProcedureCalls() function,
																 as the function is computationally expensive when data set is large */
	bool isIndirectProcedureCallsModified;					/**< Boolean used as a control on whether to use cached set */
public:
	//! Constructor for the ProcedureTableProcedure.
	/*!
		This is the constructor used to create a ProcedureTableProcedure object.
		\param name The name of the procedure that this ProcedureTableProcedure object is representing has.
		\param index The index number of the procedure that this ProcedureTableProcedure object is representing has.
	*/
	ProcedureTableProcedure(std::string name, int index);

	~ProcedureTableProcedure();

	//! Getter function for the name of the procedure.
	/*!
		Getter function for the name of the procedure; use this function to
		retrieve the name of the procedure that this object is reprensenting.
		\return The name of the procedure that this object is representing has.
	*/
	std::string getName();

	//! Getter function for members of the procedure calls vector.
	/*!
		Getter function for the members of the procedure calls vector; use this function to
		retrieve the individual pointer to procedures that are calling this procedure.
		As this function requires the passing of vector index number, which is generally
		unavailable outside, this function is recommended to be used only with the iteration
		of the entire vector.
		\param index The index number of the member of the procedure calls vector inside the vector.
		\return The pointer to the procedure in the procedure calls vector.
	*/
	ProcedureTableProcedure* getProcedureCall(int index);

	//! Getter function for the size of the procedure calls vector.
	/*!
		Getter function for the size of the procedure calls vector; use this function to
		retrieve the number of procedures that are calling this procedure.
		\return The number of procedures that are calling this procedure.
	*/
	int getProcedureCallsSize();

	//! Returns a list of index numbers of procedures that calls this procedure, whether directly or indirectly.
	/*!
		This function compiles a list of the index numbers of procedures that calls this procedure, whether directly
		or indirectly, and returns it as a set, as no duplicates are needed.
		\return A list of index numbers of procedures that calls this procedure.
	*/
	std::set<int>* getIndirectProcedureCalls();

	//! Getter function for members of the statement calls vector.
	/*!
		Getter function for the members of the statement calls vector; use this function to
		retrieve the individual statement number of statements that are calling this procedure.
		As this function requires the passing of vector index number, which is generally
		unavailable outside, this function is recommended to be used only with the iteration
		of the entire vector.
		\param index The index number of the member of the statement calls vector inside the vector.
		\return The index number of the statement in the statement calls vector.
	*/
	int getStatementCall(int index);

	//! Getter function for the size of the statement calls vector.
	/*!
		Getter function for the size of the statement calls vector; use this function to
		retrieve the number of statements that are calling this procedure.
		\return The number of statements that are calling this procedure.
	*/
	int getStatementCallsSize();

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

	//! Allows the adding of procedures that calls this procedure.
	/*!
	This function is used by the SIMPLE parser API to add procedures that call this procedure.
	\param procedureIndexNumber The index number of the procedure that call this procedure.
	\return True if this procedure is successfully added, and false if this procedure is already inside.
	*/
	bool addProcedureCalls(ProcedureTableProcedure* procedure);

	//! Allows the adding of statements that calls this procedure.
	/*!
	This function is used by the SIMPLE parser API to add statements that call this procedure.
	\param statementIndexNumber The index number of the statement that call this procedure.
	\return True if this statement is successfully added, and false if this statement is already inside.
	*/
	bool addStatementsCalls(int statementIndexNumber);
	int getUses(int index);
	int getUsesSize();
};
