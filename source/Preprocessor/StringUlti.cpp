#include <string>
#include <vector>
#include <regex>
#include "StringUlti.h"
using namespace std;

std::vector<std::string> tokenize(string line) {
	return tokenize(line, " ");
}

std::vector<std::string> tokenize(string line, string regex) {
	std::vector<std::string> vect(0);

	line = trim(line);
	while (line.find(regex) != string::npos) {
		int position = line.find(regex);
		vect.push_back(line.substr(0, position));

		line = trim(line.substr(position + 1, string::npos));
	}

	vect.push_back(line);
	return vect;
}

string StringToUpper(string strToConvert) {

	std::transform(strToConvert.begin(), strToConvert.end(), strToConvert.begin(), ::toupper);

	return strToConvert;
}

string trim(string word) {
	return std::regex_replace(word, std::regex("^ +| +$|( ) +"), "$1");
}
