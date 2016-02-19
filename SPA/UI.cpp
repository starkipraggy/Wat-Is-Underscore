/*#include "../source/SPA.h"
using namespace std;

int main() {

	std::string simple;
	simple.append("Procedure Meow {\n");
	simple.append("a = b + c + d + e + f;\n");
	simple.append("while i{ \n");
	simple.append("a = b; \n");
	simple.append("b = c; \n");
					simple.append("c = d; \n");
						simple.append("d = a; \n");
							simple.append("while j{ \n");
								simple.append("i = i - 1; \n");
									simple.append("while k{ \n");
										simple.append("j = j - 1; } } }\n");
											simple.append("c = c + d * a; \n");
												simple.append("}");
	//string statement = "variable a,b , c, v,e; stmt f, g; select a such that follows(a, v) pattern a(_, _\"y\"_)";
	//string statement = "assign a, c; select a such that uses(a, c) pattern a(_,_\"abc\"_)";
	//string statement = "assign a, c; select a pattern c(_,_\"abc\"_) such that uses(a, c) ";
	string statement = "assign a, select a";

	SPA spa;

	spa.processSimpleSourceFile(simple);
	string output = spa.evaluate(statement);
	
	std::cout << output << std::endl;
	
}*/