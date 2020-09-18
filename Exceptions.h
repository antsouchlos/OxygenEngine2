/*
 * Exceptions.h
 *
 *  Created on: Sep 23, 2015
 *      Author: andreas
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <exception>
#include <string>

using namespace std;

class UnexpectedTokenException : public exception {
public:
	UnexpectedTokenException(string msg) { this->msg += msg; }

	virtual const char* what() const throw() { return msg.c_str(); }

	virtual ~UnexpectedTokenException() throw() {}
private:
	string msg = "CLexerException: ";
};

class UnexpectedCharacterException : public exception {
public:
	UnexpectedCharacterException(string msg) { this->msg += msg; }

	virtual const char* what() const throw() { return msg.c_str(); }

	virtual ~UnexpectedCharacterException() throw() {}
private:
	string msg = "CScannerException: ";
};

#endif /* EXCEPTIONS_H_ */
