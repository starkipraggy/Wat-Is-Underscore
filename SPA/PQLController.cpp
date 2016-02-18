#include "PQLController.h"
using namespace std;

const string COMMA = ",";

Preprocessor preprocessor;
QueryEvaluator evaluator;
Projector projector;

string PQLController::process(string input) {
	string output = "";
	vector<string> result;

	try {
		preprocessor.process(input);
		result = evaluator.process();
		output = projector.process(result);
	}
	catch (const char* msg) {
		output = "";
	}
	return output;
}