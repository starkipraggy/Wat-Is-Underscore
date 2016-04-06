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
			QueryTree::Instance()->setSelect(variableVar);
			
			vector<string> output = q.process();
			
			Assert::IsTrue(output.at(0) == "AnswerForSelect", (wchar_t*)output.at(0).c_str());
		}

		TEST_METHOD(TestEvaluator_UsesFirstEvaluation)
		{
			QueryTree::Instance()->setSelect(assignVar);
			QueryTree::Instance()->addClause(usesClauseFirst);

			vector<string> output = q.process();

			Assert::IsTrue(output.at(0) == "AnswerForUses", (wchar_t*)output.at(0).c_str());
		}

		TEST_METHOD(TestEvaluator_UsesSecondEvaluation)
		{
			QueryTree::Instance()->setSelect(stmtVar);
			QueryTree::Instance()->addClause(usesClauseSecond);

			vector<string> output = q.process();

			Assert::IsTrue(output.at(0) == "AnswerForUses", (wchar_t*)output.at(0).c_str());
		}

		TEST_METHOD(TestEvaluator_UsesBothEvaluation)
		{
			QueryTree::Instance()->setSelect(stmtVar);
			QueryTree::Instance()->addClause(usesClauseBoth);

			vector<string> output = q.process();

			Assert::IsTrue(output.at(0) == "AnswerForSelect", (wchar_t*)output.at(0).c_str());
		}

		TEST_METHOD(TestEvaluator_PatternEvaluation)
		{
			QueryTree::Instance()->setSelect(assignVar);
			QueryTree::Instance()->addClause(patternClause);

			vector<string> output = q.process();

			Assert::IsTrue(output.at(0) == "AnswerForPattern", (wchar_t*)output.at(0).c_str());
		}

		TEST_METHOD(TestEvaluator_WithEvaluation)
		{
			QueryTree::Instance()->setSelect(assignVar);
			QueryTree::Instance()->addClause(withClause);

			vector<string> output = q.process();

			Assert::IsTrue(output.at(0) == "AnswerForWith", (wchar_t*)output.at(0).c_str());
		}

		TEST_METHOD(TestEvaluator_MixEvaluation)
		{
			QueryTree::Instance()->setSelect(assignVar);
			QueryTree::Instance()->addClause(usesClauseFirst);
			QueryTree::Instance()->addClause(patternClause);

			vector<string> output = q.process();

			Assert::IsTrue(output.at(0) == "AnswerForMix", (wchar_t*)output.at(0).c_str());
		}

	};
}