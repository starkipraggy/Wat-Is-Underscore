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
	
	string declarationPart; /**< declaration part of query */
	string selectPart; /**< select part of query */
	string clausesPart; /**< clauses part of query */

	//! Main method to initialize query
	/*!
	split query into three parts: declaration, select and clauses
	*/
	void initialize(string statement);

//----------------DECLARATION---------------------------------------------
	//! Main method to set input statement to declarationMap
	/*!
	tokenize input using ; and call setDeclaration to set map
	*/
	void setMap();

	//! setMap method to set each line of declaration to map
	/*!
	for each declaration, check the the design entity and synonymn is correct and add to
	map. Split into multiple add if , exist in the declaration
	*/
	void setDeclaration(string query);

//----------------SELECT---------------------------------------------------
	//! Main method to set select statement to Select Clause
	/*!
	Parse and initiatize select into select Clause
	*/
	void setSelect();

//----------------CLAUSES---------------------------------------------------
	//! Main method to set input statement to QueryTree
	/*!
	Parse and initiatize query into QueryTree
	*/
	void setQueryTree();

	//! Add string of clauses into QueryTree
	/*!
	Tokenize the string the clauses according to such that and pattern keyword and add
	each clause into QueryTree
	*/
	void processClauses(string declarationQueries);

	//! Add each clause to QueryTree
	/*!
	call addSuchThatClause, addPatternClause or addWithClause depending on condition
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

	//! specific method to add with clause to QueryTree
	/*!
	tokenized according to with clause parameter and add in QueryTree
	*/
	void addWithClause(string rawClause);

//----------------MISC METHODS-----------------------------------------------
	//! Create a such that ref
	/*!
	return a ref of such that with type derived from name
	*/
	Ref createSuchThatRef(string name);
	//! Create a pattern ref
	/*!
	return a ref of pattern with type derived from name
	*/
	Ref createPatternRef(string name);
	//! Create a with ref
	/*!
	return a ref of with with type derived from name
	*/
	Ref createWithRef(string name);
	//! Create a attr ref
	/*!
	return a ref of attrRef with type derived from name. Input must consist of "."
	*/
	Ref createAttrRef(string name);
	//! Check if type is stmtRef
	/*!
	return true if type is stmtRef
	*/
	static bool isStmtRef(Ref v);
	//! Check if type is lineRef
	/*!
	return true if type is lineRef
	*/
	static bool isLineRef(Ref v);
	//! Check if type is EntRef
	/*!
	return true if type is EntRef
	*/
	static bool isEntRef(Ref v);
	//! Check if type is VarRef
	/*!
	return true if type is VarRef
	*/
	static bool isVarRef(Ref v);
	//! Check if type is ExprSpec
	/*!
	return true if type is ExprSpec
	*/
	static bool isExprSpec(Ref v);
	//! Return declaration type given a declaration name
	/*!
	Return declaration type given a declaration name. Throw error if name not found
	*/
	string getDeclarationType(string name);
};

#endif