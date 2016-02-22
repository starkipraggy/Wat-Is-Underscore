#include "PKB.h"

class StubPKB : public PKB {
public:

	static PKB* getInstance();
	std::vector<std::string> PQLSelect(TNodeType outputType);

private:
	static PKB* instance;
	StubPKB();
	~StubPKB();
};