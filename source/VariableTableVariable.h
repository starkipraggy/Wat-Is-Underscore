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

class VariableTableVariable : public DataStructureObject {
private:
	NAME name;								/**< The name used to refer to the variable */
	
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
		Getter function for the name of the variable; use this function to retrieve the name of the variable that this object is reprensenting.
		\return The name of the variable that this object is representing has.
	*/
	NAME getName();
	
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
	
	int getProceduresUses(int index);
	
	int getProceduresUsesSize();
	int getStatementUses(int index);
	int getStatementUsesSize();
};