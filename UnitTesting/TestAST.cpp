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

        TEST_METHOD(TestAST_CompareTrees) {
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
		
		TEST_METHOD(TestAST_ExpressionTreeTest1){
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

        TEST_METHOD(TestAST_ExpressionTreeTest2) {
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

        TEST_METHOD(ExpressionWithUnderscore) {
            //Arrange
            std::vector<std::string> tokens;
            tokens.push_back("_");
            tokens.push_back("a");
            tokens.push_back("+");
            tokens.push_back("(");
            tokens.push_back("b");
            tokens.push_back("+");
            tokens.push_back("c");
            tokens.push_back(")");
            tokens.push_back("-");
            tokens.push_back("d");
            tokens.push_back("_");

            //Act
            TNode* answer = AST::constructExpressionTree(tokens);

            //Assert
            Assert::IsTrue(AST::compareTrees(basicTree, answer));
        }

        TEST_METHOD(AddAssignTNode) {
            //Arrange
            AST* expected = new AST("TestProcedure");
            TNode* assign = new TNode(Assign);
            assign->addChild(new TNode(OperatorEquals));
            assign->getChildNodes()[0]->addChild(new TNode(VariableName, "y"));
            assign->getChildNodes()[0]->addChild(advancedTree);
            expected->makeChild(expected->getTree()[0], assign);

            std::string expression = "hewlett-(a+(b+c)*d)+euler";
            std::vector<std::string> tokens = SimpleParser::tokenize(expression);
            AST* result = new AST("TestProcedure");

            //Act
            result->addAssignTNode("y", tokens, 0);

            //Assert
            Assert::IsTrue(AST::compareTrees(assign, result->getLastAddedNode()));
            Assert::IsTrue(AST::compareTrees(expected->getTree()[0], result->getTree()[0]));
        }

		TEST_METHOD(AddCallTNode) {
			//Arrange
			AST* expected = new AST("TestProcedure");
			TNode* call = new TNode(Call);
			call->setValue("test");
			expected->makeChild(expected->getTree()[0], call);

			AST* result = new AST("TestProcedure");

			//Act
			result->addCallTNode("test", 0);

			//Assert
			Assert::IsTrue(AST::compareTrees(expected->getTree()[0], result->getTree()[0]));
		}

		TEST_METHOD(AddWhileTNode) {
			//Arrange
			AST* expected = new AST("TestProcedure");
			TNode* whileNode = new TNode(While);
			whileNode->addChild(new TNode(VariableName, "x"));
			whileNode->addChild(new TNode(StmtLst));
			expected->makeChild(expected->getTree()[0], whileNode);

			std::string expression = "while x {";
			std::vector<std::string> tokens = SimpleParser::tokenize(expression);
			AST* result = new AST("TestProcedure");

			//Act
			result->addWhileTNode("x", 0);

			//Assert
			Assert::IsTrue(AST::compareTrees(expected->getTree()[0], result->getTree()[0]));
		}

		TEST_METHOD(AddWhileTNodeWithAssign) {
			//Arrange
			AST* expected = new AST("TestProcedure");
			TNode* whileNode = new TNode(While);
			whileNode->addChild(new TNode(VariableName, "x"));
			whileNode->addChild(new TNode(StmtLst));
			TNode* assign = new TNode(Assign);
			assign->addChild(new TNode(OperatorEquals));
			assign->getChildNodes()[0]->addChild(new TNode(VariableName, "y"));
			assign->getChildNodes()[0]->addChild(advancedTree);
			whileNode->getChildNodes()[1]->addChild(assign);
			expected->makeChild(expected->getTree()[0], whileNode);

			std::string expression = "hewlett-(a+(b+c)*d)+euler";
			std::vector<std::string> tokens = SimpleParser::tokenize(expression);
			AST* result = new AST("TestProcedure");

			//Act
			result->addWhileTNode("x", 0);
			result->addAssignTNode("y", tokens, 0);

			//Assert
			Assert::IsTrue(AST::compareTrees(expected->getTree()[0], result->getTree()[0]));
		}

		TEST_METHOD(AddWhileTNodeWithTwoAssign) {
			//Arrange
			AST* expected = new AST("TestProcedure");
			TNode* whileNode = new TNode(While);
			whileNode->addChild(new TNode(VariableName, "x"));
			whileNode->addChild(new TNode(StmtLst));
			TNode* assign1 = new TNode(Assign);
			assign1->addChild(new TNode(OperatorEquals));
			assign1->getChildNodes()[0]->addChild(new TNode(VariableName, "y"));
			assign1->getChildNodes()[0]->addChild(advancedTree);
			whileNode->getChildNodes()[1]->addChild(assign1);
			TNode* assign2 = new TNode(Assign);
			assign2->addChild(new TNode(OperatorEquals));
			assign2->getChildNodes()[0]->addChild(new TNode(VariableName, "z"));
			assign2->getChildNodes()[0]->addChild(basicTree);
			whileNode->getChildNodes()[1]->addChild(assign2);
			expected->makeChild(expected->getTree()[0], whileNode);

			std::string expression1 = "hewlett-(a+(b+c)*d)+euler";
			std::vector<std::string> tokens1 = SimpleParser::tokenize(expression1); 
			std::string expression2 = "a + (b + c) - d";
			std::vector<std::string> tokens2 = SimpleParser::tokenize(expression2);
			AST* result = new AST("TestProcedure");

			//Act
			result->addWhileTNode("x", 0);
			result->addAssignTNode("y", tokens1, 0);
			result->addAssignTNode("z", tokens2, 0);

			//Assert
			Assert::IsTrue(AST::compareTrees(expected->getTree()[0], result->getTree()[0]));
		}

		TEST_METHOD(AddWhileTNodeWithAssignAndCallOutsideWhileLoop) {
			//Arrange
			AST* expected = new AST("TestProcedure");
			TNode* whileNode = new TNode(While);
			whileNode->addChild(new TNode(VariableName, "x"));
			whileNode->addChild(new TNode(StmtLst));
			TNode* assign = new TNode(Assign);
			assign->addChild(new TNode(OperatorEquals));
			assign->getChildNodes()[0]->addChild(new TNode(VariableName, "y"));
			assign->getChildNodes()[0]->addChild(advancedTree);
			whileNode->getChildNodes()[1]->addChild(assign);
			TNode* call = new TNode(Call);
			call->setValue("test");
			expected->makeChild(expected->getTree()[0], whileNode);
			expected->makeChild(expected->getTree()[0], call);

			std::string expression = "hewlett-(a+(b+c)*d)+euler";
			std::vector<std::string> tokens = SimpleParser::tokenize(expression);
			AST* result = new AST("TestProcedure");

			//Act
			result->addWhileTNode("x", 0);
			result->addAssignTNode("y", tokens, 0);

			//Assert
			Assert::IsTrue(AST::compareTrees(expected->getTree()[0], result->getTree()[0]));
		}


	};
}