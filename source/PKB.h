/*! \class PKB
	\brief Used to represent the Program Knowledge Base (PKB).

	This class is used to represent the Program Knowledge Base (PKB), and contains all data
	structures necessary for said intentions and purposes. This class is a singleton for obvious
	reasons, as we do not require the existence of multiple PKBs at the same time.
*/

#pragma once

#include "SimpleParser.h"
#include "AST.h"
#include "ProcedureTable.h"
#include "StatementTable.h"
#include "VariableTable.h"
#include "ConstantTable.h"
#include "Preprocessor/Ref.h"
#include <string>
#include <stack>
#include <unordered_map>

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

class CFG;

class PKB {
private:
	static PKB* instance;

	int numberOfStatements;
	ProcedureTable* procedureTable;
	StatementTable* statementTable;
	VariableTable* variableTable;
	ConstantTable* constantTable;
    std::vector<AST*> procedureAST;

	ProcedureTableProcedure* currentProcedure;	/**< Used during SIMPLE parsing, this pointer points to the current procedure
													 that statements that are currently being inputted belongs to */
    AST* currentProcedureAST; /**< Used during SIMPLE parsing, this pointer points to the current AST Procedure tree that is being modified.*/
	std::stack<int>* statementStackTrace;		/**< Used during SIMPLE parsing, this is a list of statement numbers of statements
													 that belong to different nesting levels, used to keep track of Parent and Follow*/
    

	//! This function is used by the API for the SIMPLE parser when a new statement that has a statement number is inputted.
	StatementTableStatement* newStatement();
	//! These functions is used by the API for the SIMPLE parser to add a relationship between two items (variables, statements, procedures, etc.).
	bool addRelationship(VariableTableVariable* variable, ProcedureTableProcedure* procedure, RelationshipType relationship);
	bool addRelationship(VariableTableVariable* variable, StatementTableStatement* statement, RelationshipType relationship);
	//! This function is used by the API for the PKB parser to convert a string argument into a enum TNodeType
	TNodeType getType(std::string type);
protected:
	PKB();
	~PKB();
public:
	//! Allows access to the static reference to the lone singleton instance.
	static PKB* getInstance();

	//! Allows modification to the static reference to the lone singleton instance.
	static void PKB::setInstance(PKB* newPkb);

    //! Getter function for the procedure table.
    /*!
        Getter function for the procedure table; use this function to
        retrieve the procedure table that this object has.
        \return The procedure table that this object has.
    */
    ProcedureTable* getProcedureTable();
    
    //! Getter function for the statement table.
    /*!
        Getter function for the statement table; use this function to
        retrieve the statement table that this object has.
        \return The statement table that this object has.
    */
    StatementTable* getStatementTable();
    
    //! Getter function for the variable table.
    /*!
        Getter function for the variable table; use this function to
        retrieve the variable table that this object has.
        \return The variable table that this object has.
    */
    VariableTable* getVariableTable();


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

	//! Returns a list of items that fit the conditions of the specified item for the PQL parser.
	/*!
		One of the API functions that allows the PQL parser to extract information from the PKB.
		Call this function for selection without clauses.
		\param outputType ProcedureName (for procedures),VariableName (for variables), 
						  Assign, If, While, Call, Undefined (for any statements)
		\return the vector<string> of the statement numbers or names.
	*/
	virtual std::vector<std::string> PQLSelect(TNodeType outputType);

	//! Returns a list of items that fit Uses(x, y) conditions for the PQL parser.
	/*!
		One of the API functions that allows the PQL parser to extract information from the PKB.
		Call this function for selection of Uses clauses.
		\param input The known variable/statement/procedure in the clause
		\param argumentPosition If 1, look for statements and procedures that uses the variable named "input"
								If 2, look for variables that the statement with statement type "input" or
								      the procedure with procedure name "input" uses
		\param outputType The type of conditions to check for (eg. "procedure", "statement", "assign", "while", "if", "call", "integer value" (constant))
		\return The vector<string> of the statement numbers or variable names.
	*/
	 virtual std::vector<std::string> PQLUses(std::string input, int argumentPosition, std::string outputType);

