#include "stdafx.h"
#include "CppUnitTest.h"
#include "AST.h"
#include "CFG.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
    TEST_CLASS(TestCFG) {
        CFG testCFG;

        TEST_METHOD_INITIALIZE(InitialiseCFG) {
            testCFG = CFG();
        }

        TEST_METHOD(CFGOneStatement) {
            //Arrange

            //Act
            testCFG.addStmt();
            CFGNode* result = testCFG.CFGNodeByStmtNum(1);

            //Assert
            Assert::IsTrue(result->leftLmt == 1);
            Assert::IsTrue(result->rightLmt == 1);
        }

        TEST_METHOD(CFG10Statement) {
            //Arrange

            //Act
            for (int i = 0; i < 10; i++) {
                testCFG.addStmt();
            }
            std::vector<CFGNode*> result = testCFG.getGraph();

            //Assert
            Assert::IsTrue(result.size() == 1);
            Assert::IsTrue(result[0]->leftLmt == 1);
            Assert::IsTrue(result[0]->rightLmt == 10);
        }

        TEST_METHOD(CFGIfStatement) {
            //Arrange

            //Act
            testCFG.addIfStmt();
            testCFG.addStmt();
            testCFG.elseStmt();
            testCFG.addStmt();
            testCFG.endIfStmt();

            //Assert
            
        }
    };
}