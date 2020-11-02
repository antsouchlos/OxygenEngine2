#include <iostream>
#include <string.h>
#include <chrono>

#include "readspeed.h"


#define POINTER_ARG
#define COPY_STR


int countSpacesPointer(std::string filename) {
	int result = 0;

#ifdef POINTER_ARG
	FileWrapper fw(filename);
	const char *addr = fw.getAddr();
	const char *lineEnd = nullptr;

	while(fw.getNextLinePointer(lineEnd)) {
		while (addr != lineEnd) {
			if (*addr == ' ') ++result;
			++addr;
		} 
	}
#endif
#ifdef POINTER_RETURN
	FileWrapper fw(filename);
	const char *addr = fw.getAddr();
	const char *lineEnd = nullptr;

	while( (lineEnd = fw.getNextLinePointer()) != nullptr ) {
		while (addr != lineEnd) {
			if (*addr == ' ') ++result;
			++addr;
		} 
	}
#endif

	return result;	
} 

int countSpacesCopy(std::string filename) {
	int result = 0;

#ifdef COPY_CHAR
	FileWrapper fw(filename);
	
	char buffer[128];

	int len;

	while ( (len = fw.getNextLineCopy(buffer)) ) {
		for (int i = 0; i < len; i++) {
			if (buffer[i] == ' ') ++result;
		}
	}
#endif
#ifdef COPY_STR
	FileWrapper fw(filename);
	
	char buffer[128];

	int len;

	while ( (len = fw.getNextLineCopy(buffer)) ) {
		std::string buffer_str(buffer);
		for (int i = 0; i < len; i++) {
			if (buffer_str[i] == ' ') ++result;
		}
	}
#endif

	return result;
}

int main(int argc, char* argv[]) {
	if (argc < 3)
		throw LexerException("Not enough arguments provided");

	double runtime = 0;

	auto t_0 = std::chrono::high_resolution_clock::now();

	int spaces = 0;	

	if (strcmp(argv[1], "fstream") == 0) {
		spaces = countSpacesFstream(std::string(argv[2]));
	} else if (strcmp(argv[1], "mmap") == 0) {	
		spaces = countSpacesMmap(std::string(argv[2]));
	} else if (strcmp(argv[1], "fopen") == 0) {	
		spaces = countSpacesFopen(std::string(argv[2]));
	} else if (strcmp(argv[1], "copy") == 0) {
		spaces = countSpacesCopy(argv[2]);
	} else if (strcmp(argv[1], "pointer") == 0) {
		spaces = countSpacesPointer(argv[2]);
	} else {
		std::cout << "Unknown command '" << argv[1] << "'" << std::endl;

		return 1;
	}

	auto t_1 = std::chrono::high_resolution_clock::now();
	runtime = std::chrono::duration_cast<std::chrono::milliseconds>(t_1 - t_0).count();

	std::cout << "Number of spaces in file: " << spaces << std::endl;

	std::cout << "Runtime: " << runtime << " ms" << std::endl;

	return 0;
}
