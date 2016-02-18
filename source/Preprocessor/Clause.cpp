#include "Clause.h"

Clause::Clause() {}

Clause::Clause(string clau, Ref var1, Ref var2) {
	clause = clau;
	refOne = var1;
	refTwo = var2;
}

string Clause::getClause() {
	return clause;
}

Ref Clause::getRefOne() {
	return refOne;
}

Ref Clause::getRefTwo() {
	return refTwo;
}

string Clause::getQuery() {
	return refOne.toString() + "+" + refTwo.toString();
}

bool Clause::equals(Clause* c) {
	return clause == c->getClause() && refOne.equals(c->getRefOne())
		&& refTwo.equals(c->getRefTwo());
}

bool Clause::hasRef(Ref v) {
	return refOne.equals(v) || refTwo.equals(v);
}

bool Clause::hasLinkedRef(Ref v) {
	bool result;
	if (refOne.equals(v)) {
		result = std::regex_match(StringToUpper(refTwo.getType()), 
			std::regex("ASSIGN|STMT|WHILE|VARIABLE|CONSTANT|PROG_LINE|PROCEDURE"));
	}
	else if (refTwo.equals(v)) {
		result = std::regex_match(StringToUpper(refOne.getType()), 
			std::regex("ASSIGN|STMT|WHILE|VARIABLE|CONSTANT|PROG_LINE|PROCEDURE"));
	}
	return result;
}

Ref Clause::getLinkedRef(Ref v) {
	if (refOne.equals(v)) {
		return refTwo;
	}
	else {
		return refOne;
	}
}

bool Clause::hasAtMostOneCommonSynonym(Clause* c) {
	return !((refOne.equals(c->getRefOne()) && refTwo.equals(c->getRefTwo()))
		|| (refOne.equals(c->getRefTwo()) && refTwo.equals(c->getRefOne())));
}