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
 * 3. Line number on error: Don't continuously increment the line counter. Instead, look for the
 * line and column the character Pointer points to, once an error occurs.
 * 
 * 4. Look for an efficient binary compare function. With this, double-character-operators could
 * be checked for, without the need for backtracking. Also: find patterns in the ascii values
 * of the strings to be compared, to make things more efficient (In order to check whether a
 * character is a number, it doesn't have to be checked against every number, only against the
 * first and the last one on the ascii table).
 *
 * 5. User smart pointers in order to minimize memory leaks (Conduct a bit of research into how to 
 * use smart pointers with c-style functions like memmap. Maybe a wrapper is needed, but how can it
 * be implemented the most efficiently? Can the buffer idea from 1. be implemented in the wrapper
 * for memmap? Is a new class as a wrapper even a good idea? (The characters are read one by one.
 * If a function has to be called and an argument passed for every character, that would be an
 * incredibly large amount of function calls and memory copied))
 *
 * -------- IMPLEMENTATION PLAN --------
 *
 * 1. Probably don't use a wrapper for memmap, but make use of smart pointers nonetheless.
 *
 * 2. Think about exactly what data the Lexer will pass on to the Parser, and the Parser
 * on to the Interpreter. The data probably has to be copied a few times, but let's
 * try to minimze this.
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
