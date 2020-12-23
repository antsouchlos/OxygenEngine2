#ifndef LEXER_HPP
#define LEXER_HPP

#include <sstream>
#include <string_view>
#include <array>
#include <algorithm>
#include <stdexcept>


enum class TokenType {ident, string, integer, floatingPoint, openTagB, openClosingTagB, closeTagB,
						openListB, closeListB, eq, semicolon, comma, comment, slash, eos, undef};

struct Token {
	TokenType type = TokenType::undef;
	std::string_view content;

//	TokenType type = TokenType::ident;	// TODO: Check performance improvement, if type is
//											initialized to identifier, and not set in the
//											identifier() function
};

class LexerError {
	public:
		virtual std::string what() const throw() { return ""; };
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

const char* getTokenTypeStringRep(TokenType t);


/*
 *
 * identifier				= identifierhead
 *
 * identifierHead			= [a-zA-Z_]
 * 							| [a-zA-Z_] identifierTail
 *
 * identifierTail			= [a-zA-Z_1-9]
 * 							| [a-zA-Z_1-9] identifierTail
 *
 * string					= "\"" * "\""
 * numberHead				= "-" numberTail
 * 							| numberTail
 * numberTail				= [1-9]+
 * 							| [1-9]+ . [1-9]*
 *
 * genericTagBracket		= "<" openCloseTagBracketTail
 * 							|
 * openCloseTagBracketTail	= "/"
 *
 * closeTagBracket			= ">"
 * openClosingTagBracket	= "</"
 * eq						= "="
 * semicolon				= ";"
 * comma					= ","
 * openListBracket			= "{"
 * closeListBracket			= "}"
 * commentStartSlash		= "/" commentStart
 * 							| "/" slash
 * commentStart 			= "*"
 * (commentEnd				= "* /")	//Only aplies when inside a comment
 * slash					= eps	// Conditional epsilon, so to speak. Only takes effect,
 * 								// if the next char isn't *
 * */

// TODO: Crazy Idea - Make the Lexer class work with iterators. Token Iterators. Instead of calling
// nextToken(), the user could increment an iterator - OR DECREMENT IT

class Lexer {
	public:
		// A string is passed by reference and not a string_view, to make sure there
		// is a terminating character, which this lexer depends on
		Lexer(const std::string_view input) : input_(input), iter_(std::begin(input_)) {

			if (input.size() == 0)
				throw InvalidInputError("The input length must be greater than 0");
		}

		Token nextToken();

		std::size_t getLineNum();
		std::size_t getColNum();

	private:
		using iter_t = std::string_view::iterator;


		std::string_view input_;
		iter_t iter_;

		TokenType nextTokenType_;
		std::string_view nextTokenContent_;


		char getChar() const;
		
		void setNextTokenContent(iter_t it1, iter_t t2);

		bool isIdentifierHeadChar() const;
		bool isIdentifierTailChar() const;
		bool isDigit() const;
		bool isEOS() const;

		void skipWhitespace();

		void identifier();
		void string();
		void number();

		void openTagBracket();
		void closeTagBracket();
		void openListBracket();
		void closeListBracket();
		void eq();
		void semicolon();
		void comma();

		void commentStartSlash();
		void commentStart();
		void slash();

		void eos();
};


#endif
