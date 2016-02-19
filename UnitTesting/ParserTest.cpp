#include "stdafx.h"
#include "CppUnitTest.h"
#include "SimpleParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


TEST_CLASS(TestAST)
{
public:
	SimpleParser* sp;

	// Tokenizes Assignments
	TEST_METHOD(TestTokenisationSimple) {
		//Arrange
		std::string simple = "procedure a{a=b+c+d;}";
		std::vector<std::string> expected = { "procedure", "a","{","a","=","b","+","c","+","d",";" ,"{" };
		sp = new SimpleParser();
		//Act
		std::vector<std::string> tokens;
		simple = sp->addSpaceToString(simple);
		tokens = sp->setTokens(simple);

		//Assert
		Assert::AreEqual(expected, tokens);
	}
	
	// Tokenizes While and Assignments
	TEST_METHOD(TestTokenisationSimple2) {
		//Arrange
		std::string simple = "procedure a{while x{a=b+c;}}";
		std::vector<std::string> expected = { "procedure", "a","{","while","x","{","a","b","+","c",";" ,"{" };
		sp = new SimpleParser();
		//Act
		std::vector<std::string> tokens;
		simple = sp->addSpaceToString(simple);
		tokens = sp->setTokens(simple);

		//Assert
		Assert::AreEqual(expected, tokens);
	}

};