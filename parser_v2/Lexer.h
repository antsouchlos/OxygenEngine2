#ifndef __LEXER_H__
#define __LEXER_H__


/* -------- OPTIMIZATION IDEAS --------
 *
 * 1. Always accessing memory is probably pretty time consuming. Try loading a bunch of the file
 * contents into the CPU cache, by copying them a few Bytes / kBytes at a time into a string
 * or character array.
 *
 * 2. Rethink the whole whitespace-skipping idea: Use memchr in order to very efficiently find
 * a pointer to the next whitespace character. The memory between the previous pointer and the
 * new one is effectively the new token (Think about 1. here as well - accessing the heap for every
 * token may be very time consuming).
 *
 * 3. Line number on error: Don't continuously increment the line counter. Instead, look for the line
 * and column the character Pointer points to, once an error occurs.
 * 
 * 4. Look for an efficient binary compare function. With this, double-character-operators could
 * be checked for, without the need for backtracking. Also: find patterns in the ascii values
 * of the strings to be compared, to make things more efficient (In order to check whether a
 * character is a number, it doesn't have to be checked against every number, only against the
 * first and the last one on the ascii table).
 *
 */





#include <string>

// Only for debugging purposes
#include <iostream>

// For memory-mapped files
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "Exception.h"

class Lexer {
	public:
		Lexer(std::string filename);
		~Lexer();

		void getNextChar();

		char* getChar(); // DEBUG
	private:
		size_t file_length = -1;

		const char* addr = nullptr;
		char *charPtr = nullptr;

		inline bool isWhitespace();

//		void getNextChar();
};

#endif
