#include "stdafx.h"
#include "CppUnitTest.h"
#include "TNode.h"
#include "./Preprocessor/Clause.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestClause)
	{
	public:

		TEST_METHOD(getClause)
		{
			string clause = "AB";
			Variable v1 = Variable("A", "B");
			Variable v2 = Variable("A", "B");
			Clause testClause = Clause(clause, v1, v2);
			string newClause = testClause.getClause();
			Assert::AreEqual(clause, newClause);
		}
	};
}