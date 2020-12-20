#include "Lexer.hpp"

// TODO: Check performance, when function definitions are in WordParser.hpp
// TODO: Check performance, when the WordParser is not compiled separetely to the Parser,
// 			in order for the compiler to be able to optimize better


Token Lexer::nextToken() {
	skipWhitespace();

	switch (input_.at(index_)) {
		case '"':
			value();
			break;
		case '<':
			openBracket();
			break;
		case '>':
			closeBracket();
			break;
		case '=':
			eq();
			break;
		case ';':
			semicolon();
			break;
		case '/':
			commentStartSlash();
			break;
		default:
			if (isIdentifierHeadChar()) {
				identifier();
			} else if (isEOS()) {
				eos();
			} else {
				throw LexerException("Unknown character");
			}
			
			break;
	}

	Token newToken = { nextTokenType_, nextTokenContent_ };
	// TODO: Check performance if the following is used instead:
	// Token newToken;
	// newToken.type = nextTokenType_;

	return newToken;
}

// TODO: Make this work with non-ascii files
constexpr bool Lexer::isIdentifierHeadChar(int offset) const noexcept {
	char c = input_.at(index_ + offset);

	if ( ( (65 <= static_cast<int>(c)) && (static_cast<int>(c) <= 90) )
			|| ( (97 <= static_cast<int>(c)) && (static_cast<int>(c)<= 122) )
	  		|| ( (48 <= static_cast<int>(c)) && (static_cast<int>(c)<= 57) )
			|| (95 == static_cast<int>(c)) ) {
		return true;
	} else {
		return false;
	}
}


// TODO: Make this work with non-ascii files
constexpr bool Lexer::isIdentifierTailChar(int offset) const noexcept {
	char c = input_.at(index_ + offset);

	if ( ( (65 <= static_cast<int>(c)) && (static_cast<int>(c) <= 90) )
			|| ( (97 <= static_cast<int>(c)) && (static_cast<int>(c)<= 122) )
	  		|| ( (48 <= static_cast<int>(c)) && (static_cast<int>(c)<= 57) )
			|| (95 == static_cast<int>(c)) ) {
		return true;
	} else {
		return false;
	}
}

constexpr bool Lexer::isEOS(int offset) const noexcept {
	if (index_ + offset == input_.size()) {
		return true;
	} else {
		return false;
	}
}

void Lexer::skipWhitespace() noexcept {
	// TODO: Check performance without the string size check

	while ( (index_ < input_.size())
		 	&& (std::find(std::begin(whitespaceChars_), std::end(whitespaceChars_), input_.at(index_))
				!= std::end(whitespaceChars_)) ) {
		
		++index_;
	}
}
		
void Lexer::identifier() noexcept {
	nextTokenType_ = TokenType::ident;

	unsigned int len = 1;
	while ( (index_ + len < input_.size())
			&& isIdentifierTailChar(len) ) {

		++len;
	}

	nextTokenContent_ = input_.substr(index_, len);

	index_ += len;
}

// TODO: Check performance, if all prefix-increments are changed to postfix increments
void Lexer::value() noexcept {
	++index_;

	unsigned int len = 0;
	while( (index_ + len + 1< input_.size())
			&&  (input_.at(index_ + len) != '"') ) {

		++len;
	}

	nextTokenType_ = TokenType::value;
	nextTokenContent_ = input_.substr(index_, len);

	index_ += len + 1;
}

void Lexer::openBracket() noexcept {
	++index_;
	nextTokenType_ = TokenType::openB;
	nextTokenContent_ = input_.substr(index_ - 1, 1);
}

void Lexer::closeBracket() noexcept {
	++index_;
	nextTokenType_ = TokenType::closeB;
	nextTokenContent_ = input_.substr(index_ - 1, 1);
}

void Lexer::eq() noexcept {
	++index_;
	nextTokenType_ = TokenType::eq;
	nextTokenContent_ = input_.substr(index_ - 1, 1);
}

void Lexer::semicolon() noexcept {
	++index_;
	nextTokenType_ = TokenType::semicolon;
	nextTokenContent_ = input_.substr(index_ - 1, 1);
}

void Lexer::commentStartSlash() noexcept {
	++index_;
	
	if ( (index_ < input_.size()) 
			&& (input_.at(index_) == '*') ) {

		commentStart();
	} else {
		slash();
	}
}

void Lexer::commentStart() noexcept {
	// Only the second character has to be consumed, for the first one this already
	// happened in commentStartSlash()
	++index_;

	long long len = 0;

	while (index_ + len < input_.size()) {
		if (input_.at(index_ + len) == '*') {
			++len;

			if (index_ + len < input_.size()
					&& input_.at(index_ + len) == '/') {

				break;
			}
		} else {
			++len;	
		}
	}

	nextTokenType_ = TokenType::comment;
	nextTokenContent_ = input_.substr(index_, len - 1);
	
	index_ += len;
}

void Lexer::slash() noexcept {
	// No character has to be consumed, that already happened in commentStartSlash()
	
	nextTokenType_ = TokenType::slash;
	nextTokenContent_ = input_.substr(index_ - 1, 1);
}

void Lexer::eos() noexcept {
	nextTokenType_ = TokenType::eos;
	nextTokenContent_ = input_.substr(index_, 0);
}
