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
		Clause* usesClause, patternClause;
		vector<Clause*> oneClauses;

		TEST_METHOD_INITIALIZE(build) {
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
			patternClause = PatternClause("PATTERN", placeholderVar, partOfExpressionVar, assignVar);

			oneClauses.push_back(usesClause);

			pkb = StubPKB::getInstance();
			PKB::setInstance(pkb);

			QueryTree::Instance()->newTree();
		}

		TEST_METHOD(SimpleEvaluation)
		{
			QueryTree::Instance()->setSelect(variableVar);
			vector<string> output = q.process();
			string print = pkb->PQLSelect(VariableName).at(0);
			Assert::IsTrue(output.at(0) == "v", (wchar_t*)print.c_str());
		}

	};
}