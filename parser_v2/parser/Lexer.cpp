#include "Lexer.hpp"


// TODO: Check performance, when the Lexer is not compiled separetely to the Parser,
// 			in order for the compiler to be able to inline functions better

const char* getTokenTypeStringRep(TokenType t) {
	switch(t) {
		case TokenType::ident:
			return "Identifier";
			break;
		case TokenType::string:
			return "String";
			break;
		case TokenType::integer:
			return "Integer";
			break;
		case TokenType::floatingPoint:
			return "Float";
			break;
		case TokenType::openTagB:
			return "<";
			break;
		case TokenType::closeTagB:
			return ">";
			break;
		case TokenType::openClosingTagB:
			return "</";
			break;
		case TokenType::openListB:
			return "{";
			break;
		case TokenType::closeListB:
			return "}";
			break;
		case TokenType::eq:
			return "=";
			break;
		case TokenType::comma:
			return ",";
			break;
		case TokenType::semicolon:
			return ";";
			break;
		case TokenType::comment:
			return "Comment";
			break;
		case TokenType::slash:
			return "/";
			break;
		case TokenType::eos:
			return "EOS";
			break;
		case TokenType::undef:
			return "Undefined Token";
			break;
		default:
			return "[Unknown Type]";
	}
}


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

	Token newToken = { nextTokenType_, nextTokenContent_ };

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

void Lexer::string() {
	++iter_;

	auto temp = iter_;

	while (!isEOS()
			&& getChar() != '"') {

		++iter_;
	}

	++iter_;

	nextTokenType_ = TokenType::string;
	setNextTokenContent(temp, iter_ - 1);
}

void Lexer::number() {
	auto temp = iter_;
	nextTokenType_ = TokenType::integer;

	if (getChar() == '-')
		++iter_;

	while (isDigit()) {
		++iter_;
	}

	if (getChar() == '.') {
		++iter_;
		nextTokenType_ = TokenType::floatingPoint;

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

		nextTokenType_ = TokenType::openClosingTagB;
		setNextTokenContent(iter_ - 2, iter_);
	} else {
		nextTokenType_ = TokenType::openTagB;
		setNextTokenContent(iter_ - 1, iter_);
	}
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
