/*! \class QueryEvaluator
\brief Used to read QueryTree and call PKB to evaluate Query

This this the main component to read query from QueryTree and define and evaluate each
Query base and call PKB to process them
*/
#ifndef Source_H
#define Source_H

#include <vector>
#include <string>
#include <regex>
#include <set>
#include <unordered_map>
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
	std::vector<string> process();

private:
	int directoryIndex; /**< int to increment table count */
	unordered_map<string, int> directory; /**< map to store result col name and number */
	vector<vector<string>> result; /**< table to track output result */
	vector<vector<string>> tempResult; /**< temp for result */


	//! internal method to process one synonymn
	/*!
	Internal method to process one synonymn. Can handle either synonymn with non-synonym or vice versa
	*/
	void processOneSynonym(Ref source, Ref des, string clause, int pos);
	
	//! query a such that select clause
	/*!
	Internal method of process to query a such that select clause and add to table
	*/
	void add(Ref ref);

	//! query a such that clause
	/*!
	Internal method of process to query a such that clause given the vector and its iterator
	*/
	vector<vector<string>> query(vector<string> queryResult, int i, vector<vector<string>> temp);

	//! query a such that clause
	/*!
	Internal method of process to query a such that clause given the vector, its iterator and col number
	*/
	vector<vector<string>>::iterator query(vector<string> queryResult, vector<vector<string>>::iterator it, int col);

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
};
#endif