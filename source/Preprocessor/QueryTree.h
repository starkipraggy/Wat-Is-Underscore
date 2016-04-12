/*! \class QueryTree
\brief Used to store the processed query

This this the main component to store the processed query done by the Preprocessor for
it to be evaluated by the Evaluator
*/
#ifndef QueryTree_H
#define QueryTree_H

#include <string>
#include <vector>
#include <regex>
#include <list>
#include <iostream>

#include "Clause.h"
#include "PatternClause.h"
#include "StringUlti.h"
using namespace std;
using namespace std::regex_constants;

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

	//! Getter for Boolean Clauses
	/*!
	Return the collection of Boolean Clause
	*/
	std::vector<Clause*> getBoolClauses();

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
	std::vector<Clause*> boolClauses; /**< collection of bool clauses */
	std::vector<std::pair <Clause*, int>> weightedClauses; /**< collection of weightedClauses */

	QueryTree();
	QueryTree(QueryTree const&) {};
	QueryTree& operator=(QueryTree const&) {};
	int QueryTree::determineWeight(Clause* c);
	bool sorted = false;
	std::vector<Clause*> tempClauses = {};

	//! Sort Clauses into a tree structure
	/*!
	Sort Clauses into a tree structure with clauses related to select infront follow by its 
	dependencies
	*/
	void buildTree();
};

#endif