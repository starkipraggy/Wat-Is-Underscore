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

vector<Ref> QueryTree::getSelect() {
	return selects;
}

void QueryTree::setSelect(vector<Ref> v) {
	selects = v;
}

struct less_than_key
{
	inline bool operator() (std::pair <Clause*, int> ClausePair1, std::pair <Clause*, int> ClausePair2)
	{
		return (ClausePair1.second < ClausePair2.second);		
	}
};

void QueryTree::addClause(Clause* c) {
	std::pair <Clause*, int> ClausePair(c, determineWeight(c));
	weightedClauses.push_back(ClausePair);
	clauses.push_back(c);
	sorted = false;
}

int QueryTree::determineWeight(Clause* c) {
	string clau = StringToUpper(c->getClause());
	int weight = 0;
	if (clau.compare("WITH") == 0) {
		weight += 1;
	}
	else if (clau.compare("CALLS") == 0) {
		weight += 2;
	}
	else if (clau.compare("PARENT") == 0) {
		weight += 3;
	}
	else if (clau.compare("CALLS*") == 0) {
		weight += 4;
	}
	else if (clau.compare("MODIFIES") == 0) {
		weight += 5;
	}
	else if (clau.compare("USES") == 0) {
		weight += 6;
	}
	else if (clau.compare("FOLLOWS") == 0) {
		weight += 7;
	}
	else if (clau.compare("FOLLOWS*") == 0) {
		weight += 8;
	}
	else if (clau.compare("NEXT") == 0) {
		weight += 9;
	}
	else if (clau.compare("AFFECTS") == 0) {
		weight += 10;
	}
	else if (clau.compare("PATTERN") == 0) {
		weight += 11;
	}
	else if (clau.compare("NEXT*") == 0) {
		weight += 12;
	}
	else if (clau.compare("PARENT*") == 0) {
		weight += 13;
	}
	else if (clau.compare("AFFECTS*") == 0) {
		weight += 14;
	}
	else {
		weight += 15;
	}

	cout << "QEUERU "<< c->getQuery() << endl;
	string var1 = c->getRefOne().getType();
	string var2 = c->getRefTwo().getType();
	if ((var1 == "expr" || var1 == "constant")&&(var2 == "expr" || var2 == "constant")) {
		weight += 100;
	}
	else if ((var1 == "expr" || var1 == "constant") || (var2 == "expr" || var2 == "constant")) {
		weight += 200;
	}
	else {
		weight += 300;
	}
	return weight;
}

std::vector<Clause*> QueryTree::getClauses() {
	if (clauses.size() > 1) {

		if (sorted) {
			return tempClauses;
		}
		else {
			//for (auto x : clauses) {
			//	cout << "CLAUSeXX " << x->getClause() << endl;
			//}
			sort(weightedClauses.begin(), weightedClauses.end(), less_than_key());

			cout << "SIZE " << weightedClauses.size() << endl;
			for (auto x : weightedClauses) {
				cout << "CLAUSe " << x.first->getClause() << endl;
				cout << "CLAUSe " << x.first->getQuery() << endl;
				cout << "WEIGHT " << x.second << endl;
				tempClauses.push_back(x.first);
			}
			sorted = true;
			return tempClauses;
		}
	}
	else {
		return clauses;
	}
}

void QueryTree::newTree() {
	selects = {};
	clauses = {};
	weightedClauses = {};
	tempClauses = {};
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