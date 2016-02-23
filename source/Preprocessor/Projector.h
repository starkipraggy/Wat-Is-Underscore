/*! \class Projector
\brief Used to read return from QueryEvaluator and generate a string

This this the main component to read return from QueryEvaluator and generate a string
for output. It also generate output in the desired format
*/
#ifndef Projector_H
#define Projector_H

#include <string>
#include <vector>

class Projector {
public:
	//! Process raw result and generate string of output
	/*!
	Main method to Process raw result and generate string of output
	*/
	std::vector<std::string> process(std::vector<std::string> result);

private:
};
#endif