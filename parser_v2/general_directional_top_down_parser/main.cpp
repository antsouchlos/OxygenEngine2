#include <iostream>
#include <fstream>

#include "Parser.h"


int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cerr << "Not enough arguments provided" << std::endl << std::endl;
		std::cout << "Usage: " << argv[0] << " [string]" << std::endl;

		return 1;
	}

	// Crap Lexer
	
	std::string inputString = std::string(argv[1]);
	std::vector<std::string> inputVector;

	std::string tempElement = "";

	for (int i = 0; i < inputString.size(); i++) {
		if (inputString[i] == ' ') {
			inputVector.push_back(tempElement);
			tempElement = "";
		} else {
			tempElement += inputString[i];
		}
	}
	
	inputVector.push_back(tempElement);

//	std::cout << "[DEBUG] inputVector: {";
//
//	for (auto &s : inputVector) {
//		std::cout << "\"" << s << "\", ";
//	}
//
//	std::cout << "\b\b}" << std::endl;

	// Recognize String

	Parser p;
	bool recognized = p.recognize(inputVector);

	std::cout << "Recognized: " << recognized << std::endl;

	return 0;
}
