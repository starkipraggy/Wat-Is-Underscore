#pragma once

#include "SimpleParser.h"
#include "PKB.h"
#include <ctype.h>
#include <utility>
#include <unordered_map>
#include <unordered_set>
#include "Graph.h"
#include <conio.h>
#include <windows.h>
#include <dos.h>


std::vector<std::string> namesVariables;
std::vector<std::string> typesVariables;
std::vector<std::string> stackParenthesis;
// track ( ) in assginment statements
std::vector<std::string> stackAssignmentParenthesis;
const regex integerRegex("[[:digit:]]+");
// currentcontainers contains =  procedure / while / if / else
std::vector<std::string> currentContainer;
std::vector<std::pair <int,int>> edges;
std::unordered_set<std::string> procedureList; //does not include calls
std::vector<std::string> callList;
std::unordered_map<std::string, int> procedureMap;

std::string currentProcedure = "";

int procState = 0;

SimpleParser::SimpleParser() {

}

/* 
	This function will tokenize the Simple program into individual words and special characters.
*/
std::vector<std::string> SimpleParser::tokenize(std::string contents) {
	std::vector<std::string> tokens;
	std::string line, buffer;
	std::istringstream f(contents);
	SimpleParser* sp = new SimpleParser();
	while (std::getline(f, line)) {
		/* Adds space to the special characters */
		std::string spacedLine = addSpaceToString(line);
		std::stringstream ss(spacedLine);

		/* Splits string into tokens */
		while (ss >> buffer) {
			if (buffer.size() >= 2) {
				char first_char = buffer[0];
				std::string first_charS = buffer.substr(0, 0);
				char second_char = buffer[1];
				std::string second_charS = buffer.substr(1, 1);

				if ((first_char == '/' || sp->isCharASlash(first_charS)== 1)&& (second_char == '/' || sp->isCharASlash(second_charS) == 1)) {
					break;
				}
			}
			
			tokens.push_back(buffer);
		}
	}
	return tokens;
}

/* This function adds a space infront and behind each special character 
	as stated in the dictionary vector. */
std::string SimpleParser::addSpaceToString(std::string input) {
	std::vector<std::string> dictionary = { "{", "}", "=", "+", "-" , "*" ,";" , "(" ,")", ".","#" };
	unsigned int resultPos = 0;
	for (unsigned int i = 0;i < dictionary.size(); i++) {
		unsigned int pos = 0;
		pos = input.find(dictionary[i], pos);
		do {
			if (pos != std::string::npos) {
				input.replace(pos+resultPos, 1, " " + dictionary[i] + " ");
			}
			else {
				break;
			}
			pos = input.find(dictionary[i], pos + 2);
		} while (pos <= input.size());
	}
	return input;
}

