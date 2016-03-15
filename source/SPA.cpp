#include "SPA.h"

/*Constructor*/
SPA::SPA() {
	sp = new SimpleParser();
	fio = new FileIO();
}

bool isProgramInvalid = false;

/*Functions*/
void SPA::processSimple(std::string filename) {
	contents = fio->FileIO::get_file_contents(filename);
	tokens = sp->tokenize(contents);
	isProgramInvalid = sp->parseSimple(tokens);

	if (isProgramInvalid == true) {
		std::cout << "Program is Invalid! " << std::endl;
	}
}

std::vector<std::string> SPA::evaluate(std::string query) {
	return pql.process(query);
}