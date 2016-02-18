#include "PatternClause.h"
using namespace std;

PatternClause::PatternClause(string clau, Ref var1, Ref var2, Ref var3) : Clause(clau, var1, var2){
	assignedVariable = var3;
}

Ref PatternClause::getAssignedVariable() {
	return assignedVariable;
}

string PatternClause::getQuery() {
	return Clause::getQuery() + "+" + assignedVariable.toString();
}

bool PatternClause::equals(Clause* c) {
	bool result;
	if (PatternClause* p = dynamic_cast<PatternClause*>(c)){
		result = clause == p->getClause()
			&& refOne.equals(p->getRefOne())
			&& refTwo.equals(p->getRefTwo())
			&& assignedVariable.equals(p->getAssignedVariable());
	}
	else {
		result = false;
	}

	return result;
}