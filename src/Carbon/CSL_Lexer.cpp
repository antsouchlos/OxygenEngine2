#include <Carbon/Lexer.h>


// TODO: Check performance, when the Lexer is not compiled separetely to the Parser,
// 			in order for the compiler to be able to inline functions better

const char* getTokenTypeStringRep(CSL_TokenType t) {
	switch(t) {
		case CSL_TokenType::ident:
			return "Identifier";
			break;
		case CSL_TokenType::string:
			return "String";
			break;
		case CSL_TokenType::integer:
			return "Integer";
			break;
		case CSL_TokenType::floatingPoint:
			return "Float";
			break;
		case CSL_TokenType::openTagB:
			return "<";
			break;
		case CSL_TokenType::closeTagB:
			return ">";
			break;
		case CSL_TokenType::openClosingTagB:
			return "</";
			break;
		case CSL_TokenType::openListB:
			return "{";
			break;
		case CSL_TokenType::closeListB:
			return "}";
			break;
		case CSL_TokenType::eq:
			return "=";
			break;
		case CSL_TokenType::comma:
			return ",";
			break;
		case CSL_TokenType::semicolon:
			return ";";
			break;
		case CSL_TokenType::comment:
			return "Comment";
			break;
		case CSL_TokenType::slash:
			return "/";
			break;
		case CSL_TokenType::eos:
			return "EOS";
			break;
		case CSL_TokenType::undef:
			return "Undefined Token";
			break;
		default:
			return "[Unknown Type]";
	}
}


CSL_Token CSL_Lexer::nextToken() {
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

	CSL_Token newToken = { nextTokenType_, nextTokenContent_ };

	return newToken;
}

std::size_t CSL_Lexer::getLineNum() {
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

std::size_t CSL_Lexer::getColNum() {
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


char CSL_Lexer::getChar() const {
	return *iter_;
}

void CSL_Lexer::setNextTokenContent(iter_t first, iter_t last) {
	nextTokenContent_ = std::string_view(&*first, last - first);
}

// TODO: Make this work with non-ascii files
bool CSL_Lexer::isIdentifierHeadChar() const {
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
bool CSL_Lexer::isIdentifierTailChar() const {
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

bool CSL_Lexer::isDigit() const {
	int n = static_cast<int>(getChar());

	if ( (48 <= n) && (n <= 57) ) {
		return true;	
	} else {
		return false;
	}
}

bool CSL_Lexer::isEOS() const {
	return (iter_ == std::end(input_));
}

void CSL_Lexer::skipWhitespace() {
	while ( (getChar() == ' ') || (getChar() == '\t') || (getChar() == '\n') ) {
		++iter_;
	}
}
		
void CSL_Lexer::identifier() {
	auto temp = iter_;

	while (isIdentifierTailChar()) {
		++iter_;
	}

	nextTokenType_ = CSL_TokenType::ident;
	setNextTokenContent(temp, iter_);
}

void CSL_Lexer::string() {
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

void CSL_Lexer::number() {
	auto temp = iter_;
	nextTokenType_ = CSL_TokenType::integer;

	if (getChar() == '-')
		++iter_;

	while (isDigit()) {
		++iter_;
	}

	if (getChar() == '.') {
		++iter_;
		nextTokenType_ = CSL_TokenType::floatingPoint;

		while(isDigit()) {
			++iter_;
		}
	}

	setNextTokenContent(temp, iter_);
}

void CSL_Lexer::openTagBracket() {
	++iter_;
	
	if (getChar() == '/') {
		++iter_;

		nextTokenType_ = CSL_TokenType::openClosingTagB;
		setNextTokenContent(iter_ - 2, iter_);
	} else {
		nextTokenType_ = CSL_TokenType::openTagB;
		setNextTokenContent(iter_ - 1, iter_);
	}
}

void CSL_Lexer::closeTagBracket() {
	nextTokenType_ = CSL_TokenType::closeTagB;
	setNextTokenContent(iter_, iter_ + 1);
	++iter_;
}

void CSL_Lexer::openListBracket() {
	nextTokenType_ = CSL_TokenType::openListB;
	setNextTokenContent(iter_, iter_ + 1);
	++iter_;
}

void CSL_Lexer::closeListBracket() {
	nextTokenType_ = CSL_TokenType::closeListB;
	setNextTokenContent(iter_, iter_ + 1);
	++iter_;
}

void CSL_Lexer::eq() {
	nextTokenType_ = CSL_TokenType::eq;
	setNextTokenContent(iter_, iter_ + 1);
	++iter_;
}

void CSL_Lexer::semicolon() {
	nextTokenType_ = CSL_TokenType::semicolon;
	setNextTokenContent(iter_, iter_ + 1);
	++iter_;
}

void CSL_Lexer::comma() {
	nextTokenType_ = CSL_TokenType::comma;
	setNextTokenContent(iter_, iter_ + 1);
	++iter_;
}

void CSL_Lexer::commentStartSlash() {
	++iter_;
	
	if (getChar() == '*') {
		commentStart();
	} else {
		slash();
	}
}

void CSL_Lexer::commentStart() {
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

	nextTokenType_ = CSL_TokenType::comment;

	if (temp != std::end(input_)) {
		setNextTokenContent(temp, iter_ - 1);
	} else {
		setNextTokenContent(temp - 1, temp - 1);
	}
}

void CSL_Lexer::slash() {
	// No character has to be consumed, that has already happened in commentStartSlash()
	
	nextTokenType_ = CSL_TokenType::slash;
	setNextTokenContent(iter_ - 1, iter_);
}

void CSL_Lexer::eos() {
	nextTokenType_ = CSL_TokenType::eos;
	setNextTokenContent(iter_, iter_);
}
