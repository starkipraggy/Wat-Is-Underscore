#include "PatternIfClause.h"

PatternIfClause::PatternIfClause(string clau, Ref var1, Ref var2, Ref var3, Ref assignVar) : PatternClause(clau, var1, var2, assignVar){
	refThree = var3;
}

Ref PatternIfClause::getRefThree() {
	return refThree;
}

bool PatternIfClause::equals(Clause* c) {
	bool result;
	if (PatternIfClause* p = dynamic_cast<PatternIfClause*>(c)){
		result = clause == p->getClause()
			&& refOne.equals(p->getRefOne())
			&& refTwo.equals(p->getRefTwo())
			&& refThree.equals(p->getRefThree())
			&& assignedVariable.equals(p->getAssignedVariable());
	}
	else {
		result = false;
	}

	return result;
}