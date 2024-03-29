/*! \class PQLController
\brief Used to process a query and return the result

This is a controller for PQL. It is used to process a query and return the result
*/
#ifndef PQLController_H
#define PQLController_H

#include <string>
#include <vector>

#include "Preprocessor.h"
#include "QueryEvaluator.h"
#include "Projector.h"

class PQLController {
public:
	//! Process query and return result
	/*!
	Main method to Process query and return result
	*/
	std::vector<std::string> PQLController::process(std::string input);
};
#endif