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
std::vector<std::string> tokens;

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

	// check proc, procName, openking brace
	if (tokens.size() < 4) {
		// invalid program
		isErrorDetected = true;
	} else {

		for (int i = 0;i < tokens.size();i++) {

		}
	}
	


}

void SimpleParser::checkProcedure(std::vector<std::string> line) {
	//procedure ABC { }
	// 0 == not checked / 1 == exist / 2 == does not exist
	int isProcedureExist = 0; 
	int isProcNameExist = 0;
	
	for (int i = 0;i < line.size(); i++) {

	}
	
}

int SimpleParser::isCharABrace(std::string cChar) {
	std::string frontBrace = "{";
	std::string backBrace = "}";
	if (cChar.compare(frontBrace)) {
		return 1;
	} else if (cChar.compare(backBrace)) {
		return 2;
	} else {
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
