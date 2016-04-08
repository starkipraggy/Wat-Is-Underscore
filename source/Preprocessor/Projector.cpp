#include "Projector.h"

std::vector<std::string> Projector::process(vector<vector<string>> rawResult) {
	vector<string> result;

	for (unsigned int i = 0; i < rawResult.size(); i++) {
		result.push_back(rawResult.at(i).at(0));
	}

	return result;
}