/*! \file TypeDef.h
	\brief Contains a list of typedefs used in the implementation of the Program Knowledge Base (PKB).

	Contains a list of typedefs used in the implementation of the Program Knowledge Base (PKB). Allows flexible and easy changing 
	and modification of types and data structures as per changing system design requirements.
*/

#pragma once

#include <string>

/*! \var typedef std::string NAME
	\brief Used to represent names of variables.

	Used to represent names of variables. Consists of a letter, followed by any numbers of letters and/or
	digits. Ways to strictly enforce the name of a variable starting with a letter is manually required.
*/

typedef std::string NAME;

/*! \var typedef int INTEGER
	\brief Used to represent constant values used in factors.

	Used to represent constant values used in factors. Consists of one or more digits. Unsure if
	negative numbers are acceptable. It would not be if we were to follow it by the strict requirement
	of "INTEGER: DIGIT+", but for now let's just roll with signed integers. We can always simply
	change the integer used to unsigned any time anyway.
*/

typedef int INTEGER;

/*! \var typedef char OPERATOR
	\brief Used to represent '+', 'x', etc. in an expression involving another expression and a factor.

	Used to represent '+', 'x', etc. in an expression involving another expression and a factor. There
	may be a better implementation for the operation in an expression, but for now this should work for
	our intended purposes. Let me know if there is any ways better; I'm open to suggestions.
*/

typedef char OPERATOR;