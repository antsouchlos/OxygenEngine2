#include <fstream>
#include <iostream>
#include <string>

#include "Parser.hpp"


int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cout << "Not enough arguments provided" << std::endl;
		return 1;
	}


	std::string content = "";
	std::ifstream f(argv[1]);

	std::string line;
	while(getline(f, line))
		content += line;

	f.close();

	Parser parser(content);

	try {
		CSL_Element el = parser.parse();
	} catch(LexerException &e) {
		std::cout << "LexerException occurred:\n\t" << e.what() << std::endl;
	} catch(ParserException &e) {
		std::cout << "ParserException occurred:\n\t" << e.what() << std::endl;
	}


	return 0;
}