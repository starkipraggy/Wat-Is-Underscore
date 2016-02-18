/*! \class PKB
	\brief Used to represent the Program Knowledge Base (PKB).

	This class is used to represent the Program Knowledge Base (PKB), and contains all data
	structures necessary for said intentions and purposes. This class is a singleton for obvious
	reasons, as we do not require the existence of multiple PKBs at the same time.
*/

#pragma once

#include <string>
#include <stack>
#include "ProcedureTable.h"
#include "StatementTable.h"
#include "VariableTable.h"

enum ExpressionTokenType {
	Variable,
	Constant,
	Operator,
	Parenthesis
};

enum RelationshipType {
	Modifies,
	Uses
};

class PKB {
private:
	static PKB* instance;

	int numberOfStatements;
	ProcedureTable* procedureTable;
	StatementTable* statementTable;
	VariableTable* variableTable;

	ProcedureTableProcedure* currentProcedure;	/**< Used during SIMPLE parsing, this pointer points to the current procedure
													 that statements that are currently being inputted belongs to */
	std::stack<int>* statementStackTrace;		/**< Used during SIMPLE parsing, this is a list of statement numbers of statements
													 that belong to different nesting levels, used to keep track of Parent and Follow*/

	PKB();
	~PKB();

	//! This function is used by the API for the SIMPLE parser when a new statement that has a statement number is inputted.
	StatementTableStatement* newStatement();
	//! These functions is used by the API for the SIMPLE parser to add a relationship between two items (variables, statements, procedures, etc.).
	bool addRelationship(VariableTableVariable* variable, ProcedureTableProcedure* procedure, RelationshipType relationship);
	bool addRelationship(VariableTableVariable* variable, StatementTableStatement* statement, RelationshipType relationship);
public:
	//! Allows access to the static reference to the lone singleton instance.
	static PKB* getInstance();

	// ---------------------------------------------------------------------------------
	// API FUNCTIONS FOR SIMPLE PARSER STARTS HERE!!!
	// ---------------------------------------------------------------------------------

	//! Declares the beginning of a procedure.
	/*!
		One of the API functions that allows the SIMPLE parser to construct the PKB.
		Call this function to declare the beginning of a procedure.
		\param nameOfProcedure Name of the procedure that is beginning.
	*/
	void ProcedureStart(std::string nameOfProcedure);

	//! Declares the end of a procedure.
	/*!
		One of the API functions that allows the SIMPLE parser to construct the PKB.
		Call this function to declare the end of a procedure.
	*/
	void ProcedureEnd();

	//! Declares an assignment statement by passing in the whole expression.
	/*!
		One of the API functions that allows the SIMPLE parser to construct the PKB.
		Call this function to declare an assignment statement (ie. y = a * x + b)
		\param variable The name of the variable that is having a value assigned to
		\param expression The expression used to assign the variable with
		\return True if successful, false if an error occurs (most likely tokens and types have differing sizes)
	*/
	bool AssignStatement(NAME variable, std::vector<std::string> tokens, std::vector<ExpressionTokenType> types);

	//! Declares a procedure calling statement.
	/*!
		One of the API functions that allows the SIMPLE parser to construct the PKB.
		Call this function to declare a procedure calling statement.
		\param procedure The name of the procedure that is being called by this statement.
	*/
	void CallStatement(std::string procedure);

	//! Declares a statement starting a while-loop.
	/*!
		One of the API functions that allows the SIMPLE parser to construct the PKB.
		Call this function to declare the start of a while-loop.
		\param variable The name of the variable used to control the while-loop.
	*/
	void WhileStart(NAME variable);

	//! Declares the end of a while-loop.
	/*!
		One of the API functions that allows the SIMPLE parser to construct the PKB.
		Call this function to declare the end of a while-loop.
		\return True if successful, and false if there is no while-loop currently to end.
	*/
	bool WhileEnd();

	//! Declares the start of an if-else statement.
	/*!
		One of the API functions that allows the SIMPLE parser to construct the PKB.
		Call this function to declare the start of an if-else statement.
		All statements declared after this function is called is assumed to be in the
		"then" statement list following the if-statement. Once the end of the "then"
		statement list is reached, the ElseStart() function should be called to
		proceed onto the declaration of the "else" statement list.
		\param variable The name of the variable used to determine if the if-statement
						executes the "then" statement list, or the "else" statement list
	*/
	void IfStart(NAME variable);

	//! Declares the start of the "else" statement list of an if-else statement.
	/*!
		One of the API functions that allows the SIMPLE parser to construct the PKB.
		Declares the end of the "then" statement list in an if-else statement, and the
		start of the corresponding "else" statement list.
		Once the end of the "else" statement list is reached, the IfElseEnd() function
		should be called in order to terminate the declaration of this if-else statement.
		The calling of this function is optional should the "else" statement list be empty.
		\return True if successful, and false if there is no if-statement currently to end.
	*/
	bool ElseStart();

	//! Declares the end of an if-else statement.
	/*!
		One of the API functions that allows the SIMPLE parser to construct the PKB.
		Call this function to declare the end of an if-else statement.
		\return True if successful, and false if there is no else-statement currently to end.
	*/
	bool IfElseEnd();

	// ---------------------------------------------------------------------------------
	// API FUNCTIONS FOR SIMPLE PARSER ENDS HERE!!!
	// ---------------------------------------------------------------------------------

	// ---------------------------------------------------------------------------------
	// API FUNCTIONS FOR PQL PARSER STARTS HERE!!!
	// ---------------------------------------------------------------------------------

	//! Extraction from PKB for uses and modifies relationships without pattern.
	/*!
		One of the API functions that allows the PQL parser to extract information from the PKB.
		Call this function for uses and modifies relationships without patterns.
		\return the vector<int>* of the list of integers.
	*/
	std::vector<int>* QueryPKBWithoutPatternUsesMods(std::string queryType, std::string input, int argumentPosition, std::string outputType);

	//! Extraction from PKB for follows and parent relationships without pattern.
	/*!
		One of the API functions that allows the PQL parser to extract information from the PKB.
		Call this function for follows and parent relationships without patterns.
		\return the vector<int>* of the list of integers of statements.
	*/
	std::vector<int>* QueryPKBWithoutPatternFollowsParent(std::string queryType, int input, int argumentPosition, std::string outputType);

	//! Extraction from PKB for pattern clauses.
	/*!
		One of the API functions that allows the PQL parser to extract information from the PKB.
		Call this function for pattern clauses.
		\return the vector<int>* of the list of integers of statements.
	*/

	std::vector<int>* QueryPKBPattern(NAME leftVariable, std::string rightExpression, bool isUnderscored);

	// ---------------------------------------------------------------------------------
	// API FUNCTIONS FOR PQL PARSER ENDS HERE!!!
	// ---------------------------------------------------------------------------------
};