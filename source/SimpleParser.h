#pragma once
#include "stdafx.h"
#include "FileIO.h"

class SimpleParser {


private:
	FileIO* fio;
public:
	SimpleParser();

	void processSimple(std::string);
	void processLine(std::string);
};

