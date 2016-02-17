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

		//temp
		Variable select;
		vector<Clause*> clauses;


		Variable placeholderVar, partOfExpressionVar, exprVar, integerVar, 
			stmtVar, assignVar, whileVar, variableVar, constantVar, progLineVar, 
			invalidVar;
		Clause* usesClause, patternClause;
		vector<Clause*> oneClauses;

		const string simpleQuery = "assign a; select a",
			usesQuery = "assign a; constant c; select a such that uses(a, c)",
			patternQuery = "assign a; select a pattern a(_,_\"abc\"_)",
			combinedQuery = "assign a; constant c; select a such that uses(a, c) pattern a(_,_\"abc\"_)";
	

		TEST_METHOD_INITIALIZE(build) {
			placeholderVar = Variable("_", "placeholder");
			partOfExpressionVar = Variable("_\"abc\"_", "part_of_expr"); //only pattern
			exprVar = Variable("\"abc\"", "expr");
			integerVar = Variable("123", "integer");
			stmtVar = Variable("s", "stmt");
			assignVar = Variable("a", "assign");
			whileVar = Variable("w", "while");
			variableVar = Variable("v", "variable");
			constantVar = Variable("c", "constant");
			progLineVar = Variable("p", "prog_line");

			usesClause = new Clause("USES", assignVar, constantVar);
			patternClause = PatternClause("PATTERN", placeholderVar, partOfExpressionVar, assignVar);
			
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

		TEST_METHOD(UsesQuery)
		{
			p.process(usesQuery);
			select = QueryTree::Instance()->getSelect();
			clauses = QueryTree::Instance()->getClauses();
			
			Assert::IsTrue(select.equals(assignVar));
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