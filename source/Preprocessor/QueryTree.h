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
	Return the collection of select
	*/
	vector<Ref> getSelect();


	//! Setter for Clause
	/*!
	Set the collection of select
	*/
	void setSelect(vector<Ref> v);


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
		
private:
	static QueryTree* m_pInstance;
	vector<Ref> selects; /**< collection of select clause */
	std::vector<Clause*> clauses; /**< collection of clauses */

	QueryTree();
	QueryTree(QueryTree const&) {};
	QueryTree& operator=(QueryTree const&) {};
};

#endif