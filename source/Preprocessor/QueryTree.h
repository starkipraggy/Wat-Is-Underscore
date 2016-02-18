/*! \class QueryTree
\brief Used to store the processed query

This this the main component to store the processed query done by the Preprocessor for
it to be evaluated by the Evaluator
*/
#ifndef ProcessQuery_H
#define ProcessQuery_H

#include <string>
#include <vector>
#include <regex>
#include <list>

#include "Clause.h"
#include "PatternClause.h"
#include "StringUlti.h"
using namespace std;

class QueryTree {
public:
	static QueryTree* Instance();


	//! Getter for Select
	/*!
	Return the Variable for select
	*/
	Variable getSelect();


	//! Setter for Clause
	/*!
	Set the Variable for select
	*/
	void setSelect(Variable v);


	//! Setter for Clause
	/*!
	Set clause to QueryTree
	*/
	void addClause(Clause* c);


	//! Getter for Clauses
	/*!
	Return the collection of Clause
	*/
	std::vector<Clause*> getClauses();


	//! Set a new Tree
	/*!
	Reset all variable in QueryTree
	*/
	void newTree();

	//void buildTree();
	

	//! Special validation for Iteration One
	/*!
	As Iteration One constrain some of the functionality, this method is use as a 
	additional check to restrict these functionality
	*/
	bool v1Validation();
	
private:
	static QueryTree* m_pInstance;
	Variable selectVariable; /**< variable of the select clause */
	std::vector<Clause*> clauses; /**< collection of clauses */

	QueryTree();
	QueryTree(QueryTree const&) {};
	QueryTree& operator=(QueryTree const&) {};
};

#endif