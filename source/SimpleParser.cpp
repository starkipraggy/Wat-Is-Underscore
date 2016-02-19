#pragma once

//#include <streambuf>
//#include <algorithm>
//#include <vector>

#include "SimpleParser.h"
#include "PKB.h"
#include "TNode.h"
#include "ProcedureTableProcedure.h"
#include "StatementTableStatement.h"
#include "VariableTableVariable.h"


std::vector<std::string> namesProcedure;
std::vector<std::string> namesVariables;
std::vector<std::string> typesVariables;
std::vector<std::string> stackParenthesis;
static std::vector<std::string> tokens;

// procedure / while / if /else
std::vector<std::string> currentContainer;


int state = 0;
bool isErrorDetected = false;

SimpleParser::SimpleParser() {
	fio = new FileIO();
}

void SimpleParser::processSimple(std::string sourceFile) {
	/*Read whole text file and convert into string*/
	std::string contents = fio->FileIO::get_file_contents(sourceFile);

	/*Interate the string line by line*/
	std::istringstream f(contents);
	std::string line;

	std::string buf; // buffer string
	tokens.clear();
	while (std::getline(f, line)) {

		/* Adds space inbetween the dictionary keys */
		std::string spacedLine = addSpaceToString(line);

		/* Splits string into tokens */
		std::stringstream ss(spacedLine);

		while (ss >> buf) {
			// Tokens are by per line basis
			tokens.push_back(buf);
		}
		for (unsigned int i = 0; i < tokens.size(); ++i) {
			std::cout << tokens[i] << ' ';
		}
		std::cout << std::endl;
	}

}

std::string SimpleParser::addSpaceToString(std::string input) {
	std::vector<std::string> dictionary = { "{", "}", "=", "+", "-" , "*" ,";" };

	for (unsigned int i = 0;i < dictionary.size(); i++) {
		unsigned int pos = 0;
		int counter = 0;
		do {
			pos = input.find(dictionary[i], pos + counter);
			if (pos != std::string::npos) {
				input.replace(pos, 1, " " + dictionary[i] + " ");
			}
			else {
				break;
			}
			counter++;
		} while (pos < input.size());
	}
	return input;
}

/* This method is for unit test */
std::vector<std::string> SimpleParser::setTokens(std::string line) {
	std::string buf;
	std::stringstream ss(line);

	while (ss >> buf) {
		// Tokens are by per line basis
		tokens.push_back(buf);
	}
	return tokens;
}

void SimpleParser::processLine() {
	// if procstate== 0; means there is invalid procedure.
	// if procstate == 1; procedure is valid.
	int procState = 0;
	

	std::cout << "Tokens Size = " << tokens.size();
	std::cout << std::endl;

	if (tokens.size() < 4) {
		// invalid program
		isErrorDetected = true;
	} else {

		for (unsigned int i = 0; i < tokens.size(); i++) {
			switch (checkWord(tokens[i])) {
			case 1:
				// first word is procedure
				// eats tokens until first opening brace
				procState = 1;
				i++;
				std::cout << "ProcName = " << tokens[i];
				std::cout << std::endl;

				PKB::getInstance()->ProcedureStart(tokens[i]);
				i++;
				std::cout << "OpeningBrace = " << tokens[i];
				std::cout << std::endl;
				switch (isCharABrace(tokens[i])) {
				case 1: // "{"
					stackParenthesis.push_back(tokens[i]);
					currentContainer.push_back("procedure");
					break;
				case 2: // "}"
					stackParenthesis.pop_back();
					break;
				default:
					isErrorDetected = true;
					break;
				}

				break;
			case 2:
				// first word is while
				// eats tokens until opening brace
				if (procState == 0) {
					isErrorDetected = true;
					break;
				}
				i++;
				std::cout << "while var = " << tokens[i];
				std::cout << std::endl;
				PKB::getInstance()->WhileStart(tokens[i]);
				i++;
				std::cout << "OpeningBrace = " << tokens[i];
				std::cout << std::endl;

				switch (isCharABrace(tokens[i])) {
				case 1: // "{"
					stackParenthesis.push_back(tokens[i]);
					currentContainer.push_back("while");
					break;
				default:
					isErrorDetected = true;
					break;
				}
				break;
			case 3:
				// first word is if
				// eats tokens until opening brace
				if (procState == 0) {
					isErrorDetected = true;
					break;
				}
				break;
			case 4:
				// assign statement
				// eats tokens until semi colon
				if (procState == 0) {
					isErrorDetected = true;
					break;
				}

				if (isCharABrace(tokens[i]) == 1) {

				}
				else if (isCharABrace(tokens[i]) == 2) {
					try
					{
						std::string back = currentContainer.back();
						if (back.compare("while") == 0) {
							stackParenthesis.pop_back();
							currentContainer.pop_back();
							PKB::getInstance()->WhileEnd();
							//std::cout << "LOLLOL " <<tokens[i];
							//std::cout << std::endl;
						} else if (back.compare("procedure") == 0) {
							stackParenthesis.pop_back();
							currentContainer.pop_back();
							PKB::getInstance()->ProcedureEnd();
							//std::cout << "LOLLOL " << tokens[i];
							//std::cout << std::endl;
						}
					}
					catch (std::exception& e)
					{
						std::cout << "Invalid Program " <<'\n';
						isErrorDetected = true;
					}
					
					
				}
				else {
					i = checkAssign(i);
				}
				

				break;
			default:
				std::cout << "Cse DEf = ";
				std::cout << std::endl;
				// Invalid Program
				break;
			}

			if (isErrorDetected == true) {
				break;
			}

		}
	}



}

