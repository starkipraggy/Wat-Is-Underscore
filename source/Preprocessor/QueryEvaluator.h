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
#include <unordered_set>
#include "Clause.h"
#include "PatternClause.h"
#include "QueryTree.h"

#include "StringUlti.h"

class QueryEvaluator {
public:
	std::vector<string> process();

private:
	unordered_set<string> result; /**< map to store output result */
	set<string> eachResult; /**< temp set to store temp result during processing */

	//! Return result
	/*!
	convert result to vector<string> and return. Then reset result to {}
	*/
	vector<string> getResult();

	//! Return each result
	/*!
	convert each result to vector<string> and return. Then reset each result to {}
	*/
	vector<string> getEachResult();

	//! Add current result to each result
	/*!
	Adding current result to each result will get a each result that follows the set
	property
	*/
	void accumulate(vector<string> currResult);
	
	//! Add current result to result
	/*!
	Adding current result to result will get a each result that follows the AND
	property
	*/
	void addResult(vector<string> currResult);

	//! query a such that clause
	/*!
	Internal method of process to query a such that clause
	*/
	void query(string clause, Variable source, Variable dest, int position);
};
#endif