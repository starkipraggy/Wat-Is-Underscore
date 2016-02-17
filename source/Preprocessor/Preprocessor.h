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
	void process(string statement);
private:
	std::unordered_map<std::string, std::string> declarationMap;

	void setMap(string statement);
	void setDeclaration(string line);
	
	void setQueryTree(string statement);
	string getQuery(string statement);
	std::pair<std::string, std::string> getSelect(string query);
	void processClauses(string declarationQueries);
	void addClause(string rawClause, string condition);
	void addSuchThatClause(string rawClause);
	void addPatternClause(string rawClause);

	string findSuchThatType(string raw);
	string findPatternType(string raw);
	static bool isStmtRef(Variable v);
	static bool isEntRef(Variable v);
	static bool isExprSpec(Variable v);
};

#endif