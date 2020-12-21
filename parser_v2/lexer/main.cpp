#include <iostream>
#include <fstream>
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
		case TokenType::number:
			return "Number";
			break;
		case TokenType::openTagB:
			return "Opening Tag Bracket";
			break;
		case TokenType::closeTagB:
			return "Closing Tag Bracket";
			break;
		case TokenType::openListB:
			return "Opening List Bracket";
			break;
		case TokenType::closeListB:
			return "Closing List Bracket";
			break;
		case TokenType::eq:
			return "Equal Sign";
			break;
		case TokenType::comma:
			return "Comma";
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
		default:
			return "[Unknown Type]";
	}
}

int main(int argc, char *argv[9]) {
	if (argc < 2) {
		std::cout << "Not enough arguments provided" << std::endl;
		return 1;
	}

	std::string s = "";
	std::ifstream f(argv[1]);

	std::string line;
	while(getline(f, line))
		s += line;

	f.close();

	Lexer l(s);


	Token t;

	try {
		while (t.type != TokenType::eos) {
			t = l.nextToken();
			//std::cout << getTokenTypeStringRep(t.type) << ": \"" << t.content << "\"\n";
		}
	} catch(LexerException &e) {
		std::cerr << "LexerException occurred. Message: " << e.what() << '\n';
	}

	return 0;
}
