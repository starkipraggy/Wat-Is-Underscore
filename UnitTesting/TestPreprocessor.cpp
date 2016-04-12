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
		vector<Ref> select;
		vector<Clause*> clauses;


		Ref placeholderVar, partOfExpressionVar, exprVar, integerVar,
			stmtVar, assignVar, whileVar, variableVar, constantVar, progLineVar, 
			invalidVar, boolVar, withVar, procedureVar, ifVar;
		Clause* usesClause;
		Clause* callsClause;
		Clause* patternClause;
		Clause* patternIfClause;
		Clause* withClause;
		vector<Clause*> oneClauses;

		const string simpleQuery = "assign a; select a",
			simpleBoolQuery = "assign a; select BOOLEAN",
			simpleTupleQuery = "assign a; variable v; select <a,v>",
			usesQuery = "variable v; assign a; select v such that uses(a, v)",
			usesTupleQuery = "variable v; assign a; select <a,v> such that uses(a, v)",
			callsQuery = "procedure p; select p such that calls(p, \"abc\")",
			patternQuery = "assign a; select a pattern a(_,\"abc\")",
			patternIfQuery = "if ifstmt; select ifstmt pattern ifstmt(\"abc\",_,_)",
			withQuery = "procedure p; select p with p.procName = \"abc\"",
			combinedQuery = "variable v; assign a; select a such that uses(a, v) pattern a(_,\"abc\")";
	

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
			progLineVar = Ref("pl", "prog_line");
			procedureVar = Ref("p", "procedure");
			boolVar = Ref("", "boolean");
			ifVar = Ref("ifstmt", "if");

			usesClause = new Clause("USES", assignVar, variableVar);
			callsClause = new Clause("CALLS", procedureVar, exprVar);
			patternClause = new PatternClause("PATTERN", placeholderVar, exprVar, assignVar);
			patternIfClause = new PatternClause("PATTERN", exprVar, placeholderVar, ifVar);
			withClause = new Clause("WITH", procedureVar, exprVar);
			
			oneClauses.push_back(usesClause);

			QueryTree::Instance()->newTree();
		}

		TEST_METHOD(TestPreprocessor_SimpleQuery)
		{
			p.process(simpleQuery);
			select = QueryTree::Instance()->getSelect();
			clauses = QueryTree::Instance()->getClauses();

			Assert::IsTrue(select.at(0).equals(assignVar));
			Assert::IsTrue(clauses.empty());
		}

		TEST_METHOD(TestPreprocessor_SimpleBoolQuery)
		{
			p.process(simpleBoolQuery);
			select = QueryTree::Instance()->getSelect();
			clauses = QueryTree::Instance()->getClauses();

			Assert::IsTrue(select.at(0).equals(boolVar));
			Assert::IsTrue(clauses.empty());
		}

		TEST_METHOD(TestPreprocessor_SimpleTupleQuery)
		{
			p.process(simpleTupleQuery);
			select = QueryTree::Instance()->getSelect();
			clauses = QueryTree::Instance()->getClauses();

			Assert::IsTrue(select.at(0).equals(assignVar));
			Assert::IsTrue(select.at(1).equals(variableVar));
			Assert::IsTrue(clauses.empty());
		}

		TEST_METHOD(TestPreprocessor_UsesQuery)
		{
			p.process(usesQuery);
			select = QueryTree::Instance()->getSelect();
			clauses = QueryTree::Instance()->getClauses();
			
			Assert::IsTrue(select.at(0).equals(variableVar));
			Assert::IsTrue(clauses.at(0)->equals(usesClause));
		}


		TEST_METHOD(TestPreprocessor_UsesTupleQuery)
		{
			p.process(usesTupleQuery);
			select = QueryTree::Instance()->getSelect();
			clauses = QueryTree::Instance()->getClauses();

			Assert::IsTrue(select.at(0).equals(assignVar));
			Assert::IsTrue(select.at(1).equals(variableVar));
			Assert::IsTrue(clauses.at(0)->equals(usesClause));
		}

		TEST_METHOD(TestPreprocessor_Follows1Query)
		{
			p.process("assign a; while w; select w such that Follows(w, a)");
			select = QueryTree::Instance()->getSelect();
			clauses = QueryTree::Instance()->getClauses();

			Assert::IsTrue(select.at(0).equals(whileVar));
			Assert::IsTrue(clauses.at(0)->equals(new Clause("follows", whileVar, assignVar)));
		}

		TEST_METHOD(TestPreprocessor_Follows2Query)
		{
			p.process("assign a; while w; select w such that Follows(a, w)");
			select = QueryTree::Instance()->getSelect();
			clauses = QueryTree::Instance()->getClauses();

			Assert::IsTrue(select.at(0).equals(whileVar));
			Assert::IsTrue(clauses.at(0)->equals(new Clause("follows", assignVar, whileVar)));
		}

		TEST_METHOD(TestPreprocessor_FollowsStarQuery)
		{
			p.process("stmt s; select s such that follows*(s,123)");
			select = QueryTree::Instance()->getSelect();
			clauses = QueryTree::Instance()->getClauses();

			Assert::IsTrue(select.at(0).equals(stmtVar));
			Assert::IsTrue(clauses.at(0)->equals(new Clause("follows*", stmtVar, integerVar)));
		}

		TEST_METHOD(TestPreprocessor_CallsQuery)
		{
			p.process(callsQuery);
			select = QueryTree::Instance()->getSelect();
			clauses = QueryTree::Instance()->getClauses();

			Assert::IsTrue(select.at(0).equals(procedureVar));
			Assert::IsTrue(clauses.at(0)->equals(callsClause));
		}

		TEST_METHOD(TestPreprocessor_PatternQuery)
		{
			p.process(patternQuery);
			select = QueryTree::Instance()->getSelect();
			clauses = QueryTree::Instance()->getClauses();

			Assert::IsTrue(select.at(0).equals(assignVar));
			Assert::IsTrue(patternClause->equals(clauses.at(0)));
		}

		TEST_METHOD(TestPreprocessor_PatternIfQuery)
		{
			p.process(patternIfQuery);
			select = QueryTree::Instance()->getSelect();
			clauses = QueryTree::Instance()->getClauses();

			Assert::IsTrue(select.at(0).equals(ifVar));
			Assert::IsTrue(patternIfClause->equals(clauses.at(0)));
		}

		TEST_METHOD(TestPreprocessor_WithQuery)
		{
			p.process(withQuery);
			select = QueryTree::Instance()->getSelect();
			clauses = QueryTree::Instance()->getClauses();

			Assert::IsTrue(select.at(0).equals(procedureVar));
			Assert::IsTrue(withClause->equals(clauses.at(0)));
		}

		TEST_METHOD(TestPreprocessor_CombinedQuery)
		{
			p.process(combinedQuery);
			select = QueryTree::Instance()->getSelect();
			clauses = QueryTree::Instance()->getClauses();

			Assert::IsTrue(select.at(0).equals(assignVar));
			Assert::IsTrue(clauses.at(1)->equals(usesClause));
			Assert::IsTrue(clauses.at(0)->equals(patternClause));
		}
	};
}