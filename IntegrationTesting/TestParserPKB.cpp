#include "stdafx.h"
#include "CppUnitTest.h"
#include "PKB.h"
#include "SimpleParser.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
    TEST_CLASS(TestParserPKB)
    {
    public:
        PKB* pkb;
        SimpleParser* sp;
        ProcedureTableProcedure* ptp;
        StatementTableStatement* sts;
        VariableTableVariable* vtv;
        std::string varName;
        std::vector<std::string> tokens;
        std::vector<ExpressionTokenType> types;

        // Basic uses/modifies for procedure/statements
        TEST_METHOD(testParserPKB_Valid_Variables_01) {
            PKB::setInstance(pkb);
            std::string simple = "procedure p{a=b;} procedure q{b=c;}";
            std::vector<std::string> results;
            sp = new SimpleParser();
            int test = 0;
            std::string testString = "a";

            results = sp->tokenize(simple);
            sp->parseSimple(results);

            test = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(0)->getIndex();
            Assert::AreEqual(0, test);
            test = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(1)->getIndex();
            Assert::AreEqual(1, test);
            test = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(2)->getIndex();
            Assert::AreEqual(2, test);

            testString = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(0)->getName();
            Assert::IsTrue("a" == testString);
            testString = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(1)->getName();
            Assert::IsTrue("b" == testString);
            testString = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(2)->getName();
            Assert::IsTrue("c" == testString);

            test = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(0)->getProceduresModifiesSize();
            Assert::AreEqual(1, test);
            test = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(1)->getProceduresModifiesSize();
            Assert::AreEqual(1, test);
            test = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(2)->getProceduresModifiesSize();
            Assert::AreEqual(0, test);

            test = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(0)->getProceduresModifies(0);
            Assert::AreEqual(0, test);
            test = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(1)->getProceduresModifies(0);
            Assert::AreEqual(1, test);

            test = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(0)->getProceduresUsesSize();
            Assert::AreEqual(0, test);
            test = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(1)->getProceduresUsesSize();
            Assert::AreEqual(1, test);
            test = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(2)->getProceduresUsesSize();
            Assert::AreEqual(1, test);

            test = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(1)->getProceduresUses(0);
            Assert::AreEqual(0, test);
            test = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(2)->getProceduresUses(0);
            Assert::AreEqual(1, test);

            test = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(0)->getStatementModifiesSize();
            Assert::AreEqual(1, test);
            test = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(1)->getStatementModifiesSize();
            Assert::AreEqual(1, test);
            test = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(2)->getStatementModifiesSize();
            Assert::AreEqual(0, test);

            test = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(0)->getStatementModifies(0);
            Assert::AreEqual(1, test);
            test = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(1)->getStatementModifies(0);
            Assert::AreEqual(2, test);

            test = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(0)->getStatementUsesSize();
            Assert::AreEqual(0, test);
            test = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(1)->getStatementUsesSize();
            Assert::AreEqual(1, test);
            test = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(2)->getStatementUsesSize();
            Assert::AreEqual(1, test);

            test = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(1)->getStatementUses(0);
            Assert::AreEqual(1, test);
            test = PKB::getInstance()->getVariableTable()->getVariableUsingVariableIndexNumber(2)->getStatementUses(0);
            Assert::AreEqual(2, test);
        }

        // Tests uses/modifies/parent/parent*/children/children*/followedby/follows
        TEST_METHOD(TestParserPKB_Valid_Statements_01) {
            PKB::setInstance(pkb);
            std::string simple = "procedure p{a=b;while b{b=c;}}";
            std::vector<std::string> results;
            sp = new SimpleParser();
            int test = 0;

            results = sp->tokenize(simple);
            sp->parseSimple(results);

            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(1)->getUsesSize();
            Assert::AreEqual(1, test);
            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(2)->getUsesSize();
            Assert::AreEqual(2, test);
            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(3)->getUsesSize();
            Assert::AreEqual(1, test);

            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(1)->getUses(0);
            Assert::AreEqual(1, test);
            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(2)->getUses(0);
            Assert::AreEqual(1, test);
            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(2)->getUses(1);
            Assert::AreEqual(2, test);
            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(3)->getUses(0);
            Assert::AreEqual(2, test);

            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(1)->getModifiesSize();
            Assert::AreEqual(1, test);
            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(2)->getModifiesSize();
            Assert::AreEqual(1, test);
            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(3)->getModifiesSize();
            Assert::AreEqual(1, test);

            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(1)->getModifies(0);
            Assert::AreEqual(0, test);
            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(2)->getModifies(0);
            Assert::AreEqual(1, test);
            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(3)->getModifies(0);
            Assert::AreEqual(1, test);

            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(1)->getChildrenSize();
            Assert::AreEqual(0, test);
            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(2)->getChildrenSize();
            Assert::AreEqual(1, test);
            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(3)->getChildrenSize();
            Assert::AreEqual(0, test);

            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(2)->getChildren(0);
            Assert::AreEqual(3, test);

            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(1)->getChildrenStarSize();
            Assert::AreEqual(0, test);
            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(2)->getChildrenStarSize();
            Assert::AreEqual(1, test);
            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(3)->getChildrenStarSize();
            Assert::AreEqual(0, test);

            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(2)->getChildrenStar(0);
            Assert::AreEqual(3, test);

            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(1)->getParentStarSize();
            Assert::AreEqual(0, test);
            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(2)->getParentStarSize();
            Assert::AreEqual(0, test);
            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(3)->getParentStarSize();
            Assert::AreEqual(1, test);

            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(3)->getParentStar(0);
            Assert::AreEqual(2, test);

            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(1)->getFollowsStarSize();
            Assert::AreEqual(0, test);
            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(2)->getFollowsStarSize();
            Assert::AreEqual(1, test);
            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(3)->getFollowsStarSize();
            Assert::AreEqual(0, test);

            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(2)->getFollowsStar(0);
            Assert::AreEqual(1, test);

            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(1)->getFollowedByStarSize();
            Assert::AreEqual(1, test);
            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(2)->getFollowedByStarSize();
            Assert::AreEqual(0, test);
            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(3)->getFollowedByStarSize();
            Assert::AreEqual(0, test);

            test = PKB::getInstance()->getStatementTable()->getStatementUsingStatementNumber(1)->getFollowedByStar(0);
            Assert::AreEqual(2, test);

        }

        // Tests modifies/uses/calls/calledby
        TEST_METHOD(TestParserPKB_Valid_Procedures_01) {
            PKB::setInstance(pkb);
            std::string simple = "procedure p{a=b; call q;} procedure q{b=c;}";
            std::vector<std::string> results;
            sp = new SimpleParser();
            int test = 0;
            std::string testString = "a";
            ptp = new ProcedureTableProcedure(testString, test);

            results = sp->tokenize(simple);
            sp->parseSimple(results);

            testString = PKB::getInstance()->getProcedureTable()->getProcedure(0)->getName();
            Assert::IsTrue("p" == testString);
            testString = PKB::getInstance()->getProcedureTable()->getProcedure(1)->getName();
            Assert::IsTrue("q" == testString);

            test = PKB::getInstance()->getProcedureTable()->getProcedure(0)->getIndex();
            Assert::AreEqual(0, test);
            test = PKB::getInstance()->getProcedureTable()->getProcedure(1)->getIndex();
            Assert::AreEqual(1, test);

            test = PKB::getInstance()->getProcedureTable()->getProcedure(0)->getModifiesSize();
            Assert::AreEqual(2, test);
            test = PKB::getInstance()->getProcedureTable()->getProcedure(1)->getModifiesSize();
            Assert::AreEqual(1, test);

            test = PKB::getInstance()->getProcedureTable()->getProcedure(0)->getModifies(0);
            Assert::AreEqual(0, test);
            test = PKB::getInstance()->getProcedureTable()->getProcedure(0)->getModifies(1);
            Assert::AreEqual(1, test);
            test = PKB::getInstance()->getProcedureTable()->getProcedure(1)->getModifies(0);
            Assert::AreEqual(1, test);

            test = PKB::getInstance()->getProcedureTable()->getProcedure(0)->getUsesSize();
            Assert::AreEqual(2, test);
            test = PKB::getInstance()->getProcedureTable()->getProcedure(1)->getUsesSize();
            Assert::AreEqual(1, test);

            test = PKB::getInstance()->getProcedureTable()->getProcedure(0)->getUses(0);
            Assert::AreEqual(1, test);
            test = PKB::getInstance()->getProcedureTable()->getProcedure(0)->getUses(1);
            Assert::AreEqual(2, test);
            test = PKB::getInstance()->getProcedureTable()->getProcedure(1)->getUses(0);
            Assert::AreEqual(2, test);

            test = PKB::getInstance()->getProcedureTable()->getProcedure(0)->getProcedureCallBySize();
            Assert::AreEqual(0, test);
            test = PKB::getInstance()->getProcedureTable()->getProcedure(1)->getProcedureCallBySize();
            Assert::AreEqual(1, test);

            ptp = PKB::getInstance()->getProcedureTable()->getProcedure(1)->getProcedureCallBy(0);
            Assert::IsTrue(PKB::getInstance()->getProcedureTable()->getProcedure(0) == ptp);

            test = PKB::getInstance()->getProcedureTable()->getProcedure(0)->getProcedureCallsSize();
            Assert::AreEqual(1, test);
            test = PKB::getInstance()->getProcedureTable()->getProcedure(1)->getProcedureCallsSize();
            Assert::AreEqual(0, test);

            ptp = PKB::getInstance()->getProcedureTable()->getProcedure(0)->getProcedureCalls(0);
            Assert::IsTrue(PKB::getInstance()->getProcedureTable()->getProcedure(1) == ptp);
        }

    };
}
