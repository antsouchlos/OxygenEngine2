std::unique_ptr<CSL_Element> Parser::parse() {
	std::unique_ptr<CSL_Element> result;
	
	nextToken();

	if (token_.type == TokenType::openTagB) {
		result = std::move(element());
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

float Parser::parseFloat() const {
//	std::string s(token_.content);
//	return std::stof(s);

	// Although the standard defines it for c++17, g++ does not implement
	// std::from_chars(const char*, const char*, double), only
	// std::from_chars(const char*, const char*, int), so we have to get a little bit
	// hacky here. Floats are parsed in terms of two ints: One before and one after
	// the dot

	std::size_t i1 = 0;
	std::size_t i2 = 0;

	auto [c1, ec1] = std::from_chars(std::begin(token_.content), std::end(token_.content), i1);
//	// TODO: Is this necessary or even desireable?
//	if (ec1 == std::errc()) {
//		throw ParserException("Unexpected character in number");
//	}

	std::size_t length1  = c1 - std::begin(token_.content);

	auto [c2, ec2]
		= std::from_chars(std::begin(token_.content) + length1 + 1, std::end(token_.content), i2);
//	// TODO: Is this necessary or even desireable?
//	if (ec2 == std::errc()) {
//		throw ParserException("Unexpected character in number");
//	}
	
	//std::size_t length2 = ceil( (i2 ? log10(i2) : 1) );
	std::size_t length2  = c2 - (std::begin(token_.content) + length1 + 1);

	float f = i1 + i2 / static_cast<float>(pow(10, length2));

	return f;
}

std::size_t Parser::parseInt() const {
	std::size_t i;

	auto [c1, ec1] = std::from_chars(std::begin(token_.content), std::end(token_.content), i);

//	// TODO: Is this necessary or even desireable?
//	if (ec1 == std::errc()) {
//		throw ParserException("Unexpected character in number");
//	}
//
	
	return i;
}
/**/
std::unique_ptr<CSL_Element> Parser::element() {
	std::unique_ptr<CSL_Element> result = std::make_unique<CSL_Element>();

	std::pair< std::string_view, std::vector<CSL_GenericAssignment> > openTagResult = openTag();

	result->name = openTagResult.first;

	std::move(openTagResult.second.begin(), openTagResult.second.end(),
			std::back_inserter(result->attributes));

	while (token_.type == TokenType::openTagB
			|| token_.type == TokenType::ident) {

		if (token_.type == TokenType::openTagB) {
			result->elements.push_back(element());
		} else if (token_.type == TokenType::ident) {
			result->assignments.push_back(genericAssignment());
		}
	}

	if (token_.type == TokenType::openClosingTagB) {
		closeTag(result->name);
	} else {
		throw ParserException("Unexpected Symbol. Expected \"</\"");	
	}

	return result;
}

std::pair< std::string_view, std::vector<CSL_GenericAssignment> >
Parser::openTag() {
	std::pair< std::string_view, std::vector<CSL_GenericAssignment> > result;

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

CSL_GenericAssignment Parser::genericAssignment() {
	std::string_view name = token_.content;

	nextToken();

	if (token_.type == TokenType::eq) {
		nextToken();

		if ( (token_.type == TokenType::floatingPoint)
				|| (token_.type == TokenType::integer)) {
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

std::unique_ptr<CSL_Assignment> Parser::singleAssignment(std::string_view name) {
	std::unique_ptr<CSL_Assignment> result = std::make_unique<CSL_Assignment>();

	// TODO: Do this with an initialization
	result->name = name;
	result->element = token_.content;

	nextToken();

	return result;
}


std::unique_ptr<CSL_ListAssignment> Parser::listAssignment(std::string_view name) {
	std::unique_ptr<CSL_ListAssignment> result = std::make_unique<CSL_ListAssignment>();
	// TODO: Do this with an initialization
	result->name = name;

	nextToken();

	if (token_.type == TokenType::integer) {
		result->elements.push_back(parseInt());
		nextToken();
	} else if (token_.type == TokenType::floatingPoint) {
		result->elements.push_back(parseFloat());
		nextToken();
	} else {
		throw ParserException("Unexpected Symbol. Expected number");
	}

	while (token_.type == TokenType::semicolon) {
		nextToken();
		if (token_.type == TokenType::integer) {
			result->elements.push_back(parseInt());
			nextToken();
		} else if (token_.type == TokenType::floatingPoint) {
			result->elements.push_back(parseFloat());
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
