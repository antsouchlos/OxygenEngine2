#include <iostream>
#include <string.h>
#include <chrono>

#include "readspeed.h"


#define REP_COUNT 500000000

#define READSPEED


inline bool inlineFunction(int i) {
	return i%2;
}

bool standardFunction(int i) {
	return i%2;
}

int main(int argc, char* argv[]) {
	if (argc < 3)
		throw LexerException("Not enough arguments provided");

	double runtime = 0;

	auto t_0 = std::chrono::high_resolution_clock::now();

#ifdef READSPEED
	int spaces = 0;	

	if (strcmp(argv[1], "fstream") == 0) {
		spaces = countSpacesFstream(std::string(argv[2]));
	} else if (strcmp(argv[1], "mmap") == 0) {	
		spaces = countSpacesMmap(std::string(argv[2]));
	} else if (strcmp(argv[1], "fopen") == 0) {	
		spaces = countSpacesFopen(std::string(argv[2]));
	}
#endif
#ifdef MAIN
	// Nothing else to test yet	
#endif
	else {
		std::cout << "Unknown command '" << argv[1] << "'" << std::endl;

		return 1;
	}

	auto t_1 = std::chrono::high_resolution_clock::now();
	runtime = std::chrono::duration_cast<std::chrono::milliseconds>(t_1 - t_0).count();

#ifdef READSPEED
	std::cout << "Number of spaces in file: " << spaces << std::endl;
#endif
#ifdef MAIN
	// Nothing else to test yet
#endif

	std::cout << "Runtime: " << runtime << " ms" << std::endl;

	return 0;
}