	//! Returns a list of items that fit Modifies(x, y) conditions for the PQL parser.
	/*!
		One of the API functions that allows the PQL parser to extract information from the PKB.
		Call this function for selection of Modifies clauses.
		\param input The known variable/statement/procedure in the clause
		\param argumentPosition If 1, look for statements and procedures that uses the variable named "input"
								If 2, look for variables that the statement with statement type "input" or
								      the procedure with procedure name "input" uses
		\param outputType The type of conditions to check for (eg. "procedure", "stmt", "assign", "while", "if", "call")
		\return The vector<string> of the statement numbers or variable names.
	*/
	 virtual std::vector<std::string> PQLModifies(std::string input, int argumentPosition, std::string outputType);

	//! Returns a list of items that fit Follows(x, y) conditions for the PQL parser.
	/*!
		One of the API functions that allows the PQL parser to extract information from the PKB.
		Call this function for selection of Follows clauses.
		\param input The known statement in the clause
		\param argumentPosition If 1, look for the statement s that this statement is following (ie. Follows(s, this))
								If 2, look for the statement s that this statement is followed by (ie. Follows(this, s))
		\param outputType Optional argument - the type of conditions to check for
						  (eg. "stmt", "assign", "while", "if", "call"). Default is "stmt".
		\return The vector<string> of the statement numbers.
	*/
	 virtual std::vector<std::string> PQLFollows(int input, int argumentPosition, std::string outputType = "stmt");

	//! Returns a list of items that fit Follows*(x, y) conditions for the PQL parser.
	/*!
		One of the API functions that allows the PQL parser to extract information from the PKB.
		Call this function for selection of Follows* clauses.
		\param statementNumber The known statement in the clause
		\param argumentPosition If 1, look for the statements s that this statement is indirectly following (ie. Follows*(s, this))
								If 2, look for the statements s that this statement is indirectly followed by (ie. Follows*(this, s))
		\param outputType Optional argument - the type of conditions to check for
						  (eg. "stmt", "assign", "while", "if", "call"). Default is "stmt".
		\return The vector<string> of the statement numbers.
	*/
	 virtual std::vector<std::string> PQLFollowsStar(int statementNumber, int argumentPosition, std::string outputType = "stmt");

	//! Returns a list of items that fit Parent(x, y) conditions for the PQL parser.
	/*!
		One of the API functions that allows the PQL parser to extract information from the PKB.
		Call this function for selection of Parent clauses.
		\param statementNumber The known statement in the clause
		\param argumentPosition If 1, look for the statements that this statement is a parent of (ie. Parent(s, _))
								If 2, look for the statements that this statement has as a parent (ie. Parent(_, s))
		\param outputType Optional argument - the type of conditions to check for
						  (eg. "stmt", "assign", "while", "if", "call"). Default is "stmt".
		\return The vector<string> of the statement numbers.
	*/
	 virtual std::vector<std::string> PQLParent(int statementNumber, int argumentPosition, std::string outputType = "stmt");

	//! Returns a list of items that fit the conditions of the specified item for the PQL parser.
	/*!
		One of the API functions that allows the PQL parser to extract information from the PKB.
		Call this function for selection of parent* clauses.
		\param statementNumber The known statement in the clause
		\param argumentPosition the position of the input in the clause
		\param outputType Optional argument - the type of conditions to check for
						  (eg. "stmt", "assign", "while", "if", "call"). Default is "stmt".
		\return The vector<string> of the statement numbers.
	*/
	 virtual std::vector<std::string> PQLParentStar(int statementNumber, int argumentPosition, std::string outputType = "stmt");

	//! Returns a list of items that fit the specified pattern condition.
	/*!
		One of the API functions that allows the PQL parser to extract information from the PKB.
		Call this function for pattern clauses.
		\param type The type of statements that you are looking for (eg. "Statement", "Assign", "While", etc.)
		\param left The first item in the parentheses of the pattern clause
		\param right The second item in the parentheses of the pattern clause
		\return The vector<string> of the list of integers of statements.
	*/
	 virtual std::vector<std::string> PQLPattern(TNodeType type, Ref left, Ref right);

