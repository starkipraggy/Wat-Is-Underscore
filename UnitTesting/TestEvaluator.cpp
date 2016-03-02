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
			invalidVar;
		Clause* usesClause;
		Clause* followsClause;
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

			usesClause = new Clause("USES", assignVar, constantVar);
			followsClause = new Clause("FOLLOWS", integerVar, stmtVar);
			patternClause = new PatternClause("PATTERN", placeholderVar, partOfExpressionVar, assignVar);

			oneClauses.push_back(usesClause);

			pkb = StubPKB::getInstance();
			PKB::setInstance(pkb);

			QueryTree::Instance()->newTree();
		}

		TEST_METHOD(SimpleEvaluation)
		{
			QueryTree::Instance()->setSelect(variableVar);
			
			vector<string> output = q.process();
			
			Assert::IsTrue(output.at(0) == "AnswerForSelect", (wchar_t*)output.at(0).c_str());
		}

		TEST_METHOD(UsesEvaluation)
		{
			QueryTree::Instance()->setSelect(variableVar);
			QueryTree::Instance()->addClause(usesClause);

			vector<string> output = q.process();

			Assert::IsTrue(output.at(1) == "AnswerForUses", (wchar_t*)output.at(0).c_str());
		}

		TEST_METHOD(secondArgVariable_Follows)
		{
			QueryTree::Instance()->setSelect(variableVar);
			QueryTree::Instance()->addClause(followsClause);

			vector<string> output = q.process();

			Assert::IsTrue(output.at(1) == "AnswerForFollows", (wchar_t*)output.at(0).c_str());
		}

		TEST_METHOD(PatternEvaluation)
		{
			QueryTree::Instance()->setSelect(variableVar);
			QueryTree::Instance()->addClause(new PatternClause("PATTERN", placeholderVar, partOfExpressionVar, assignVar));

			vector<string> output = q.process();

			Assert::IsTrue(output.at(0) == "AnswerForPattern", (wchar_t*)output.at(0).c_str());
		}

		TEST_METHOD(MixEvaluation)
		{
			QueryTree::Instance()->setSelect(variableVar);
			QueryTree::Instance()->addClause(usesClause);
			QueryTree::Instance()->addClause(new PatternClause("PATTERN", placeholderVar, partOfExpressionVar, assignVar));

			vector<string> output = q.process();

			Assert::IsTrue(output.at(0) == "AnswerForMix", (wchar_t*)output.at(0).c_str());
		}

	};
}