bool SimpleParser::parseSimple(std::vector<std::string> tokens) {
	// if procstate == 0; procedure is invalid.
	// if procstate == 1; procedure is valid.
	procState = 0;
	isErrorDetected = false;
	namesVariables.clear();
	typesVariables.clear();
	stackParenthesis.clear();
	currentContainer.clear();
	procedureList.clear();
	stackAssignmentParenthesis.clear();
	callList.clear();
	procedureMap.clear();
	currentProcedure = "";
	edges.clear();

	if (tokens.size() < 4) {
		// invalid program
		isErrorDetected = true;
	} else {
		// Progress Bar Background
		std::cout << "Parsing Simple Program " << std::endl << std::endl;
		std::cout << "\t";
		char a = 177, b = 219;
		for (unsigned int k = 0;k < 65;k++)
			std::cout << a;
		std::cout << "\r";
		std::cout << "\t";
		int h = 0;

		for (unsigned int i = 0; i < tokens.size(); i++) {
			// Progress Bar Foreground
			int  j = (double)i / (double)tokens.size()*65.0;
			for (h; h<= j; h++) {
				std::cout << b;
			}

			switch (checkFirstWord(tokens[i])) {
			case 1:
				// first word is procedure
				// eats tokens until first opening brace
				i = checkProcedure(i,tokens);
				break;
			case 2:
				// first word is while
				// eats tokens until opening brace
				i = checkWhile(i, tokens);
				break;
			case 3:
				// first word is if
				// eats tokens until opening brace
				i = checkIf(i, tokens);
				break;
			case 6:
				// first word is call
				// eats tokens until semi colon
				i = checkCall(i,tokens);
				break;
			case 7:
				// first word is not the rest
				// eats tokens until semi colon
				i = checkAssign(i, tokens);
				break;
			default:
				// Invalid Program
				isErrorDetected = true;
				break;
			}
			

			if (isErrorDetected == true) {
				break;
			}
		}
	}

	std::cout << std::endl;
	std::cout << std::endl;

	if (stackParenthesis.empty() == false) {
		std::cout << "Program ended abruptly! " << std::endl;
		isErrorDetected = true;
		return isErrorDetected;
	}

	// check the call list whether the proc exist
	std::unordered_set<std::string>::const_iterator got;
	std::cout << "Check Calls. " << std::endl;
	if (callList.empty() == false) {
		for (auto x : callList) {
			got = procedureList.find(x);
			if (got == procedureList.end()) {
				// procedure not found
				std::cout << "Procedure " << x << " does not exist! "<< std::endl;
				isErrorDetected = true;
				return isErrorDetected;
			}
		}
	}

	std::cout << "Check Cycles." << std::endl;
	// check cyclic
	
	if (edges.empty() == false) {
		Graph g(procedureMap.size());
		for (auto x : edges) {
			g.addEdge(x.first,x.second);
		}

		if (g.isCyclic()) {
			std::cout << "Graph contains cycle. Invalid program! ";
			isErrorDetected = true;
			return isErrorDetected;
		}
	}

	return isErrorDetected;
}

/* 
	This function checks the validity of the 2nd and 3rd token, the procedure name 
	and opening brace respectively. If its wrong, then the program is invalid. If
	valid, it will call PKB->ProcedureStart();

	Returns the next position of the token
*/
int SimpleParser::checkProcedure(unsigned int position, std::vector<std::string> tokens) {
	// Prints procedure
	//std::cout << tokens[position] << " ";
	position++;
	// Prints procedure name
	//std::cout << tokens[position] << " " ;
	std::string procName = tokens[position];
	char first_char = procName[0];
	std::string first_charS = procName.substr(0,0);

	if (isCharABrace(tokens[position]) || isCharAnOperator(tokens[position]) || first_char == '_' || isCharAnInteger(first_charS)) {
		std::cout << std::endl <<"Procedure name is invalid! " << std::endl;
		procState = 0;
		isErrorDetected = true;
		return position;
	}
	position++;
	// Prints opening brace
	//std::cout << tokens[position] << std::endl;

	std::unordered_set<std::string>::const_iterator got;
	try {
		switch (isCharABrace(tokens[position])) {
		case 1: // "{"
			procState = 1;
			stackParenthesis.push_back(tokens[position]);
			currentContainer.push_back("procedure");
			currentProcedure = tokens[position - 1];
			// need hashmap, to check duplicate procs
			// anther hash mapfor all proc inclufding call.
			got = procedureList.find(currentProcedure);
			if (got == procedureList.end()||procedureList.size() ==0) {
				procedureList.insert(currentProcedure);
				procedureMap.insert(std::pair<std::string, int>(currentProcedure, (procedureMap.size())));
				PKB::getInstance()->ProcedureStart(tokens[position - 1]);
			}
			else {
				std::cout << "Cannot have duplicate procedure names! " << std::endl;
				isErrorDetected = true;
				return position;
			}
			break;
		case 2: // "}" invalid program
			stackParenthesis.pop_back();
		default:
			procState = 0;
			isErrorDetected = true;
			break;
		}
	} catch (std::exception& e) {
		std::cout << "Error in parsing Procedure! " << std::endl;
		isErrorDetected = true;
	}

	try {
		switch (isCharABrace(tokens[position + 1])) {
		case 2: // "}" invalid program
			stackParenthesis.pop_back();
			procState = 0;
			isErrorDetected = true;
			break;
		}
	}
	catch (std::exception& e) {
		std::cout << "PROCEDURE must contain a statement" << std::endl;
		e;
		isErrorDetected = true;
	}

	return position;
}

