#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "SimpleParser.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
    TEST_CLASS(TestParserPKB)
    {
    public:
        PKB* pkb;
        ProcedureTable* pt;
        SimpleParser* sp;
        std::string varName;
        std::vector<std::string> tokens;
        std::vector<ExpressionTokenType> types;

        TEST_METHOD(TestParserPKB_Valid_Assignment) {
            std::string simple = "procedure p{x=b+c;}";
            std::vector<std::string> results;
            sp = new SimpleParser();
            pt = new ProcedureTable();

            results = sp->tokenize(simple);
            sp->parseSimple(results);

            
            int i = pt->getNumberOfProcedures();
            Assert::AreEqual(1, i);
        }

    };
}