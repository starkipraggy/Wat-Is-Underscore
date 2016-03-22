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
            PKB::getInstance()->ProcedureStart("validProcAssign");

            varName = "apple";
            tokens = { "apple", "=", "(", "(", "1", "+", "2", ")", "*", "baNaNa", ")" };
            types = { Variable, Operator, Operator, Operator, Constant, Operator, Constant, Operator, Operator, Variable, Operator,  };
            Assert::IsTrue(PKB::getInstance()->AssignStatement(varName, tokens, types));

            PKB::getInstance()->ProcedureEnd();
        }

        TEST_METHOD(TestPKB_Invalid_Assignment) {
            PKB::getInstance()->ProcedureStart("invalidProcAssign");

            // Invalid Assignment, no Right-Hand Expression
            varName = "a";
            tokens = { "a", "=" };
            types = { Variable, Operator, Constant };
            Assert::IsFalse(PKB::getInstance()->AssignStatement(varName, tokens, types));
            
            PKB::getInstance()->ProcedureEnd();
        }

        TEST_METHOD(TestPKB_Valid_While) {
            PKB::getInstance()->ProcedureStart("validProcWhile");

            PKB::getInstance()->WhileStart("apple");
            varName = "apple";
            tokens = { "apple", "=", "(", "(", "1", "+", "2", ")", "*", "baNaNa", ")" };
            types = { Variable, Operator, Operator, Operator, Constant, Operator, Constant, Operator, Operator, Variable, Operator, };
            PKB::getInstance()->AssignStatement(varName, tokens, types);
            Assert::IsTrue(PKB::getInstance()->WhileEnd());

            PKB::getInstance()->ProcedureEnd();
        }

        // Missing WhileStart
        TEST_METHOD(TestPKB_Invalid_While) {
            PKB::getInstance()->ProcedureStart("invalidProcWhile");

            varName = "apple";
            tokens = { "apple", "=", "(", "(", "1", "+", "2", ")", "*", "baNaNa", ")" };
            types = { Variable, Operator, Operator, Operator, Constant, Operator, Constant, Operator, Operator, Variable, Operator, };
            PKB::getInstance()->AssignStatement(varName, tokens, types);
            Assert::IsFalse(PKB::getInstance()->WhileEnd());

            PKB::getInstance()->ProcedureEnd();
        }

        TEST_METHOD(TestPKB_Valid_ElseStart) {
            PKB::getInstance()->ProcedureStart("validProcElseStart");

            PKB::getInstance()->IfStart("apple");
            varName = "apple";
            tokens = { "apple", "=", "(", "(", "1", "+", "2", ")", "*", "baNaNa", ")" };
            types = { Variable, Operator, Operator, Operator, Constant, Operator, Constant, Operator, Operator, Variable, Operator, };
            PKB::getInstance()->AssignStatement(varName, tokens, types);
            Assert::IsTrue(PKB::getInstance()->ElseStart());

            PKB::getInstance()->ProcedureEnd();
        }
        
        // This test works fine alone
        // but assert fails when running with Valid_ElseStart
        TEST_METHOD(TestPKB_Invalid_ElseStart) {
            PKB::getInstance()->ProcedureStart("invalidProcElseStart");

            varName = "apple";
            tokens = { "apple", "=", "(", "(", "1", "+", "2", ")", "*", "baNaNa", ")" };
            types = { Variable, Operator, Operator, Operator, Constant, Operator, Constant, Operator, Operator, Variable, Operator, };
            PKB::getInstance()->AssignStatement(varName, tokens, types);
            Assert::IsFalse(PKB::getInstance()->ElseStart());

            PKB::getInstance()->ProcedureEnd();
        }

        TEST_METHOD(TestPKB_Valid_IfElse) {
            PKB::getInstance()->ProcedureStart("validProcIfElse");

            PKB::getInstance()->IfStart("apple");
            varName = "apple";
            tokens = { "apple", "=", "(", "(", "1", "+", "2", ")", "*", "baNaNa", ")" };
            types = { Variable, Operator, Operator, Operator, Constant, Operator, Constant, Operator, Operator, Variable, Operator, };
            PKB::getInstance()->AssignStatement(varName, tokens, types);

            PKB::getInstance()->ElseStart();
            varName = "apple";
            tokens = { "apple", "=", "(", "(", "3", "+", "2", ")", "*", "baNaNa", ")" };
            types = { Variable, Operator, Operator, Operator, Constant, Operator, Constant, Operator, Operator, Variable, Operator, };
            PKB::getInstance()->AssignStatement(varName, tokens, types);

            Assert::IsTrue(PKB::getInstance()->IfElseEnd());

            PKB::getInstance()->ProcedureEnd();
        }

        // Empty Else
        TEST_METHOD(TestPKB_Invalid_IfElse_01) {
            PKB::getInstance()->ProcedureStart("validProcIfElse");

            PKB::getInstance()->IfStart("apple");
            varName = "apple";
            tokens = { "apple", "=", "(", "(", "1", "+", "2", ")", "*", "baNaNa", ")" };
            types = { Variable, Operator, Operator, Operator, Constant, Operator, Constant, Operator, Operator, Variable, Operator, };
            PKB::getInstance()->AssignStatement(varName, tokens, types);
            PKB::getInstance()->ElseStart();
            Assert::IsFalse(PKB::getInstance()->IfElseEnd());

            PKB::getInstance()->ProcedureEnd();
        }

        // Missing Else Block
        TEST_METHOD(TestPKB_Invalid_IfElse_02) {
            PKB::getInstance()->ProcedureStart("invalidProcIfElse");

            PKB::getInstance()->IfStart("apple");
            varName = "apple";
            tokens = { "apple", "=", "(", "(", "1", "+", "2", ")", "*", "baNaNa", ")" };
            types = { Variable, Operator, Operator, Operator, Constant, Operator, Constant, Operator, Operator, Variable, Operator, };
            PKB::getInstance()->AssignStatement(varName, tokens, types);
            Assert::IsFalse(PKB::getInstance()->IfElseEnd());

            PKB::getInstance()->ProcedureEnd();
        }

        // This test works fine alone
        // but assert fails when running with Valid_ElseStart
        // Missing If Block
        TEST_METHOD(TestPKB_Invalid_IfElse_03) {
            PKB::getInstance()->ProcedureStart("invalidProcIfElseAgain");

            varName = "apple";
            tokens = { "apple", "=", "(", "(", "1", "+", "2", ")", "*", "baNaNa", ")" };
            types = { Variable, Operator, Operator, Operator, Constant, Operator, Constant, Operator, Operator, Variable, Operator, };
            PKB::getInstance()->AssignStatement(varName, tokens, types);
            PKB::getInstance()->ElseStart();
            Assert::IsFalse(PKB::getInstance()->IfElseEnd());

            PKB::getInstance()->ProcedureEnd();
        }
    };
}