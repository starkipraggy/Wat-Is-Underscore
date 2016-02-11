#include "FileIO.h"

/*
 * Reads SIMPLE program file
 * Returns a string "contents" containing the contents of the program file
 */
std::string FileIO::get_file_contents(std::string str) {
	
	const char * filename = str.c_str();
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.reserve(in.tellg());
		in.seekg(0, std::ios::beg);
		std::copy((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>(), std::back_inserter(contents));
		in.close();
		return(contents);
	}
	throw(errno);
}
