#include "DataStructureObject.h"

template<typename T>
bool DataStructureObject::addIntoVector(T item, std::vector<T>* vector) {
	unsigned int size = vector->size();
	for (unsigned int i = 0; i < size; i++) {
		if (item == vector->at(i)) {
			return false;
		}
	}
	vector->push_back(item);
	return true;
}

int DataStructureObject::getIndex() {
	return index;
}