#include "Variable.h"

Variable::Variable() {}

Variable::Variable(string nam, string typ) {
	name = nam;
	type = typ;
}

string Variable::getName() {
	return name;
}

string Variable::getType() {
	return type;
}

string Variable::toString() {
	return "<" + name + "," + type + ">";
}

bool Variable::equals(Variable v) {
	return name == v.getName() && type == v.getType();
}