	 //! Returns a list of procedures that this procedure calls, whether directly or indirectly.
	 /*!
		 One of the API functions that allows the PQL parser to extract information from the PKB.
		 Call this function for Calls and Calls* clauses.
		 \param procedureName The name of the current procedure
		 \param isDirectCalls True for direct calls (Calls), and false for indirect calls (Calls*)
		 \return The vector<string> of the list of names of procedures that this procedure calls.
	 */
	 virtual std::vector<std::string> PQLCalls(std::string procedureName, bool isDirectCalls);

	 //! Returns a list of procedures that calls this procedure, whether directly or indirectly.
	 /*!
		 One of the API functions that allows the PQL parser to extract information from the PKB.
		 Call this function for Calls and Calls* clauses.
		 \param procedureName The name of the current procedure
		 \param isDirectCalls True for direct calls (Calls), and false for indirect calls (Calls*)
		 \return The vector<string> of the list of names of procedures that call this procedure.
	 */
	 virtual std::vector<std::string> PQLCalledBy(std::string procedureName, bool isDirectCalls);

	 //! Returns a list of statements that has this statement after, whether directly or indirectly, in the CFG
	 /*!
		One of the API functions that allows the PQL parser to extract information from the PKB.
		Call this function for Next and Next* clauses.
		\param statementNumber The statement number of the statement
		\param isDirectCalls True for direct (Next), and false for indirect (Next*)
		\param isBip Default is false. Set this to true to include inter-procedures
		\return The vector<string> of the statement numbers of statements that has this statement after in the CFG
	 */
	 virtual std::vector<std::string> PQLPrevious(int statementNumber, bool isDirect, bool isBIP = false);

	 //! Returns a list of statements that comes after this statement, whether directly or indirectly, in the CFG
	 /*!
		One of the API functions that allows the PQL parser to extract information from the PKB.
		Call this function for Next and Next* clauses.
		\param statementNumber The statement number of the statement
		\param isDirectCalls True for direct (Next), and false for indirect (Next*)
		\param isBip Default is false. Set this to true to include inter-procedures
		\return The vector<string> of the statement numbers of statements that comes after this statement after in the CFG
	 */
	 virtual std::vector<std::string> PQLNext(int statementNumber, bool isDirect, bool isBIP = false);

	 //! Returns a list of statements that affects this statement, whether directly or indirectly
	 /*!
		One of the API functions that allows the PQL parser to extract information from the PKB.
		Call this function for Affects and Affects* clauses.
		\param statementNumber The statement number of the statement
		\param isDirectCalls True for direct (Affects), and false for indirect (Affects*)
		\param isBip Default is false. Set this to true to include inter-procedures
		\return The vector<string> of the statement numbers of statements that affects this statement
	 */
	 virtual std::vector<std::string> PQLAffectsThis(int statementNumber, bool isDirect, bool isBip = false);

	 //! Returns a list of statements that this statement affects, whether directly or indirectly
	 /*!
		One of the API functions that allows the PQL parser to extract information from the PKB.
		Call this function for Affects and Affects* clauses.
		\param statementNumber The statement number of the statement
		\param isDirectCalls True for direct (Affects), and false for indirect (Affects*)
		\param isBip Default is false. Set this to true to include inter-procedures
		\return The vector<string> of the statement numbers of statements that this statement affects
	 */
	 virtual std::vector<std::string> PQLAffectedByThis(int statementNumber, bool isDirect, bool isBip = false);

	 //! Returns a list of statements that this statement has as siblings
	 /*!
		 One of the API functions that allows the PQL parser to extract information from the PKB.
		 Call this function for Sibling clauses.
		 \param statementNumber The statement number of the statement
		 \return The vector<string> of the statement numbers of statements that this statement has as siblings
	 */
	 virtual std::vector<std::string> PQLSibling(int statementNumber);

	// ---------------------------------------------------------------------------------
	// API FUNCTIONS FOR PQL PARSER ENDS HERE!!!
	// ---------------------------------------------------------------------------------
};