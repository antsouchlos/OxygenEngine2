#include <iostream>
#include <fstream>
#include <string.h>
#include <chrono>

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "LexerException.h"


#define FOPEN_MALLOC_MEMCHR
#define FSTREAM_IF

int countSpacesMmap(std::string filename) {
	int result = 0;

	int fd = open(filename.c_str(), O_RDONLY);

	if (fd == -1)
		throw LexerException("Error opening file '" + filename + "'");

	// Determine file size
	struct stat sb;
	if (fstat(fd, &sb) == -1)
		throw LexerException("Error determining file size");

	size_t file_length = sb.st_size;

	// Map file to memory
	const char* addr = static_cast<const char*>(mmap(NULL, file_length, PROT_READ, MAP_PRIVATE, fd, 0u));
	if (addr == MAP_FAILED)
		throw LexerException("Failed to map file to memory");

	close(fd);

	// Count whitespace
	const char* charPtr = addr;
	const char* charMax = addr + file_length;

	while(charPtr/* && charPtr!=1*/)
		if ((charPtr = static_cast<const char*>(memchr(charPtr, ' ', charMax-charPtr)))) {
			++result; ++charPtr;
		}
	
	while (false);


	munmap(const_cast<char*>(addr), file_length);

	return result;
}

int countSpacesFstream(std::string filename) {
	int result = 0;

#ifdef FSTREAM_IF
	std::string line;

	std::ifstream fin(filename);

	while(getline(fin, line)) {
		for (int i = 0; i < line.size(); i++) {
			if (line[i] == ' ') ++result;
		}
	}

	fin.close();
#endif
#ifdef FSTREAM_COUNT
	std::string line;

	std::ifstream fin(filename);

	while(getline(fin, line)) {
		result += std::count(line.begin(), line.end(), ' ');	
	}

	fin.close();
#endif

	return result;
}

int countSpacesFopen(std::string filename) {
	int result = 0;

	FILE* f = fopen(filename.c_str(), "r");

	if (f == NULL)
		throw LexerException("Failed to open file '" + filename + "'");

	// Determine the size of the file
	fseek(f, 0, SEEK_END);
	unsigned long int fileSize = ftell(f);
	fseek(f, 0, SEEK_SET);

#ifdef FOPEN_MALLOC_MEMCHR
 	char* addr = static_cast<char*>(malloc(fileSize));

	fread(addr, 1, fileSize, f);

	char* charPtr = addr;
	char* charMax = charPtr + fileSize;

	while(charPtr)
		if ( (charPtr = static_cast<char*>(memchr(charPtr, ' ', charMax - charPtr))) ) {
		   	++charPtr;
			++result;
		}


	free(addr);
#endif
#ifdef FOPEN_MALLOC_IF
	char* addr = static_cast<char*>(malloc(fileSize));

	fread(addr, 1, fileSize, f);

	for (int i = 0; i < fileSize; i++) {
		if (addr[i] == ' ') ++result;
	}

	free(addr);
#endif
#ifdef FOPEN_FOR_MEMCHR
	char addr[128];

 	for (int i = 0; i < fileSize / 128; i++) {
		fread(addr, 1, 128, f);

		char* charPtr = addr;
		char* charMax = charPtr + 128;

		while(charPtr)
			if ( (charPtr = static_cast<char*>(memchr(charPtr, ' ', 128))) ) {
				++charPtr;
				++result;
			}
	}
#endif
#ifdef FOPEN_FOR_IF
	char addr[128];

 	for (int i = 0; i < fileSize / 128; i++) {
		fread(addr, 1, 128, f);

		for (int i = 0; i < 128; i++) {
			if (addr[i] == ' ') ++result;
		}
	}
#endif

	fclose(f);

	return result;
}

int main(int argc, char* argv[]) {
	if (argc < 3)
		throw LexerException("Not enough arguments provided");

	int spaces = 0;	
	double runtime = 0;

	auto t_0 = std::chrono::high_resolution_clock::now();

	if (strcmp(argv[1], "fstream") == 0) {
		spaces = countSpacesFstream(std::string(argv[2]));
	} else if (strcmp(argv[1], "mmap") == 0) {	
		spaces = countSpacesMmap(std::string(argv[2]));
	} else if (strcmp(argv[1], "fopen") == 0) {	
		spaces = countSpacesFopen(std::string(argv[2]));
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
