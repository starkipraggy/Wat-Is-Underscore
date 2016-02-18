#pragma once

#include "stdafx.h"
#include "SimpleParser.h"
#include "PKB.h"
#include "./Preprocessor/PQLController.h"

class SPA {

private:
	SimpleParser* sp;
	PQLController pql;

public:
	SPA();
	void processSimpleSourceFile(std::string);
	std::string evaluate(std::string);
};