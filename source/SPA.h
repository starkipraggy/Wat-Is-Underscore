/*! \class SPA
\brief Represents the controller handling flow of data from the AutoTester to SimpleParser & PQLController

This class is used to represent the controller handling flow of data from the 
AutoTester to SimpleParser & PQLController. This class is a singleton for obvious
reasons, as we do not require the existence of multiple SimpleParsers or PQLControllers.
*/

#pragma once

#include "PKB.h"
#include "FileIO.h"
#include "stdafx.h"
#include "SimpleParser.h"
#include "./Preprocessor/PQLController.h"

class SPA {

private:
	FileIO* fio;
	SimpleParser* sp;
	PQLController pql;

	std::string contents;
	std::vector<std::string> tokens;

public:
	SPA();
	void processSimple(std::string);
	std::string evaluate(std::string);
};