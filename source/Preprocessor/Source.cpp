#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <regex>
using namespace std;

class Clause {
public:
	string clause;
	string query;
	Clause(void);
	Clause(string clau, string que);
};

Clause::Clause(void) {
	clause = "";
	query = "";
}

Clause::Clause(string clau, string que) {
	clause = clau;
	query = que;
}

//declaration
std::unordered_map<std::string, std::string> processDeclaration(string statement);
std::pair<std::string, std::string> toDeclarationPair(string line);
bool isEntity(string word);
bool isSynoymn(string word);
/*
//query
string processQuery(string statement);
string getQuery(string statement);
std::pair<std::string, std::string> getSelect(string query);
std::vector<Clause> getClauses(string declarationQueries);
*/
//string
string StringToUpper(string strToConvert);
string trim(string word);
std::vector<std::string> tokenize(string line);

int main() {
	string statement = "while a;assign b; stmt c;select a such that uses(a, 'x')";
	/*
	try {
		std::unordered_map<std::string, std::string> result = processDeclaration(statement);
		for (auto& x : result) {
			std::cout << x.first << ": " << x.second << std::endl;
		}
	}
	catch (const char* msg) {
		std::cout << msg << std::endl;
	}
	*/
	/*try {
		std::cout << processQuery(statement) << std::endl;
	}
	catch (const char* msg) {
		std::cout << msg << std::endl;
	}*/

	std::cin.get();

}
/*
string processQuery(string statement) {
	string output = "empty";
	try {
		string query = getQuery(statement);
		std::pair<std::string, std::string> selectPair = getSelect(query);
		string querySynonym = selectPair.first;
		string declarationQueries = StringToUpper(selectPair.second);

		std::vector<Clause> clauses = getClauses(declarationQueries);
		if (clauses.size() > 0) {
			string output = clauses[0].clause + "+" + clauses[0].query;
		}
	}
	catch (const char* msg) {
		std::cout << msg << std::endl;
	}
	return output;
}

string getQuery(string statement) {
	if (statement == "") {
		throw "Query is empty";
	}
	else if (statement.find_last_of(";") != string::npos) {
		statement = statement.substr(statement.find_last_of(";") + 1, statement.length());
	}
	//else statement does not have declaration

	return statement;
}

//pair<querySynonym, declarationQueries>
std::pair<std::string, std::string> getSelect(string query) {
	string querySynonym, declarationQueries;

	query = trim(query);

	if (query.find(' ') != string::npos) {
		int position = query.find(' ');
		if (StringToUpper(query.substr(0, position)) != "SELECT") {
			throw "Query must start with Select";
		}
		query = trim(query.substr(position + 1, query.length()));
	}
	else {
		throw "Query cannot be one word";
	}

	if (query.find(' ') != string::npos) {
		int position = query.find(' ');
		querySynonym = query.substr(0, position);

		declarationQueries = trim(query.substr(position + 1, query.length()));
	}
	else {
		throw "Query cannot be two word";
	}

	std::pair<std::string, std::string> myPair(querySynonym, declarationQueries);
	return myPair;
}

std::vector<Clause> getClauses(string declarationQueries) {
	std::vector<Clause> vect(0);

	std::vector<std::string> tokenizedQueries = tokenize(declarationQueries);

	string prevClause = "";
	string prevQuery = "";
	int size = tokenizedQueries.size();
	for (int i = 0; i < size; i++) {
		if (StringToUpper(tokenizedQueries[i]).compare("SUCH") == 0 && i != tokenizedQueries.size()) {
			if (StringToUpper(tokenizedQueries[i + 1]).compare("THAT") == 0) {
				if (prevClause.compare("") == 0) {
					Clause clause(prevClause, prevQuery);
					vect.push_back(clause);
					prevClause = "SUCH THAT";
					prevQuery = "";
					i++;
				}
			}
			else {
				throw "such clause have to follow by that";
			}
		}
		else if (StringToUpper(tokenizedQueries[i]).compare("AND") == 0 && i != 0) {
			if (prevClause.compare("") == 0) {
				Clause clause(prevClause, prevQuery);
				vect.push_back(clause);
				prevClause = "AND";
				prevQuery = "";
			}
		}
		else {
			prevQuery += tokenizedQueries[i];
		}
	}

	if (StringToUpper(tokenizedQueries[tokenizedQueries.size() - 1]).compare("AND") != 0) {
		Clause clause(prevClause, prevQuery);
		vect.push_back(clause);
	}
	else {
		throw "Cannot end query with and clause";
	}

	return vect;
}
*/
std::unordered_map<std::string, std::string> processDeclaration(string statement) {
	std::unordered_map<std::string, std::string> declarationMap;

	while (statement.find(';') != string::npos) {
		int position = statement.find(';');
		string declaration = statement.substr(0, position);
		declaration = trim(declaration);
		pair<std::string, std::string> declarationPair;
		try {
			declarationPair = toDeclarationPair(declaration);
		}
		catch (const char* msg) {
			throw msg;
		}

		declarationMap.insert(declarationPair);
		statement = statement.substr(position + 1, statement.length());
	}

	return declarationMap;
}

//declaration:<entity> <synoymn> - user input
//pair:<synoymn> <entity> - for the purpose of map
std::pair<std::string, std::string> toDeclarationPair(string line) {
	if (line.find(" ") != string::npos) {
		int position = line.find(" ");

		string first = line.substr(0, position);
		first = trim(first);
		if (!isEntity(first)) {
			throw "Invalid entity";
		}

		string second = line.substr(position + 1, line.length());
		second = trim(second);
		if (!isSynoymn(second)) {
			throw "Invalid synoymn";
		}

		return std::make_pair(second, first);
	}
	else {
		throw "Declaration must have only one entity and one synoymn";
	}
}

bool isEntity(string word) {
	return std::regex_match(StringToUpper(word), std::regex("ASSIGN|STMT|WHILE"));
}

bool isSynoymn(string word) {
	bool check = !std::regex_match(word, std::regex("\s"));
	return check;
}

std::vector<std::string> tokenize(string line) {
	std::vector<std::string> vect(0);

	line = trim(line);
	while (line.find(' ') != string::npos) {
		int position = line.find(' ');
		vect.push_back(line.substr(0, position));

		line = trim(line.substr(position + 1, line.length()));
	}

	return vect;
}

string StringToUpper(string strToConvert) {

	std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::toupper);

	return strToConvert;
}

string trim(string word) {
	return std::regex_replace(word, std::regex("^ +| +$|( ) +"), "$1");
}
