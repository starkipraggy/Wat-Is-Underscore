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
        
        TEST_METHOD_INITIALIZE(InitialiseBasicTree) {
            
        }

        TEST_METHOD(CompareTrees) {
            //Arrange
            basicTree->setNodeType(OperatorPlus);
            basicTree->addChild(&TNode(VariableName, "a"));
            basicTree->addChild(&TNode(OperatorMinus, ""));
            basicTree->getChildNodes()[1]->addChild(&TNode(OperatorPlus, ""));
            basicTree->getChildNodes()[1]->addChild(&TNode(VariableName, "d"));
            basicTree->getChildNodes()[1]->getChildNodes()[0]->addChild(&TNode(VariableName, "b"));
            basicTree->getChildNodes()[1]->getChildNodes()[0]->addChild(&TNode(VariableName, "c"));

            //Act
            bool success = AST::compareTrees(basicTree, basicTree);

            //Assert
            Assert::IsTrue(success);
        }
		
		TEST_METHOD(ExpressionTreeTest1){
            //Arrange
            basicTree->setNodeType(OperatorPlus);
            basicTree->addChild(&TNode(VariableName, "a"));
            basicTree->addChild(&TNode(OperatorMinus, ""));
            basicTree->getChildNodes()[1]->addChild(&TNode(OperatorPlus, ""));
            basicTree->getChildNodes()[1]->addChild(&TNode(VariableName, "d"));
            basicTree->getChildNodes()[1]->getChildNodes()[0]->addChild(&TNode(VariableName, "b"));
            basicTree->getChildNodes()[1]->getChildNodes()[0]->addChild(&TNode(VariableName, "c"));

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
            basicTree->setNodeType(OperatorPlus);
            basicTree->addChild(&TNode(VariableName, "a"));
            basicTree->addChild(&TNode(OperatorMinus, ""));
            basicTree->getChildNodes()[1]->addChild(&TNode(OperatorPlus, ""));
            basicTree->getChildNodes()[1]->addChild(&TNode(VariableName, "d"));
            basicTree->getChildNodes()[1]->getChildNodes()[0]->addChild(&TNode(VariableName, "b"));
            basicTree->getChildNodes()[1]->getChildNodes()[0]->addChild(&TNode(VariableName, "c"));
            std::string expression = "a+(b+c)-d";

            //Act
            TNode* answer = AST::constructExpressionTree(expression);

            //Assert
            Assert::IsTrue(AST::compareTrees(basicTree, answer));
        }

	};
}