/*
	This function checks the validity of the 2nd and 3rd token, the while name
	and opening brace respectively. If its wrong, then the program is invalid. If
	valid, it will call PKB->WhileStart();

	Returns the next position of the token
*/
int SimpleParser::checkWhile(unsigned int position, std::vector<std::string> tokens) {
	// Prints while
	//std::cout << tokens[position] << " ";
	if (procState == 0) {
		isErrorDetected = true;
		return position;
	}
	position++;
	// Prints while variable
	//std::cout << tokens[position] << " ";
	std::string procName = tokens[position];
	char first_char = procName[0];
	std::string first_charS = procName.substr(0, 0);

	if (isCharABrace(tokens[position]) || isCharAnOperator(tokens[position]) || first_char == '_' || isCharAnInteger(first_charS)) {
		std::cout << "While control variable is invalid! " << std::endl;
		procState = 0;
		isErrorDetected = true;
		return position;
	}

	position++;
	// Prints opening brace
	//std::cout << tokens[position] << " ";
	std::string back = currentContainer.back();
	try {
		switch (isCharABrace(tokens[position])) {
		case 1: // "{"
			stackParenthesis.push_back(tokens[position]);
			currentContainer.push_back("while");
			PKB::getInstance()->WhileStart(tokens[position-1]);
			break;
		case 2: // "}" invalid program
			stackParenthesis.pop_back();
		default:
			procState = 0;
			isErrorDetected = true;
			break;
		}
	}
	catch (std::exception& e) {
		std::cout << "Error in parsing WHILE! " << std::endl;
		e;
		isErrorDetected = true;
	}

	try {
		switch (isCharABrace(tokens[position+1])) {
		case 2: // "}" invalid program
			stackParenthesis.pop_back();
			procState = 0;
			isErrorDetected = true;
			break;
		}
	}
	catch (std::exception& e) {
		std::cout << "WHILE must contain a statement" << std::endl;
		e;
		isErrorDetected = true;
	}
	return position;
}

/*
This function checks the validity of the 2nd and 3rd and 4th token, the if var, "then" key word
and opening brace respectively. If its wrong, then the program is invalid. If
valid, it will call PKB->IfStart();

Returns the next position of the token
*/
int SimpleParser::checkIf(unsigned int position, std::vector<std::string> tokens) {
	// Prints if
	//std::cout << tokens[position] << " ";
	if (procState == 0) {
		isErrorDetected = true;
		return position;
	}

	position++;
	// Prints if variable
	//std::cout << tokens[position] << " ";
	std::string procName = tokens[position];
	char first_char = procName[0];
	std::string first_charS = procName.substr(0, 0);

	if (isCharABrace(tokens[position]) || isCharAnOperator(tokens[position]) || first_char == '_' || isCharAnInteger(first_charS)) {
		std::cout << "IF control variable is invalid! " << std::endl;
		procState = 0;
		isErrorDetected = true;
		return position;
	}

	position++;
	// Prints Then
	//std::cout << tokens[position] << " ";
	if (checkFirstWord(tokens[position]) != 4) {
		std::cout << "Where is the, THEN, keyword? " << std::endl;
		procState = 0;
		isErrorDetected = true;
		return position;
	}

	position++;
	// Prints opening brace
	//std::cout << tokens[position] << " ";
	std::string back = currentContainer.back();
	try {
		switch (isCharABrace(tokens[position])) {
		case 1: // "{"
			stackParenthesis.push_back(tokens[position]);
			currentContainer.push_back("if");
			PKB::getInstance()->IfStart(tokens[position - 2]);
			break;
		case 2: // "}" invalid program
			stackParenthesis.pop_back();
		default:
			procState = 0;
			std::cout << "Error in parsing IF! " << std::endl;
			isErrorDetected = true;
			break;
		}
	}
	catch (std::exception& e) {
		std::cout << "Error in parsing IF! " << std::endl;
		e;
		isErrorDetected = true;
	}

	try {
		switch (isCharABrace(tokens[position + 1])) {
		case 2: // "}" invalid program
			stackParenthesis.pop_back();
			procState = 0;
			isErrorDetected = true;
			break;
		}
	}
	catch (std::exception& e) {
		std::cout << "IF block must contain a statement" << std::endl;
		e;
		isErrorDetected = true;
	}


	return position;
}


