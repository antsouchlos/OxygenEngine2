#include "Lexer.hpp"


// TODO: Check performance, when the Lexer is not compiled separetely to the Parser,
// 			in order for the compiler to be able to inline functions better

Token Lexer::nextToken() {
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
			value();
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
				throw LexerException("Unknown character");
			}
			
			break;
	}

	Token newToken = { nextTokenType_, nextTokenContent_ };

	return newToken;
}

char Lexer::getChar() const {
	return *iter_;
}

void Lexer::setNextTokenContent(iter_t first, iter_t last) {
	nextTokenContent_ = std::string_view(&*first, last - first);
}

// TODO: Make this work with non-ascii files
bool Lexer::isIdentifierHeadChar() const {
	int n = static_cast<int>(getChar());

	if (	(65 <= n) && (n <= 90)
			|| (97 <= n) && (n <= 122)
			|| (95 == n) ) {
		return true;
	} else {
		return false;
	}
}

// TODO: Make this work with non-ascii files
bool Lexer::isIdentifierTailChar() const {
	int n = static_cast<int>(getChar());

	if (	(65 <= n) && (n <= 90)
			|| (97 <= n) && (n <= 122)
	  		|| (48 <= n) && (n <= 57)
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

	nextTokenType_ = TokenType::ident;
	setNextTokenContent(temp, iter_);
}

void Lexer::value() {
	++iter_;

	auto temp = iter_;

	while (!isEOS()
			&& getChar() != '"') {

		++iter_;
	}

	++iter_;

	nextTokenType_ = TokenType::value;
	setNextTokenContent(temp, iter_ - 1);
}

void Lexer::number() {
	auto temp = iter_;

	if (getChar() == '-')
		++iter_;

	while (isDigit()) {
		++iter_;
	}

	if (getChar() == '.') {
		++iter_;

		while(isDigit()) {
			++iter_;
		}
	}

	nextTokenType_ = TokenType::number;
	setNextTokenContent(temp, iter_);
}

void Lexer::openTagBracket() {
	nextTokenType_ = TokenType::openTagB;
	setNextTokenContent(iter_, iter_ + 1);
	++iter_;
}

void Lexer::closeTagBracket() {
	nextTokenType_ = TokenType::closeTagB;
	setNextTokenContent(iter_, iter_ + 1);
	++iter_;
}

void Lexer::openListBracket() {
	nextTokenType_ = TokenType::openListB;
	setNextTokenContent(iter_, iter_ + 1);
	++iter_;
}

void Lexer::closeListBracket() {
	nextTokenType_ = TokenType::closeListB;
	setNextTokenContent(iter_, iter_ + 1);
	++iter_;
}

void Lexer::eq() {
	nextTokenType_ = TokenType::eq;
	setNextTokenContent(iter_, iter_ + 1);
	++iter_;
}

void Lexer::semicolon() {
	nextTokenType_ = TokenType::semicolon;
	setNextTokenContent(iter_, iter_ + 1);
	++iter_;
}

void Lexer::comma() {
	nextTokenType_ = TokenType::comma;
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

	nextTokenType_ = TokenType::comment;

	if (temp != std::end(input_)) {
		setNextTokenContent(temp, iter_ - 1);
	} else {
		setNextTokenContent(temp - 1, temp - 1);
	}
}

void Lexer::slash() {
	// No character has to be consumed, that has already happened in commentStartSlash()
	
	nextTokenType_ = TokenType::slash;
	setNextTokenContent(iter_ - 1, iter_);
}

void Lexer::eos() {
	nextTokenType_ = TokenType::eos;
	setNextTokenContent(iter_, iter_);
}
