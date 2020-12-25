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

				// TODO: Check overhead of having if statement
				const iterator& operator++() {
					if (t_.type != token::eos)
						t_ = lexer_->nextToken();

					return *this;
				};
				const iterator operator++(int) { iterator tmp(*this); ++(*this); return *this; };

				const token& operator*() { return t_; };
				const token* operator->() { return &t_; };

				bool operator==(iterator& rhs) { return ( (this == &rhs)
														|| this->t_.type == token::eos
															&& rhs->type == token::eos ); }
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

		token::token_type next_token_type_;
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


#endif
