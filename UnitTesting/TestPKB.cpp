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

        TEST_METHOD(TestPKB_Valid_While) {
            PKB::getInstance()->ProcedureStart("validProcWhile");

            PKB::getInstance()->WhileStart("orange");
            varName = "coconut";
            tokens = { "coconut", "=", "(", "(", "1", "+", "2", ")", "*", "hotdog", ")" };
            types = { Variable, Operator, Operator, Operator, Constant, Operator, Constant, Operator, Operator, Variable, Operator, };
            PKB::getInstance()->AssignStatement(varName, tokens, types);
            Assert::IsTrue(PKB::getInstance()->WhileEnd());

            PKB::getInstance()->ProcedureEnd();
        }

        TEST_METHOD(TestPKB_Valid_If) {
            PKB::getInstance()->ProcedureStart("validProcIf");

            PKB::getInstance()->IfStart("banana");
            varName = "donut";
            tokens = { "donut", "=", "(", "(", "1", "+", "2", ")", "*", "durian", ")" };
            types = { Variable, Operator, Operator, Operator, Constant, Operator, Constant, Operator, Operator, Variable, Operator, };
            PKB::getInstance()->AssignStatement(varName, tokens, types);

            PKB::getInstance()->ElseStart();
            varName = "escargo";
            tokens = { "escargo", "=", "(", "(", "3", "+", "2", ")", "*", "chicken", ")" };
            types = { Variable, Operator, Operator, Operator, Constant, Operator, Constant, Operator, Operator, Variable, Operator, };
            PKB::getInstance()->AssignStatement(varName, tokens, types);

            Assert::IsTrue(PKB::getInstance()->IfElseEnd());

            PKB::getInstance()->ProcedureEnd();
        }

        TEST_METHOD(TestPKB_Valid_IfNestedInWhile) {
            PKB::getInstance()->ProcedureStart("validProcIfNestedInWhile");

            PKB::getInstance()->WhileStart("romeo");
            
            PKB::getInstance()->IfStart("juliet");
            varName = "kiss";
            tokens = { "kiss", "=", "(", "(", "1", "+", "2", ")", "*", "under", ")" };
            types = { Variable, Operator, Operator, Operator, Constant, Operator, Constant, Operator, Operator, Variable, Operator, };
            PKB::getInstance()->AssignStatement(varName, tokens, types);

            PKB::getInstance()->ElseStart();
            varName = "big";
            tokens = { "big", "=", "(", "(", "3", "+", "2", ")", "*", "tree", ")" };
            types = { Variable, Operator, Operator, Operator, Constant, Operator, Constant, Operator, Operator, Variable, Operator, };
            PKB::getInstance()->AssignStatement(varName, tokens, types);

            Assert::IsTrue(PKB::getInstance()->IfElseEnd()); 
            
            Assert::IsTrue(PKB::getInstance()->WhileEnd());

            PKB::getInstance()->ProcedureEnd();
        }

        TEST_METHOD(TestPKB_Valid_IfNestedInIf) {
            PKB::getInstance()->ProcedureStart("validProcIfNestedInIf");

            PKB::getInstance()->IfStart("OutErIf");
            
            PKB::getInstance()->IfStart("INNERIf");
            varName = "ReMal";
            tokens = { "ReMal", "=", "(", "(", "1", "+", "2", ")", "*", "LOLW", ")" };
            types = { Variable, Operator, Operator, Operator, Constant, Operator, Constant, Operator, Operator, Variable, Operator, };
            PKB::getInstance()->AssignStatement(varName, tokens, types);

            PKB::getInstance()->ElseStart();
            varName = "Ascension";
            tokens = { "Ascension", "=", "(", "(", "3", "+", "2", ")", "*", "heavenly", ")" };
            types = { Variable, Operator, Operator, Operator, Constant, Operator, Constant, Operator, Operator, Variable, Operator, };
            PKB::getInstance()->AssignStatement(varName, tokens, types);

            Assert::IsTrue(PKB::getInstance()->IfElseEnd());

            PKB::getInstance()->ElseStart();
            
            PKB::getInstance()->IfStart("SecondInnerIf");
            varName = "GoingMad";
            tokens = { "GoingMad", "=", "(", "(", "1", "+", "2", ")", "*", "nestnest", ")" };
            types = { Variable, Operator, Operator, Operator, Constant, Operator, Constant, Operator, Operator, Variable, Operator, };
            PKB::getInstance()->AssignStatement(varName, tokens, types);

            PKB::getInstance()->ElseStart();
            varName = "lastelse";
            tokens = { "lastelse", "=", "(", "(", "3", "+", "2", ")", "*", "finally", ")" };
            types = { Variable, Operator, Operator, Operator, Constant, Operator, Constant, Operator, Operator, Variable, Operator, };
            PKB::getInstance()->AssignStatement(varName, tokens, types);

            Assert::IsTrue(PKB::getInstance()->IfElseEnd());

            Assert::IsTrue(PKB::getInstance()->IfElseEnd());

            PKB::getInstance()->ProcedureEnd();
        }

        TEST_METHOD(TestPKB_Valid_WhileNestedInIf) {
            PKB::getInstance()->ProcedureStart("validProcWhileNestedInIf");

            PKB::getInstance()->IfStart("soHunGry");
            
            PKB::getInstance()->WhileStart("andTired");
            varName = "Sleep";
            tokens = { "Sleep", "=", "(", "(", "1", "+", "2", ")", "*", "Eatt", ")" };
            types = { Variable, Operator, Operator, Operator, Constant, Operator, Constant, Operator, Operator, Variable, Operator, };
            PKB::getInstance()->AssignStatement(varName, tokens, types);
            Assert::IsTrue(PKB::getInstance()->WhileEnd());

            PKB::getInstance()->ElseStart();
            
            PKB::getInstance()->WhileStart("cr4zy");
            varName = "m4dn355";
            tokens = { "m4dn355", "=", "(", "(", "3", "+", "2", ")", "*", "lolwut", ")" };
            types = { Variable, Operator, Operator, Operator, Constant, Operator, Constant, Operator, Operator, Variable, Operator, };
            PKB::getInstance()->AssignStatement(varName, tokens, types);
            Assert::IsTrue(PKB::getInstance()->WhileEnd());

            Assert::IsTrue(PKB::getInstance()->IfElseEnd());

            PKB::getInstance()->ProcedureEnd();
        }

        TEST_METHOD(TestPKB_Valid_WhileNestedInWhile) {
            PKB::getInstance()->ProcedureStart("validProcWhileNestedInWhile");
            
            PKB::getInstance()->WhileStart("outerwhile");
            varName = "reusingtemplate";
            tokens = { "reusingtemplate", "=", "(", "(", "3", "+", "2", ")", "*", "justbecauseimlazy", ")" };
            types = { Variable, Operator, Operator, Operator, Constant, Operator, Constant, Operator, Operator, Variable, Operator, };
            PKB::getInstance()->AssignStatement(varName, tokens, types);

            PKB::getInstance()->WhileStart("innerwhile");
            varName = "whysolazy";
            tokens = { "whysolazy", "=", "(", "(", "3", "+", "2", ")", "*", "tiredmax99", ")" };
            types = { Variable, Operator, Operator, Operator, Constant, Operator, Constant, Operator, Operator, Variable, Operator, };
            PKB::getInstance()->AssignStatement(varName, tokens, types);

            Assert::IsTrue(PKB::getInstance()->WhileEnd());
            
            Assert::IsTrue(PKB::getInstance()->WhileEnd());

            PKB::getInstance()->ProcedureEnd();
        }
    };
}