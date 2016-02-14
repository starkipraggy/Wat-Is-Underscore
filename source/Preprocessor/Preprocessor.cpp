#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <regex>
#include "Preprocessor.h"
using namespace std;

int main() {
	string statement = "variable a,b , c, v,e;select a such that follows(a, v) such that follows(b, e) such that follows(c, b) pattern a(_, \"y\")";

	try {
		DeclarationMap::Instance()->setMap(statement);
		std::unordered_map<std::string, std::string> result = DeclarationMap::Instance()->getMap();
		for (auto& x : result) {
			std::cout << x.first << ": " << x.second << std::endl;
		}
	}
	catch (const char* msg) {
		std::cout << msg << std::endl;
	}

	try {
		QueryTree queryTree = QueryTree(statement);

		Variable selectVariable = queryTree.getSelect();
		std::cout << "Select:" << selectVariable.toString() << std::endl;

		std::vector<Clause> clauses = queryTree.getClauses();
		for (auto& x : clauses) {
			std::cout << x.getClause() << ": " << x.getQuery() << std::endl;
		}
	}
	catch (const char* msg) {
		std::cout << msg << std::endl;
	}

	std::cin.get();

}