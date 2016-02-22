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
	string var = "v";
	vector<string> output;
	output.push_back(var);
	return output;
}