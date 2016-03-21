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

        TEST_METHOD_INITIALIZE(InitialisePKB) {
            PKB::getInstance()->ProcedureStart("firstTestProc");

            varName = "a";
            tokens = { "a", "=", "1", ";" };
            types = { Variable, Operator, Constant };
            PKB::getInstance()->AssignStatement(varName, tokens, types);

            PKB::getInstance()->ProcedureEnd();
        }

        TEST_METHOD(TestPKB_ProcedureTableProcedure) {
           
            //Assert
        } 
        
        TEST_METHOD(TestPKB_ProcedureTable) {
            int actualNumProc = PKB::getInstance()->procedureTable.getNumberOfProcedures();
            int expectedNumProc = 1;
            
            std::string actualProcName = PKB::getInstance()->procedureTable.getProcedure(1);
            std::string expectedProcName = "firstTestProc";
            
            //Assert
            Assert::IsTrue(actualNumProc == expectedNumProc);
            Assert::IsTrue(actualProcName == expectedProcName);
        }
        
        TEST_METHOD(TestPKB_StatementTableStatement) {
           
            //Assert
        }
        
        TEST_METHOD(TestPKB_StatementTable) {
            int actualNumStatements = PKB::getInstance()->statementTable.getNumberOfStatements();
            int expectedNumStatements = 1;

            //Assert
            Assert::IsTrue(actualNumStatements == expectedNumStatements);
        }

        TEST_METHOD(TestPKB_VariableTableVariable) {
           
            //Assert
        }

        TEST_METHOD(TestPKB_VariableTable) {
            
            //Assert
        }
    };
}