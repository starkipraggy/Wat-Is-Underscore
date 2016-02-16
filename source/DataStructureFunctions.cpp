#include "DataStructureFunctions.h"

bool addIntoVector(int item, std::vector<int>* vector) {
	unsigned int size = vector->size();
	for (unsigned int i = 0; i < size; i++) {
		if (item == vector->at(i)) {
			return false;
		}
	}
	vector->push_back(item);
	return true;
}