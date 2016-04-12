/*! \class QueryEvaluator
\brief Used to read QueryTree and call PKB to evaluate Query

This this the main component to read query from QueryTree and define and evaluate each
Query base and call PKB to process them
*/
#ifndef QueryEvaluator_H
#define QueryEvaluator_H

#include <vector>
#include <string>
#include <regex>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "Clause.h"
#include "PatternClause.h"
#include "QueryTree.h"
#include "../PKB.h"
#include "Ref.h"

#include "StringUlti.h"

class QueryEvaluator {
public:
	//! Process QueryTree and generate result
	/*!
	Main method to Process QueryTree and generate result
	*/
	vector<vector<string>> process();

private:
	int directoryIndex; /**< int to increment table count */
	unordered_map<string, int> directory; /**< map to store result col name and number */
	vector<vector<string>> result; /**< table to track output result */
	vector<vector<string>> tempResult; /**< temp for result */
	bool isTrueStatement; /**< true if non-synonym clauses are true */

	//! internal method to process one synonymn
	/*!
	Internal method to process one synonymn. Can handle either synonymn with non-synonym or vice versa
	*/
	void processOneSynonym(Ref source, Ref des, string clause, int pos);

	//! add to table
	/*!
	Internal method of process to add queryResult to all row in table
	*/
	void add(vector<string> queryResult, string name);
	//! add to table
	/*!
	Internal method of process to add queryResult to all row in table
	*/
	vector<vector<string>> add(vector<string> queryResult, int i, vector<vector<string>> temp);

	//! query a such that clause
	/*!
	Internal method of process to query a such that clause given the vector, its iterator and col number
	*/
	vector<vector<string>>::iterator query(unordered_set<string> queryResult, vector<vector<string>>::iterator it, int col);

	//! Convert a string type to its TNodeType
	/*!
	Convert a string type to its TNodeType to parse to PKB
	*/
	TNodeType toTNodeType(string type);

	//! Query PKB
	/*!
	Query the individual PKB method by the clause type
	*/
	vector<string> queryPKB(string clause, string input, int argumentPosition, string outputType);

	//! remove from result table
	/*!
	Remove a row if the entry in pos1 equals to entry in pos2
	*/
	void remove(int pos1, int pos2);

	//! remove from result table
	/*!
	Remove a row if the entry in pos1 equals to a string input
	*/
	void remove(int pos, string input);

	//! check if an input is valid
	/*!
	Throw an error message of input does not belong to a PKB table of the specified type
	*/
	void checkValid(string input, string type);

	//! add a entry to directory
	/*!
	add a entry to directory. The entry is to keep track that the synonym is in result
	*/
	void addDirectory(string name);
};
#endif