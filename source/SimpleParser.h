#pragma once
#include "stdafx.h"
#include "FileIO.h"
#include <regex>

class SimpleParser {

private:
	FileIO* fio;

public:
	SimpleParser();

	void processSimple(std::string);
	void processLine();
	void checkProcedure(std::vector<std::string>);
	int isCharABrace(std::string cChar);
	std::string SimpleParser::addSpaceToString(std::string);
};

