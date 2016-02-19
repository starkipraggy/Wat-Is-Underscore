/*! \class VariableTableVariable
	\brief Used to represent the individual variables in the VariableTable in the PKB.

	This class is used to represent the individual variables that makes up the table of
	variables in the Program Knowledge Base (PKB). Each VariableTableVariable object
	keeps track of individual information such as name of variable, the index given to
	it, as well as lists of its relationships with statements.
*/

#pragma once

#include "TypeDef.h"
#include "DataStructureObject.h"

class VariableTableVariable {
private:
	NAME name;								/**< The name used to refer to the variable */
	int index;								/**< The index number assigned to the item this object is representing */
	
	std::vector<int>* statementModifies;	/**< A list of the statement numbers of statements that modifies this variable */
	std::vector<int>* statementUses;		/**< A list of the statement numbers of statements that uses this variable */
	std::vector<int>* procedureModifies;	/**< A list of the index numbers of procedures that modifies this variable */
	std::vector<int>* procedureUses;		/**< A list of the index numbers of procedures that uses this variable */
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
		Getter function for the name of the variable; use this function to retrieve the name of the variable that this object is representing.
		\return The name of the variable that this object is representing has.
	*/
	NAME getName();

	//! Getter function for the index number of the item.
	/*!
		Getter function for the index number of the item; use this function
		to retrieve the index number of the item that this object is representing.
		\return The index number of the variable that this item is representing has.
	*/
	int getIndex();
	
	//! Allows the adding of statements that modify this variable.
	/*!
		This function is used by the SIMPLE parser API to add statements that modify this variable.
		\param statementNumber The statement number of the statement that modify this variable.
		\return True if this statement is successfully added, and false if this statement is already inside.
	*/
	bool addStatementModifies(int statementNumber);

	//! Allows the adding of statements that use this variable.
	/*!
		This function is used by the SIMPLE parser API to add statements that use this variable.
		\param statementNumber The statement number of the statement that use this variable.
		\return True if this statement is successfully added, and false if this statement is already inside.
	*/
	bool addStatementUses(int statementNumber);
	
	//! Allows the adding of procedures that modify this variable.
	/*!
		This function is used by the SIMPLE parser API to add procedures that modify this variable.
		\param procedureNumber The index number of the procedure that modify this variable.
		\return True if this procedure is successfully added, and false if this procedure is already inside.
	*/
	bool addProcedureModifies(int procedureNumber);

	//! Allows the adding of procedures that use this variable.
	/*!
		This function is used by the SIMPLE parser API to add procedures that use this variable.
		\param procedureNumber The index number of the procedure that use this variable.
		\return True if this procedure is successfully added, and false if this procedure is already inside.
	*/
	bool addProcedureUses(int procedureNumber);
	
	//! Getter function for members of the procedureUses vector.
	/*!
		Getter function for the members of the procedureUses vector; use this function to
		retrieve the individual procedure index numbers that uses the variable.
		As this function requires the passing of vector index number, which is generally
		unavailable outside, this function is recommended to be used only with the iteration
		of the entire vector.
		\param index The index number of the member of the procedure inside the vector.
		\return The index number of the procedures in the procedureUses vector.
	*/
	int getProceduresUses(int index);
	
	//! Getter function for the size of the procedureUses vector.
	/*!
		Getter function for the size of the procedureUses vector; use this function to
		retrieve the number of procedures that uses this variable.
		\return The number of procedures that uses this variable.
	*/
	int getProceduresUsesSize();
	
	//! Getter function for members of the procedureModifies vector.
	/*!
		Getter function for the members of the procedureModifies vector; use this function to
		retrieve the individual procedure index numbers that modifies the variable.
		As this function requires the passing of vector index number, which is generally
		unavailable outside, this function is recommended to be used only with the iteration
		of the entire vector.
		\param index The index number of the member of the procedure inside the vector.
		\return The index number of the procedures in the procedureModifies vector.
	*/
	int getProceduresModifies(int index);

	//! Getter function for the size of the procedureModifies vector.
	/*!
		Getter function for the size of the procedureModifies vector; use this function to
		retrieve the number of procedures that modifies this variable.
		\return The number of procedures that modifies this variable.
	*/
	int getProceduresModifiesSize();
	
	//! Getter function for members of the statementUses vector.
	/*!
		Getter function for the members of the statementUses vector; use this function to
		retrieve the individual statement numbers that uses the variable.
		As this function requires the passing of vector index number, which is generally
		unavailable outside, this function is recommended to be used only with the iteration
		of the entire vector.
		\param index The index number of the member of the statement inside the vector.
		\return The statement number of the statement in the statementUses vector.
	*/
	int getStatementUses(int index);

	//! Getter function for the size of the statementUses vector.
	/*!
		Getter function for the size of the statementUses vector; use this function to
		retrieve the number of statements that uses this variable.
		\return The number of statements that uses this variable.
	*/
	int getStatementUsesSize();

	//! Getter function for members of the statementModifies vector.
	/*!
		Getter function for the members of the statementModifies vector; use this function to
		retrieve the individual statement numbers that modifies the variable.
		As this function requires the passing of vector index number, which is generally
		unavailable outside, this function is recommended to be used only with the iteration
		of the entire vector.
		\param index The index number of the member of the statement inside the vector.
		\return The statement number of the statement in the statementModifies vector.
	*/
	int getStatementModifies(int index);

	//! Getter function for the size of the statementModifies vector.
	/*!
		Getter function for the size of the statementModifies vector; use this function to
		retrieve the number of statements that modifies this variable.
		\return The number of statements that modifies this variable.
	*/
	int getStatementModifiesSize();
};