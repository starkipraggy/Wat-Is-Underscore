/*! \class Ref
\brief Used to represent each reference made in query

This class is used to represent each reference made in query. These reference are:
synonymn, integer, ident, placeholder, partOfExpr
*/

#ifndef Ref_H
#define Ref_H

#include <string>
#include <regex>
#include "StringUlti.h"
using namespace std;

class Ref {
public:
	Ref();
	Ref::Ref(string nam, string typ);

	//! Getter for Name
	/*!
	Return Name for object of this instance.
	*/
	string getName();

	//! Getter for Type
	/*!
	Return Type for object of this instance.
	*/
	string getType();

	//! Get string representation of Variable
	/*!
	Return the string representation of Clause in the format Name+Type
	*/
	string toString();

	//! Check equals to another Variable
	/*!
	Return true if Variable has the same name and type with another Variable
	*/
	bool equals(Ref v);
private:
	string name; /**< name of the reference */
	string type; /**< type of the reference */
};

#endif