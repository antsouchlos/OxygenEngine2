#include <iostream>
#include <string_view>

#include <string>

#include "Lexer.hpp"

// Only for debug purposes
std::string getTokenTypeStringRep(TokenType t) {
	switch(t) {
		case TokenType::ident:
			return "Identifier";
			break;
		case TokenType::value:
			return "Value";
			break;
		case TokenType::openB:
			return "Opening Bracket";
			break;
		case TokenType::closeB:
			return "Closing Bracket";
			break;
		case TokenType::eq:
			return "Equal Sign";
			break;
		case TokenType::semicolon:
			return "Semicolon";
			break;
		case TokenType::comment:
			return "Comment";
			break;
		case TokenType::slash:
			return "Slash";
			break;
		case TokenType::eos:
			return "EOS";
			break;
		case TokenType::undef:
			return "Undefined";
			break;
	}
}

int main() {
	std::string s =
		"<World name=\"world1\">"
			"<Scene name=\"scene1\">"
				"<Object name=\"object1\">"
					"a = \"3\";"
					"b = \"4\";"
					"c = \"5\";"
					"/* This is just some comment */"
				"</Object>"
			"</Scene>"
		"</World>";

	Lexer l(s);


	Token t;

	try {
		while (t.type != TokenType::eos) {
			t = l.nextToken();
			std::cout << getTokenTypeStringRep(t.type) << ": \"" << t.content << "\"\n";
		}
	} catch(LexerException &e) {
		std::cerr << "LexerException occurred. Message: " << e.what() << '\n';
	}

	return 0;
}
