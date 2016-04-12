#include "stdafx.h"
#include "CppUnitTest.h"
#include "./Preprocessor/QueryEvaluator.h"
#include "StubPKB.h"
#include <windows.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestEvaluator)
	{
	public:
		QueryEvaluator q;

		//temp
		Ref select;
		vector<Clause*> clauses;
		PKB* pkb;
		vector<vector<string>> output;

		Ref placeholderVar, partOfExpressionVar, exprVar, integerVar,
			stmtVar, assignVar, whileVar, variableVar, constantVar, progLineVar,
			invalidVar, withVar;
		Clause* usesClauseFirst;
		Clause* usesClauseSecond;
		Clause* usesClauseBoth;
		Clause* followsClause;
		Clause* withClause;
		Clause* patternClause;
		vector<Clause*> oneClauses;

		TEST_METHOD_INITIALIZE(STARTUP) {
			placeholderVar = Ref("_", "placeholder");
			partOfExpressionVar = Ref("abc", "part_of_expr"); //only pattern
			exprVar = Ref("abc", "expr");
			integerVar = Ref("123", "integer");
			stmtVar = Ref("s", "stmt");
			assignVar = Ref("a", "assign");
			whileVar = Ref("w", "while");
			variableVar = Ref("v", "variable");
			constantVar = Ref("c", "constant");
			progLineVar = Ref("p", "prog_line");
			withVar = Ref("AnswerForWith", "expr");

			usesClauseFirst = new Clause("USES", assignVar, integerVar);
			usesClauseSecond = new Clause("USES", integerVar, stmtVar);
			usesClauseBoth = new Clause("USES", assignVar, stmtVar);
			followsClause = new Clause("FOLLOWS", integerVar, stmtVar);
			withClause = new Clause("WITH", assignVar, withVar);
			patternClause = new PatternClause("PATTERN", placeholderVar, partOfExpressionVar, assignVar);

			oneClauses.push_back(usesClauseFirst);

			pkb = StubPKB::getInstance();
			PKB::setInstance(pkb);

			QueryTree::Instance()->newTree();
		}

		TEST_METHOD(TestEvaluator_SelectEvaluation)
		{
			QueryTree::Instance()->setSelect({ variableVar });
			QueryTree::Instance()->buildTree();
			
			output = q.process();
			
			Assert::IsTrue(output.at(0).at(0) == "AnswerForSelect");
		}

		TEST_METHOD(TestEvaluator_TupleEvaluation)
		{
			QueryTree::Instance()->setSelect({ variableVar, assignVar });
			QueryTree::Instance()->buildTree();

			output = q.process();

			Assert::IsTrue(output.at(0).at(0) == "AnswerForSelect");
			Assert::IsTrue(output.at(0).at(1) == "AnswerForSelect");
		}

		TEST_METHOD(TestEvaluator_UsesFirstEvaluation)
		{
			QueryTree::Instance()->setSelect({ assignVar });
			QueryTree::Instance()->addClause(usesClauseFirst);
			QueryTree::Instance()->buildTree();

			output = q.process();

			Assert::IsTrue(output.at(0).at(0) == "AnswerForUses");
		}

		TEST_METHOD(TestEvaluator_UsesSecondEvaluation)
		{
			QueryTree::Instance()->setSelect({ stmtVar });
			QueryTree::Instance()->addClause(usesClauseSecond);
			QueryTree::Instance()->buildTree();

			output = q.process();

			Assert::IsTrue(output.at(0).at(0) == "AnswerForUses");
		}

		TEST_METHOD(TestEvaluator_UsesBothEvaluation)
		{
			QueryTree::Instance()->setSelect({ stmtVar });
			QueryTree::Instance()->addClause(usesClauseBoth);
			QueryTree::Instance()->buildTree();

			output = q.process();

			Assert::IsTrue(output.at(0).at(0) == "AnswerForUses");
		}

		TEST_METHOD(TestEvaluator_PatternEvaluation)
		{
			QueryTree::Instance()->setSelect({ assignVar });
			QueryTree::Instance()->addClause(patternClause);
			QueryTree::Instance()->buildTree();

			output = q.process();

			Assert::IsTrue(output.at(0).at(0) == "AnswerForPattern");
		}

		TEST_METHOD(TestEvaluator_WithEvaluation)
		{
			QueryTree::Instance()->setSelect({ assignVar });
			QueryTree::Instance()->addClause(withClause);
			QueryTree::Instance()->buildTree();

			output = q.process();

			Assert::IsTrue(output.at(0).at(0) == "AnswerForWith");
		}

		TEST_METHOD(TestEvaluator_MixEvaluation)
		{
			QueryTree::Instance()->setSelect({ assignVar });
			QueryTree::Instance()->addClause(usesClauseFirst);
			QueryTree::Instance()->addClause(patternClause);
			QueryTree::Instance()->buildTree();

			output = q.process();

			Assert::IsTrue(output.at(0).at(0) == "AnswerForMix");
		}

	};
}