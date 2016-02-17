#include "PatternClause.h"
using namespace std;

PatternClause::PatternClause(string clau, Variable var1, Variable var2, Variable var3) : Clause(clau, var1, var2){
	assignedVariable = var3;
}

Variable PatternClause::getAssignedVariable() {
	return assignedVariable;
}

string PatternClause::getQuery() {
	return Clause::getQuery() + "+" + assignedVariable.toString();
}

bool PatternClause::equals(Clause* c) {
	bool result;
	if (PatternClause* p = dynamic_cast<PatternClause*>(c)){
		result = clause == p->getClause()
			&& variableOne.equals(p->getVariableOne())
			&& variableTwo.equals(p->getVariableTwo())
			&& assignedVariable.equals(p->getAssignedVariable());
	}
	else {
		result = false;
	}

	return result;
}