int SimpleParser::checkElse(unsigned int position, std::vector<std::string> tokens) {
	if (checkFirstWord(tokens[position]) != 5) {
		std::cout << "Start with IF first instead of ELSE " << std::endl;
		isErrorDetected = true;
		return position;
	}
	else {
		// Prints ELSE
		//std::cout << tokens[position] << " ";
		position++;
		try {
			switch (isCharABrace(tokens[position])) {
			case 1: // "{"
				if (currentContainer.back().compare("if") == 0) {
					// Prints Opening Brace
					//std::cout << tokens[position] << " ";
					stackParenthesis.pop_back();
					currentContainer.pop_back();
					stackParenthesis.push_back(tokens[position]);
					currentContainer.push_back("else");
					PKB::getInstance()->ElseStart();
				}
				else {
					procState = 0;
					isErrorDetected = true;
					break;
				}
				break;
			case 2: // "}" invalid program
				stackParenthesis.pop_back();
			default:
				procState = 0;
				std::cout << "Error in parsing ELSE! " << std::endl;
				isErrorDetected = true;
				break;
			}
		}
		catch (std::exception& e) {
			std::cout << "Error in parsing ELSE! " << std::endl;
			e;
			isErrorDetected = true;
		}
	}

	try {
		switch (isCharABrace(tokens[position + 1])) {
		case 2: // "}" invalid program
			stackParenthesis.pop_back();
			procState = 0;
			isErrorDetected = true;
			break;
		}
	}
	catch (std::exception& e) {
		std::cout << "ELSE must contain a statement" << std::endl;
		e;
		isErrorDetected = true;
	}

	return position;
}

/*
This function checks the validity of the 2nd token, whether the procedure exists
and semi colon respectively. If its wrong, then the program is invalid. If
valid, it will call PKB->CallStart();

Returns the next position of the token
*/
int SimpleParser::checkCall(unsigned int position, std::vector<std::string> tokens) {
	// Prints call
	//std::cout << tokens[position] << " ";

	if (procState == 0) {
		isErrorDetected = true;
		return position;
	}

	position++;
	// Prints call name
	//std::cout << tokens[position] << " ";
	std::string callName = tokens[position];
	char first_char = callName[0];
	std::string first_charS = callName.substr(0, 0);

	if (isCharABrace(tokens[position]) || isCharAnOperator(tokens[position]) || first_char == '_' || isCharAnInteger(first_charS)) {
		std::cout << std::endl << "Call name is invalid! " << std::endl;
		isErrorDetected = true;
		return position;
	}

	if (StringToUpper(currentProcedure).compare(StringToUpper(callName)) == 0) {
		std::cout << std::endl << "No recursive call! " << std::endl;
		isErrorDetected = true;
		return position;
	}

	position++;
	// Prints semi colon
	//std::cout << tokens[position] << " ";

	std::unordered_map<std::string,int>::const_iterator got;
	std::unordered_map<std::string, int>::const_iterator got2;

	std::pair <int,int> callEdge;
	try {
		switch (isCharAnOperator(tokens[position])) {
		case 5: // ";"
			//callEdge = std::make_pair(currentProcedure, callName);
			//edges.push_back(callEdge);
			//procedureMap.insert(std::pair<std::string, int>(callName, (procedureMap.size())));

			got = procedureMap.find(callName);
			if (got == procedureMap.end()) {
				//new call proc,add to map
				procedureMap.insert(std::pair<std::string, int>(callName, (procedureMap.size())));
				got = procedureMap.find(currentProcedure);
				got2 = procedureMap.find(callName);
				callEdge = std::make_pair(got->second,got2->second);
				edges.push_back(callEdge);
			}
			else {
				got = procedureMap.find(currentProcedure);
				got2 = procedureMap.find(callName);
				callEdge = std::make_pair(got->second, got2->second);
				edges.push_back(callEdge);
			}

			callList.push_back(callName); 

			if (edges.empty() == false) {
				Graph g(procedureMap.size());
				for (auto x : edges) {
					g.addEdge(x.first, x.second);
				}

				if (g.isCyclic()) {
					std::cout << "Graph contains cycle. Invalid program! ";
					isErrorDetected = true;
					return position;
				}
			}

			PKB::getInstance()->CallStatement(tokens[position - 1]);
			break;
		default:
			isErrorDetected = true;
			break;
		}
	}
	catch (std::exception& e) {
		std::cout << "Error in parsing Call! " << std::endl;
		e;
		isErrorDetected = true;
	}
	return position;
}

