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
	static std::vector<std::string> SimpleParser::tokenize(std::string);

	//! Adds space before and after special characters (=,+,-,*,;,{,})
	/*!
	Adds space before and after special characters (=,+,-,*,;,{,})
	\param contents of the Simple Program
	\return String input with spaces
	*/
	static std::string SimpleParser::addSpaceToString(std::string);

	//! The main function where the simple program actually starts parsing
	/*!
	The main function where the simple program actually starts parsing
	\param vector of tokens
	\return true for correct parsing, false otherwise
	*/
	bool SimpleParser::parseSimple(std::vector<std::string>);

	//! Checks the first token to be either procedure/while/if/else/call
	/*!
	Checks the first token to be either procedure/while/if/else/call
	\param string token
	\return 1 = procedure, 2=while, 3 = if, 4= call, 5= anything else
	*/
	int SimpleParser::checkFirstWord(std::string);

	//! Checks whether the string is a brace "{" , "}"
	/*!
	Checks whether the string is a brace "{" , "}"
	\param string token
	\return 1 = {, 2 = }, 0= anything else
	*/
	int SimpleParser::isCharABrace(std::string);

	//! Checks whether the string is an operator "=","+","*","-",";" 
	/*!
	Checks whether the string is a brace "=","+","*","-",";" 
	\param string token
	\return 1 = =, 2 = +, 3 = *, 4 = -, 5 = ;, 0 = anything else
	*/
	int SimpleParser::isCharAnOperator(std::string);

	//! Checks whether the string is an integer
	/*!
	Checks whether the string is an integer
	\param string token
	\return true for integer, false if not
	*/
	int SimpleParser::isCharAnInteger(std::string);

	//! Checks the current occurrence of the procedure statement
	/*!
	Checks the current occurrence of the procedure statement
	\param int position, string token
	\return next available position for the token if the procedure is correct, else it will still
	return the position of the token but set the isInvalidProgram boolean to true;
	*/
	int SimpleParser::checkProcedure(unsigned int position, std::vector<std::string>);

	//! Checks the current occurrence of the while statement
	/*!
	Checks the current occurrence of the while statement
	\param int position, string token
	\return next available position for the token if the procedure is correct, else it will still
	return the position of the token but set the isInvalidProgram boolean to true;
	*/
	int SimpleParser::checkWhile(unsigned int position, std::vector<std::string>);

	//! Checks the current occurrence of any other statement
	/*!
	Checks the current occurrence of any other statement. When entering this function, 
	Check for closing brace first, to signify end of while / if / else /procedure.
	Else it will iterate the tokens from the passed in position, until reaching semi colon for assignment statement.
	\param int position, string token
	\return next available position for the token if the procedure is correct, else it will still
	return the position of the token but set the isInvalidProgram boolean to true;
	*/
	int SimpleParser::checkAssign(unsigned int, std::vector<std::string>);

	/* Not implemented yet */
	int SimpleParser::checkIf(unsigned int, std::vector<std::string>);
	int SimpleParser::checkElse(unsigned int, std::vector<std::string>);
	/* Not implemented yet */
	int SimpleParser::checkCall(unsigned int);
};

