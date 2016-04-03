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
            Assert::IsTrue(result->getLeftLmt() == 1);
            Assert::IsTrue(result->getRightLmt() == 1);
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
            Assert::IsTrue(result[0]->getLeftLmt() == 1);
            Assert::IsTrue(result[0]->getRightLmt() == 10);
        }

        TEST_METHOD(CFGIfStatement) {
            //Arrange

            //Act
            testCFG.addIfStmt();
            testCFG.addStmt();
            testCFG.elseStmt();
            testCFG.addStmt();
            testCFG.endIfStmt();
            testCFG.addStmt();
            std::vector<CFGNode*> result = testCFG.getGraph();

            //Assert
            Assert::IsTrue(result.size() == 4);
            Assert::IsTrue(result[0]->getLeftLmt() == 1);
            Assert::IsTrue(result[0]->getRightLmt() == 1);
            Assert::IsTrue(result[1]->getLeftLmt() == 2);
            Assert::IsTrue(result[1]->getRightLmt() == 2);
            Assert::IsTrue(result[2]->getLeftLmt() == 3);
            Assert::IsTrue(result[2]->getRightLmt() == 3);
            Assert::IsTrue(result[3]->getLeftLmt() == 4);
            Assert::IsTrue(result[3]->getRightLmt() == 4);
        }

        TEST_METHOD(CFGIfManyStmt) {
            //Arrange

            //Act
            testCFG.addIfStmt();
            testCFG.addStmt();
            testCFG.addStmt();
            testCFG.elseStmt();
            testCFG.addStmt();
            testCFG.addStmt();
            testCFG.addStmt();
            testCFG.endIfStmt();
            testCFG.addStmt();
            testCFG.addStmt();
            testCFG.addStmt();
            testCFG.addStmt();
            testCFG.addStmt();
            std::vector<CFGNode*> result = testCFG.getGraph();

            //Assert
            Assert::IsTrue(result.size() == 4);
            Assert::IsTrue(result[0]->getLeftLmt() == 1);
            Assert::IsTrue(result[0]->getRightLmt() == 1);
            Assert::IsTrue(result[1]->getLeftLmt() == 2);
            Assert::IsTrue(result[1]->getRightLmt() == 3);
            Assert::IsTrue(result[2]->getLeftLmt() == 4);
            Assert::IsTrue(result[2]->getRightLmt() == 6);
            Assert::IsTrue(result[3]->getLeftLmt() == 7);
            Assert::IsTrue(result[3]->getRightLmt() == 11);
        }

        TEST_METHOD(CFGWhileStmt) {
            //Arrange

            //Act
            testCFG.addWhileStmt();
            testCFG.addStmt();
            testCFG.endWhileStmt();
            testCFG.addStmt();
            std::vector<CFGNode*> result = testCFG.getGraph();

            //Assert
            Assert::IsTrue(result.size() == 3);
            Assert::IsTrue(result[0]->getLeftLmt() == 1);
            Assert::IsTrue(result[0]->getRightLmt() == 1);
            Assert::IsTrue(result[1]->getLeftLmt() == 2);
            Assert::IsTrue(result[1]->getRightLmt() == 2);
            Assert::IsTrue(result[2]->getLeftLmt() == 3);
            Assert::IsTrue(result[2]->getRightLmt() == 3);
        }

        TEST_METHOD(CFGWhileManyStmt) {
            //Arrange

            //Act
            testCFG.addWhileStmt();
            testCFG.addStmt();
            testCFG.addStmt();
            testCFG.endWhileStmt();
            testCFG.addStmt();
            testCFG.addStmt();
            testCFG.addStmt();
            std::vector<CFGNode*> result = testCFG.getGraph();

            //Assert
            Assert::IsTrue(result.size() == 3);
            Assert::IsTrue(result[0]->getLeftLmt() == 1);
            Assert::IsTrue(result[0]->getRightLmt() == 1);
            Assert::IsTrue(result[1]->getLeftLmt() == 2);
            Assert::IsTrue(result[1]->getRightLmt() == 3);
            Assert::IsTrue(result[2]->getLeftLmt() == 4);
            Assert::IsTrue(result[2]->getRightLmt() == 6);
        }

        TEST_METHOD(CFGNext) {
            //Arrange
            testCFG.addWhileStmt();
            testCFG.addStmt();
            testCFG.addStmt();
            testCFG.endWhileStmt();
            testCFG.addStmt();
            testCFG.addStmt();
            testCFG.addStmt();

            //Act
            std::vector<int> result = testCFG.nextStmt(2);

            //Assert
            Assert::IsTrue(result.size() == 1);
            Assert::IsTrue(result[0] == 3);
        }

        TEST_METHOD(CFGNextFromWhile) {
            //Arrange
            testCFG.addWhileStmt();
            testCFG.addStmt();
            testCFG.addStmt();
            testCFG.endWhileStmt();
            testCFG.addStmt();
            testCFG.addStmt();
            testCFG.addStmt();

            //Act
            std::vector<int> result = testCFG.nextStmt(1);

            //Assert
            Assert::IsTrue(result.size() == 2);
            Assert::IsTrue(result[0] == 2);
            Assert::IsTrue(result[1] == 4);
        }

        TEST_METHOD(CFGNextFromEndWhile) {
            //Arrange
            testCFG.addWhileStmt();
            testCFG.addStmt();
            testCFG.addStmt();
            testCFG.endWhileStmt();
            testCFG.addStmt();
            testCFG.addStmt();
            testCFG.addStmt();

            //Act
            std::vector<int> result = testCFG.nextStmt(3);

            //Assert
            Assert::IsTrue(result.size() == 1);
            Assert::IsTrue(result[0] == 1);
        }

        TEST_METHOD(CFGPrev) {
            //Arrange
            testCFG.addWhileStmt();
            testCFG.addStmt();
            testCFG.addStmt();
            testCFG.endWhileStmt();
            testCFG.addStmt();
            testCFG.addStmt();
            testCFG.addStmt();

            //Act
            std::vector<int> result = testCFG.prevStmt(3);

            //Assert
            Assert::IsTrue(result.size() == 1);
            Assert::IsTrue(result[0] == 2);
        }

        TEST_METHOD(CFGPrevOtherNode) {
            //Arrange
            testCFG.addWhileStmt();
            testCFG.addStmt();
            testCFG.addStmt();
            testCFG.endWhileStmt();
            testCFG.addStmt();
            testCFG.addStmt();
            testCFG.addStmt();

            //Act
            std::vector<int> result = testCFG.prevStmt(2);

            //Assert
            Assert::IsTrue(result.size() == 1);
            Assert::IsTrue(result[0] == 1);
        }

        TEST_METHOD(CFGNextStar) {
            //Arrange
            testCFG.addWhileStmt();
            testCFG.addStmt();
            testCFG.addStmt();
            testCFG.endWhileStmt();
            testCFG.addStmt();
            testCFG.addStmt();
            testCFG.addStmt();

            //Act
            std::vector<int> result = testCFG.nextStmtStar(1);

            //Assert
            Assert::IsTrue(result.size() == 5);
            std::sort(result.begin(), result.end());
            for (int i = 0; i < 5; i++) {
                Assert::IsTrue(result[i] == i+2);
            }
        }

        TEST_METHOD(CFGPrevStar){
            //Arrange
            testCFG.addWhileStmt();
            testCFG.addStmt();
            testCFG.addStmt();
            testCFG.endWhileStmt();
            testCFG.addStmt();
            testCFG.addStmt();
            testCFG.addStmt();

            //Act
            std::vector<int> result = testCFG.prevStmtStar(6);

            //Assert
            Assert::IsTrue(result.size() == 5);
            std::sort(result.begin(), result.end());
            for (int i = 0; i < 5; i++) {
                Assert::IsTrue(result[i] == i + 1);
            }
        }
    };
}