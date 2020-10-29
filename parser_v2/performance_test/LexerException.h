#ifndef __LEXEREXCEPTION_H__
#define __LEXEREXCEPTION_H__

#include <stdexcept>

class LexerException : public std::runtime_error {
	public:
		LexerException(std::string error) : runtime_error(error) {};
};

#endif
