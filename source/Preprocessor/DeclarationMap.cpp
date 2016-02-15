#include "DeclarationMap.h"
#include "StringUlti.h"
#include <string>
#include <unordered_map>
#include <regex>
using namespace std;

DeclarationMap* DeclarationMap::m_pInstance = NULL;

DeclarationMap* DeclarationMap::Instance(){
	if (!m_pInstance) {
		m_pInstance = new DeclarationMap;
	}
	return m_pInstance;
}

DeclarationMap::DeclarationMap() {}

void DeclarationMap::setMap(string statement) {

	while (statement.find(';') != string::npos) {
		int position = statement.find(';');
		string declaration = statement.substr(0, position);
		declaration = trim(declaration);
		try {
			setDeclaration(declaration);
		}
		catch (const char* msg) {
			throw msg;
		}
		statement = statement.substr(position + 1, statement.length());
	}

}

string DeclarationMap::find(string key) {
	string output = "";
	std::unordered_map<std::string, std::string>::const_iterator got = declarationMap.find(key);

	if (got == declarationMap.end())
		throw "no such variable" + key;
	else
		output = got->second;

	return output;
}

std::unordered_map<std::string, std::string> DeclarationMap::getMap() {
	return declarationMap;
}


//private

//declaration:<entity> <synoymn> - user input
//pair:<synoymn> <entity> - for the purpose of map
void DeclarationMap::setDeclaration(string line) {
	if (line.find(" ") != string::npos) {
		int position = line.find(" ");

		string first = line.substr(0, position);
		first = trim(first);
		if (!std::regex_match(StringToUpper(first), std::regex("ASSIGN|STMT|WHILE|VARIABLE|CONSTANT|PROG_LINE|PROCEDURE"))) {
			throw "Invalid entity";
		}

		string second = line.substr(position + 1, line.length());
		second = trim(second);
		vector<string> variables = tokenize(second, ",");
		for (auto& x : variables) {
			if (!std::regex_match(x, std::regex("\s"))) {
				std::pair<std::string, std::string> newPair(trim(x), first);
				declarationMap.insert(newPair);
			}
			else {
				throw "variable '" + x + "' cannot have space";
			}
		}
	}
}