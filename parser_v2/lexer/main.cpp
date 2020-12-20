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

	std::string_view sv = s;

	Lexer l(sv);

/*	std::cout << "a: " << static_cast<int>('a') << std::endl;
	std::cout << "b: " << static_cast<int>('z') << std::endl;
	std::cout << "A: " << static_cast<int>('A') << std::endl;
	std::cout << "Z: " << static_cast<int>('Z') << std::endl;
	std::cout << "0: " << static_cast<int>('0') << std::endl;
	std::cout << "9: " << static_cast<int>('9') << std::endl;
	std::cout << "_: " << static_cast<int>('_') << std::endl;
*/


	/*std::cout << "[DEBUG] TokenType::openB: " << static_cast<int>(TokenType::openB) << std::endl;
	std::cout << "[DEBUG] TokenType::closeB: " << static_cast<int>(TokenType::closeB) << std::endl;
	std::cout << "[DEBUG] TokenType::eq: " << static_cast<int>(TokenType::eq) << std::endl;
	std::cout << "[DEBUG] TokenType::slash: " << static_cast<int>(TokenType::slash) << std::endl;
	std::cout << "[DEBUG] TokenType::comStart: " << static_cast<int>(TokenType::comStart) << std::endl;
	std::cout << "[DEBUG] TokenType::comEnd: " << static_cast<int>(TokenType::comEnd) << std::endl;
	std::cout << "[DEBUG] TokenType::value: " << static_cast<int>(TokenType::value) << std::endl;
	std::cout << "[DEBUG] TokenType::ident: " << static_cast<int>(TokenType::ident) << std::endl;
	std::cout << static_cast<int>(l.nextToken().type) << std::endl;
	std::cout << static_cast<int>(l.nextToken().type) << std::endl;
	std::cout << static_cast<int>(l.nextToken().type) << std::endl;
	std::cout << static_cast<int>(l.nextToken().type) << std::endl;
	std::cout << static_cast<int>(l.nextToken().type) << std::endl;
	std::cout << static_cast<int>(l.nextToken().type) << std::endl;
	std::cout << static_cast<int>(l.nextToken().type) << std::endl;

	Token t = l.nextToken();

	std::cout << static_cast<int>(t.type) << std::endl;
	std::cout << t.content << std::endl;
	
	t = l.nextToken();

	std::cout << static_cast<int>(t.type) << std::endl;
	std::cout << t.content << std::endl;
	*/

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
