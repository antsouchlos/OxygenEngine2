CSL_Element Parser::parse() {
	CSL_Element result;
	
	nextToken();

	if (token_.type == TokenType::openTagB) {
		result = element();
	} else {
		throw ParserException("Unexpected Symbol. Expexted '<'");
	}
	
	return result;
}

//std::string Parser::getTokenTypeStringRep() const {
//	switch(token_.type) {
//		case TokenType::ident:
//			return "Identifier";
//			break;
//		case TokenType::string:
//			return "String";
//			break;
//		case TokenType::number:
//			return "Number";
//			break;
//		case TokenType::openTagB:
//			return "Opening Tag Bracket";
//			break;
//		case TokenType::closeTagB:
//			return "Closing Tag Bracket";
//			break;
//		case TokenType::openListB:
//			return "Opening List Bracket";
//			break;
//		case TokenType::closeListB:
//			return "Closing List Bracket";
//			break;
//		case TokenType::eq:
//			return "Equal Sign";
//			break;
//		case TokenType::comma:
//			return "Comma";
//			break;
//		case TokenType::semicolon:
//			return "Semicolon";
//			break;
//		case TokenType::comment:
//			return "Comment";
//			break;
//		case TokenType::slash:
//			return "Slash";
//			break;
//		case TokenType::eos:
//			return "EOS";
//			break;
//		case TokenType::undef:
//			return "Undefined";
//			break;
//		default:
//			return "[Unknown Type]";
//	}
//}

void Parser::nextToken() {
	token_ = lexer_.nextToken();
}

CSL_Element Parser::element() {
	CSL_Element result;

	std::pair< std::string_view,
		std::vector<std::variant<CSL_Assignment, CSL_ListAssignment>> >
			openTagResult = openTag();

	result.name = openTagResult.first;
	result.attributes = openTagResult.second;

//	if (token_.type == TokenType::openTagB) {
//		openTag();
//	} else {
//		throw ParserException("Unexpected Symbol. Expected '<' or identifier");	
//	}

	while (token_.type == TokenType::openTagB
			|| token_.type == TokenType::ident) {

		if (token_.type == TokenType::openTagB) {
			result.elements.push_back(element());
		} else if (token_.type == TokenType::ident) {
			result.assignments.push_back(genericAssignment());
		}
	}

	if (token_.type == TokenType::openClosingTagB) {
		closeTag(result.name);
	} else {
		throw ParserException("Unexpected Symbol. Expected \"</\"");	
	}

	return result;
}

std::pair<std::string_view,
			std::vector<std::variant<CSL_Assignment, CSL_ListAssignment>>
				> Parser::openTag() {

	std::pair< std::string_view,
		std::vector<std::variant<CSL_Assignment, CSL_ListAssignment>> > result;

	nextToken();

	if (token_.type == TokenType::ident) {
		result.first = token_.content;
	} else {
		throw ParserException("Unexpected Symbol. Expected identifier");
	}

	nextToken();

	while (token_.type == TokenType::ident) {
		result.second.push_back(genericAssignment());
	}

	if (token_.type == TokenType::closeTagB) {
		nextToken();
	} else {
		throw ParserException("Unexpected Symbol. Expected '>'");
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

	if (token_.type == TokenType::closeTagB) {
		nextToken();
	} else {
		throw ParserException("Unexpected Symbol. Expected '>'");
	}
}

std::variant<CSL_Assignment, CSL_ListAssignment>
Parser::genericAssignment() {
	std::string_view name = token_.content;

	nextToken();

	if (token_.type == TokenType::eq) {
		nextToken();

		if (token_.type == TokenType::number) {
			return singleAssignment(name);
		} else if (token_.type == TokenType::openListB) {
			return listAssignment(name);
		} else if (token_.type == TokenType::string) {
			return singleAssignment(name);
		} else {
			throw ParserException("Unexpected Symbol. Expected '{', number or string");
		}
	} else {
		throw ParserException("Unexpected Symbol. Expected '='");
	}
}

CSL_Assignment Parser::singleAssignment(std::string_view name) {
	CSL_Assignment result;

	result.name = name;
	result.element = token_.content;

	nextToken();

	return result;
}

CSL_ListAssignment Parser::listAssignment(std::string_view name) {
	CSL_ListAssignment result;
	result.name = name;

	nextToken();

	if (token_.type == TokenType::number) {
		double f;
		std::from_chars(token_.content.data(), token_.content.data() + token_.content.size(), f);
		result.elements.push_back(f);
		nextToken();
	} else {
		throw ParserException("Unexpected Symbol. Expected number");
	}

	while (token_.type == TokenType::semicolon) {
		nextToken();

		if (token_.type == TokenType::number) {
			double f;
			std::from_chars(token_.content.data(), token_.content.data() + token_.content.size(), f);
			result.elements.push_back(f);
			nextToken();
		} else {
			throw ParserException("Unexpected Symbol. Expected number");
		}
	}

	if (token_.type == TokenType::closeListB) {
		nextToken();
	} else {
		throw ParserException("Unexpected Symbol. Expected '}'");
	}

	return result;
}

