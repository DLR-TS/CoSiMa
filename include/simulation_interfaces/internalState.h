#ifndef INTERNALSTATE_H
#define INTERNALSTATE_H

#include <vector>
#include <string>

struct integerValue {
	std::string basename;
	int value;
} ;


class internalState {
public:
	std::vector<int> integers;
	std::vector<float> floats;
	std::vector<double> doubles;
	std::vector<bool> bools;
	std::vector<std::string> strings;
};

#endif // !INTERNALSTATE_H
