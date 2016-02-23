#include "StubPKB.h"

StubPKB::StubPKB() {}
StubPKB::~StubPKB() {}

PKB* StubPKB::instance = NULL;

PKB* StubPKB::getInstance() {
	if (instance == NULL)
		instance = new StubPKB();
	return instance;
}

std::vector<std::string> StubPKB::PQLSelect(TNodeType outputType) {
	string var = "AnswerForSelect";
	vector<string> output;
	output.push_back(var);
	return output;
}

std::vector<std::string> StubPKB::PQLUses(std::string input, int argumentPosition, 
	std::string outputType) {
	string var = "AnswerForUses";
	string varMix = "AnswerForMix";
	vector<string> output;
	output.push_back(var);
	output.push_back(varMix);
	return output;
}

std::vector<std::string> StubPKB::PQLPattern(TNodeType type, Ref left, Ref right) {
	string var = "AnswerForPattern";
	string varMix = "AnswerForMix";
	vector<string> output;
	output.push_back(var);
	output.push_back(varMix);
	return output;
}
