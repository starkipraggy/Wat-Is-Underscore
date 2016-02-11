#pragma once

#include "stdafx.h"
#include "SimpleParser.h"

class SPA {

private:
	SimpleParser* sp;


public:
	SPA();
	void processSimpleSourceFile(std::string);
};