/*
	When entering this function, it will iterate the tokens from the passed in position,
	until reaching semi colon.
	Check for closing brace first, to signify end of while / if / else /procedure.
	Then check for assignment statement.
*/
int SimpleParser::checkAssign(unsigned int position, std::vector<std::string> tokens) {
	if (procState == 0) {
		isErrorDetected = true;
		std::cout << "Where is the PROCEDURE?? " << std::endl;
		return position;
	}

	// Check closing brace first
	if (isCharABrace(tokens[position]) == 1) {
		// Another opening brace
		// Invalid program
		isErrorDetected = true;
		std::cout << "Extra opening brace detected. " << std::endl;
		return position;
	}
	else if (isCharABrace(tokens[position]) == 2) {
		// Prints Closing brace 
		//std::cout << tokens[position] << std::endl;
		try {
			std::string back = currentContainer.back();
			if (back.compare("while") == 0) {
				stackParenthesis.pop_back();
				currentContainer.pop_back();
				PKB::getInstance()->WhileEnd();
			}
			else if (back.compare("procedure") == 0) {
				stackParenthesis.pop_back();
				currentContainer.pop_back();
				PKB::getInstance()->ProcedureEnd();
			}
			else if (back.compare("if") == 0) {
				// if closing brace of if block found, then next word must be else, anything else is invalid
				position = checkElse(position+1, tokens);
				return position;
			}
			else if (back.compare("else") == 0) {
				stackParenthesis.pop_back();
				currentContainer.pop_back();
				PKB::getInstance()->IfElseEnd();
			}
		}
		catch (std::exception& e) {
			std::cout << "Error parsing the closing brace! " << std::endl;
			isErrorDetected = true;
			e;
			return position;
		}
	}
	else {
		// If its not a closing brace, then check for assignment statement.
		std::vector<ExpressionTokenType> types;
		// if checkLeftVar = false, means that leftVar has not parsed 
		bool checkLeftVar = false;
		// if checkRightVar = false, means that RightVar has not parsed 
		bool checkRightVar = false;
		// if checkOperator = false, means that operator has not parsed 
		bool checkOperator = false;
		// if checkEqualsOperatorr = false, means that EqualsOperator has not parsed 
		bool checkEqualsOperator = false;
		// Left side of assignment statement before =
		std::string leftVar;
		// Right side of assignment statement after = 
		std::vector<std::string> rightVariables;


		for (position;position < tokens.size();position++) {
			// Prints the token
			//std::cout << tokens[position] << " ";
			switch (isCharAnOperator(tokens[position])) {
			case 0:
				// Token is either variable, constant or unknown
				if (checkEqualsOperator == false && checkLeftVar == false) {
					// Token is left variable
					if (isCharAnInteger(tokens[position])) {
						// Assignment statement cannot start with integer
						isErrorDetected = true;
						return position;
					}
					else {
						leftVar = tokens[position];
						checkLeftVar = true;
					}
				}
				else  if (checkEqualsOperator == true && checkLeftVar == true) {
					// check right var
					if (isCharAnInteger(tokens[position])) {
						rightVariables.push_back(tokens[position]);
						types.push_back(Constant);
						checkRightVar = true;
						checkOperator = false;
					}
					else {
						rightVariables.push_back(tokens[position]);
						types.push_back(Variable);
						checkRightVar = true;
						checkOperator = false;
					}
				}
				else {
					isErrorDetected = true;
					return position;
				}
				break;
			case 1:
				// Token is =
				if (checkEqualsOperator == false && checkLeftVar == true) {
					checkEqualsOperator = true;
				}
				else {
					isErrorDetected = true;
					return position;
				}
				break;
			case 2:
				// Token is +
			case 3:
				// Token is *
			case 4:
				// Token is -
				if (checkLeftVar == false || checkEqualsOperator == false) {
					// Assignment statement started with +/*/- or occurs before =, invalid program
					isErrorDetected = true;
					return position;
				}
				else if (checkLeftVar == true && checkEqualsOperator == true 
					&& checkRightVar == true && checkOperator == false) {
					checkOperator = true;
					checkRightVar = false;
					rightVariables.push_back(tokens[position]);
					types.push_back(Operator);
				}
				else {
					isErrorDetected = true;
					return position;
				}
				break;
			case 5:
				// Token is ;
				if (checkLeftVar == false || checkEqualsOperator == false) {
					// Assignment statement started with ;, invalid program
					isErrorDetected = true;
					return position;
				}
				else if (checkLeftVar == true && checkEqualsOperator == true && checkRightVar == true && checkOperator == false) {
					
					if (stackAssignmentParenthesis.size() > 0) {
						isErrorDetected = true;
						return position;
					}
					
					PKB::getInstance()->AssignStatement(leftVar, rightVariables, types);

					rightVariables.clear();
					types.clear();
					leftVar = "";
					return position;
				}
				else {
					isErrorDetected = true;
					return position;
				}
				break;
			case 6:
				// Token is  (
				if (checkLeftVar == false || checkEqualsOperator == false) {
					// Token occurs before =, invalid program
					isErrorDetected = true;
					return position;
				}
				else {
					// check var before ( and operator after (
					// chec position -1 and position +1
					if (isCharAnOperator(tokens[position - 1]) == 0 || (isCharAnOperator(tokens[position + 1]) == (2||3||4||5))) {
						isErrorDetected = true;
						return position;
					}
					else {
						stackAssignmentParenthesis.push_back("(");
						rightVariables.push_back(tokens[position]);
						types.push_back(Parenthesis);
					}
				}
				break;
			case 7:
				// Token is )
				if (checkLeftVar == false || checkEqualsOperator == false) {
					// Token occurs before =, invalid program
					isErrorDetected = true;
					return position;
				}
				else {
					if (isCharAnOperator(tokens[position + 1]) == 0 || (isCharAnOperator(tokens[position - 1]) == (2 || 3 || 4 || 5))) {
						isErrorDetected = true;
						return position;
					}
					else {
						if (stackAssignmentParenthesis.size() < 1) {
							isErrorDetected = true;
							return position;
						}

						try {
							std::string back = stackAssignmentParenthesis.back();
							if (back.compare("(") == 0) {
								stackAssignmentParenthesis.pop_back();
								rightVariables.push_back(tokens[position]);
								types.push_back(Parenthesis);
							}
						}
						catch (std::exception& e) {
							std::cout << "Error parsing the assignment parenthesis! " << std::endl;
							isErrorDetected = true;
							e;
							return position;
						}
					}
				}


				break;
			default:
				isErrorDetected = true;
				return position;
			}
		}
	}

	return position;
}

