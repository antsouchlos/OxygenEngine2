/*
 * CSL_Exceptions.h
 *
 *  Created on: Sep 23, 2015
 *      Author: andreas
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <types/OE_Libs.h>

using namespace std;

class UnknownIDException : public exception {
public:
	UnknownIDException(string msg) {this->msg += msg; }

	virtual const char* what() const throw() { return msg.c_str(); }

	virtual ~UnknownIDException() throw() {}
private:
	string msg = "CSLInterpreterException: ";
};

class UnexpectedTypeException : public exception {
public:
	UnexpectedTypeException(string msg) { this->msg += msg; }

	virtual const char* what() const throw() { return msg.c_str(); }

	virtual ~UnexpectedTypeException() throw() {}
private:
	string msg = "CSLInterpreterException: ";
};

class UnknownTypeException : public exception {
public:
	UnknownTypeException(string msg) {this-> msg += msg; }

	virtual const char* what() const throw() {return msg.c_str(); }

	virtual ~UnknownTypeException() throw() {}
private:
	string msg = "CSLInterpreterException: ";
};

class UnexpectedTokenException : public exception {
public:
	UnexpectedTokenException(string msg) { this->msg += msg; }

	virtual const char* what() const throw() { return msg.c_str(); }

	virtual ~UnexpectedTokenException() throw() {}
private:
	string msg = "CSLParserException: ";
};

class UnexpectedCharacterException : public exception {
public:
	UnexpectedCharacterException(string msg) { this->msg += msg; }

	virtual const char* what() const throw() { return msg.c_str(); }

	virtual ~UnexpectedCharacterException() throw() {}
private:
	string msg = "CLexerException: ";
};

#endif /* EXCEPTIONS_H_ */
