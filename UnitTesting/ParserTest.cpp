#include "stdafx.h"
#include "CppUnitTest.h"
#include "SimpleParser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


TEST_CLASS(TestAST)
{
public:
	SimpleParser* sp;

	TEST_METHOD(SimpleParserTest_AddSpacesToString) {
		//Arrange
		std::string sample = "{a=b+x-c;}";
		std::string expected = " { a = b + x - c ;  } ";

		//Act
		std::string result = sp->addSpaceToString(sample);

		//Assert
		Assert::AreEqual(expected, result);
	}

	TEST_METHOD(SimpleParserTest_Tokenisation_General) {
		//Arrange
		std::string simple = "ab}a{;+9g-n";
		std::vector<std::string> expected = { "ab", "}","a","{",";","+","9g","-","n" };
		std::vector<std::string> results;
		sp = new SimpleParser();
		//Act
		std::vector<std::string> tokens;
		results = sp->tokenize(simple);

		//Assert
		for (unsigned int i = 0; i < expected.size();i++) {
			Assert::AreEqual(expected[i], results[i]);
		}
	}

	// Tokenizes Assignments
	TEST_METHOD(SimpleParserTest_Tokenisation_Simple_1) {
		//Arrange
		std::string simple = "procedure a{a=b+c+d;}";
		std::vector<std::string> expected = { "procedure", "a","{","a","=","b","+","c","+","d",";" ,"}" };
		std::vector<std::string> results;
		sp = new SimpleParser();
		//Act
		std::vector<std::string> tokens;
		results = sp->tokenize(simple);

		//Assert
		for (unsigned int i = 0; i < expected.size();i++) {
			Assert::AreEqual(expected[i], results[i]);
		}
	}
	
	// Tokenizes While and Assignments
	TEST_METHOD(SimpleParserTest_Tokenisation_Simple_2) {
		//Arrange
		std::string simple = "procedure xs{while b{x+4+z;}";
		std::vector<std::string> expected = { "procedure", "xs","{","while","b","{","x","+","4","+","z",";" ,"}" };
		std::vector<std::string> results;
		sp = new SimpleParser();
		//Act
		std::vector<std::string> tokens2;
		results = sp->tokenize(simple);

		//Assert
		for (unsigned int i = 0; i < expected.size();i++) {
			Assert::AreEqual(expected[i], results[i]);
		}
	}

	TEST_METHOD(SimpleParserTest_Tokenisation_Simple_3) {
		//Arrange
		std::string simple = "procedure re{while b{ if c then {a = 1;} else {r=z+2;} }}";
		std::vector<std::string> expected = { "procedure", "re","{","while","b","{","if","c","then","{","a","=","1",";","}","else","{","r","=","z","+","2" ,";","}","}","}" };
		std::vector<std::string> results;
		sp = new SimpleParser();
		//Act
		std::vector<std::string> tokens2;
		results = sp->tokenize(simple);

		//Assert
		for (unsigned int i = 0; i < expected.size();i++) {
			Assert::AreEqual(expected[i], results[i]);
		}
	}

	TEST_METHOD(SimpleParserTest_Tokenisation_Simple_4) {
		//Arrange
		std::string simple = "procedure re{while b{ if c then {call saul;} else {r=z+2;} }}procedure saul {l=o+l;}";
		std::vector<std::string> expected = { "procedure", "re","{","while","b","{","if","c","then","{","call","saul",";","}","else","{","r","=","z","+","2" ,";","}","}","}","procedure","saul","{" ,"l","=","o","+","l",";" };
		std::vector<std::string> results;
		sp = new SimpleParser();
		//Act
		std::vector<std::string> tokens2;
		results = sp->tokenize(simple);

		//Assert
		for (unsigned int i = 0; i < expected.size();i++) {
			Assert::AreEqual(expected[i], results[i]);
		}
	}

	// Valid Procedure
	TEST_METHOD(SimpleParserTest_ValidProgram_Procedure) {
		//Arrange
		bool invalid;
		std::string simple = "procedure a{}";
		std::vector<std::string> results;
		sp = new SimpleParser();

		//Act
		std::vector<std::string> tokens;
		results = sp->tokenize(simple);
		invalid = sp->parseSimple(results);

		//Assert
		// If invalid is true, then simple program is wrong.
		Assert::AreEqual(invalid, false);
	}

	// Valid Procedure with Assignment
	TEST_METHOD(SimpleParserTest_ValidProgram_Procedure_w_Assignment) {
		//Arrange
		bool invalid;
		std::string simple = "procedure a{x=b+c;}";
		std::vector<std::string> results;
		sp = new SimpleParser();

		//Act
		std::vector<std::string> tokens;
		results = sp->tokenize(simple);
		invalid = sp->parseSimple(results);

		//Assert
		// If invalid is true, then simple program is wrong.
		Assert::AreEqual(invalid, false);
	}

};