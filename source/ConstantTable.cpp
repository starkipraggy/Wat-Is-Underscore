#include "ConstantTable.h"

ConstantTable::ConstantTable() {
	constants = new std::vector<int>();
}

ConstantTable::~ConstantTable() {
	delete constants;
}

bool ConstantTable::addConstant(int constant) {
	unsigned int size = getNumberOfConstants();
	for (unsigned int i = 0; i < size; i++) {
		if (getConstant(i) == constant) {
			return false;
		}
	}
	constants->push_back(constant);
	return true;
}

unsigned int ConstantTable::getNumberOfConstants() {
	return constants->size();
}

int ConstantTable::getConstant(int index) {
	return constants->at(index);
}