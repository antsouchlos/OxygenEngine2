#include "Parser.hpp"


CSL_Element Parser::parse() {
	CSL_Element result;

	if (token_.type == TokenType::openTagB) {
		element();
	} else {
		throw ParserException("Unexpected Symbol. Expected '<'");
	}
	
	return result;
}


void Parser::nextToken() {
	token_ = lexer_.nextToken();
}

void Parser::element() {
	std::string_view tagIdentifier = openTag();

	if (token_.type == TokenType::openTagB) {
		element();
	} else if (token_.type == TokenType::ident) {
		genericAssignment();
	} else {
		throw ParserException("Unexpected Symbol. Expected '<' or identifier");	
	}	

	if (token_.type == TokenType::closeTagB) {
		closeTag(tagIdentifier);
	} else {
		throw ParserException("Unexpected Symbol. Expected \"</\"");	
	}
}

std::string_view Parser::openTag() {
	std::string_view result;

	nextToken();

	if (token_.type == TokenType::ident) {
		result = token_.content;
	} else {
		throw ParserException("Unexpected Symbol. Expected identifier");
	}

	nextToken();

	while (token_.type == TokenType::ident) {
		genericAssignment();
	}

	return result;
}

void Parser::closeTag(std::string_view tagIdentifier) {
	nextToken();

	if (token_.type == TokenType::ident) {
		if (tagIdentifier != token_.content) {
			throw ParserException("Closing tag identifier does not match opening tag identifier");
		}
	} else {
		throw ParserException("Unexpected Symbol. Expected identifier");
	}

	nextToken();
}

void Parser::genericAssignment() {
	nextToken();

	if (token_.type == TokenType::eq) {
		nextToken();

		// TODO: It may be possible to template the function singleAssignment, in order to know
		// the template type of CSL_Assignment
		if (token_.type == TokenType::number) {
			singleAssignment();		
		} else if (token_.type == TokenType::openListB) {
			listAssignment();
		} else if (token_.type == TokenType::string) {
			singleAssignment();
		} else {
			throw ParserException("Unexpected Symbol. Expected '{', number or string");
		}
	} else {
		throw ParserException("Unexpected Symbol. Expected '='");
	}
}

void Parser::singleAssignment() {
	nextToken();
}

void Parser::listAssignment() {
	nextToken();

	if (token_.type == TokenType::number) {
	
	} else {
		throw ParserException("Unexpected Symbol. Expected number");
	}

	while (token_.type == TokenType::semicolon) {
		nextToken();

		if (token_.type == TokenType::number) {
		
		} else {
			throw ParserException("Unexpected Symbol. Expected number");
		}
	}
}

