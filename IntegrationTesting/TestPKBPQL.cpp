#include "stdafx.h"
#include "./Preprocessor/QueryEvaluator.h"
#include "./Preprocessor/PQLController.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
    TEST_CLASS(TestPKBPQL)
    {
    public:
        PKB* pkb;
        PQLController pqlc;

        std::vector<std::string> expectedResult;
        std::vector<std::string> actualResult;


        TEST_METHOD(TEST_PKBPQL_01) {
            PKB::setInstance(pkb);
           
            PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(5)->setFollows(PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(4));
            
            actualResult = pqlc.process("stmt s; Select s such that Follows(s, 5)");
            expectedResult.push_back("4");
            Assert::IsTrue(expectedResult == actualResult);
        }

    };
}
