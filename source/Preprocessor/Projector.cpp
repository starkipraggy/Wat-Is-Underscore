#include "Projector.h"

std::vector<std::string> Projector::process(vector<vector<string>> rawResult) {
	vector<string> result;
	string temp;

	if (rawResult.at(0).size() <= 1) {
		for (unsigned int i = 0; i < rawResult.size(); i++) {
			result.push_back(rawResult.at(i).at(0));
		}
	}
	else {
		for (unsigned int i = 0; i < rawResult.size(); i++) {
			temp = "";
			for (unsigned int j = 0; j < rawResult.at(i).size(); j++) {
				temp += rawResult.at(i).at(j) + " ";
			}
			temp = trim(temp);

			result.push_back(temp);
		}
	}

	return result;
}