#include "Lexer.h"

Lexer::Lexer(std::string filename) {
	int fd = open(filename.c_str(), O_RDONLY);

	if (fd == -1)
		throw LexerException("Error opening file '" + filename + "'");

	// Determine file size
	struct stat sb;
	if (fstat(fd, &sb) == -1)
		throw LexerException("Error determining file size");

	file_length = sb.st_size;

	addr = static_cast<const char*>(mmap(NULL, file_length, PROT_READ, MAP_PRIVATE, fd, 0u));
	if (addr == MAP_FAILED)
		throw LexerException("Failed to map file to memory");

	charPtr = const_cast<char*>(addr);
}

Lexer::~Lexer() {
	munmap(const_cast<char*>(addr), file_length);
}

inline bool Lexer::isWhitespace() {
	/* -------- Whitespace chars --------
	 *
	 * 9	001001
	 * 10	001010
	 * 13	001101
	 * 32	100000
	 *
	 */


	char c = *charPtr;

	if (c == (char) 10 || c == (char) 13) { 			// Character is breakline
		return true;	
	} else if (c  == (char) 9 || c == (char) 32) {		// Character is other whitespace
		return true;
	}


/*
	// TODO: Test these implementations to see if there is a significant performance difference


	// Implementation 1
	if (*charPtr == (char) 9 || *charPtr == (char) 10 ||*charPtr == (char) 13
			|| *charPtr == (char) 32)
		return true;


	// Implementation 2
	char c = *charPtr;

	if (c == (char) 9 || c == (char) 10 ||c  == (char) 13
			|| c == (char) 32)
		return true;


	// Implementation 3 (TODO: Order conditions according to frequency of characters in file)
	if (*charPtr == (char) 13 || *charPtr == (char) 9 ||*charPtr == (char) 10
			|| *charPtr == (char) 32)
		return true;
*/
	return false;
	// TODO: Check if there is a more efficient implementation, looking at the bits
}

void Lexer::getNextChar() {
	charPtr++;
	while(isWhitespace()) charPtr++;
}

char* Lexer::getChar() {
	return charPtr;
}
