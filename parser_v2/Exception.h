#ifndef __EXCEPTION_H__
#define __EXCEPTION_H__

#include <stdexcept> 

class LexerException : public std::runtime_error {
	public:
		LexerException(std::string error) : runtime_error(error) {};
};

#endif
