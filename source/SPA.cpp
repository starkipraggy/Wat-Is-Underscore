#include "SPA.h"

/*Constructor*/
SPA::SPA() {
	sp = new SimpleParser();
}

/*Functions*/
void SPA::processSimpleSourceFile(std::string sourceFile) {
	sp->processSimple(sourceFile);
	sp->processLine();


	std::cin;
}