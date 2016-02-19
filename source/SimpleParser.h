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
	int checkWord(std::string);
	int isCharABrace(std::string cChar);
	int isCharAnOperator(std::string);
	std::string SimpleParser::addSpaceToString(std::string);
	int SimpleParser::checkAssign(unsigned int);
	std::vector<std::string> setTokens(std::string);

};

