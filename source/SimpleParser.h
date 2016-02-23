/*! \class SimpleParser
\brief Used to parse Simple Program codes given in a text file.

Contains the necessary functions to parse Simple program from a text file.
*/

#pragma once
#include "stdafx.h"
#include "FileIO.h"
#include <regex>

class SimpleParser {

private:
	//! If true, parsing will stop and return "Invalid program!"
	bool isErrorDetected = false;

public:
	SimpleParser();

	//! Tokenizes the Simple program
	/*!
	Tokenizes the Simple program
	\param contents of the Simple Program
	\return Vector of string tokens
	*/
	std::vector<std::string> SimpleParser::tokenize(std::string);

	//! Adds space before and after special characters (=,+,-,*,;,{,})
	/*!
	Adds space before and after special characters (=,+,-,*,;,{,})
	\param contents of the Simple Program
	\return String input with spaces
	*/
	std::string SimpleParser::addSpaceToString(std::string);

	//! The main function where the simple program actually starts parsing
	/*!
	The main function where the simple program actually starts parsing
	\param vector of tokens
	*/
	void SimpleParser::parseSimple(std::vector<std::string>);

	//! Checks the first token to be either procedure/while/if/else/call
	/*!
	Checks the first token to be either procedure/while/if/else/call
	\param string token
	/return 1 = procedure, 2=while, 3 = if, 4= call, 5= anything else
	*/
	int SimpleParser::checkFirstWord(std::string);
	int SimpleParser::isCharABrace(std::string);
	int SimpleParser::isCharAnOperator(std::string);
	int SimpleParser::isCharAnInteger(std::string);
	int SimpleParser::checkProceure(unsigned int position, std::vector<std::string>);
	int SimpleParser::checkWhile(unsigned int position, std::vector<std::string>);
	int SimpleParser::checkAssign(unsigned int, std::vector<std::string>);


	/* Not implemented yet */
	int SimpleParser::checkIf(unsigned int);
	/* Not implemented yet */
	int SimpleParser::checkCall(unsigned int);
};

