#include "Projector.h"
using namespace std;

const string COMMA = ",";

string Projector::process(vector<string> result) {
	string output = "";
	if (!result.empty()) {
		if (result.size() == 1) {
			output = result.at(0);
		}
		else {
			for (auto& x : result) {
				output = x + COMMA;
			}
			output.substr(0, output.length() - 1);

		}
	}

	return output;
}