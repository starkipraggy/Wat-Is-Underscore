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
		results = sp->tokenize(simple);

		//Assert
		for (unsigned int i = 0; i < expected.size();i++) {
			Assert::AreEqual(expected[i], results[i]);
		}
	}
	
	// Tokenizes While and Assignments
	TEST_METHOD(SimpleParserTest_Tokenisation_Simple_2) {
		//Arrange
		std::string simple = "procedure xs{while b{x-4*z;}}";
		std::vector<std::string> expected = { "procedure", "xs","{","while","b","{","x","-","4","*","z",";" ,"}","}" };
		std::vector<std::string> results;
		sp = new SimpleParser();
		//Act
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
		results = sp->tokenize(simple);

		//Assert
		for (unsigned int i = 0; i < expected.size();i++) {
			Assert::AreEqual(expected[i], results[i]);
		}
	}

    // Tokenizes empty While
    TEST_METHOD(SimpleParserTest_Tokenization_Simple_7) {
        //Arrange
        std::string simple = "procedure ew{while zzz{}}";
        std::vector<std::string> expected = { "procedure", "ew", "{", "while", "zzz", "{", "}", "}" };
        std::vector<std::string> results;
        sp = new SimpleParser();
        //Act
        results = sp->tokenize(simple);

        //Assert
        for (unsigned int i = 0; i < expected.size(); i++) {
            Assert::AreEqual(expected[i], results[i]);
        }
    }

    // Tokenizes uneven spacing
    TEST_METHOD(SimpleParserTest_Tokenisation_Simple_5) {
        //Arrange
        std::string sample = "{laXadasiCal =LaZy+ carEleSs -enThuSiasm + typo ;}";
        std::string expected = " { laXadasiCal  = LaZy +  carEleSs  - enThuSiasm  +  typo  ;  } ";

        //Act
        std::string result = sp->addSpaceToString(sample);

        //Assert
        Assert::AreEqual(expected, result);
    }

    // Tokenizes uneven spacing 2
    TEST_METHOD(SimpleParserTest_Tokenisation_Simple_6) {
        //Arrange
        std::string sample = "{a     =       b     *            x         -c+d;}";
        std::string expected = " { a      =        b      *             x          - c + d ;  } ";

        //Act
        std::string result = sp->addSpaceToString(sample);

        //Assert
        Assert::AreEqual(expected, result);
    }

    // Invalid Procedure (missing open brace)
    TEST_METHOD(SimpleParserTest_InvalidProgram_01) {
        //Arrange
        bool invalid;
        std::string simple = "procedure b}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    // Invalid Procedure (missing closing brace)
    TEST_METHOD(SimpleParserTest_InvalidProgram_02) {
        //Arrange
        bool invalid;
        std::string simple = "procedure c{";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    // Invalid Procedure (non-digit/letter for name)
    TEST_METHOD(SimpleParserTest_InvalidProgram_03) {
        //Arrange
        bool invalid;
        std::string simple = "procedure ___{}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    // Invalid Procedure (empty stmtLst)
    TEST_METHOD(SimpleParserTest_InvalidProgram_04) {
        //Arrange
        bool invalid;
        std::string simple = "procedure p{}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    // Invalid Program (Contains only while loop)
    TEST_METHOD(SimpleParserTest_InvalidProgram_Only_While) {
        //Arrange
        bool invalid;
        std::string simple = "while i{x=a+b;}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

	// Invalid Program (Contains only if block)
	TEST_METHOD(SimpleParserTest_InvalidProgram_Only_If) {
		//Arrange
		bool invalid;
		std::string simple = "if i then {x=a+b;}";
		std::vector<std::string> results;
		sp = new SimpleParser();

		//Act
		results = sp->tokenize(simple);
		invalid = sp->parseSimple(results);

		//Assert
		// If invalid is true, then simple program is wrong.
		Assert::AreEqual(invalid, true);
	}

	// Invalid Program (Contains only else block)
    TEST_METHOD(SimpleParserTest_InvalidProgram_Only_Else) {
        //Arrange
        bool invalid;
		std::string simple = "else { a=b+c;}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    // Invalid While (missing ending braces)
    TEST_METHOD(SimpleParserTest_InvalidProgram_05) {
        //Arrange
		bool invalid = false;
		std::string simple = "procedure a{while i{a=d;}";
		std::vector<std::string> results;
		sp = new SimpleParser();

		//Act
		results = sp->tokenize(simple);
		invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    // Invalid Procedure with Double Equality in Assignment
    TEST_METHOD(SimpleParserTest_InvalidProgram_06) {
        //Arrange
        bool invalid = false;
        std::string simple = "procedure q{a==b;}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    // Invalid Procedure with Double + Operators in Assignment
    TEST_METHOD(SimpleParserTest_InvalidProgram_07) {
        //Arrange
        bool invalid = false;
        std::string simple = "procedure p{j=a++8;}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    // Invalid Procedure with Double - Operators in Assignment
    TEST_METHOD(SimpleParserTest_InvalidProgram_08) {
        //Arrange
        bool invalid = false;
        std::string simple = "procedure z{p=k--2;}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    //Invalid Procedure with Double * Operators in Assignment
    TEST_METHOD(SimpleParserTest_InvalidProgram_09) {
        //Arrange
        bool invalid = false;
        std::string simple = "procedure k{m=y**8;}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    //Invalid Procedure with Double ;
    TEST_METHOD(SimpleParserTest_InvalidProgram_10) {
        //Arrange
        bool invalid = false;
        std::string simple = "procedure k{m=y+8;;}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    //Invalid Procedure with Spacing in Variable Name
    TEST_METHOD(SimpleParserTest_InvalidProgram_11) {
        //Arrange
        bool invalid = false;
        std::string simple = "procedure juliet{rom eo=handsome+lovely;}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    //Invalid Procedure with Spacing in Procedure Name
    TEST_METHOD(SimpleParserTest_InvalidProgram_12) {
        //Arrange
        bool invalid = false;
        std::string simple = "procedure jul iet{romeo=handsome+lovely;}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    //Invalid Procedure with If statement without Else
    TEST_METHOD(SimpleParserTest_InvalidProgram_13) {
        //Arrange
        bool invalid = false;
        std::string simple = "procedure p{if i then {me=win;}}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }
    
    //Invalid Procedure with If statement without If
    TEST_METHOD(SimpleParserTest_InvalidProgram_14) {
        //Arrange
        bool invalid = false;
        std::string simple = "procedure p{else{me=win;}}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    //Invalid Procedure with Invalid Procedure Name starting with Digit
    TEST_METHOD(SimpleParserTest_InvalidProgram_15) {
        //Arrange
        bool invalid = false;
        std::string simple = "procedure 1337procname{a=b}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    // Invalid Procedure with Empty While
    TEST_METHOD(SimpleParserTest_InvalidProgram_16) {
        //Arrange
        bool invalid = false;
        std::string simple = "procedure b{while i{}}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    // Invalid Procedure with Empty If
    TEST_METHOD(SimpleParserTest_InvalidProgram_17) {
        //Arrange
        bool invalid = false;
        std::string simple = "procedure q{if j then {}else {a=1;}}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

    // Invalid Procedure with Empty Else
    TEST_METHOD(SimpleParserTest_InvalidProgram_18) {
        //Arrange
        bool invalid = false;
        std::string simple = "procedure q{if j then {a=b;}else {}}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, true);
    }

	// Invalid Procedure with Duplicate procedure
	TEST_METHOD(SimpleParserTest_InvalidProgram_19) {
		//Arrange
		bool invalid = false;
		std::string simple = "procedure q{x=s;} procedure q{l=p;}";
		std::vector<std::string> results;
		sp = new SimpleParser();

		//Act
		results = sp->tokenize(simple);
		invalid = sp->parseSimple(results);

		//Assert
		// If invalid is true, then simple program is wrong.
		Assert::AreEqual(invalid, true);
	}

	// Valid Procedure with invalid call statement.
	TEST_METHOD(SimpleParserTest_InvalidProgram_20) {
		//Arrange
		bool invalid = false;
		std::string simple = "procedure q{x=s; call b;} ";
		std::vector<std::string> results;
		sp = new SimpleParser();

		//Act
		results = sp->tokenize(simple);
		invalid = sp->parseSimple(results);

		//Assert
		// If invalid is true, then simple program is wrong.
		Assert::AreEqual(invalid, true);
	}

	// Invalid Procedure with cyclic calls.
	TEST_METHOD(SimpleParserTest_InvalidProgram_21) {
		//Arrange
		bool invalid = false;
		std::string simple = "procedure q{call a;} procedure a{call q;}";
		std::vector<std::string> results;
		sp = new SimpleParser();

		//Act
		results = sp->tokenize(simple);
		invalid = sp->parseSimple(results);

		//Assert
		// If invalid is true, then simple program is wrong.
		Assert::AreEqual(invalid, true);
	}

	// Invalid Procedure with cyclic calls.
	TEST_METHOD(SimpleParserTest_InvalidProgram_22) {
		//Arrange
		bool invalid = false;
		std::string simple = "procedure a{call b;} procedure b{call c;} procedure c{call a;}";
		std::vector<std::string> results;
		sp = new SimpleParser();

		//Act
		results = sp->tokenize(simple);
		invalid = sp->parseSimple(results);

		//Assert
		// If invalid is true, then simple program is wrong.
		Assert::AreEqual(invalid, true);
	}

    // Valid Procedure with one Valid Nested While with Assignment in If 
    TEST_METHOD(SimpleParserTest_ValidProgram_01) {
        //Arrange
        bool invalid = false;
        std::string simple = "procedure p{if validVarHere then {while validVarAgain{a=a+1;}} else{a=a-1;}}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, false);
    }

    // Valid Procedure with one Valid Nested If with Assignment in While 
    TEST_METHOD(SimpleParserTest_ValidProgram_02) {
        //Arrange
        bool invalid = false;
        std::string simple = "procedure p{while validVarHere{if j then {a=b;}else {a=c;}}}";
        std::vector<std::string> results;
        sp = new SimpleParser();

        //Act
        results = sp->tokenize(simple);
        invalid = sp->parseSimple(results);

        //Assert
        // If invalid is true, then simple program is wrong.
        Assert::AreEqual(invalid, false);
    }

	TEST_METHOD(SimpleParserTest_ValidProgram_03) {
		//Arrange
		bool invalid = false;
		std::string simple = "procedure p{call b;} procedure b{z=v;}";
		std::vector<std::string> results;
		sp = new SimpleParser();

		//Act
		results = sp->tokenize(simple);
		invalid = sp->parseSimple(results);

		//Assert
		// If invalid is true, then simple program is wrong.
		Assert::AreEqual(invalid, false);
	}

	// Valid Procedure with Assignment (small P)
	TEST_METHOD(SimpleParserTest_ValidProgram_04) {
		//Arrange
		bool invalid = false;
		std::string simple = "procedure p{x=b+c;}";
		std::vector<std::string> results;
		sp = new SimpleParser();

		//Act
		results = sp->tokenize(simple);
		invalid = sp->parseSimple(results);

		//Assert
		// If invalid is true, then simple program is wrong.
		Assert::AreEqual(invalid, false);
	}

	// Valid Procedure with Assignment (capital P)
	TEST_METHOD(SimpleParserTest_ValidProgram_05) {
		//Arrange
		bool invalid = false;
		std::string simple = "Procedure p{x=b+c;}";
		std::vector<std::string> results;
		sp = new SimpleParser();

		//Act
		results = sp->tokenize(simple);
		invalid = sp->parseSimple(results);

		//Assert
		// If invalid is true, then simple program is wrong.
		Assert::AreEqual(invalid, false);
	}

	// Valid Procedure with While (Small W)
	TEST_METHOD(SimpleParserTest_ValidProgram_06) {
		//Arrange
		bool invalid = false;
		std::string simple = "procedure b{while i{x=b+c;}}";
		std::vector<std::string> results;
		sp = new SimpleParser();

		//Act
		results = sp->tokenize(simple);
		invalid = sp->parseSimple(results);

		//Assert
		// If invalid is true, then simple program is wrong.
		Assert::AreEqual(invalid, false);
	}

	// Valid Procedure with While (Capital W)
	TEST_METHOD(SimpleParserTest_ValidProgram_07) {
		//Arrange
		bool invalid = false;
		std::string simple = "procedure b{While i{x=b+c;}}";
		std::vector<std::string> results;
		sp = new SimpleParser();

		//Act
		results = sp->tokenize(simple);
		invalid = sp->parseSimple(results);

		//Assert
		// If invalid is true, then simple program is wrong.
		Assert::AreEqual(invalid, false);
	}

	// Valid Procedure with If (small I/T/E)
	TEST_METHOD(SimpleParserTest_ValidProgram_08) {
		//Arrange
		bool invalid = false;
		std::string simple = "procedure p{if i then {p=e;}else {l=b;}}";
		std::vector<std::string> results;
		sp = new SimpleParser();

		//Act
		results = sp->tokenize(simple);
		invalid = sp->parseSimple(results);

		//Assert
		// If invalid is true, then simple program is wrong.
		Assert::AreEqual(invalid, false);
	}

	// Valid Procedure with If (capital I/T/E)
	TEST_METHOD(SimpleParserTest_ValidProgram_09) {
		//Arrange
		bool invalid = false;
		std::string simple = "procedure p{If i Then {p=e;}Else {l=b;}}";
		std::vector<std::string> results;
		sp = new SimpleParser();

		//Act
		results = sp->tokenize(simple);
		invalid = sp->parseSimple(results);

		//Assert
		// If invalid is true, then simple program is wrong.
		Assert::AreEqual(invalid, false);
	}

	// Valid Procedure with 2 Procedures
	TEST_METHOD(SimpleParserTest_ValidProgram_10) {
		//Arrange
		bool invalid = false;
		std::string simple = "procedure p{a=b;} procedure q{c=d;}";
		std::vector<std::string> results;
		sp = new SimpleParser();

		//Act
		results = sp->tokenize(simple);
		invalid = sp->parseSimple(results);

		//Assert
		// If invalid is true, then simple program is wrong.
		Assert::AreEqual(invalid, false);
	}

	// Valid Procedure with 2 Procedures, 2nd procedure is empty
	TEST_METHOD(SimpleParserTest_ValidProgram_11) {
		//Arrange
		bool invalid = false;
		std::string simple = "procedure p{a=b;} procedure q{}";
		std::vector<std::string> results;
		sp = new SimpleParser();

		//Act
		results = sp->tokenize(simple);
		invalid = sp->parseSimple(results);

		//Assert
		// If invalid is true, then simple program is wrong.
		Assert::AreEqual(invalid, true);
	}
};