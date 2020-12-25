/*
 * CSL_Exceptions.h
 *
 *  Created on: Sep 23, 2015
 *      Author: andreas
 */

#ifndef CSL_EXCEPTIONS_H_
#define CSL_EXCEPTIONS_H_

#include <types/OE_Libs.h>


// Defined here, because it is needed for UnexpectedSymbolError
// TODO: Find a better place to put this
namespace csl {

struct token {
	enum token_type {ident, string, integer, floatingPoint, openTagB, openClosingTagB,
							closeTagB, openListB, closeListB, eq, semicolon, comma, comment,
							slash, eos, undef};

	token_type type = undef;
	std::string_view content;
};

inline const char* getTokenTypeStringRep(token::token_type t) {
	switch(t) {
		case token::ident:
			return "Identifier";
			break;
		case token::string:
			return "String";
			break;
		case token::integer:
			return "Integer";
			break;
		case token::floatingPoint:
			return "Float";
			break;
		case token::openTagB:
			return "<";
			break;
		case token::closeTagB:
			return ">";
			break;
		case token::openClosingTagB:
			return "</";
			break;
		case token::openListB:
			return "{";
			break;
		case token::closeListB:
			return "}";
			break;
		case token::eq:
			return "=";
			break;
		case token::comma:
			return ",";
			break;
		case token::semicolon:
			return ";";
			break;
		case token::comment:
			return "Comment";
			break;
		case token::slash:
			return "/";
			break;
		case token::eos:
			return "EOS";
			break;
		case token::undef:
			return "Undefined Token";
			break;
		default:
			return "[Unknown Type]";
	}
}


class LexerError {
	public:
		virtual std::string what() const throw() = 0;
		virtual ~LexerError() = default;
};

class UnknownCharacterError : LexerError {
	public:
		UnknownCharacterError(char c, std::size_t lineNum, std::size_t colNum)
			: c_(c), lineNum_(lineNum), colNum_(colNum) {};

		std::string what() const throw () {
			std::stringstream result_ss;

			result_ss << "Unknown character '" << c_ << "' at line " << lineNum_ << ':' << colNum_;

			return result_ss.str();
		}
	private:
		const char c_;
		const std::size_t lineNum_;
		const std::size_t colNum_;
};

class InvalidInputError : LexerError {
	public:
		InvalidInputError(std::string msg) : msg_(msg) {};

		std::string what() const throw() {
			return msg_;
		}
	private:
		const std::string msg_;
};



// Sadly, ParseError cannon inherit from std::exception, since that would not allow the return type
// to be a string, which is necessary in this case in order to get a proper error message
// (Returning "result_ss.str().c_str()" doesn't work, since it returns a pointer to an object that
// gets destroyed a soon as the scope of "what()" is left, thereby basically returning garbage)
class ParserError {
	public:
		virtual std::string what() const throw() = 0;
		virtual ~ParserError() = default;
};

class UnexpectedSymbolError : ParserError {
	public:
		UnexpectedSymbolError(std::string_view unexpected, token::token_type expected,
							const std::size_t lineNum, const std::size_t colNum)
			: unexpected_(unexpected), expected_(expected), lineNum_(lineNum), colNum_(colNum) {};

		std::string what() const throw() {
			std::stringstream result_ss;
			result_ss << "Unexpetcted Symbol \"" << unexpected_ << "\" ";
		   	result_ss << "at line " << lineNum_ << ':' << (colNum_ - unexpected_.size()) << ": ";
			result_ss << "Expected \"";
			result_ss << getTokenTypeStringRep(expected_);
			result_ss << "\"";

			return result_ss.str().c_str();
		}
	private:
		std::string_view unexpected_;
		token::token_type expected_;
		const std::size_t lineNum_;
		const std::size_t colNum_;
};


class InterpreterError {
	public:
		virtual std::string what() const throw() = 0;
		virtual ~InterpreterError() = default;
};

// TODO: Find a way to get line and column number
template<const char *T>
class UnknownMemberError : InterpreterError {
	public:
		UnknownMemberError(std::string_view unknown) : unknown_(unknown) {};

		std::string what() const throw() {
			std::stringstream result_ss;

			result_ss << "Unknown member " << T << " \"" << unknown_ << "\"";

			return result_ss.str();
		}
	private:
		const std::string_view unknown_;
};

// TODO: Find a way to get line and column number
class SemanticError : ParserError, InterpreterError {
	public:
		SemanticError(const char* msg) : msg_(msg) {};

		std::string what() const throw() { return msg_; };
	private:
		const char* msg_;
};


inline extern char const CSL_IteratorElementString[] = "element";
inline extern char const CSL_IteratorAttributeString[] = "attribute";
inline extern char const CSL_IteratorVariableString[] = "variable";

using UnknownMemberElementError = UnknownMemberError<CSL_IteratorElementString>;
using UnknownMemberAttributeError = UnknownMemberError<CSL_IteratorAttributeString>;
using UnknownMemberVariableError = UnknownMemberError<CSL_IteratorVariableString>;

}

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
