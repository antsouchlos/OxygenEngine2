#include <OE_API.h>
#include<Carbon/CSL_Lexer.h>


int main(){
	/*
	std::string input = "";

	csl::Lexer l(input);
	csl::token t;

	csl::lexer<std::string> l(input);

	while(l.next(t)) {
		if (t.type == csl::token::floating_point) {
		
		} else if (t.type == csl::token::integer) {
		
		} else if (t.type == csl::token::string) {
		
		} else {
		
		}
	}
*/

	std::string input = "";
	std::ifstream f("csl_very_large_object_test.csl");

	std::string line;
	while(getline(f, line))
		input += line;

	f.close();

	csl::Lexer lexer(input);

	for (auto t : lexer) {
		std::cout << static_cast<int>(t.token_type) << ": " << t.content << std::endl;
	}


    return 0;
}
