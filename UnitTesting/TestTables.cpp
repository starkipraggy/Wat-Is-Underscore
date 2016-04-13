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

        // Basic VarTable Functionality Test
        TEST_METHOD(TestPKB_VarTable01) {
            ProcedureTable newProcedureTable;
            StatementTable newStatementTable;
            VariableTable newVariableTable;

            std::string testProcName = "testProc01";
            ProcedureTableProcedure::ProcedureTableProcedure(testProcName, 1);
            newProcedureTable.getProcedure(testProcName);

            StatementTableStatement::StatementTableStatement(1);
            newStatementTable.addStatement(1);
            //newProcedureTable.getProcedure(testProcName)->addStatement(1); //@sorry

            NAME a = "a";
            VariableTableVariable::VariableTableVariable(a, 1);
            newVariableTable.getVariableUsingName(a);

            Assert::IsTrue(newProcedureTable.getProcedure(testProcName)->addModifies(1));
            Assert::IsTrue(newProcedureTable.getProcedure(testProcName)->addUses(1));
            Assert::IsFalse(newProcedureTable.getProcedure(testProcName)->addModifies(1));
            Assert::IsFalse(newProcedureTable.getProcedure(testProcName)->addUses(1));

            Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(1)->addModifies(1));
            Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(1)->addUses(1));
            Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(1)->addModifies(1));
            Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(1)->addUses(1));

            Assert::IsTrue(newVariableTable.getVariableUsingName(a)->addProcedureModifies(1));
            Assert::IsTrue(newVariableTable.getVariableUsingName(a)->addProcedureUses(1));
            Assert::IsTrue(newVariableTable.getVariableUsingName(a)->addStatementModifies(1));
            Assert::IsTrue(newVariableTable.getVariableUsingName(a)->addStatementUses(1));
            Assert::IsFalse(newVariableTable.getVariableUsingName(a)->addProcedureModifies(1));
            Assert::IsFalse(newVariableTable.getVariableUsingName(a)->addProcedureUses(1));
            Assert::IsFalse(newVariableTable.getVariableUsingName(a)->addStatementModifies(1));
            Assert::IsFalse(newVariableTable.getVariableUsingName(a)->addStatementUses(1));

            Assert::AreEqual(1, newVariableTable.getNumberOfVariables());
            Assert::AreEqual(1, newVariableTable.getVariableUsingName(a)->getProceduresUsesSize());
            Assert::AreEqual(1, newVariableTable.getVariableUsingName(a)->getProceduresUses(0));
            Assert::AreEqual(1, newVariableTable.getVariableUsingName(a)->getProceduresModifiesSize());
            Assert::AreEqual(1, newVariableTable.getVariableUsingName(a)->getProceduresModifies(0));
            Assert::AreEqual(1, newVariableTable.getVariableUsingName(a)->getStatementUsesSize());
            Assert::AreEqual(1, newVariableTable.getVariableUsingName(a)->getStatementUses(0));
            Assert::AreEqual(1, newVariableTable.getVariableUsingName(a)->getStatementModifiesSize());
            Assert::AreEqual(1, newVariableTable.getVariableUsingName(a)->getStatementModifies(0));
        }

        // Basic StmtTable Functionality Test
        TEST_METHOD(TestPKB_StmtTable01) {
            ProcedureTable newProcedureTable;
            StatementTable newStatementTable;
            VariableTable newVariableTable;

            std::string testProcName = "testProc01";
            ProcedureTableProcedure::ProcedureTableProcedure(testProcName, 1);
            newProcedureTable.getProcedure(testProcName);

            StatementTableStatement::StatementTableStatement(1);
            newStatementTable.addStatement(1);
            //newProcedureTable.getProcedure(testProcName)->addStatement(1); //@sorry

            NAME a = "a";
            VariableTableVariable::VariableTableVariable(a, 1);
            newVariableTable.getVariableUsingName(a);

            Assert::IsTrue(newProcedureTable.getProcedure(testProcName)->addModifies(1));
            Assert::IsTrue(newProcedureTable.getProcedure(testProcName)->addUses(1));
            Assert::IsFalse(newProcedureTable.getProcedure(testProcName)->addModifies(1));
            Assert::IsFalse(newProcedureTable.getProcedure(testProcName)->addUses(1));

            Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(1)->addModifies(1));
            Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(1)->addUses(1));
            Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(1)->addModifies(1));
            Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(1)->addUses(1));

            Assert::IsTrue(newVariableTable.getVariableUsingName(a)->addProcedureModifies(1));
            Assert::IsTrue(newVariableTable.getVariableUsingName(a)->addProcedureUses(1));
            Assert::IsTrue(newVariableTable.getVariableUsingName(a)->addStatementModifies(1));
            Assert::IsTrue(newVariableTable.getVariableUsingName(a)->addStatementUses(1));
            Assert::IsFalse(newVariableTable.getVariableUsingName(a)->addProcedureModifies(1));
            Assert::IsFalse(newVariableTable.getVariableUsingName(a)->addProcedureUses(1));
            Assert::IsFalse(newVariableTable.getVariableUsingName(a)->addStatementModifies(1));
            Assert::IsFalse(newVariableTable.getVariableUsingName(a)->addStatementUses(1));

            Assert::AreEqual(1, newStatementTable.getNumberOfStatements());
            Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(1)->getStatementNumber());
            Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(1)->hasFollows());
            Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(1)->hasFollowedBy());
            Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(1)->hasParent());
            Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(1)->getUses(0));
            Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(1)->getUsesSize());
            Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(1)->getModifies(0));
            Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(1)->getModifiesSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(1)->getFollowsStarSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(1)->getFollowedByStarSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(1)->getChildrenSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(1)->getParentStarSize());
        }

        // Follows/Parent Test for Stmt Tables
        TEST_METHOD(TestPKB_StmtTable02) {
            /*
            Program for this test

            procedure testProc1{
                while a{                        //1
                    call testProc2;}}           //2
            procedure testProc2{
                if b then {                     //3
                    c = d + e * f;              //4
                    a = 29;                     //5
                    g = 9 - a; } else{          //6
                        while a{                //7
                            a = a - 1;}}}       //8
            */

            // Creating Tables
            ProcedureTable newProcedureTable;
            StatementTable newStatementTable;
            VariableTable newVariableTable;

            // Creating Procedures
            std::string testProcName = "testProc1";
            std::string testProcName2 = "testProc2";
            ProcedureTableProcedure::ProcedureTableProcedure(testProcName, 1);
            ProcedureTableProcedure::ProcedureTableProcedure(testProcName2, 2);
            newProcedureTable.getProcedure(testProcName);
            newProcedureTable.getProcedure(testProcName2);
            Assert::AreEqual(2, newProcedureTable.getNumberOfProcedures());

            // Creating Statements
            StatementTableStatement::StatementTableStatement(1);
            StatementTableStatement::StatementTableStatement(2);
            StatementTableStatement::StatementTableStatement(3);
            StatementTableStatement::StatementTableStatement(4);
            StatementTableStatement::StatementTableStatement(5);
            StatementTableStatement::StatementTableStatement(6);
            StatementTableStatement::StatementTableStatement(7);
            StatementTableStatement::StatementTableStatement(8);
            newStatementTable.addStatement(1);
            newStatementTable.addStatement(2);
            newStatementTable.addStatement(3);
            newStatementTable.addStatement(4);
            newStatementTable.addStatement(5);
            newStatementTable.addStatement(6);
            newStatementTable.addStatement(7);
            newStatementTable.addStatement(8);
			//@sorry
			/*
            newProcedureTable.getProcedure(testProcName)->addStatement(1);
            newProcedureTable.getProcedure(testProcName)->addStatement(2);
            newProcedureTable.getProcedure(testProcName2)->addStatement(3);
            newProcedureTable.getProcedure(testProcName2)->addStatement(4);
            newProcedureTable.getProcedure(testProcName2)->addStatement(5);
            newProcedureTable.getProcedure(testProcName2)->addStatement(6);
            newProcedureTable.getProcedure(testProcName2)->addStatement(7);
            newProcedureTable.getProcedure(testProcName2)->addStatement(8);
			*/
            Assert::AreEqual(8, newStatementTable.getNumberOfStatements());
            Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(1)->getStatementNumber());
            Assert::AreEqual(2, newStatementTable.getStatementUsingStatementNumber(2)->getStatementNumber());
            Assert::AreEqual(3, newStatementTable.getStatementUsingStatementNumber(3)->getStatementNumber());
            Assert::AreEqual(4, newStatementTable.getStatementUsingStatementNumber(4)->getStatementNumber());
            Assert::AreEqual(5, newStatementTable.getStatementUsingStatementNumber(5)->getStatementNumber());
            Assert::AreEqual(6, newStatementTable.getStatementUsingStatementNumber(6)->getStatementNumber());
            Assert::AreEqual(7, newStatementTable.getStatementUsingStatementNumber(7)->getStatementNumber());
            Assert::AreEqual(8, newStatementTable.getStatementUsingStatementNumber(8)->getStatementNumber());

            // Setting Stmt Types and Checking
            TNodeType AssignStmt = Assign;
            TNodeType CallStmt = Call;
            TNodeType IfStmt = If;
            TNodeType WhileStmt = While;
            newStatementTable.getStatementUsingStatementNumber(1)->setType(WhileStmt);
            newStatementTable.getStatementUsingStatementNumber(2)->setType(CallStmt);
            newStatementTable.getStatementUsingStatementNumber(3)->setType(IfStmt);
            newStatementTable.getStatementUsingStatementNumber(4)->setType(AssignStmt);
            newStatementTable.getStatementUsingStatementNumber(5)->setType(AssignStmt);
            newStatementTable.getStatementUsingStatementNumber(6)->setType(AssignStmt);
            newStatementTable.getStatementUsingStatementNumber(7)->setType(WhileStmt);
            newStatementTable.getStatementUsingStatementNumber(8)->setType(AssignStmt);
            Assert::IsTrue(WhileStmt == newStatementTable.getStatementUsingStatementNumber(1)->getType());
            Assert::IsTrue(CallStmt == newStatementTable.getStatementUsingStatementNumber(2)->getType());
            Assert::IsTrue(IfStmt == newStatementTable.getStatementUsingStatementNumber(3)->getType());
            Assert::IsTrue(AssignStmt == newStatementTable.getStatementUsingStatementNumber(4)->getType());
            Assert::IsTrue(AssignStmt == newStatementTable.getStatementUsingStatementNumber(5)->getType());
            Assert::IsTrue(AssignStmt == newStatementTable.getStatementUsingStatementNumber(6)->getType());
            Assert::IsTrue(WhileStmt == newStatementTable.getStatementUsingStatementNumber(7)->getType());
            Assert::IsTrue(AssignStmt == newStatementTable.getStatementUsingStatementNumber(8)->getType());

            // Setting other Stmt portions
            NAME controlVariableForWhile = "i";
            NAME controlVariableForIf = "j";
            std::string stmtFourExpression = "d+e*f";
            std::string stmtFiveExpression = "29";
            std::string stmtSixExpression = "9-a";
            std::string stmtEightExpression = "a-1";
            newStatementTable.getStatementUsingStatementNumber(1)->setControlVariable(controlVariableForWhile);
            newStatementTable.getStatementUsingStatementNumber(3)->setControlVariable(controlVariableForIf);
            newStatementTable.getStatementUsingStatementNumber(4)->setRightHandSideExpression(stmtFourExpression);
            newStatementTable.getStatementUsingStatementNumber(5)->setRightHandSideExpression(stmtFiveExpression);
            newStatementTable.getStatementUsingStatementNumber(6)->setRightHandSideExpression(stmtSixExpression);
            newStatementTable.getStatementUsingStatementNumber(8)->setRightHandSideExpression(stmtEightExpression);
            Assert::IsTrue(controlVariableForWhile == newStatementTable.getStatementUsingStatementNumber(1)->getControlVariable());
            Assert::IsTrue(controlVariableForIf == newStatementTable.getStatementUsingStatementNumber(3)->getControlVariable());
            Assert::IsTrue(stmtFourExpression == newStatementTable.getStatementUsingStatementNumber(4)->getRightHandSideExpression());
            Assert::IsTrue(stmtFiveExpression == newStatementTable.getStatementUsingStatementNumber(5)->getRightHandSideExpression());
            Assert::IsTrue(stmtSixExpression == newStatementTable.getStatementUsingStatementNumber(6)->getRightHandSideExpression());
            Assert::IsTrue(stmtEightExpression == newStatementTable.getStatementUsingStatementNumber(8)->getRightHandSideExpression());

            // Checking Parent/Child relations
            newStatementTable.getStatementUsingStatementNumber(1)->addChild(newStatementTable.getStatementUsingStatementNumber(2));
            newStatementTable.getStatementUsingStatementNumber(2)->setParent(newStatementTable.getStatementUsingStatementNumber(1));
            newStatementTable.getStatementUsingStatementNumber(3)->addChild(newStatementTable.getStatementUsingStatementNumber(4));
            newStatementTable.getStatementUsingStatementNumber(3)->addChild(newStatementTable.getStatementUsingStatementNumber(5));
            newStatementTable.getStatementUsingStatementNumber(3)->addChild(newStatementTable.getStatementUsingStatementNumber(6));
            newStatementTable.getStatementUsingStatementNumber(3)->addChild(newStatementTable.getStatementUsingStatementNumber(7));
            newStatementTable.getStatementUsingStatementNumber(7)->addChild(newStatementTable.getStatementUsingStatementNumber(8));
            newStatementTable.getStatementUsingStatementNumber(4)->setParent(newStatementTable.getStatementUsingStatementNumber(3));
            newStatementTable.getStatementUsingStatementNumber(5)->setParent(newStatementTable.getStatementUsingStatementNumber(3));
            newStatementTable.getStatementUsingStatementNumber(6)->setParent(newStatementTable.getStatementUsingStatementNumber(3));
            newStatementTable.getStatementUsingStatementNumber(7)->setParent(newStatementTable.getStatementUsingStatementNumber(3));
            newStatementTable.getStatementUsingStatementNumber(8)->setParent(newStatementTable.getStatementUsingStatementNumber(7));
            Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(1)->hasParent());
            Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(2)->hasParent());
            Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(3)->hasParent());
            Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(4)->hasParent());
            Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(5)->hasParent());
            Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(6)->hasParent());
            Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(7)->hasParent());
            Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(8)->hasParent());
            Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(2)->getParent());
            Assert::AreEqual(3, newStatementTable.getStatementUsingStatementNumber(4)->getParent());
            Assert::AreEqual(3, newStatementTable.getStatementUsingStatementNumber(5)->getParent());
            Assert::AreEqual(3, newStatementTable.getStatementUsingStatementNumber(6)->getParent());
            Assert::AreEqual(3, newStatementTable.getStatementUsingStatementNumber(7)->getParent());
            Assert::AreEqual(7, newStatementTable.getStatementUsingStatementNumber(8)->getParent());
            Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(1)->getChildrenSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(2)->getChildrenSize());
            Assert::AreEqual(4, newStatementTable.getStatementUsingStatementNumber(3)->getChildrenSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(4)->getChildrenSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(5)->getChildrenSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(6)->getChildrenSize());
            Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(7)->getChildrenSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(8)->getChildrenSize());
            Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(1)->getChildrenStarSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(2)->getChildrenStarSize());
            Assert::AreEqual(5, newStatementTable.getStatementUsingStatementNumber(3)->getChildrenStarSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(4)->getChildrenStarSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(5)->getChildrenStarSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(6)->getChildrenStarSize());
            Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(7)->getChildrenStarSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(8)->getChildrenStarSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(1)->getParentStarSize());
            Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(2)->getParentStarSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(3)->getParentStarSize());
            Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(4)->getParentStarSize());
            Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(5)->getParentStarSize());
            Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(6)->getParentStarSize());
            Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(7)->getParentStarSize());
            Assert::AreEqual(2, newStatementTable.getStatementUsingStatementNumber(8)->getParentStarSize()); 
            Assert::AreEqual(2, newStatementTable.getStatementUsingStatementNumber(1)->getChildren(0));
            Assert::AreEqual(4, newStatementTable.getStatementUsingStatementNumber(3)->getChildren(0));
            Assert::AreEqual(5, newStatementTable.getStatementUsingStatementNumber(3)->getChildren(1));
            Assert::AreEqual(6, newStatementTable.getStatementUsingStatementNumber(3)->getChildren(2));
            Assert::AreEqual(7, newStatementTable.getStatementUsingStatementNumber(3)->getChildren(3));
            Assert::AreEqual(8, newStatementTable.getStatementUsingStatementNumber(7)->getChildren(0));
            Assert::AreEqual(2, newStatementTable.getStatementUsingStatementNumber(1)->getChildrenStar(0));
            Assert::AreEqual(4, newStatementTable.getStatementUsingStatementNumber(3)->getChildrenStar(0));
            Assert::AreEqual(5, newStatementTable.getStatementUsingStatementNumber(3)->getChildrenStar(1));
            Assert::AreEqual(6, newStatementTable.getStatementUsingStatementNumber(3)->getChildrenStar(2));
            Assert::AreEqual(7, newStatementTable.getStatementUsingStatementNumber(3)->getChildrenStar(3));
            Assert::AreEqual(8, newStatementTable.getStatementUsingStatementNumber(3)->getChildrenStar(4));
            Assert::AreEqual(8, newStatementTable.getStatementUsingStatementNumber(7)->getChildrenStar(0));
            Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(2)->getParentStar(0));
            Assert::AreEqual(3, newStatementTable.getStatementUsingStatementNumber(4)->getParentStar(0));
            Assert::AreEqual(3, newStatementTable.getStatementUsingStatementNumber(5)->getParentStar(0));
            Assert::AreEqual(3, newStatementTable.getStatementUsingStatementNumber(6)->getParentStar(0));
            Assert::AreEqual(3, newStatementTable.getStatementUsingStatementNumber(7)->getParentStar(0));
            Assert::AreEqual(7, newStatementTable.getStatementUsingStatementNumber(8)->getParentStar(0));
            Assert::AreEqual(3, newStatementTable.getStatementUsingStatementNumber(8)->getParentStar(1));

            // Checking Follows/FollowedBy relations
            newStatementTable.getStatementUsingStatementNumber(4)->setFollowedBy(newStatementTable.getStatementUsingStatementNumber(5));
            newStatementTable.getStatementUsingStatementNumber(5)->setFollowedBy(newStatementTable.getStatementUsingStatementNumber(6));
            newStatementTable.getStatementUsingStatementNumber(5)->setFollows(newStatementTable.getStatementUsingStatementNumber(4));
            newStatementTable.getStatementUsingStatementNumber(6)->setFollows(newStatementTable.getStatementUsingStatementNumber(5));
            Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(1)->hasFollows());
            Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(2)->hasFollows());
            Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(3)->hasFollows());
            Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(4)->hasFollows());
            Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(5)->hasFollows());
            Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(6)->hasFollows());
            Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(7)->hasFollows());
            Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(8)->hasFollows());
            Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(1)->hasFollowedBy());
            Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(2)->hasFollowedBy());
            Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(3)->hasFollowedBy());
            Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(4)->hasFollowedBy());
            Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(5)->hasFollowedBy());
            Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(6)->hasFollowedBy());
            Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(7)->hasFollowedBy());
            Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(8)->hasFollowedBy());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(1)->getFollowsStarSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(2)->getFollowsStarSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(3)->getFollowsStarSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(4)->getFollowsStarSize());
            Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(5)->getFollowsStarSize());
            Assert::AreEqual(2, newStatementTable.getStatementUsingStatementNumber(6)->getFollowsStarSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(7)->getFollowsStarSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(8)->getFollowsStarSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(1)->getFollowedByStarSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(2)->getFollowedByStarSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(3)->getFollowedByStarSize());
            Assert::AreEqual(2, newStatementTable.getStatementUsingStatementNumber(4)->getFollowedByStarSize());
            Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(5)->getFollowedByStarSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(6)->getFollowedByStarSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(7)->getFollowedByStarSize());
            Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(8)->getFollowedByStarSize());
            Assert::AreEqual(4, newStatementTable.getStatementUsingStatementNumber(5)->getFollows());
            Assert::AreEqual(5, newStatementTable.getStatementUsingStatementNumber(6)->getFollows());
            Assert::AreEqual(5, newStatementTable.getStatementUsingStatementNumber(4)->getFollowedBy());
            Assert::AreEqual(6, newStatementTable.getStatementUsingStatementNumber(5)->getFollowedBy());
            Assert::AreEqual(4, newStatementTable.getStatementUsingStatementNumber(5)->getFollowsStar(0));
            Assert::AreEqual(5, newStatementTable.getStatementUsingStatementNumber(6)->getFollowsStar(0));
            Assert::AreEqual(4, newStatementTable.getStatementUsingStatementNumber(6)->getFollowsStar(1));
            Assert::AreEqual(5, newStatementTable.getStatementUsingStatementNumber(4)->getFollowedByStar(0));
            Assert::AreEqual(6, newStatementTable.getStatementUsingStatementNumber(4)->getFollowedByStar(1));
            Assert::AreEqual(6, newStatementTable.getStatementUsingStatementNumber(5)->getFollowedByStar(0));
        }

        // Basic ProcTable Functionality Test
        TEST_METHOD(TestPKB_ProcTable01) {
            ProcedureTable newProcedureTable;
            StatementTable newStatementTable;
            VariableTable newVariableTable;

            std::string testProcName = "testProc01";
            ProcedureTableProcedure::ProcedureTableProcedure(testProcName, 1);
            newProcedureTable.getProcedure(testProcName);

            StatementTableStatement::StatementTableStatement(1);
            newStatementTable.addStatement(1);
            //newProcedureTable.getProcedure(testProcName)->addStatement(1); // @sorry

            NAME a = "a";
            VariableTableVariable::VariableTableVariable(a, 1);
            newVariableTable.getVariableUsingName(a);

            Assert::IsTrue(newProcedureTable.getProcedure(testProcName)->addModifies(1));
            Assert::IsTrue(newProcedureTable.getProcedure(testProcName)->addUses(1));
            Assert::IsFalse(newProcedureTable.getProcedure(testProcName)->addModifies(1));
            Assert::IsFalse(newProcedureTable.getProcedure(testProcName)->addUses(1));

            Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(1)->addModifies(1));
            Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(1)->addUses(1));
            Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(1)->addModifies(1));
            Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(1)->addUses(1));

            Assert::IsTrue(newVariableTable.getVariableUsingName(a)->addProcedureModifies(1));
            Assert::IsTrue(newVariableTable.getVariableUsingName(a)->addProcedureUses(1));
            Assert::IsTrue(newVariableTable.getVariableUsingName(a)->addStatementModifies(1));
            Assert::IsTrue(newVariableTable.getVariableUsingName(a)->addStatementUses(1));
            Assert::IsFalse(newVariableTable.getVariableUsingName(a)->addProcedureModifies(1));
            Assert::IsFalse(newVariableTable.getVariableUsingName(a)->addProcedureUses(1));
            Assert::IsFalse(newVariableTable.getVariableUsingName(a)->addStatementModifies(1));
            Assert::IsFalse(newVariableTable.getVariableUsingName(a)->addStatementUses(1));

            Assert::AreEqual(1, newProcedureTable.getNumberOfProcedures());
            Assert::AreEqual(testProcName, newProcedureTable.getProcedure(testProcName)->getName());
            Assert::AreEqual(0, newProcedureTable.getProcedure(testProcName)->getIndex());
            Assert::AreEqual(0, newProcedureTable.getProcedure(testProcName)->getProcedureCallBySize());
            Assert::AreEqual(0, newProcedureTable.getProcedure(testProcName)->getStatementCallBySize());
            Assert::AreEqual(1, newProcedureTable.getProcedure(testProcName)->getUsesSize());
            Assert::AreEqual(1, newProcedureTable.getProcedure(testProcName)->getUses(0));
            Assert::AreEqual(1, newProcedureTable.getProcedure(testProcName)->getModifiesSize());
            Assert::AreEqual(1, newProcedureTable.getProcedure(testProcName)->getModifies(0));
        }

        // Modifies/Uses Test for Var/Stmt/Proc Tables
		TEST_METHOD(TestPKB_Tables01) {
			/*
			Program for this test

			procedure testProc1{
				while a{                        //1
					call testProc2;}}           //2
			procedure testProc2{
				if b then {                     //3
					c = d + e * f;              //4
					a = 29; } else{             //5
					g = 9 - a;}}                //6
			*/

			// Creating Tables
			ProcedureTable newProcedureTable;
			StatementTable newStatementTable;
			VariableTable newVariableTable;

			// Creating Procedures
			std::string testProcName = "testProc1";
			std::string testProcName2 = "testProc2";
			ProcedureTableProcedure::ProcedureTableProcedure(testProcName, 1);
			ProcedureTableProcedure::ProcedureTableProcedure(testProcName2, 2);
			newProcedureTable.getProcedure(testProcName);
			newProcedureTable.getProcedure(testProcName2);
			Assert::AreEqual(2, newProcedureTable.getNumberOfProcedures());

			// Creating Statements
			StatementTableStatement::StatementTableStatement(1);
			StatementTableStatement::StatementTableStatement(2);
			StatementTableStatement::StatementTableStatement(3);
			StatementTableStatement::StatementTableStatement(4);
			StatementTableStatement::StatementTableStatement(5);
			StatementTableStatement::StatementTableStatement(6);
			newStatementTable.addStatement(1);
			newStatementTable.addStatement(2);
			newStatementTable.addStatement(3);
			newStatementTable.addStatement(4);
			newStatementTable.addStatement(5);
			newStatementTable.addStatement(6);
			// @sorry
			/*
			newProcedureTable.getProcedure(testProcName)->addStatement(1);
			newProcedureTable.getProcedure(testProcName)->addStatement(2);
			newProcedureTable.getProcedure(testProcName2)->addStatement(3);
			newProcedureTable.getProcedure(testProcName2)->addStatement(4);
			newProcedureTable.getProcedure(testProcName2)->addStatement(5);
			newProcedureTable.getProcedure(testProcName2)->addStatement(6);
			*/
			Assert::AreEqual(6, newStatementTable.getNumberOfStatements());

			// Creating Variables
			NAME a = "a";
			NAME b = "b";
			NAME c = "c";
			NAME d = "d";
			NAME e = "e";
			NAME f = "f";
			NAME g = "g";
			VariableTableVariable::VariableTableVariable(a, 1);
			VariableTableVariable::VariableTableVariable(b, 2);
			VariableTableVariable::VariableTableVariable(c, 3);
			VariableTableVariable::VariableTableVariable(d, 4);
			VariableTableVariable::VariableTableVariable(e, 5);
			VariableTableVariable::VariableTableVariable(f, 6);
			VariableTableVariable::VariableTableVariable(g, 7);
			newVariableTable.getVariableUsingName(a);
			newVariableTable.getVariableUsingName(b);
			newVariableTable.getVariableUsingName(c);
			newVariableTable.getVariableUsingName(d);
			newVariableTable.getVariableUsingName(e);
			newVariableTable.getVariableUsingName(f);
			newVariableTable.getVariableUsingName(g);
			Assert::AreEqual(7, newVariableTable.getNumberOfVariables());

			// Adding Uses/Modifies Relationships for Variables
			Assert::IsTrue(newVariableTable.getVariableUsingName(a)->addProcedureModifies(1));
			Assert::IsTrue(newVariableTable.getVariableUsingName(a)->addProcedureModifies(2));
			Assert::IsTrue(newVariableTable.getVariableUsingName(c)->addProcedureModifies(1));
			Assert::IsTrue(newVariableTable.getVariableUsingName(c)->addProcedureModifies(2));
			Assert::IsTrue(newVariableTable.getVariableUsingName(g)->addProcedureModifies(1));
			Assert::IsTrue(newVariableTable.getVariableUsingName(g)->addProcedureModifies(2));
			Assert::IsTrue(newVariableTable.getVariableUsingName(a)->addProcedureUses(1));
			Assert::IsTrue(newVariableTable.getVariableUsingName(a)->addProcedureUses(2));
			Assert::IsTrue(newVariableTable.getVariableUsingName(b)->addProcedureUses(1));
			Assert::IsTrue(newVariableTable.getVariableUsingName(b)->addProcedureUses(2));
			Assert::IsTrue(newVariableTable.getVariableUsingName(d)->addProcedureUses(1));
			Assert::IsTrue(newVariableTable.getVariableUsingName(d)->addProcedureUses(2));
			Assert::IsTrue(newVariableTable.getVariableUsingName(e)->addProcedureUses(1));
			Assert::IsTrue(newVariableTable.getVariableUsingName(e)->addProcedureUses(2));
			Assert::IsTrue(newVariableTable.getVariableUsingName(f)->addProcedureUses(1));
			Assert::IsTrue(newVariableTable.getVariableUsingName(f)->addProcedureUses(2));
			Assert::IsFalse(newVariableTable.getVariableUsingName(a)->addProcedureModifies(1));
			Assert::IsFalse(newVariableTable.getVariableUsingName(a)->addProcedureModifies(2));
			Assert::IsFalse(newVariableTable.getVariableUsingName(c)->addProcedureModifies(1));
			Assert::IsFalse(newVariableTable.getVariableUsingName(c)->addProcedureModifies(2));
			Assert::IsFalse(newVariableTable.getVariableUsingName(g)->addProcedureModifies(1));
			Assert::IsFalse(newVariableTable.getVariableUsingName(g)->addProcedureModifies(2));
			Assert::IsFalse(newVariableTable.getVariableUsingName(a)->addProcedureUses(1));
			Assert::IsFalse(newVariableTable.getVariableUsingName(a)->addProcedureUses(2));
			Assert::IsFalse(newVariableTable.getVariableUsingName(b)->addProcedureUses(1));
			Assert::IsFalse(newVariableTable.getVariableUsingName(b)->addProcedureUses(2));
			Assert::IsFalse(newVariableTable.getVariableUsingName(d)->addProcedureUses(1));
			Assert::IsFalse(newVariableTable.getVariableUsingName(d)->addProcedureUses(2));
			Assert::IsFalse(newVariableTable.getVariableUsingName(e)->addProcedureUses(1));
			Assert::IsFalse(newVariableTable.getVariableUsingName(e)->addProcedureUses(2));
			Assert::IsFalse(newVariableTable.getVariableUsingName(f)->addProcedureUses(1));
			Assert::IsFalse(newVariableTable.getVariableUsingName(f)->addProcedureUses(2));

			// Checks that the Uses/Modifies are not empty
			// If empty, do not check the next section
			Assert::AreEqual(2, newVariableTable.getVariableUsingName(a)->getProceduresModifiesSize());
			Assert::AreEqual(0, newVariableTable.getVariableUsingName(b)->getProceduresModifiesSize());
			Assert::AreEqual(2, newVariableTable.getVariableUsingName(c)->getProceduresModifiesSize());
			Assert::AreEqual(0, newVariableTable.getVariableUsingName(d)->getProceduresModifiesSize());
			Assert::AreEqual(0, newVariableTable.getVariableUsingName(e)->getProceduresModifiesSize());
			Assert::AreEqual(0, newVariableTable.getVariableUsingName(f)->getProceduresModifiesSize());
			Assert::AreEqual(2, newVariableTable.getVariableUsingName(g)->getProceduresModifiesSize());
			Assert::AreEqual(2, newVariableTable.getVariableUsingName(a)->getProceduresUsesSize());
			Assert::AreEqual(2, newVariableTable.getVariableUsingName(b)->getProceduresUsesSize());
			Assert::AreEqual(0, newVariableTable.getVariableUsingName(c)->getProceduresUsesSize());
			Assert::AreEqual(2, newVariableTable.getVariableUsingName(d)->getProceduresUsesSize());
			Assert::AreEqual(2, newVariableTable.getVariableUsingName(e)->getProceduresUsesSize());
			Assert::AreEqual(2, newVariableTable.getVariableUsingName(f)->getProceduresUsesSize());
			Assert::AreEqual(0, newVariableTable.getVariableUsingName(g)->getProceduresUsesSize());

			// Check that the Uses/Modifies relationships are saved properly
			Assert::AreEqual(1, newVariableTable.getVariableUsingName(a)->getProceduresModifies(0));
			Assert::AreEqual(2, newVariableTable.getVariableUsingName(a)->getProceduresModifies(1));
			Assert::AreEqual(1, newVariableTable.getVariableUsingName(c)->getProceduresModifies(0));
			Assert::AreEqual(2, newVariableTable.getVariableUsingName(c)->getProceduresModifies(1));
			Assert::AreEqual(1, newVariableTable.getVariableUsingName(g)->getProceduresModifies(0));
			Assert::AreEqual(2, newVariableTable.getVariableUsingName(g)->getProceduresModifies(1));
			Assert::AreEqual(1, newVariableTable.getVariableUsingName(a)->getProceduresUses(0));
			Assert::AreEqual(2, newVariableTable.getVariableUsingName(a)->getProceduresUses(1));
			Assert::AreEqual(1, newVariableTable.getVariableUsingName(b)->getProceduresUses(0));
			Assert::AreEqual(2, newVariableTable.getVariableUsingName(b)->getProceduresUses(1));
			Assert::AreEqual(1, newVariableTable.getVariableUsingName(d)->getProceduresUses(0));
			Assert::AreEqual(2, newVariableTable.getVariableUsingName(d)->getProceduresUses(1));
			Assert::AreEqual(1, newVariableTable.getVariableUsingName(e)->getProceduresUses(0));
			Assert::AreEqual(2, newVariableTable.getVariableUsingName(e)->getProceduresUses(1));
			Assert::AreEqual(1, newVariableTable.getVariableUsingName(f)->getProceduresUses(0));
			Assert::AreEqual(2, newVariableTable.getVariableUsingName(f)->getProceduresUses(1));

			// Checks that the three different ways to retrieve Variables are equivalent       
			Assert::IsTrue(newVariableTable.getVariableUsingName(a)->getProceduresUsesSize() ==
				newVariableTable.getVariableUsingVariableIndexNumber(0)->getProceduresUsesSize());
			Assert::IsTrue(newVariableTable.getVariableUsingVariableIndexNumber(0)->getProceduresUsesSize() ==
				newVariableTable.getVariableUsingVectorIndexNumber(0)->getProceduresUsesSize());
			Assert::IsTrue(newVariableTable.getVariableUsingName(b)->getProceduresUsesSize() ==
				newVariableTable.getVariableUsingVariableIndexNumber(1)->getProceduresUsesSize());
			Assert::IsTrue(newVariableTable.getVariableUsingVariableIndexNumber(1)->getProceduresUsesSize() ==
				newVariableTable.getVariableUsingVectorIndexNumber(1)->getProceduresUsesSize());
			Assert::IsTrue(newVariableTable.getVariableUsingName(c)->getProceduresUsesSize() ==
				newVariableTable.getVariableUsingVariableIndexNumber(2)->getProceduresUsesSize());
			Assert::IsTrue(newVariableTable.getVariableUsingVariableIndexNumber(2)->getProceduresUsesSize() ==
				newVariableTable.getVariableUsingVectorIndexNumber(2)->getProceduresUsesSize());
			Assert::IsTrue(newVariableTable.getVariableUsingName(d)->getProceduresUsesSize() ==
				newVariableTable.getVariableUsingVariableIndexNumber(3)->getProceduresUsesSize());
			Assert::IsTrue(newVariableTable.getVariableUsingVariableIndexNumber(3)->getProceduresUsesSize() ==
				newVariableTable.getVariableUsingVectorIndexNumber(3)->getProceduresUsesSize());
			Assert::IsTrue(newVariableTable.getVariableUsingName(e)->getProceduresUsesSize() ==
				newVariableTable.getVariableUsingVariableIndexNumber(4)->getProceduresUsesSize());
			Assert::IsTrue(newVariableTable.getVariableUsingVariableIndexNumber(4)->getProceduresUsesSize() ==
				newVariableTable.getVariableUsingVectorIndexNumber(4)->getProceduresUsesSize());
			Assert::IsTrue(newVariableTable.getVariableUsingName(f)->getProceduresUsesSize() ==
				newVariableTable.getVariableUsingVariableIndexNumber(5)->getProceduresUsesSize());
			Assert::IsTrue(newVariableTable.getVariableUsingVariableIndexNumber(5)->getProceduresUsesSize() ==
				newVariableTable.getVariableUsingVectorIndexNumber(5)->getProceduresUsesSize());
			Assert::IsTrue(newVariableTable.getVariableUsingName(g)->getProceduresUsesSize() ==
				newVariableTable.getVariableUsingVariableIndexNumber(6)->getProceduresUsesSize());
			Assert::IsTrue(newVariableTable.getVariableUsingVariableIndexNumber(6)->getProceduresUsesSize() ==
				newVariableTable.getVariableUsingVectorIndexNumber(6)->getProceduresUsesSize());

			// Adding Uses/Modifies Relationships for Statements
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(1)->addModifies(1));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(1)->addModifies(3));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(1)->addModifies(7));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(2)->addModifies(1));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(2)->addModifies(3));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(2)->addModifies(7));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(3)->addModifies(1));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(3)->addModifies(3));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(3)->addModifies(7));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(4)->addModifies(3));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(5)->addModifies(1));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(6)->addModifies(7));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(1)->addUses(1));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(1)->addUses(2));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(1)->addUses(4));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(1)->addUses(5));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(1)->addUses(6));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(2)->addUses(1));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(2)->addUses(2));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(2)->addUses(4));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(2)->addUses(5));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(2)->addUses(6));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(3)->addUses(1));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(3)->addUses(2));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(3)->addUses(4));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(3)->addUses(5));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(3)->addUses(6));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(4)->addUses(4));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(4)->addUses(5));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(4)->addUses(6));
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(6)->addUses(1));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(1)->addModifies(1));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(1)->addModifies(3));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(1)->addModifies(7));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(2)->addModifies(1));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(2)->addModifies(3));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(2)->addModifies(7));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(3)->addModifies(1));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(3)->addModifies(3));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(3)->addModifies(7));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(4)->addModifies(3));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(5)->addModifies(1));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(6)->addModifies(7));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(1)->addUses(1));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(1)->addUses(2));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(1)->addUses(4));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(1)->addUses(5));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(1)->addUses(6));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(2)->addUses(1));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(2)->addUses(2));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(2)->addUses(4));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(2)->addUses(5));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(2)->addUses(6));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(3)->addUses(1));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(3)->addUses(2));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(3)->addUses(4));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(3)->addUses(5));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(3)->addUses(6));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(4)->addUses(4));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(4)->addUses(5));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(4)->addUses(6));
			Assert::IsFalse(newStatementTable.getStatementUsingStatementNumber(6)->addUses(1));

			// Checks that the Uses/Modifies relationships are not empty
			Assert::AreEqual(3, newStatementTable.getStatementUsingStatementNumber(1)->getModifiesSize());
			Assert::AreEqual(3, newStatementTable.getStatementUsingStatementNumber(2)->getModifiesSize());
			Assert::AreEqual(3, newStatementTable.getStatementUsingStatementNumber(3)->getModifiesSize());
			Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(4)->getModifiesSize());
			Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(5)->getModifiesSize());
			Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(6)->getModifiesSize());
			Assert::AreEqual(5, newStatementTable.getStatementUsingStatementNumber(1)->getUsesSize());
			Assert::AreEqual(5, newStatementTable.getStatementUsingStatementNumber(2)->getUsesSize());
			Assert::AreEqual(5, newStatementTable.getStatementUsingStatementNumber(3)->getUsesSize());
			Assert::AreEqual(3, newStatementTable.getStatementUsingStatementNumber(4)->getUsesSize());
			Assert::AreEqual(0, newStatementTable.getStatementUsingStatementNumber(5)->getUsesSize());
			Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(6)->getUsesSize());

			// Checks that the Uses/Modifies relationships are saved properly
			Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(1)->getModifies(0));
			Assert::AreEqual(3, newStatementTable.getStatementUsingStatementNumber(1)->getModifies(1));
			Assert::AreEqual(7, newStatementTable.getStatementUsingStatementNumber(1)->getModifies(2));
			Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(2)->getModifies(0));
			Assert::AreEqual(3, newStatementTable.getStatementUsingStatementNumber(2)->getModifies(1));
			Assert::AreEqual(7, newStatementTable.getStatementUsingStatementNumber(2)->getModifies(2));
			Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(3)->getModifies(0));
			Assert::AreEqual(3, newStatementTable.getStatementUsingStatementNumber(3)->getModifies(1));
			Assert::AreEqual(7, newStatementTable.getStatementUsingStatementNumber(3)->getModifies(2));
			Assert::AreEqual(3, newStatementTable.getStatementUsingStatementNumber(4)->getModifies(0));
			Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(5)->getModifies(0));
			Assert::AreEqual(7, newStatementTable.getStatementUsingStatementNumber(6)->getModifies(0));
			Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(1)->getUses(0));
			Assert::AreEqual(2, newStatementTable.getStatementUsingStatementNumber(1)->getUses(1));
			Assert::AreEqual(4, newStatementTable.getStatementUsingStatementNumber(1)->getUses(2));
			Assert::AreEqual(5, newStatementTable.getStatementUsingStatementNumber(1)->getUses(3));
			Assert::AreEqual(6, newStatementTable.getStatementUsingStatementNumber(1)->getUses(4));
			Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(2)->getUses(0));
			Assert::AreEqual(2, newStatementTable.getStatementUsingStatementNumber(2)->getUses(1));
			Assert::AreEqual(4, newStatementTable.getStatementUsingStatementNumber(2)->getUses(2));
			Assert::AreEqual(5, newStatementTable.getStatementUsingStatementNumber(2)->getUses(3));
			Assert::AreEqual(6, newStatementTable.getStatementUsingStatementNumber(2)->getUses(4));
			Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(3)->getUses(0));
			Assert::AreEqual(2, newStatementTable.getStatementUsingStatementNumber(3)->getUses(1));
			Assert::AreEqual(4, newStatementTable.getStatementUsingStatementNumber(3)->getUses(2));
			Assert::AreEqual(5, newStatementTable.getStatementUsingStatementNumber(3)->getUses(3));
			Assert::AreEqual(6, newStatementTable.getStatementUsingStatementNumber(3)->getUses(4));
			Assert::AreEqual(4, newStatementTable.getStatementUsingStatementNumber(4)->getUses(0));
			Assert::AreEqual(5, newStatementTable.getStatementUsingStatementNumber(4)->getUses(1));
			Assert::AreEqual(6, newStatementTable.getStatementUsingStatementNumber(4)->getUses(2));
			Assert::AreEqual(1, newStatementTable.getStatementUsingStatementNumber(6)->getUses(0));

			// Checks that the two different ways to retrieve Statements are equivalent
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(1)->getUsesSize() ==
				newStatementTable.getStatementUsingVectorIndexNumber(0)->getUsesSize());
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(2)->getUsesSize() ==
				newStatementTable.getStatementUsingVectorIndexNumber(1)->getUsesSize());
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(3)->getUsesSize() ==
				newStatementTable.getStatementUsingVectorIndexNumber(2)->getUsesSize());
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(4)->getUsesSize() ==
				newStatementTable.getStatementUsingVectorIndexNumber(3)->getUsesSize());
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(5)->getUsesSize() ==
				newStatementTable.getStatementUsingVectorIndexNumber(4)->getUsesSize());
			Assert::IsTrue(newStatementTable.getStatementUsingStatementNumber(6)->getUsesSize() ==
				newStatementTable.getStatementUsingVectorIndexNumber(5)->getUsesSize());

			// Adding Uses/Modifies Relationships for Procedures
			Assert::IsTrue(newProcedureTable.getProcedure(testProcName)->addModifies(1));
			Assert::IsTrue(newProcedureTable.getProcedure(testProcName)->addModifies(3));
			Assert::IsTrue(newProcedureTable.getProcedure(testProcName)->addModifies(7));
			Assert::IsTrue(newProcedureTable.getProcedure(testProcName2)->addModifies(1));
			Assert::IsTrue(newProcedureTable.getProcedure(testProcName2)->addModifies(3));
			Assert::IsTrue(newProcedureTable.getProcedure(testProcName2)->addModifies(7));
			Assert::IsTrue(newProcedureTable.getProcedure(testProcName)->addUses(1));
			Assert::IsTrue(newProcedureTable.getProcedure(testProcName)->addUses(2));
			Assert::IsTrue(newProcedureTable.getProcedure(testProcName)->addUses(4));
			Assert::IsTrue(newProcedureTable.getProcedure(testProcName)->addUses(5));
			Assert::IsTrue(newProcedureTable.getProcedure(testProcName)->addUses(6));
			Assert::IsTrue(newProcedureTable.getProcedure(testProcName2)->addUses(1));
			Assert::IsTrue(newProcedureTable.getProcedure(testProcName2)->addUses(2));
			Assert::IsTrue(newProcedureTable.getProcedure(testProcName2)->addUses(4));
			Assert::IsTrue(newProcedureTable.getProcedure(testProcName2)->addUses(5));
			Assert::IsTrue(newProcedureTable.getProcedure(testProcName2)->addUses(6));
			Assert::IsFalse(newProcedureTable.getProcedure(testProcName)->addModifies(1));
			Assert::IsFalse(newProcedureTable.getProcedure(testProcName)->addModifies(3));
			Assert::IsFalse(newProcedureTable.getProcedure(testProcName)->addModifies(7));
			Assert::IsFalse(newProcedureTable.getProcedure(testProcName2)->addModifies(1));
			Assert::IsFalse(newProcedureTable.getProcedure(testProcName2)->addModifies(3));
			Assert::IsFalse(newProcedureTable.getProcedure(testProcName2)->addModifies(7));
			Assert::IsFalse(newProcedureTable.getProcedure(testProcName)->addUses(1));
			Assert::IsFalse(newProcedureTable.getProcedure(testProcName)->addUses(2));
			Assert::IsFalse(newProcedureTable.getProcedure(testProcName)->addUses(4));
			Assert::IsFalse(newProcedureTable.getProcedure(testProcName)->addUses(5));
			Assert::IsFalse(newProcedureTable.getProcedure(testProcName)->addUses(6));
			Assert::IsFalse(newProcedureTable.getProcedure(testProcName2)->addUses(1));
			Assert::IsFalse(newProcedureTable.getProcedure(testProcName2)->addUses(2));
			Assert::IsFalse(newProcedureTable.getProcedure(testProcName2)->addUses(4));
			Assert::IsFalse(newProcedureTable.getProcedure(testProcName2)->addUses(5));
			Assert::IsFalse(newProcedureTable.getProcedure(testProcName2)->addUses(6));

			// Checks that the Uses/Modifies relationships are not empty
			Assert::AreEqual(3, newProcedureTable.getProcedure(testProcName)->getModifiesSize());
			Assert::AreEqual(3, newProcedureTable.getProcedure(testProcName2)->getModifiesSize());
			Assert::AreEqual(5, newProcedureTable.getProcedure(testProcName)->getUsesSize());
			Assert::AreEqual(5, newProcedureTable.getProcedure(testProcName2)->getUsesSize());

			// Checks that the Uses/Modifies relationships are saved properly
			Assert::AreEqual(1, newProcedureTable.getProcedure(testProcName)->getModifies(0));
			Assert::AreEqual(3, newProcedureTable.getProcedure(testProcName)->getModifies(1));
			Assert::AreEqual(7, newProcedureTable.getProcedure(testProcName)->getModifies(2));
			Assert::AreEqual(1, newProcedureTable.getProcedure(testProcName2)->getModifies(0));
			Assert::AreEqual(3, newProcedureTable.getProcedure(testProcName2)->getModifies(1));
			Assert::AreEqual(7, newProcedureTable.getProcedure(testProcName2)->getModifies(2));
			Assert::AreEqual(1, newProcedureTable.getProcedure(testProcName)->getUses(0));
			Assert::AreEqual(2, newProcedureTable.getProcedure(testProcName)->getUses(1));
			Assert::AreEqual(4, newProcedureTable.getProcedure(testProcName)->getUses(2));
			Assert::AreEqual(5, newProcedureTable.getProcedure(testProcName)->getUses(3));
			Assert::AreEqual(6, newProcedureTable.getProcedure(testProcName)->getUses(4));
			Assert::AreEqual(1, newProcedureTable.getProcedure(testProcName2)->getUses(0));
			Assert::AreEqual(2, newProcedureTable.getProcedure(testProcName2)->getUses(1));
			Assert::AreEqual(4, newProcedureTable.getProcedure(testProcName2)->getUses(2));
			Assert::AreEqual(5, newProcedureTable.getProcedure(testProcName2)->getUses(3));
			Assert::AreEqual(6, newProcedureTable.getProcedure(testProcName2)->getUses(4));

			// Checks that the two different ways to retrieve Procedures are equivalent
			Assert::IsTrue(newProcedureTable.getProcedure(testProcName)->getUsesSize() ==
				newProcedureTable.getProcedure(0)->getUsesSize());
			Assert::IsTrue(newProcedureTable.getProcedure(testProcName2)->getUsesSize() ==
				newProcedureTable.getProcedure(1)->getUsesSize());
		}
    };
}