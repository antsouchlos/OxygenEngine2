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

char Lexer::getChar() const {
	return *iter_;
}

void Lexer::setNextTokenContent(std::string_view::iterator first,
									std::string_view::iterator last) {

	nextTokenContent_ = std::string_view(&*first, last - first);
}

// TODO: Make this work with non-ascii files
bool Lexer::isIdentifierHeadChar() const {
	char c = getChar();

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
bool Lexer::isIdentifierTailChar() const {
	char c = getChar();

	if ( ( (65 <= static_cast<int>(c)) && (static_cast<int>(c) <= 90) )
			|| ( (97 <= static_cast<int>(c)) && (static_cast<int>(c)<= 122) )
	  		|| ( (48 <= static_cast<int>(c)) && (static_cast<int>(c)<= 57) )
			|| (95 == static_cast<int>(c)) ) {
		return true;
	} else {
		return false;
	}
}

bool Lexer::isEOS() const {
	return (iter_ == std::end(input_));
}

void Lexer::skipWhitespace() {
	// TODO: Check performance without the string size check

	while ( std::find(std::begin(whitespaceChars_), std::end(whitespaceChars_), input_.at(index_))
				!= std::end(whitespaceChars_) ) {
		
		++iter_;
	}
}
		
void Lexer::identifier() {
	nextTokenType_ = TokenType::ident;

	auto temp = iter_;

	while (isIdentifierTailChar()) {
		++iter_;
	}

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

	nextTokenType_ = TokenType::value;
	setNextTokenContent(iter_, iter_);
}

void Lexer::openBracket() {
	nextTokenType_ = TokenType::openB;
	setNextTokenContent(iter_, iter_ + 1);
	++iter_;
}

void Lexer::closeBracket() {
	nextTokenType_ = TokenType::closeB;
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
