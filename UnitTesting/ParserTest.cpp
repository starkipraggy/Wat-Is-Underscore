#include "stdafx.h"
#include "CppUnitTest.h"
#include "SimpleParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


TEST_CLASS(TestAST)
{
public:

    TEST_METHOD(TestTokenisationSimple) {
        //Arrange
        std::string query = "stmt s; select s;";
        std::string expected;

        //Act
        std::string tokens;

        //Assert
        Assert::AreEqual(expected, tokens);
    }

    TEST_METHOD(TestTokenisation) {

    }
};