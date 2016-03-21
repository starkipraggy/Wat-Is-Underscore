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

        TEST_METHOD(TestPKB_Assignment_01) { 
            PKB::getInstance()->ProcedureStart("firstTestProc");

            varName = "a";
            tokens = { "a", "=", "1", ";" };
            types = { Variable, Operator, Constant };
            Assert::IsTrue(PKB::getInstance()->AssignStatement(varName, tokens, types));

            PKB::getInstance()->ProcedureEnd();
        }

    };
}