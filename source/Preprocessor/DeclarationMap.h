#include <string>
#include <unordered_map>
using namespace std;

class DeclarationMap {
public:
	static DeclarationMap* Instance();

	std::unordered_map<std::string, std::string> getMap();
	void setMap(string statement);
	string find(string key);

private:	
	static DeclarationMap* m_pInstance;
	std::unordered_map<std::string, std::string> declarationMap;

	DeclarationMap();
	DeclarationMap(DeclarationMap const&) {};
	DeclarationMap& operator=(DeclarationMap const&) {};

	void setDeclaration(string line);
};

