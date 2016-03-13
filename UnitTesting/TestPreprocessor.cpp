#include "stdafx.h"
#include "CppUnitTest.h"
#include "TNode.h"
#include "./Preprocessor/Preprocessor.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestPreprocessor)
	{
	public:
		Preprocessor p;

		//setup
		Ref select;
		vector<Clause*> clauses;


		Ref placeholderVar, partOfExpressionVar, exprVar, integerVar,
			stmtVar, assignVar, whileVar, variableVar, constantVar, progLineVar, 
			invalidVar, boolVar, withVar;
		Clause* usesClause, patternClause, withClause;
		vector<Clause*> oneClauses;

		const string simpleQuery = "assign a; select a",
			simpleBoolQuery = "assign a; select BOOLEAN",
			usesQuery = "variable v; constant c; select v such that uses(c, v)",
			patternQuery = "assign a; select a pattern a(_,\"abc\")",
			withQuery = "assign a; select a with a.stmt# = 123",
			combinedQuery = "variable v; constant c; assign a; select a such that uses(c, v) pattern a(_,\"abc\")";
	

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
			boolVar = Ref("", "boolean");
			withVar = Ref("a", "assign-stmt#");

			usesClause = new Clause("USES", constantVar, variableVar);
			patternClause = PatternClause("PATTERN", placeholderVar, exprVar, assignVar);
			withClause = Clause("WITH", withVar, integerVar);
			
			oneClauses.push_back(usesClause);

			QueryTree::Instance()->newTree();
		}

		TEST_METHOD(SimpleQuery)
		{
			p.process(simpleQuery);
			select = QueryTree::Instance()->getSelect();
			clauses = QueryTree::Instance()->getClauses();

			Assert::IsTrue(select.equals(assignVar));
			Assert::IsTrue(clauses.empty());
		}

		TEST_METHOD(SimpleBoolQuery)
		{
			p.process(simpleBoolQuery);
			select = QueryTree::Instance()->getSelect();
			clauses = QueryTree::Instance()->getClauses();

			Assert::IsTrue(select.equals(boolVar));
			Assert::IsTrue(clauses.empty());
		}

		TEST_METHOD(UsesQuery)
		{
			p.process(usesQuery);
			select = QueryTree::Instance()->getSelect();
			clauses = QueryTree::Instance()->getClauses();
			
			Assert::IsTrue(select.equals(variableVar));
			Assert::IsTrue(clauses.at(0)->equals(usesClause));
		}

		TEST_METHOD(PatternQuery)
		{
			p.process(patternQuery);
			select = QueryTree::Instance()->getSelect();
			clauses = QueryTree::Instance()->getClauses();

			Assert::IsTrue(select.equals(assignVar));
			Assert::IsTrue(patternClause.equals(clauses.at(0)));
		}

		TEST_METHOD(WithQuery)
		{
			p.process(withQuery);
			select = QueryTree::Instance()->getSelect();
			clauses = QueryTree::Instance()->getClauses();

			Assert::IsTrue(select.equals(assignVar));
			Assert::IsTrue(withClause.equals(clauses.at(0)));
		}

		TEST_METHOD(CombinedQuery)
		{
			p.process(combinedQuery);
			select = QueryTree::Instance()->getSelect();
			clauses = QueryTree::Instance()->getClauses();

			Assert::IsTrue(select.equals(assignVar));
			Assert::IsTrue(clauses.at(0)->equals(usesClause));
			Assert::IsTrue(patternClause.equals(clauses.at(1)));
		}
	};
}