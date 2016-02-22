#include "PKB.h"

class StubPKB : public PKB {
public:

	static PKB* getInstance();
	std::vector<std::string> PQLSelect(TNodeType outputType);
	std::vector<std::string> PQLUses(std::string input, int argumentPosition, std::string outputType);
	std::vector<std::string> PQLPattern(TNodeType type, Ref left, Ref right);
private:
	static PKB* instance;
	StubPKB();
	~StubPKB();
};