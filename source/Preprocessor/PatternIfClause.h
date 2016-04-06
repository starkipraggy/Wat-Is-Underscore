/*! \class PatternIfClause
\brief Used to represent each if pattern clause in a Query.

This is the child of the PatternClause. It override a few methods of the PatternClause class
to present a if pattern clause.
*/

#ifndef PatternIfClause_H
#define PatternIfClause_H

#include <string>
#include "Ref.h"
#include "PatternClause.h"

class PatternIfClause : public PatternClause{
public:
	PatternIfClause(string clau, Ref var1, Ref var2, Ref var3, Ref var4);

	//! Getter for refThree
	/*!
	Return refThree for object of this instance. RefThree is the additional arguement for if pattern clause
	*/
	Ref getRefThree();
	//! Check equals to another Clause
	/*!
	Override method to return true if Clause has the same clause, refOne, 
	refTwo, refThree and assignedVariable with another PatternClause
	*/
	bool equals(Clause* c);
protected:
	Ref refThree;  /**< The third arguement of if pattern clause */
};

#endif