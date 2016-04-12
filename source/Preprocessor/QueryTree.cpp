#include "QueryTree.h"

const regex designEntityRegex("^(procedure|stmtLst|stmt|assign|call|while|if|variable|constant|prog_line|plus|minus|times)$", icase);
const regex nonDesignEntityRegex("^(expr|integer|part_of_expr)$", icase);


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
	//std::pair <Clause*, int> ClausePair(c, determineWeight(c));
	//weightedClauses.push_back(ClausePair);
	clauses.push_back(c);
	//sorted = false;
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

	//cout << "QEUERU "<< c->getQuery() << endl;
	string var1 = c->getRefOne().getType();
	string var2 = c->getRefTwo().getType();
	if (regex_match(var1, nonDesignEntityRegex) && regex_match(var2, nonDesignEntityRegex)) {
		weight += 100;
	}
	else 	if (regex_match(var1, nonDesignEntityRegex) || regex_match(var2, nonDesignEntityRegex)) {
		weight += 200;
	}
	else {
		weight += 300;
	}
	return weight;
}

std::vector<Clause*> QueryTree::getClauses() {
	buildTree();

	if (weightedClauses.size() > 1) {

		if (sorted) {
			return tempClauses;
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
				tempClauses.push_back(x.first);
			}
			sorted = true;
			return tempClauses;
		}
	}
	else if(weightedClauses.size() == 1){
		return{ weightedClauses.at(0).first };
	}
	else {
		return {};
	}
}

std::vector<Clause*> QueryTree::getBoolClauses() {
	return boolClauses;
}

void QueryTree::newTree() {
	selects = {};
	clauses = {};
	weightedClauses = {};
	tempClauses = {};
}

void QueryTree::buildTree() {
	vector<string> currSet, nextSet;
	Ref ref1, ref2;
	string name1, name2, type1, type2;
	bool find1, find2;
	int weight = 1000;

	if (StringToUpper(selects.at(0).getType()) != "BOOLEAN") {
		for (unsigned int i = 0; i < selects.size(); i++) {
			currSet.push_back(selects.at(i).getName());
		}
	}

	while (!currSet.empty()) {
		for (vector<Clause*>::iterator it = clauses.begin(); it != clauses.end();) {
			Clause* c = *it;

			if (StringToUpper(c->getClause()) == "PATTERN") {
				PatternClause* p = dynamic_cast<PatternClause*>(c);
				ref1 = p->getRefOne();
				ref2 = p->getAssignedVariable();
			}
			else {
				ref1 = c->getRefOne();
				ref2 = c->getRefTwo();
			}
			name1 = ref1.getName();
			name2 = ref2.getName();
			type1 = ref1.getType();
			type2 = ref2.getType();
			find1 = false;
			find2 = false;

			if (regex_match(type1, designEntityRegex)) {
				if (find(currSet.begin(), currSet.end(), name1) != currSet.end()) {
					find1 = true;
				}
			}
			if (regex_match(type2, designEntityRegex)) {
				if (find(currSet.begin(), currSet.end(), name2) != currSet.end()) {
					find2 = true;
				}
			}

			if (find1 || find2) {
				if (!find1 && regex_match(type1, designEntityRegex)) {
					nextSet.push_back(name1);
				}
				if (!find2 && regex_match(type2, designEntityRegex)) {
					nextSet.push_back(name2);
				}
				std::pair <Clause*, int> ClausePair(c, weight + determineWeight(c));
				weightedClauses.push_back(ClausePair);
				sorted = false;
				it = clauses.erase(it);
			}
			else {
				++it;
			}
		}

		currSet = nextSet;
		nextSet = {};
		weight += 1000;
	}

	boolClauses = clauses;
}