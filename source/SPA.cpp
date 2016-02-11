#include "SPA.h"

/*Constructor*/
SPA::SPA() {
	sp = new SimpleParser();
}

/*Functions*/
void SPA::processSimpleSourceFile(std::string sourceFile){
	sp->processSimple(sourceFile);
	//std::vector<std::string> r = sp->readSourceFile(sourceFile);
	//sp->execute(r);
	std::cin;
}
