#include "QueryTree.h"
using namespace std;

QueryTree::QueryTree() {}

QueryTree* QueryTree::m_pInstance = NULL;

QueryTree* QueryTree::Instance() {
	if (!m_pInstance) {
		m_pInstance = new QueryTree;
	}
	return m_pInstance;
}

Ref QueryTree::getSelect() {
	return selectVariable;
}

void QueryTree::setSelect(Ref v) {
	selectVariable = v;
}

void QueryTree::addClause(Clause* c) {
	clauses.push_back(c);
}
std::vector<Clause*> QueryTree::getClauses() {
	return clauses;
}

void QueryTree::newTree() {
	selectVariable = Ref();
	clauses = {};
}

/*void QueryTree::buildTree() {
	std::list<Clause*> tree;
	std::list<Clause*> remaining;
	std::list<Clause*> temp(clauses.begin(), clauses.end());
	std::list<Variable> newLink;
	newLink.push_back(selectVariable);
	Variable currentVariable;

	while (!newLink.empty()) {
		currentVariable = newLink.front();
		newLink.pop_front();
		for (auto& x : temp) {
			if (x->hasVariable(currentVariable)) {
				tree.push_back(x);
				if (x->hasLinkedVariable(currentVariable)) {
					newLink.push_back(x->getLinkedVariable(currentVariable));
				}
			}
			else {
				remaining.push_back(x);
			}
		}
		temp.assign(remaining.begin(), remaining.end());
	}
}*/