int SimpleParser::checkAssign(unsigned int position) {
	/* 
	When entering this function, it will iterate the tokens from the passed in position,
	until reaching semi colon.
	It will check whether the assignment statement's validity.
	*/
	//std::cout << "Start Assignment ";
	//std::cout << std::endl;
	
	std::vector<ExpressionTokenType> types;

	// If an equal operator is found, = true
	bool isEquals = false;
	// If maths operator is found, = true
	// If the next token is another operator,
	// Invalid program.
	bool isOperator = false;
	
	// Left side of assignment statement
	std::string leftVar;
	// Right side of assignment statement
	std::vector<std::string> rightVariables;
	

	for (position;position < tokens.size();position++) {
		switch (isCharAnOperator(tokens[position])) {
		case 1:
			// token is "=";
			// First "=" in statement
			isEquals = true;
			break;
		case 2:
			// token is "+";
			if (isOperator == true) {
				isErrorDetected = true;
				break;
			}

			isOperator = true;
			rightVariables.push_back(tokens[position]);
			types.push_back(Operator);
			break;
		case 3:
			// token is "*";
			if (isOperator == true) {
				isErrorDetected = true;
				break;
			}
			isOperator = true;
			rightVariables.push_back(tokens[position]);
			types.push_back(Operator);
			break;
		case 4:
			// token is "-";
			if (isOperator == true) {
				isErrorDetected = true;
				break;
			}
			isOperator = true;
			rightVariables.push_back(tokens[position]);
			types.push_back(Operator);
			break;
		case 5:
			// token is ";"
			if (isEquals == true && isOperator == false) {
				PKB::getInstance()->AssignStatement(leftVar,rightVariables,types);
			} else{
				isErrorDetected = true;
			}

			for (unsigned int j = 0;j < rightVariables.size();j++) {
				std::cout<< rightVariables[j] << " ";
			}
			std::cout << std::endl;



			rightVariables.clear();
			types.clear();
			leftVar = "";
			//std::cout << position;
			//std::cout << std::endl;
			return position;
		case 0:
			isOperator = false;
			if (isEquals == false && isOperator == false) {
				// First variable in statement
				leftVar = tokens[position];
			}
			else if (isEquals == true && isOperator == false) {
				rightVariables.push_back(tokens[position]);
				types.push_back(Variable);
			}
			else
			{
				isErrorDetected = true;
				break;
			}
			break;
		default:
			break;
		}
		

	}
	
	return position;
}

int SimpleParser::checkWord(std::string word) {
	if (word.compare("procedure") == 0) {
		return 1;
	}
	else if (word.compare("while") == 0) {
		return 2;
	}
	else if (word.compare("if") == 0) {
		return 3;
	}
	else {
		return 4;
	}
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


/*
for (auto x : line) {
// Always checks the first variable to be procedure, if fails, then program is invalid
std::cout << "x = " +x;
std::cout << std::endl;
if (isProcedureExist < 1) {
std::string str = "procedure";
if (x.compare(str) == 0) {
// Program is valid
std::cout << "VALID";
std::cout << std::endl;
isProcedureExist = 1;
} else {
// Program is invalid
isProcedureExist = 2;
}

// Program is valid
} else if (isProcedureExist == 1) {
// Checks whether the 2nd variable is a name or brace
if (isCharABrace(x)==0&& isProcNameExist==false) {
namesProcedure.push_back(x);
isProcNameExist = true;
std::cout << "push_back";
std::cout << std::endl;
// If brace is detected, then invalid program
}
else if (isCharABrace(x) == 1 && isProcNameExist == true) {
stackParenthesis.push_back(x);
std::cout << "push_back { ";
std::cout << std::endl;
}
else if (isCharABrace(x) == 2 && isProcNameExist == true) {
stackParenthesis.pop_back();
std::cout << "pop_back } ";
std::cout << std::endl;
}
else if ((isCharABrace(x) == 1 || isCharABrace(x) == 2) && isProcNameExist == false) {
isErrorDetected = true;

break;
}

} else {
isErrorDetected = true;
break;
}
}
*/

//tokenize(input, "+")
//trim()
