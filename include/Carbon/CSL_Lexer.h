#ifndef LEXER_H
#define LEXER_H

#include <sstream>
#include <string_view>
#include <array>
#include <algorithm>
#include <stdexcept>

#include <Carbon/CSL_Exceptions.h>


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
namespace csl {

class Lexer {
	public:
		class iterator {
			public:
				// TODO: Make this forward at some point
				using iterator_category = std::input_iterator_tag;
				using value_type = token;
				using difference_type = std::size_t;
				using reference = token&;
				using pointer = token*;

				iterator(Lexer* lexer) : lexer_(lexer) {}
				iterator(Lexer* lexer, token t) : lexer_(lexer), t_(t) {};

				const iterator& operator++() {
					if (t_.token_type != token::type::eos)
						t_ = lexer_->nextToken();

					return *this;
				};
				const iterator operator++(int) { iterator tmp(*this); ++(*this); return *this; };

				const token& operator*() { return t_; };
				const token* operator->() { return &t_; };

				bool operator==(iterator& rhs) {
					return ( (this == &rhs) || this->t_.token_type == token::type::eos
												&& rhs->token_type == token::type::eos ); }
				bool operator!=(iterator& rhs) { return !((*this) == rhs); };
			private:
				Lexer *lexer_;
				token t_;

		};

		// A string is passed by reference and not a string_view, to make sure there
		// is a terminating character, which this lexer depends on
		Lexer(const std::string &input) : input_(input), iter_(std::begin(input_)) {
			if (input.size() == 0)
				throw InvalidInputError("The input length must be greater than 0");
		}

		token nextToken();

		std::size_t getLineNum();
		std::size_t getColNum();

		iterator begin();
		iterator end();

	private:
		using iter_t = std::string_view::iterator;


		std::string_view input_;
		iter_t iter_;

