#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
    TEST_CLASS(TestPKB)
    {
    public:
        PKB* pkb;
        std::string varName;
        std::vector<std::string> tokens;
        std::vector<ExpressionTokenType> types;

        TEST_METHOD(TestPKB_Valid_Assignment) {
            PKB::getInstance()->ProcedureStart("validProc");

            // Valid Assignment Statement
            varName = "a";
            tokens = { "a", "=", "1" };
            types = { Variable, Operator, Constant };
            Assert::IsTrue(PKB::getInstance()->AssignStatement(varName, tokens, types));

            PKB::getInstance()->ProcedureEnd();
        }

        TEST_METHOD(TestPKB_Invalid_Assignment) {
            PKB::getInstance()->ProcedureStart("validProc");

            // Invalid Assignment, no Right-Hand Expression
            varName = "a";
            tokens = { "a", "=" };
            types = { Variable, Operator, Constant };
            Assert::IsFalse(PKB::getInstance()->AssignStatement(varName, tokens, types));

            PKB::getInstance()->ProcedureEnd();
        }

    };
}