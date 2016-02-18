/*! \class Preprocessor
\brief Used to process input query to QueryTree

This this the main component to read a query statement and parse it into QueryTree
*/
#ifndef Preprocessor_H
#define Preprocessor_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <regex>

#include "Clause.h"
#include "PatternClause.h"
#include "QueryTree.h"

#include "StringUlti.h"

class Preprocessor {
public:
	//! Process statement and generate QueryTree
	/*!
	Main method to Process statement and generate QueryTree
	*/
	void process(string statement);
private:
	std::unordered_map<std::string, std::string> declarationMap;   /**< map to store declaration */

//----------------DECLARATION---------------------------------------------
	//! Main method to set input statement to declarationMap
	/*!
	tokenize input using ; and call setDeclaration to set map
	*/
	void setMap(string statement);

	//! setMap method to set each line of declaration to map
	/*!
	for each declaration, check the the design entity and synonymn is correct and add to
	map. Split into multiple add if , exist in the declaration
	*/
	void setDeclaration(string line);
	
//----------------CLAUSES---------------------------------------------------
	//! Main method to set input statement to QueryTree
	/*!
	Parse and initiatize query into QueryTree
	*/
	void setQueryTree(string statement);

	//! Get the select query from the statement 
	/*!
	Return select statement without declaration
	*/
	string getQuery(string statement);

	//! Split select synonym and clause from the statement 
	/*!
	Return pair with select synonymn in first and clauses in second
	*/
	std::pair<std::string, std::string> getSelect(string query);

	//! Add string of clauses into QueryTree
	/*!
	Tokenize the string the clauses according to such that and pattern keyword and add
	each clause into QueryTree
	*/
	void processClauses(string declarationQueries);

	//! Add each clause to QueryTree
	/*!
	call addSuchThatClause or addPatternClause depending on condition
	*/
	void addClause(string rawClause, string condition);

	//! specific method to add such that clause to QueryTree
	/*!
	tokenized according to such that clause parameter and add in QueryTree
	*/
	void addSuchThatClause(string rawClause);
	//! specific method to add pattern clause to QueryTree
	/*!
	tokenized according to pattern clause parameter and add in QueryTree
	*/
	void addPatternClause(string rawClause);

//----------------MISC METHODS-----------------------------------------------
	//! Find type of such that variable
	/*!
	return the type of variable according to such that clause
	*/
	string findSuchThatType(string raw);
	//! Find type of pattern variable
	/*!
	return the type of variable according to pattern clause
	*/
	string findPatternType(string raw);
	//! Check if type is stmtRef
	/*!
	return true if type is stmtRef
	*/
	static bool isStmtRef(Ref v);
	//! Check if type is EntRef
	/*!
	return true if type is EntRef
	*/
	static bool isEntRef(Ref v);
	//! Check if type is ExprSpec
	/*!
	return true if type is ExprSpec
	*/
	static bool isExprSpec(Ref v);
};

#endif