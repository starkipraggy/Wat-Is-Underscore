#pragma once

#include "SimpleParser.h"
#include "PKB.h"
#include <ctype.h>

std::vector<std::string> namesProcedure;
std::vector<std::string> namesVariables;
std::vector<std::string> typesVariables;
std::vector<std::string> stackParenthesis;
const regex integerRegex("[[:digit:]]+");
// currentcontainers contains =  procedure / while / if / else
std::vector<std::string> currentContainer;

SimpleParser::SimpleParser() {

}

/* 
	This function will tokenize the Simple program into individual words and special characters.
*/
std::vector<std::string> SimpleParser::tokenize(std::string contents) {
	std::vector<std::string> tokens;
	std::string line, buffer;
	std::istringstream f(contents);
	
	while (std::getline(f, line)) {
		/* Adds space to the special characters */
		std::string spacedLine = addSpaceToString(line);
		std::stringstream ss(spacedLine);

		/* Splits string into tokens */
		while (ss >> buffer) {
			tokens.push_back(buffer);
		}
	}
	return tokens;
}

/* This function adds a space infront and behind each special character 
	as stated in the dictionary vector. */
std::string SimpleParser::addSpaceToString(std::string input) {
	std::vector<std::string> dictionary = { "{", "}", "=", "+", "-" , "*" ,";" , "(" ,")" };
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

int procState = 0;
bool isStatementDetectedProcedure = false;

bool SimpleParser::parseSimple(std::vector<std::string> tokens) {
	// if procstate == 0; procedure is invalid.
	// if procstate == 1; procedure is valid.
	procState = 0;
	isStatementDetectedProcedure = false;
	isErrorDetected = false;
	namesProcedure.clear();
	namesVariables.clear();
	typesVariables.clear();
	stackParenthesis.clear();
	currentContainer.clear();

	if (tokens.size() < 4) {
		// invalid program
		
		isErrorDetected = true;
	} else {
		for (unsigned int i = 0; i < tokens.size(); i++) {
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
				
				checkCall(i);
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
	if (isStatementDetectedProcedure == false) {
		std::cout << "Procedure must contain statements! " << std::endl;
		isErrorDetected = true;
	} 
	if (stackParenthesis.empty() == false) {
		std::cout << "Program ended abruptly! " << std::endl;
		isErrorDetected = true;
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
	std::cout << tokens[position] << " ";
	position++;
	// Prints procedure name
	std::cout << tokens[position] << " " ;
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
	std::cout << tokens[position] << std::endl;
	try {
		switch (isCharABrace(tokens[position])) {
		case 1: // "{"
			procState = 1;
			stackParenthesis.push_back(tokens[position]);
			currentContainer.push_back("procedure");
			isStatementDetectedProcedure = false; // Reset variable for new procedures
			PKB::getInstance()->ProcedureStart(tokens[position-1]);
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
	std::cout << tokens[position] << " ";
	if (procState == 0) {
		isErrorDetected = true;
		return position;
	}
	position++;
	// Prints while variable
	std::cout << tokens[position] << " ";
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
	std::cout << tokens[position] << " ";
	std::string back = currentContainer.back();
	try {
		switch (isCharABrace(tokens[position])) {
		case 1: // "{"
			stackParenthesis.push_back(tokens[position]);
			currentContainer.push_back("while");
			//enterWhileFlag = 1;
			//if (back.compare("if") == 0) {
			//	enterIfFlag = 0;
			//}
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
	std::cout << tokens[position] << " ";
	if (procState == 0) {
		isErrorDetected = true;
		return position;
	}

	position++;
	// Prints if variable
	std::cout << tokens[position] << " ";
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
	std::cout << tokens[position] << " ";
	if (checkFirstWord(tokens[position]) != 4) {
		std::cout << "Where is the, THEN, keyword? " << std::endl;
		procState = 0;
		isErrorDetected = true;
		return position;
	}

	position++;
	// Prints opening brace
	std::cout << tokens[position] << " ";
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
		std::cout << tokens[position] << " ";
		position++;
		try {
			switch (isCharABrace(tokens[position])) {
			case 1: // "{"
				if (currentContainer.back().compare("if") == 0) {
					std::cout << tokens[position] << " ";
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
int SimpleParser::checkCall(unsigned int position) {
	return -1;
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

	//std::cout << tokens[position] << " ";

	// Check closing brace first
	if (isCharABrace(tokens[position]) == 1) {
		// Another opening brace
		// Invallid program
		isErrorDetected = true;
		std::cout << "Why is there an opening brace here? " << std::endl;
		return position;
	}
	else if (isCharABrace(tokens[position]) == 2) {
		// Closing brace 
		std::cout << tokens[position] << std::endl;
		try {
			std::string back = currentContainer.back();
			if (back.compare("while") == 0) {
				stackParenthesis.pop_back();
				currentContainer.pop_back();
				isStatementDetectedProcedure = true;
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
				isStatementDetectedProcedure = true;
				PKB::getInstance()->IfElseEnd();
			}
		}
		catch (std::exception& e) {
			std::cout << "Error parsing the closing brace! " << std::endl;
			isErrorDetected = true;
			return position;
		}
	}
	else {
		std::vector<ExpressionTokenType> types;

		// If an equal operator is found, = true
		bool isEquals = false;
		// If maths operator is found, = true
		// If the next token is another operator,
		// Invalid program.
		bool isOperator = false;
		// If variable/constant is found, = true
		// If the next token is another variable/constant,
		// Invalid program.
		bool isVariable = false;

		// Left side of assignment statement
		std::string leftVar;
		// Right side of assignment statement
		std::vector<std::string> rightVariables;
		// to check for subsequent variables, which is an invalid program
		bool isSubsequent = false;

		for (position;position < tokens.size();position++) {
			// Prints the token
			std::cout << tokens[position] << " ";
			switch (isCharAnOperator(tokens[position])) {
			case 0:
				// Current token is possibly a variable/constant or anything else thats not the operators
				isOperator = false;

				// Check if equals operator has parsed and it is not an operator
				if (isEquals == false && isOperator == false) {
					// First variable in statement
					if (isCharAnInteger(tokens[position])) {
						// Assignment statement cannot start with integer
						isErrorDetected = true;
						return position;
					} else { 
						if (isSubsequent == true) {
							isErrorDetected = true;
							return position;
						}
						else {
							isSubsequent = true;
							leftVar = tokens[position];
						}
					}
				}
				else if (isEquals == true && isOperator == false && isVariable == false) {
					// Subsequent variables after the equals operator and before an actual operator
					if (isCharAnInteger(tokens[position])) {
						rightVariables.push_back(tokens[position]);
						types.push_back(Constant);
						isVariable = true;
					} else {
						rightVariables.push_back(tokens[position]);
						types.push_back(Variable);
						isVariable = true;
					}
					
				} else {
					isErrorDetected = true;
					break;
				}
				break;
			case 1:
				// token is "=";
				// First "=" in statement
				if (isEquals == true) {
					isErrorDetected = true;
					break;
				}
				else {
					isEquals = true;
					isSubsequent = false;
				}
				break;
			case 2:
				// token is "+";
			case 3:
				// token is "*";
			case 4:
				// token is "-";
				if (isOperator == true) {
					isErrorDetected = true;
					break;
				}
				isOperator = true;
				isVariable = false;
				isSubsequent = false;
				rightVariables.push_back(tokens[position]);
				types.push_back(Operator);
				break;
			case 5:
				// token is ";"
				if (isEquals == true && isOperator == false) {
					isStatementDetectedProcedure = true;
					isSubsequent = false;
					std::string back = currentContainer.back();
					PKB::getInstance()->AssignStatement(leftVar, rightVariables, types);
				} else {
					isErrorDetected = true;
				}

				/* Debug Print outs
				for (unsigned int j = 0;j < rightVariables.size();j++) {
					std::cout << rightVariables[j] << " ";
				}
				std::cout << std::endl;
				*/

				rightVariables.clear();
				types.clear();
				leftVar = "";
				//std::cout << position;
				//std::cout << std::endl;
				return position;
			default:
				break;
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
	else {
		return 0;
	}
}