/*! \class StringUlti
\brief Used additional utilility class of String

This this a utility class that consist of some method to manipulate string
*/
#include <string>
#include <vector>
#include <regex>
using namespace std;

//! Convert string to upper case
/*!
Convert every char in string to upper case
*/
string StringToUpper(string strToConvert);

//! trim the string
/*!
Remove all white space infront of the first char and behind the first char of the word
*/
string trim(string word);

//! tokenized the string into a vector of string
/*!
tokenized the string into a vector of string using the default " " condition
*/
std::vector<std::string> tokenize(string line);
//! tokenized the string into a vector of string
/*!
tokenized the string into a vector of string using the condition set by the regex 
input
*/
std::vector<std::string> tokenize(string line, string regex);

//! remove all space from the string
/*!
Remove all white space from the string
*/
string removeSpace(string word);