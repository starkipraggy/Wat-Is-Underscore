#include "stdafx.h"
#include "CppUnitTest.h"
#include "SimpleParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


TEST_CLASS(TestAST)
{
public:
	SimpleParser* sp;

    TEST_METHOD(TestTokenisationSimple) {
        //Arrange
		std::string simple = "procedure a{a=b+c+d;}";
		std::vector<std::string> expected = { "procedure", "a","{","a","=","b","+","c","+","d",";" ,"{" };
		sp = new SimpleParser();
        //Act
		std::vector<std::string> tokens;
		tokens = sp->setTokens(simple);

        //Assert
        Assert::AreEqual(expected, tokens);
    }
};