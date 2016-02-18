/*! \class PatternClause
\brief Used to represent each pattern clause in a Query.

This this the child of the PatternClause. It override a few methods of the Clause class
to present a pattern clause.
*/

#ifndef PatternClause_H
#define PatternClause_H

#include <string>
#include "Variable.h"
#include "Clause.h"

class PatternClause : public Clause{
public:
	PatternClause(string clau, Variable var1, Variable var2, Variable var3);

	//! Getter for AssignedVariable
	/*!
	Return AssignedVariable for object of this instance.
	*/
	Variable getAssignedVariable();

	//! Get string representation of Clause
	/*!
	Override method to return the string representation of Clause in the format 
	VariableOne+VariableTwo+AssignedVariable
	*/
	string getQuery();

	//! Check equals to another Clause
	/*!
	Override method to return true if Clause has the same clause, variableOne, 
	variableTwo and assignedVariable with another PatternClause
	*/
	bool equals(Clause* c);
protected:
	Variable assignedVariable;  /**< syn-assign of Pattern Clause */
};

#endif