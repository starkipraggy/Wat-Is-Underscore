#include "stdafx.h"
#include "CppUnitTest.h"
#include "AST.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{		
	TEST_CLASS(TestAST)
	{
	public:
        TNode* basicTree = new TNode();
        TNode* advancedTree = new TNode();
        
        TEST_METHOD_INITIALIZE(InitialiseTrees) {
            basicTree->setNodeType(OperatorPlus);
            basicTree->addChild(new TNode(VariableName, "a"));
            basicTree->addChild(new TNode(OperatorMinus, ""));
            basicTree->getChildNodes()[1]->addChild(new TNode(OperatorPlus, ""));
            basicTree->getChildNodes()[1]->addChild(new TNode(VariableName, "d"));
            basicTree->getChildNodes()[1]->getChildNodes()[0]->addChild(new TNode(VariableName, "b"));
            basicTree->getChildNodes()[1]->getChildNodes()[0]->addChild(new TNode(VariableName, "c"));

            advancedTree->setNodeType(OperatorMinus);
            advancedTree->addChild(new TNode(VariableName, "hewlett"));
            advancedTree->addChild(new TNode(OperatorPlus, ""));
            advancedTree->getChildNodes()[1]->addChild(new TNode(OperatorPlus, ""));
            advancedTree->getChildNodes()[1]->addChild(new TNode(VariableName, "euler"));
            advancedTree->getChildNodes()[1]->getChildNodes()[0]->addChild(new TNode(VariableName, "a"));
            advancedTree->getChildNodes()[1]->getChildNodes()[0]->addChild(new TNode(OperatorTimes, ""));
            advancedTree->getChildNodes()[1]->getChildNodes()[0]->getChildNodes()[1]->addChild(new TNode(OperatorPlus, ""));
            advancedTree->getChildNodes()[1]->getChildNodes()[0]->getChildNodes()[1]->addChild(new TNode(VariableName, "d"));
            advancedTree->getChildNodes()[1]->getChildNodes()[0]->getChildNodes()[1]->getChildNodes()[0]->addChild(new TNode(VariableName, "b"));
            advancedTree->getChildNodes()[1]->getChildNodes()[0]->getChildNodes()[1]->getChildNodes()[0]->addChild(new TNode(VariableName, "c"));
        }

        TEST_METHOD(CompareTrees) {
            //Arrange

            //Act
            bool success = AST::compareTrees(basicTree, basicTree);

            //Assert
            Assert::IsTrue(success);
        }

        TEST_METHOD(CompareDifferentTrees) {
            //Arrange

            //Act
            bool success = AST::compareTrees(basicTree, advancedTree);

            //Assert
            Assert::IsFalse(success);
        }
		
		TEST_METHOD(ExpressionTreeTest1){
            //Arrange
            std::vector<std::string> tokens;
            tokens.push_back("a");
            tokens.push_back("+");
            tokens.push_back("(");
            tokens.push_back("b");
            tokens.push_back("+");
            tokens.push_back("c");
            tokens.push_back(")");
            tokens.push_back("-");
            tokens.push_back("d");

            //Act
            TNode* answer = AST::constructExpressionTree(tokens);

            //Assert
            Assert::IsTrue(AST::compareTrees(basicTree, answer));
		}

        TEST_METHOD(ExpressionTreeTest2) {
            //Arrange
            std::string expression = "a+(b+c)-d";

            //Act
            TNode* answer = AST::constructExpressionTree(expression);

            //Assert
            Assert::IsTrue(AST::compareTrees(basicTree, answer));
        }

        TEST_METHOD(ExpressionAdvancedTree) {
            //Arrange
            std::string expression = "hewlett-(a+(b+c)*d)+euler";

            //Act
            TNode* answer = AST::constructExpressionTree(expression);

            //Assert
            Assert::IsTrue(AST::compareTrees(advancedTree, answer));
        }
	};
}