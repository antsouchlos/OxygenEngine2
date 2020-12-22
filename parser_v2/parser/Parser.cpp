template<typename NumberType>
CSL_Element<NumberType> Parser<NumberType>::parse() {
	CSL_Element<NumberType> result;
	
	nextToken();

	if (token_.type == TokenType::openTagB) {
		result = element();
	} else {
		throw ParserException("Unexpected Symbol. Expexted '<'");
	}
	
	return result;
}

//std::string Parser<NumberType>::getTokenTypeStringRep() const {
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

template<typename NumberType>
void Parser<NumberType>::nextToken() {
	token_ = lexer_.nextToken();
}

template<typename NumberType>
CSL_Element<NumberType> Parser<NumberType>::element() {
	std::string_view tagIdentifier = openTag();

//	if (token_.type == TokenType::openTagB) {
//		openTag();
//	} else {
//		throw ParserException("Unexpected Symbol. Expected '<' or identifier");	
//	}

	while (token_.type == TokenType::openTagB
			|| token_.type == TokenType::ident) {

		if (token_.type == TokenType::openTagB) {
			element();
		} else if (token_.type == TokenType::ident) {
			genericAssignment();
		}
	}

	if (token_.type == TokenType::openClosingTagB) {
		closeTag(tagIdentifier);
	} else {
		throw ParserException("Unexpected Symbol. Expected \"</\"");	
	}
}

template<typename NumberType>
std::string_view Parser<NumberType>::openTag() {
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

	// TODO: Move the function call out of the if statement and look for performance changes
	if (token_.type == TokenType::closeTagB) {
		nextToken();
	} else {
		throw ParserException("Unexpected Symbol. Expected '>'");
	}

	return result;
}

template<typename NumberType>
void Parser<NumberType>::closeTag(std::string_view tagIdentifier) {
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

template<typename NumberType>
std::variant<CSL_Assignment, CSL_ListAssignment<NumberType>>
Parser<NumberType>::genericAssignment() {
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

template<typename NumberType>
CSL_Assignment Parser<NumberType>::singleAssignment() {
	nextToken();
}

template<typename NumberType>
CSL_ListAssignment<NumberType> Parser<NumberType>::listAssignment() {
	nextToken();

	if (token_.type == TokenType::number) {
		nextToken();
	} else {
		throw ParserException("Unexpected Symbol. Expected number");
	}

	while (token_.type == TokenType::semicolon) {
		nextToken();

		if (token_.type == TokenType::number) {
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
}

