#include "stdafx.h"
#include "CppUnitTest.h"
#include "SimpleParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


TEST_CLASS(TestAST)
{
public:
	SimpleParser* sp;

	TEST_METHOD(TestTokenisation_General) {
		//Arrange
		std::string simple = "ab}a{;+9g-n";
		std::vector<std::string> expected = { "ab", "}","a","{",";","+","9g","-","n" };
		std::vector<std::string> results;
		sp = new SimpleParser();
		//Act
		std::vector<std::string> tokens;
		results = sp->tokenize(simple);

		//Assert
		for (unsigned int i = 0; i < expected.size();i++) {
			Assert::AreEqual(expected[i], results[i]);
		}
	}

	// Tokenizes Assignments
	TEST_METHOD(TestTokenisation_Simple_1) {
		//Arrange
		std::string simple = "procedure a{a=b+c+d;}";
		std::vector<std::string> expected = { "procedure", "a","{","a","=","b","+","c","+","d",";" ,"}" };
		std::vector<std::string> results;
		sp = new SimpleParser();
		//Act
		std::vector<std::string> tokens;
		results = sp->tokenize(simple);

		//Assert
		for (unsigned int i = 0; i < expected.size();i++) {
			Assert::AreEqual(expected[i], results[i]);
		}
	}
	
	// Tokenizes While and Assignments
	TEST_METHOD(TestTokenisation_Simple_2) {
		//Arrange
		//std::string simple2 = "procedure b{ b=x+y; c=w+h+j;}";
		//std::vector<std::string> expected2 = { "procedure", "b","{","b","=","x","+","y",";","c","=","w","+", "h","j",";","}" };
		std::string simple = "procedure xs{while b{x+4+z;}";
		std::vector<std::string> expected = { "procedure", "xs","{","while","b","{","x","+","4","+","z",";" ,"}" };
		std::vector<std::string> results;
		sp = new SimpleParser();
		//Act
		std::vector<std::string> tokens2;
		results = sp->tokenize(simple);

		//Assert
		for (unsigned int i = 0; i < expected.size();i++) {
			Assert::AreEqual(expected[i], results[i]);
		}
	}

};