#ifndef Variable_H
#define Variable_H

#include <string>
using namespace std;

class Variable {
public:
	Variable();
	Variable::Variable(string nam, string typ);

	string getName();
	string getType();
	string toString();
	bool equals(Variable v);
	static string findSuchThatType(string raw);
	static string findPatternType(string raw);
private:
	string name;
	string type;

	static bool isName(string raw);
	static bool isExpr(string raw);
};

#endif