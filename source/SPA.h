#pragma once

#include "stdafx.h"
#include "SimpleParser.h"
#include "PKB.h"

class SPA {

private:
	SimpleParser* sp;

public:
	SPA();
	void processSimpleSourceFile(std::string);
};