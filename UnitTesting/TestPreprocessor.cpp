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
		Clause* usesClause;
		vector<Clause*> oneClauses;

		const string simpleQuery = "variable a; select a";
		const string usesQuery = "variable a; constant b; select a such that uses(a, b)";

		TEST_METHOD_INITIALIZE(build) {
			placeholderVar = Variable("_", "placeholder");
			partOfExpressionVar = Variable("_\"abc\"_", "part_of_expr"); //only pattern
			exprVar = Variable("\"abc\"", "expr");
			integerVar = Variable("123", "integer");
			stmtVar = Variable("a", "stmt");
			assignVar = Variable("a", "assign");
			whileVar = Variable("a", "while");
			variableVar = Variable("a", "variable");
			constantVar = Variable("b", "constant");
			progLineVar = Variable("a", "prog_line");

			usesClause = new Clause("USES", variableVar, constantVar);
			
			oneClauses.push_back(usesClause);

			QueryTree::Instance()->newTree();
		}

		TEST_METHOD(SimpleQuery)
		{
			p.process(simpleQuery);
			select = QueryTree::Instance()->getSelect();
			clauses = QueryTree::Instance()->getClauses();

			Assert::IsTrue(select.equals(variableVar));
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
	};
}