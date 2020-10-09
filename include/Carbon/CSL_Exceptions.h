/*
 * CSL_Exceptions.h
 *
 *  Created on: Sep 23, 2015
 *      Author: andreas
 */

#ifndef CSL_EXCEPTIONS_H_
#define CSL_EXCEPTIONS_H_

#include <types/OE_Libs.h>


class CSL_UnknownIDException : public std::runtime_error {
public:
	CSL_UnknownIDException(std::string msg) : std::runtime_error(msg) {this->msg += msg; }

	virtual const char* what() const throw() { return msg.c_str(); }

	virtual ~CSL_UnknownIDException() throw() {}
private:
	std::string msg = "CSLInterpreterException: ";
};

class CSL_UnexpectedTypeException : public std::runtime_error {
public:
	CSL_UnexpectedTypeException(std::string msg): std::runtime_error(msg) { this->msg += msg; }

	virtual const char* what() const throw() { return msg.c_str(); }

	virtual ~CSL_UnexpectedTypeException() throw() {}
private:
	std::string msg = "CSLInterpreterException: ";
};

class CSL_UnknownTypeException : public std::runtime_error {
public:
	CSL_UnknownTypeException(std::string msg) : std::runtime_error(msg) {this-> msg += msg; }

	virtual const char* what() const throw() {return msg.c_str(); }

	virtual ~CSL_UnknownTypeException() throw() {}
private:
	std::string msg = "CSLInterpreterException: ";
};

class CSL_UnexpectedTokenException : public std::runtime_error {
public:
	CSL_UnexpectedTokenException(std::string msg) : std::runtime_error(msg) { this->msg += msg; }

	virtual const char* what() const throw() { return msg.c_str(); }

	virtual ~CSL_UnexpectedTokenException() throw() {}
private:
	std::string msg = "CSLParserException: ";
};

class CSL_UnexpectedCharacterException : public std::runtime_error {
public:
	CSL_UnexpectedCharacterException(std::string msg) : std::runtime_error(msg) { this->msg += msg; }

	virtual const char* what() const throw() { return msg.c_str(); }

	virtual ~CSL_UnexpectedCharacterException() throw() {}
private:
	std::string msg = "CLexerException: ";
};

#endif /* EXCEPTIONS_H_ */