int SimpleParser::checkFirstWord(std::string word) {
	word = StringToUpper(word);	
	if (word.compare("PROCEDURE") == 0) {
		return 1;
	}
	else if (word.compare("WHILE") == 0) {
		return 2;
	}
	else if (word.compare("IF") == 0) {
		return 3;
	}
	else if (word.compare("THEN") == 0) {
		return 4;
	}
	else if (word.compare("ELSE") == 0) {
		return 5;
	}
	else if (word.compare("CALL") == 0) {
		return 6;
	}
	else {
		return 7;
	}
}

int SimpleParser::isCharAnInteger(std::string s) {
	return regex_match(s, integerRegex);
}

int SimpleParser::isCharABrace(std::string cChar) {
	if (cChar.compare("{") == 0) {
		return 1;
	}
	else if (cChar.compare("}")==0) {
		return 2;
	}
	else {
		return 0;
	}
}

int SimpleParser::isCharAnOperator(std::string cChar) {
	if (cChar.compare("=") == 0) {
		return 1;
	}
	else if (cChar.compare("+") == 0) {
		return 2;
	}
	else if (cChar.compare("*") == 0) {
		return 3;
	}
	else if (cChar.compare("-") == 0) {
		return 4;
	}
	else if (cChar.compare(";") == 0) {
		return 5;
	}
	else if (cChar.compare("(") == 0) {
		return 6;
	}
	else if (cChar.compare(")") == 0) {
		return 7;
	}
	else {
		return 0;
	}
}

int SimpleParser::isCharASlash(std::string cChar) {
	if (cChar.compare("/") == 0) {
		return 1;
	}
	else {
		return 0;
	}
}