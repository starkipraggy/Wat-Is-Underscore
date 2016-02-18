#pragma once

//#include <streambuf>
//#include <algorithm>
//#include <vector>

#include "SimpleParser.h"
#include "PKB.h"
#include "TNode.h"

std::vector<std::string> namesProcedure;
std::vector<std::string> namesVariables;
std::vector<std::string> typesVariables;
std::vector<std::string> stackParenthesis;
static std::vector<std::string> tokens;

int state = 0;
bool isErrorDetected = false;
//pkb = new PKB();

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
		for (int i = 0; i < tokens.size(); ++i) {
			std::cout << tokens[i] << ' ';
		}
		std::cout << std::endl;
	}

}

std::string SimpleParser::addSpaceToString(std::string input) {
	std::vector<std::string> dictionary = { "{", "}", "=", "+", "-" , "*" ,";" };

	for (int i = 0;i < dictionary.size(); i++) {
		int pos = 0;
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

void SimpleParser::processLine() {
	// if procstate== 0; means there is invalid procedure.
	// if procstate == 1; procedure is valid.
	int procState = 0;

	std::cout << "Tokens Size = " << tokens.size();
	std::cout << std::endl;

	// check proc, procName, opening brace
	if (tokens.size() < 4) {
		// invalid program
		isErrorDetected = true;
	}
	else {
		for (int i = 0;i < tokens.size();i++) {
			switch (checkWord(tokens[i])) {
			case 1:
				// first word is procedure
				// eats tokens until first opening brace
				procState = 1;
				i++;
				std::cout << "ProcName = " << tokens[i];
				std::cout << std::endl;
				//PKB::ProcedureStart(tokens[i]);
				i++;
				std::cout << "OpeningBrace = " << tokens[i];
				std::cout << std::endl;
				switch (isCharABrace(tokens[i])) {
				case 1:
					stackParenthesis.push_back(tokens[i]);
					break;
				case 2:
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
				// PKB::WhileStart(tokens[i]);
				i++;
				std::cout << "OpeningBrace = " << tokens[i];
				std::cout << std::endl;
				
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
				std::cout << "Cse 4 = ";
				std::cout << std::endl;
				if (procState == 0) {
					isErrorDetected = true;
					break;
				}

				bool isEquals = false;
				bool isOperator = false;

				for (i;i < tokens.size();i++) {
					switch (isCharAnOperator(tokens[j])) {
					case 1:
						break;
					case 2:
						break;
					case 3:
						break;
					case 4:
						break;
					case 5:
						break;
					default:
						break;
					}

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
	if (cChar.compare("{")==0) {
		return 1;
	}
	else if (cChar.compare("}")) {
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
	else if (cChar.compare("+")) {
		return 2;
	}
	else if (cChar.compare("*")) {
		return 3;
	}
	else if (cChar.compare("-")) {
		return 4;
	}
	else if (cChar.compare(";")) {
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
