#include "SPA.h"

/*Constructor*/
SPA::SPA() {
	sp = new SimpleParser();
	fio = new FileIO();
}

/*Functions*/
void SPA::processSimple(std::string filename) {
	contents = fio->FileIO::get_file_contents(filename);
	tokens = sp->tokenize(contents);
	sp->parseSimple(tokens);
}

string SPA::evaluate(std::string query) {
	return pql.process(query);
}