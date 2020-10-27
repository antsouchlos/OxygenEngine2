#include <iostream>

#include "Lexer.h"

int main(int argc, char* argv[]) {

	Lexer lexer("main.cpp");

	char* charPtr = nullptr;

	for (int i = 0; i < 100; i++) {
		charPtr = lexer.getChar();
		//std::cout << "i: " << i << "; " << *charPtr << std::endl;
		std::cout << *charPtr;
		lexer.getNextChar();
	}

	std::cout << std::endl;


	return 0;
}