		token::type next_token_type_;
		std::string_view next_token_content_;


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

}

namespace csl {


token Lexer::nextToken() {
	// TODO: Check performance penalty of this if statement
	if (next_token_type_ == token::type::eos) {
		return {next_token_type_, next_token_content_};
	}

	skipWhitespace();

	switch (getChar()) {
		case ';':
			semicolon();
			break;
		case '=':
			eq();
			break;
		case '{':
			openListBracket();
			break;
		case '}':
			closeListBracket();
			break;
		case '<':
			openTagBracket();
			break;
		case '>':
			closeTagBracket();
			break;
		case '/':
			commentStartSlash();
			break;
		case '"':
			string();
			break;
		case ',':
			comma();
			break;
		default:
			if (isDigit() || getChar() == '-') {
				number();
			} else if (isIdentifierHeadChar()) {
				identifier();
			} else if (isEOS()) {
				eos();
			} else {
				throw UnknownCharacterError(getChar(), getLineNum(), getColNum());
			}
			
			break;
	}

	token newToken = { next_token_type_, next_token_content_ };

	return newToken;
}

std::size_t Lexer::getLineNum() {
	auto it = input_.begin();

	std::size_t lineNum = 1;

	while (it != iter_) {
		if (*it == '\n') {
			++lineNum;
		}

		++it;
	}

	return lineNum;
}

std::size_t Lexer::getColNum() {
	if (iter_ == input_.begin()) {
		return 0;
	}

	auto it = iter_ - 1;

	std::size_t colNum = 1;

	while (it != input_.begin()) {
		if (*it == '\n') {
			break;
		}

		++colNum;
		--it;
	}

	return colNum;
}

Lexer::iterator Lexer::begin() {
	return iterator(this);
}

Lexer::iterator Lexer::end() {
	return iterator(this, {token::type::eos, ""});
}

char Lexer::getChar() const {
	return *iter_;
}

void Lexer::setNextTokenContent(iter_t first, iter_t last) {
	next_token_content_ = std::string_view(&*first, last - first);
}

// TODO: Make this work with non-ascii files
bool Lexer::isIdentifierHeadChar() const {
	int n = static_cast<int>(getChar());

	if (	( (65 <= n) && (n <= 90) ) 
		 || ( (97 <= n) && (n <= 122) )
		 || (95 == n) ) {
		return true;
	} else {
		return false;
	}
}

// TODO: Make this work with non-ascii files
bool Lexer::isIdentifierTailChar() const {
	int n = static_cast<int>(getChar());

	if (	( (65 <= n) && (n <= 90) )
		 || ( (97 <= n) && (n <= 122) )
	  	 || ( (48 <= n) && (n <= 57) )
		 || (95 == n) ) {
		return true;
	} else {
		return false;
	}
}

bool Lexer::isDigit() const {
	int n = static_cast<int>(getChar());

	if ( (48 <= n) && (n <= 57) ) {
		return true;	
	} else {
		return false;
	}
}

bool Lexer::isEOS() const {
	return (iter_ == std::end(input_));
}

void Lexer::skipWhitespace() {
	while ( (getChar() == ' ') || (getChar() == '\t') || (getChar() == '\n') ) {
		++iter_;
	}
}
		
void Lexer::identifier() {
	auto temp = iter_;

	while (isIdentifierTailChar()) {
		++iter_;
	}

	next_token_type_ = token::type::ident;
	setNextTokenContent(temp, iter_);
}

void Lexer::string() {
	++iter_;

	auto temp = iter_;

	while (!isEOS()
			&& getChar() != '"') {

		++iter_;
	}

	++iter_;

	next_token_type_ = token::type::string;
	setNextTokenContent(temp, iter_ - 1);
}

void Lexer::number() {
	auto temp = iter_;
	next_token_type_ = token::type::integer;

	if (getChar() == '-')
		++iter_;

	while (isDigit()) {
		++iter_;
	}

	if (getChar() == '.') {
		++iter_;
		next_token_type_ = token::type::floatingPoint;

		while(isDigit()) {
			++iter_;
		}
	}

	setNextTokenContent(temp, iter_);
}

void Lexer::openTagBracket() {
	++iter_;
	
	if (getChar() == '/') {
		++iter_;

		next_token_type_ = token::type::openClosingTagB;
		setNextTokenContent(iter_ - 2, iter_);
	} else {
		next_token_type_ = token::type::openTagB;
		setNextTokenContent(iter_ - 1, iter_);
	}
}

void Lexer::closeTagBracket() {
	next_token_type_ = token::type::closeTagB;
	setNextTokenContent(iter_, iter_ + 1);
	++iter_;
}

void Lexer::openListBracket() {
	next_token_type_ = token::type::openListB;
	setNextTokenContent(iter_, iter_ + 1);
	++iter_;
}

void Lexer::closeListBracket() {
	next_token_type_ = token::type::closeListB;
	setNextTokenContent(iter_, iter_ + 1);
	++iter_;
}

void Lexer::eq() {
	next_token_type_ = token::type::eq;
	setNextTokenContent(iter_, iter_ + 1);
	++iter_;
}

void Lexer::semicolon() {
	next_token_type_ = token::type::semicolon;
	setNextTokenContent(iter_, iter_ + 1);
	++iter_;
}

void Lexer::comma() {
	next_token_type_ = token::type::comma;
	setNextTokenContent(iter_, iter_ + 1);
	++iter_;
}

void Lexer::commentStartSlash() {
	++iter_;
	
	if (getChar() == '*') {
		commentStart();
	} else {
		slash();
	}
}

void Lexer::commentStart() {
	// Only the second character has to be consumed, for the first one this happened already,
	// in commentStartSlash()
	++iter_;

	auto temp = iter_;

	while (!isEOS()) {
		if (getChar() == '*') {
			++iter_;

			if (getChar() == '/') {
				break;
			}
		} else {
			++iter_;	
		}
	}

	next_token_type_ = token::type::comment;

	if (temp != std::end(input_)) {
		setNextTokenContent(temp, iter_ - 1);
	} else {
		setNextTokenContent(temp - 1, temp - 1);
	}
}

void Lexer::slash() {
	// No character has to be consumed, that has already happened in commentStartSlash()
	
	next_token_type_ = token::type::slash;
	setNextTokenContent(iter_ - 1, iter_);
}

void Lexer::eos() {
	next_token_type_ = token::type::eos;
	setNextTokenContent(iter_, iter_);
}

}

#endif
