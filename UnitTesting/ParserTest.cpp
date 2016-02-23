#include "stdafx.h"
#include "CppUnitTest.h"
#include "SimpleParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


TEST_CLASS(TestAST)
{
public:
	SimpleParser* sp;
	/*
	// Tokenizes Assignments
	TEST_METHOD(TestTokenisationSimple) {
		//Arrange
		std::string simple = "procedure a{a=b+c+d;}";
		std::vector<std::string> expected = { "procedure", "a","{","a","=","b","+","c","+","d",";" ,"}" };
		sp = new SimpleParser();
		//Act
		std::vector<std::string> tokens;
		simple = sp->addSpaceToString(simple);
		tokens = sp->setTokens(simple);

		//Assert
		for (unsigned int i = 0; i < expected.size();i++) {
			Assert::AreEqual(expected[i], tokens[i]);
		}
	}
	
	// Tokenizes While and Assignments
	TEST_METHOD(TestTokenisationSimple2) {
		//Arrange
		//std::string simple2 = "procedure b{ b=x+y; c=w+h+j;}";
		//std::vector<std::string> expected2 = { "procedure", "b","{","b","=","x","+","y",";","c","=","w","+", "h","j",";","}" };
		std::string simple2 = "procedure a{a=x+y+z;}";
		std::vector<std::string> expected2 = { "procedure", "a","{","a","=","x","+","y","+","z",";" ,"}" };

		sp = new SimpleParser();
		//Act
		std::vector<std::string> tokens2;
		simple2 = sp->addSpaceToString(simple2);
		tokens2 = sp->setTokens(simple2);

		//Assert
		for (unsigned int i = 0; i < expected2.size();i++) {
			Assert::AreEqual(expected2[i], tokens2[i]);
		}
	}*/

};