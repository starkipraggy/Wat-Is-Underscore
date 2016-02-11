#pragma once

//#include <streambuf>
//#include <algorithm>
//#include <vector>

#include "SimpleParser.h"
#include "PKB.h"
#include "TNode.h"

SimpleParser::SimpleParser () {
	fio = new FileIO();
}

void SimpleParser::processSimple(std::string sourceFile) {
	/*Read whole text file and convert into string*/
	std::string contents = fio->FileIO::get_file_contents(sourceFile); 

	/*Interate the string line by line*/
	std::istringstream f(contents);
	std::string line;
	std::vector<std::string> tokens;
	std::string buf; // buffer string
	while (std::getline(f, line)) {
		/*Split the string by spaces*/
		std::stringstream ss(line);
		std::string n = "SIZE= "+ std::to_string(tokens.size()) +" \n";
		
		std::cout << n;
		tokens.clear();
		while (ss >> buf) {
		tokens.push_back(buf);
		}
		for (int i = 0; i < tokens.size(); ++i) {
			std::cout << tokens[i] << ' ';
		}
	}
	
}

void SimpleParser::processLine(std::string line) {

}



