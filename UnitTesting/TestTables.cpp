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


        TEST_METHOD(TestPKB_ProcTable) {
            ProcedureTable newProcedureTable;
            StatementTable newStatementTable;
            VariableTable newVariableTable;

            std::string testProcName = "testProc01";
            ProcedureTableProcedure::ProcedureTableProcedure(testProcName, 1);
            newProcedureTable.getProcedure(testProcName);

            StatementTableStatement::StatementTableStatement(1);
            newStatementTable.addStatement(1);
            newProcedureTable.getProcedure(testProcName)->addStatement(1);

            NAME a;
            VariableTableVariable::VariableTableVariable(a, 1);
            newVariableTable.getVariableUsingName(a);

            newProcedureTable.getProcedure(testProcName)->addStatement(1);
            Assert::IsTrue(newProcedureTable.getProcedure(testProcName)->addModifies(1));
            Assert::IsTrue(newProcedureTable.getProcedure(testProcName)->addUses(1));

            Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(1)->addModifies(1));
            Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(1)->addUses(1));

            Assert::IsTrue(newVariableTable.getVariableUsingName(a)->addProcedureModifies(1));
            Assert::IsTrue(newVariableTable.getVariableUsingName(a)->addProcedureUses(1));
            Assert::IsTrue(newVariableTable.getVariableUsingName(a)->addStatementModifies(1));
            Assert::IsTrue(newVariableTable.getVariableUsingName(a)->addStatementUses(1));

            Assert::AreEqual(1, newProcedureTable.getNumberOfProcedures());
            Assert::AreEqual(testProcName, newProcedureTable.getProcedure(testProcName)->getName());
            Assert::AreNotEqual(testProcName, newProcedureTable.getProcedure("testProcName2")->getName());
          //Assert::AreEqual(1, newProcedureTable.getProcedure(testProcName)->getIndex());
            Assert::AreEqual(0, newProcedureTable.getProcedure(testProcName)->getProcedureCallsSize());
            Assert::AreEqual(0, newProcedureTable.getProcedure(testProcName)->getStatementCallsSize());
            Assert::AreEqual(1, newProcedureTable.getProcedure(testProcName)->getUsesSize());
          //Assert::AreEqual(1, newProcedureTable.getProcedure(testProcName)->getUses(1));
            Assert::AreEqual(1, newProcedureTable.getProcedure(testProcName)->getModifiesSize());
          //Assert::AreEqual(1, newProcedureTable.getProcedure(testProcName)->getModifies(1));

        }

       

        TEST_METHOD(TestPKB_TableInitialize) {
           
        }
    };
}