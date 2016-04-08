#include "PQLController.h"
using namespace std;

Preprocessor preprocessor;
QueryEvaluator evaluator;
Projector projector;

std::vector<std::string> PQLController::process(std::string input) {
	vector<vector<string>> rawResult;
	vector<string> result;

	try {
		preprocessor.process(input);
		rawResult = evaluator.process();
		result = projector.process(rawResult);
	}
	catch (...) {
		result = {};
	}
	return result;
}