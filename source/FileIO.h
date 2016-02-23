/*! \class FileIO
\brief Used to read the contents of a textfile and returns a string.

This class is used to read the contents of a textfile given the file name. 
This class is a singleton for obvious reasons.
*/

#pragma once
#include "stdafx.h"

class FileIO {
private:

public:
	std::string get_file_contents(std::string);
};
