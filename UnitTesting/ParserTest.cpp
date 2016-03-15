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
		std::string simple = "procedure xs{while b{x+4+z;}}";
		std::vector<std::string> expected = { "procedure", "xs","{","while","b","{","x","+","4","+","z",";" ,"}","}" };
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

    // Tokenizes empty While
    TEST_METHOD(SimpleParserTest_Tokenization_Simple_5) {
        //Arrange
        std::string simple = "procedure ew{while zzz{}}";
        std::vector<std::string> expected = { "procedure", "ew", "{", "while", "zzz", "{", "}", "}" };
        std::vector<std::string> results;
        sp = new SimpleParser();
        //Act
        std::vector<std::string> tokens2;
        results = sp->tokenize(simple);

        //Assert
        for (unsigned int i = 0; i < expected.size(); i++) {
            Assert::AreEqual(expected[i], results[i]);
        }
    }

    // Tokenizes uneven spacing
    TEST_METHOD(SimpleParserTest_Tokenisation_Simple_6) {
        //Arrange
        std::string sample = "{laXadasiCal =LaZy+ carEleSs -enThuSiasm + typo ;}";
        std::string expected = " { laXadasiCal = LaZy + carEleSs - enThuSiasm + typo ;  } ";

        //Act
        std::string result = sp->addSpaceToString(sample);

        //Assert
        Assert::AreEqual(expected, result);
    }

    // Tokenizes uneven spacing 2
    TEST_METHOD(SimpleParserTest_Tokenisation_Simple_7) {
        //Arrange
        std::string sample = "{a     =       b     *            x         -c+d;}";
        std::string expected = " { a = b * x - c + d ;  } ";

        //Act
        std::string result = sp->addSpaceToString(sample);

        //Assert
        Assert::AreEqual(expected, result);
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

    // Invalid Procedure (missing open brace)
    TEST_METHOD(SimpleParserTest_InvalidProgram_Procedure) {
        //Arrange
        bool invalid;
        std::string simple = "procedure b}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        std::vector<std::string> tokens;
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    // Invalid Procedure (mising close brace)
    TEST_METHOD(SimpleParserTest_InvalidProgram_Procedure_2) {
        //Arrange
        bool invalid;
        std::string simple = "procedure c{";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        std::vector<std::string> tokens;
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    // Invalid Procedure (non-digit/letter for name)
    TEST_METHOD(SimpleParserTest_InvalidProgram_Procedure_3) {
        //Arrange
        bool invalid;
        std::string simple = "procedure ___{}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        std::vector<std::string> tokens;
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    // Invalid Procedure (empty stmtLst)
    TEST_METHOD(SimpleParserTest_InvalidProgram_Procedure_4) {
        //Arrange
        bool invalid;
        std::string simple = "procedure p{}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        std::vector<std::string> tokens;
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    // Invalid Procedure (capital P)
    TEST_METHOD(SimpleParserTest_InvalidProgram_Procedure_5) {
        //Arrange
        bool invalid;
        std::string simple = "Procedure p{x=b+c;}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        std::vector<std::string> tokens;
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    // Valid While
    TEST_METHOD(SimpleParserTest_ValidWhile) {
        //Arrange
        bool invalid;
        std::string simple = "while i{x=a+b;}";
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

    // Invalid While (empty)
    TEST_METHOD(SimpleParserTest_InvalidWhile) {
        //Arrange
        bool invalid;
        std::string simple = "while i{}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        std::vector<std::string> tokens;
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    // Invalid While (missing opening braces)
    TEST_METHOD(SimpleParserTest_InvalidWhile_2) {
        //Arrange
        bool invalid;
        std::string simple = "while i}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        std::vector<std::string> tokens;
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    // Invalid While (missing ending braces)
    TEST_METHOD(SimpleParserTest_InvalidWhile_3) {
        //Arrange
        bool invalid;
        std::string simple = "while i{";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        std::vector<std::string> tokens;
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    // Invalid While (Capital W)
    TEST_METHOD(SimpleParserTest_InvalidWhile_4) {
        //Arrange
        bool invalid;
        std::string simple = "While i{x=b+c;}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        std::vector<std::string> tokens;
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    // Valid If
    TEST_METHOD(SimpleParserTest_ValidIf) {
        //Arrange
        bool invalid;
        std::string simple = "if i then {";
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

    // Invalid If (empty)
    TEST_METHOD(SimpleParserTest_InvalidIf) {
        //Arrange
        bool invalid;
        std::string simple = "if i then {}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        std::vector<std::string> tokens;
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    // Invalid If (missing opening brace)
    TEST_METHOD(SimpleParserTest_InvalidIf_2) {
        //Arrange
        bool invalid;
        std::string simple = "if i then";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        std::vector<std::string> tokens;
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

};