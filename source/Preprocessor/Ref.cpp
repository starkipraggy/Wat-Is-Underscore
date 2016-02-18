#include "Ref.h"

Ref::Ref() {}

Ref::Ref(string nam, string typ) {
	name = nam;
	type = typ;
}

string Ref::getName() {
	return name;
}

string Ref::getType() {
	return type;
}

string Ref::toString() {
	return "<" + name + "," + type + ">";
}

bool Ref::equals(Ref v) {
	return name == v.getName() && type == v.getType();
}