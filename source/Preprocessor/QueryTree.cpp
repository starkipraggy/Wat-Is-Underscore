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
	if (clau.compare("WITH") == 0) {
		return 10;
	}
	else if (clau.compare("MODIFIES") == 0) {
		return 20;
	}
	else if (clau.compare("USES") == 0) {
		return 25;
	}
	else if (clau.compare("FOLLOWS") == 0) {
		return 30;
	}
	else if (clau.compare("FOLLOWS*") == 0) {
		return 40;
	}
	else if (clau.compare("PARENT") == 0) {
		return 50;
	}
	else if (clau.compare("PARENT*") == 0) {
		return 60;
	}
	else if (clau.compare("CALLS") == 0) {
		return 70;
	}
	else if (clau.compare("NEXT") == 0) {
		return 80;
	}
	else if (clau.compare("PATTERN") == 0) {
		return 90;
	}
	else if (clau.compare("CALLS*") == 0) {
		return 100;
	}
	else if (clau.compare("NEXT*") == 0) {
		return 110;
	}
	else if (clau.compare("AFFECTS") == 0) {
		return 120;
	}
	else if (clau.compare("AFFECTS*") == 0) {
		return 130;
	}
	else {
		return 500;
	}
}

std::vector<Clause*> QueryTree::getClauses() {
	if (clauses.size() > 3) {

		if (sorted) {
			return wClauses;
		}
		else {
			//for (auto x : clauses) {
			//	cout << "CLAUSeXX " << x->getClause() << endl;
			//}
			sort(weightedClauses.begin(), weightedClauses.end(), less_than_key());

			//cout << "SIZE " << weightedClauses.size() << endl;
			for (auto x : weightedClauses) {
				//cout << "CLAUSe " << x.first->getClause() << endl;
				//cout << "CLAUSe " << x.first->getQuery() << endl;
				//cout << "WEIGHT " << x.second << endl;
				wClauses.push_back(x.first);
			}
			sorted = true;
			return wClauses;
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
	wClauses = {};
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