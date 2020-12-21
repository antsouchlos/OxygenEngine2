#include "Lexer.hpp"

// TODO: Check performance, when function definitions are in WordParser.hpp
// TODO: Check performance, when the WordParser is not compiled separetely to the Parser,
// 			in order for the compiler to be able to optimize better


Token Lexer::nextToken() {
	skipWhitespace();

	switch (getChar()) {
		case '"':
			value();
			break;
		case '<':
			openTagBracket();
			break;
		case '>':
			closeTagBracket();
			break;
		case '{':
			openListBracket();
			break;
		case '}':
			closeListBracket();
			break;
		case '=':
			eq();
			break;
		case ',':
			comma();
			break;
		case ';':
			semicolon();
			break;
		case '/':
			commentStartSlash();
			break;
		default:
			if (isNumber()) {
				number();
			} else if (isIdentifierHeadChar()) {
				identifier();
			} else if (isEOS()) {
				eos();
			} else {
				std::string msg = "Unknown character: '";
				//msg += getChar() + "'";
				throw LexerException(msg.c_str());
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

bool Lexer::isNumber() const {
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
	while ( std::find(std::begin(whitespaceChars_), std::end(whitespaceChars_), getChar())
				!= std::end(whitespaceChars_) ) {
		
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

// TODO: Check performance, if all prefix-increments are changed to postfix increments
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

	while (isNumber()) {
		++iter_;
	}

	if (getChar() == '.') {
		++iter_;

		while(isNumber()) {
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
	// Only the second character has to be consumed, for the first one this already
	// happened in commentStartSlash()
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

	// TODO: What if isEOS() is true the first time the while loop executes? In that case iter_ - 1
	// will point to the element before temp
	nextTokenType_ = TokenType::comment;
	setNextTokenContent(temp, iter_ - 1);
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
