/*! \class Clause
\brief Used to represent each such that and pattern clause in a Query.

This class is used to represent each such that and pattern clause in a Query. It also 
has some comparative method such as hasLinkedVariable and hasAtMostOneCommonSynonym.
*/

#ifndef Clause_H
#define Clause_H

#include <string>
#include <regex>

#include "Variable.h"
#include "Clause.h"
#include "StringUlti.h"
using namespace std;

class Clause {
public:
	Clause();
	Clause::Clause(string clau, Variable var1, Variable var2);

	//! Getter for Clause
	/*!
	Return Clause for object of this instance.
	*/
	string getClause();

	//! Getter for VariableOne
	/*!
	Return VariableOne for object of this instance.
	*/
	Variable getVariableOne();

	//! Getter for VariableTwo
	/*!
	Return VariableTwo for object of this instance.
	*/
	Variable getVariableTwo();

	//! Get string representation of Clause
	/*!
	Return the string representation of Clause in the format VariableOne+VariableTwo
	*/
	virtual string getQuery();

	//! Check equals to another Clause
	/*!
	Return true if Clause has the same clause, variableOne and variableTwo with another
	Clause
	*/
	virtual bool equals(Clause* c);

	//! Check if Clause contain a particular Variable
	/*!
	Return true if variableOne or variableTwo is equals to input Variable
	*/
	bool hasVariable(Variable v);

	//! Check if Clause contain a particular linkable Variable
	/*!
	Return true if variableOne or variableTwo is equals to input Variable and the other
	arguement is a linkable Variable

	A linkable Variable has to have the type of design-entity
	*/
	bool hasLinkedVariable(Variable v);

	//! Return the variable linked to input variable
	/*!
	Return the opposite variable of the input variable

	A linkable Variable has to have the type of design-entity
	*/
	Variable getLinkedVariable(Variable v);

	//! Check if input Clause has at most one common Synonymn with Clause
	/*!
	Return true if not both Variable are the same and of the type design-entity
	*/
	bool hasAtMostOneCommonSynonym(Clause* c);
protected:
	string clause; /**< Contain relCond of a such that clause */
	Variable variableOne; /**< The first arguement of the relCond */
	Variable variableTwo; /**< The second arguement of the relCond */
};

#endif