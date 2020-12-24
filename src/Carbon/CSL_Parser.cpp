#include <Carbon/CSL_Parser.h>


CSL_Element_ptr CSL_Parser::parse() {
	CSL_Element_ptr result;
	
	nextToken();

	if (token_.type == CSL_TokenType::openTagB) {
		result = std::move(element());
	} else {
		throw UnexpectedSymbolError(token_.content, CSL_TokenType::openTagB,
										lexer_.getLineNum(), lexer_.getColNum());
	}
	
	return result;
}

void CSL_Parser::nextToken() {
	token_ = lexer_.nextToken();
}


namespace csl {

float parseFloat(std::string_view s) {
	// Although the standard defines it for c++17, g++ does not implement
	// std::from_chars(const char*, const char*, double), only
	// std::from_chars(const char*, const char*, int), so we have to get a little bit
	// hacky here. Floats are parsed in terms of two ints: One before and one after
	// the dot

	std::size_t i1 = 0;
	std::size_t i2 = 0;

	auto [c1, ec1] = std::from_chars(std::begin(s), std::end(s), i1);
	if (ec1 == std::errc()) {
		throw SemanticError("Unexpected character in number");
	}

	std::size_t length1  = c1 - std::begin(s);

	auto [c2, ec2] = std::from_chars(std::begin(s) + length1 + 1, std::end(s), i2);	
	if (ec2 == std::errc()) {
		throw SemanticError("Unexpected character in number");
	}
	
	std::size_t length2  = c2 - (std::begin(s) + length1 + 1);

	return ( i1 + i2 / static_cast<float>(pow(10, length2)) );
}

std::size_t parseInt(std::string_view s) {
	std::size_t i = 0;

	auto [c1, ec1] = std::from_chars(std::begin(s), std::end(s), i);

	if (ec1 == std::errc()) {
		throw SemanticError("Error while parsing integer: Unexpected character");
	}

	
	return i;
}

}

CSL_Element_ptr CSL_Parser::element() {
	CSL_Element_ptr result = std::make_unique<CSL_Element>();

	auto [name, attributes] = openTag();

	result->name = name;
	std::move(attributes.begin(), attributes.end(), std::back_inserter(result->attributes));

	while ( token_.type == CSL_TokenType::openTagB
			|| token_.type == CSL_TokenType::ident ) {

		if (token_.type == CSL_TokenType::openTagB) {
			result->elements.push_back(element());
		} else if (token_.type == CSL_TokenType::ident) {
			result->assignments.push_back(genericAssignment());
		}
	}

	if (token_.type == CSL_TokenType::openClosingTagB) {
		closeTag(result->name);
	} else {
		throw UnexpectedSymbolError(token_.content, CSL_TokenType::openClosingTagB,
										lexer_.getLineNum(), lexer_.getColNum());
	}

	return result;
}

CSL_OpenTagResult CSL_Parser::openTag() {
	CSL_OpenTagResult result;

	nextToken();

	if (token_.type == CSL_TokenType::ident) {
		result.first = token_.content;
	} else {
		throw UnexpectedSymbolError(token_.content, CSL_TokenType::ident,
										lexer_.getLineNum(), lexer_.getColNum());
	}

	nextToken();

	while (token_.type == CSL_TokenType::ident) {
		result.second.push_back(genericAssignment());
	}

	if (token_.type == CSL_TokenType::closeTagB) {
		nextToken();
	} else {
		throw UnexpectedSymbolError(token_.content, CSL_TokenType::closeTagB,
										lexer_.getLineNum(), lexer_.getColNum());
	}

	return result;
}

void CSL_Parser::closeTag(std::string_view tagIdentifier) {
	nextToken();

	if (token_.type == CSL_TokenType::ident) {
		if (tagIdentifier != token_.content) {
			throw SemanticError("Closing tag identifier does not match opening tag identifier");
		}
	} else {
		throw UnexpectedSymbolError(token_.content, CSL_TokenType::ident,
										lexer_.getLineNum(), lexer_.getColNum());
	}

	nextToken();

	if (token_.type == CSL_TokenType::closeTagB) {
		nextToken();
	} else {
		throw UnexpectedSymbolError(token_.content, CSL_TokenType::closeTagB,
										lexer_.getLineNum(), lexer_.getColNum());
	}
}

CSL_GenericAssignment_ptr CSL_Parser::genericAssignment() {
	std::string_view name = token_.content;

	nextToken();

	if (token_.type == CSL_TokenType::eq) {
		nextToken();

		if ( (token_.type == CSL_TokenType::floatingPoint)
				|| (token_.type == CSL_TokenType::integer)) {
			return singleAssignment(name);
		} else if (token_.type == CSL_TokenType::openListB) {
			return listAssignment(name);
		} else if (token_.type == CSL_TokenType::string) {
			return singleAssignment(name);
		} else {
//			throw UnexpectedSymbolError("Unexpected Symbol. Expected '{', number or string");
			throw UnexpectedSymbolError(token_.content, CSL_TokenType::openListB,
											lexer_.getLineNum(), lexer_.getColNum());
		}
	} else {
		throw UnexpectedSymbolError(token_.content, CSL_TokenType::eq,
										lexer_.getLineNum(), lexer_.getColNum());
	}
}

CSL_Assignment_ptr CSL_Parser::singleAssignment(std::string_view name) {
	CSL_Assignment_ptr result = std::make_unique<CSL_Assignment>();

	result->name = name;
	result->value = token_.content;

	nextToken();

	return result;
}


CSL_ListAssignment_ptr CSL_Parser::listAssignment(std::string_view name) {
	CSL_ListAssignment_ptr result = std::make_unique<CSL_ListAssignment>();

	result->name = name;

	nextToken();

	if (token_.type == CSL_TokenType::integer) {
		result->values.push_back(csl::parseInt(token_.content));
		nextToken();
	} else if (token_.type == CSL_TokenType::floatingPoint) {
		result->values.push_back(csl::parseFloat(token_.content));
		nextToken();
	} else {
//		throw UnexpectedSymbolError("Unexpected Symbol. Expected float or integer");
		throw UnexpectedSymbolError(token_.content, CSL_TokenType::floatingPoint,
										lexer_.getLineNum(), lexer_.getColNum());
	}

	while (token_.type == CSL_TokenType::semicolon) {
		nextToken();

		if (token_.type == CSL_TokenType::integer) {
			result->values.push_back(csl::parseInt(token_.content));
			nextToken();
		} else if (token_.type == CSL_TokenType::floatingPoint) {
			result->values.push_back(csl::parseFloat(token_.content));
			nextToken();
		} else {
//			throw UnexpectedSymbolError("Unexpected Symbol. Expected float or integer");
			throw UnexpectedSymbolError(token_.content, CSL_TokenType::floatingPoint,
											lexer_.getLineNum(), lexer_.getColNum());
		}
	}

	if (token_.type == CSL_TokenType::closeListB) {
		nextToken();
	} else {
		throw UnexpectedSymbolError(token_.content, CSL_TokenType::closeListB,
										lexer_.getLineNum(), lexer_.getColNum());
	}

	return result;
}
