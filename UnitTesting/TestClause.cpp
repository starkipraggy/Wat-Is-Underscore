#include "stdafx.h"
#include "CppUnitTest.h"
#include "TNode.h"
#include "./Preprocessor/Clause.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestClause)
	{
	public:
		string clause;
		Variable v1;
		Variable v2;
		Clause testClause;

		TEST_METHOD_INITIALIZE(build) {
			clause = "AB";
			v1 = Variable("A", "B");
			v2 = Variable("A", "B");
			testClause = Clause(clause, v1, v2);
		}
		
		TEST_METHOD(getClause)
		{
			string text = "AB";
			string newClause = testClause.getClause();
			Assert::AreEqual(text, newClause);
		}
	};
}