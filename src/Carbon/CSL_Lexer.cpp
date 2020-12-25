#include <Carbon/CSL_Lexer.h>


// TODO: Check performance, when the Lexer is not compiled separetely to the Parser,
// 			in order for the compiler to be able to inline functions better

namespace csl {


token Lexer::nextToken() {
	// TODO: Check performance penalty of this if statement
	if (next_token_type_ == token::eos) {
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
	return iterator(this, {token::eos, ""});
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

	next_token_type_ = token::ident;
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

	next_token_type_ = token::string;
	setNextTokenContent(temp, iter_ - 1);
}

void Lexer::number() {
	auto temp = iter_;
	next_token_type_ = token::integer;

	if (getChar() == '-')
		++iter_;

	while (isDigit()) {
		++iter_;
	}

	if (getChar() == '.') {
		++iter_;
		next_token_type_ = token::floatingPoint;

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

		next_token_type_ = token::openClosingTagB;
		setNextTokenContent(iter_ - 2, iter_);
	} else {
		next_token_type_ = token::openTagB;
		setNextTokenContent(iter_ - 1, iter_);
	}
}

void Lexer::closeTagBracket() {
	next_token_type_ = token::closeTagB;
	setNextTokenContent(iter_, iter_ + 1);
	++iter_;
}

void Lexer::openListBracket() {
	next_token_type_ = token::openListB;
	setNextTokenContent(iter_, iter_ + 1);
	++iter_;
}

void Lexer::closeListBracket() {
	next_token_type_ = token::closeListB;
	setNextTokenContent(iter_, iter_ + 1);
	++iter_;
}

void Lexer::eq() {
	next_token_type_ = token::eq;
	setNextTokenContent(iter_, iter_ + 1);
	++iter_;
}

void Lexer::semicolon() {
	next_token_type_ = token::semicolon;
	setNextTokenContent(iter_, iter_ + 1);
	++iter_;
}

void Lexer::comma() {
	next_token_type_ = token::comma;
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

	next_token_type_ = token::comment;

	if (temp != std::end(input_)) {
		setNextTokenContent(temp, iter_ - 1);
	} else {
		setNextTokenContent(temp - 1, temp - 1);
	}
}

void Lexer::slash() {
	// No character has to be consumed, that has already happened in commentStartSlash()
	
	next_token_type_ = token::slash;
	setNextTokenContent(iter_ - 1, iter_);
}

void Lexer::eos() {
	next_token_type_ = token::eos;
	setNextTokenContent(iter_, iter_);
}


}
