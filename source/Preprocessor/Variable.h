#ifndef Variable_H
#define Variable_H

#include <string>
#include <regex>
#include "StringUlti.h"
using namespace std;

class Variable {
public:
	Variable();
	Variable::Variable(string nam, string typ);

	string getName();
	string getType();
	string toString();
	bool equals(Variable v);
private:
	string name;
	string type;
};

#endif