#include "PQLController.h"
using namespace std;

Preprocessor preprocessor;
QueryEvaluator evaluator;
Projector projector;

std::vector<std::string> PQLController::process(std::string input) {
	vector<string> result;

	try {
		preprocessor.process(input);
		result = evaluator.process();
		result = projector.process(result);
	}
	catch (...) {
		result = {};
	}
	return result;
}