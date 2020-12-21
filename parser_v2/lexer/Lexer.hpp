#include <string_view>
#include <array>
#include <algorithm>
#include <stdexcept>


enum class TokenType {ident, value, openB, closeB, eq, semicolon, comment, slash, eos, undef};

struct Token {
	TokenType type;
	std::string_view content;

//	TokenType type = TokenType::ident;	// TODO: Check performance improvement, if type is
//											initialized to identifier, and not set in the
//											identifier() function
};


class LexerException : std::exception {
	public:
		LexerException(const char *msg) : msg_(msg) {};

		const char *what() const throw () { return msg_; }
	private:
		const char* msg_;
};


/*
 *
 * identifier			= identifierhead
 *
 * identifierHead		= [a-zA-Z_]
 * 						| [a-zA-Z_] identifierTail
 *
 * identifierTail		= [a-zA-Z_1-9]
 * 						| [a-zA-Z_1-9] identifierTail
 *
 * value				= "\"" * "\""
 * openBracket			= "<"
 * closeBracket			= ">"
 * eq					= "="
 * semicolon			= ";"
 * commentStartSlash	= "/" commentStart
 * 						| "/" slash
 * commentStart 		= "*"
 * (commentEnd			= "* /")	//Only aplies when inside a comment
 * slash				= eps	// Conditional epsilon, so to speak. Only takes effect,
 * 								// if the next char isn't *
 * */


class Lexer {
	public:
		// A string is passed by reference and not a string_view, to make sure there
		// is a terminating character, which this lexer depends on
		Lexer(const std::string &input) : input_(input), currentChar_(input.at(0)),
										iter_(std::begin(input_)) {

			if (input.size() == 0)
				throw LexerException("The input string length must be greater than 0");
		}

		Token nextToken();
	private:
		const std::array<char, 3> whitespaceChars_ = {{ ' ', '\t', '\n' }};

		const std::string_view input_;
		std::size_t index_ = 0;

		std::string_view::iterator iter_;
		char currentChar_;

		TokenType nextTokenType_;
		std::string_view nextTokenContent_;


		char getChar() const;
		
		void setNextTokenContent(std::string_view::iterator it1, std::string_view::iterator t2);

		bool isIdentifierHeadChar() const;
		bool isIdentifierTailChar() const;

		bool isEOS() const;

		void skipWhitespace();

		void identifier();

		void value();

		void openBracket();
		void closeBracket();
		void eq();
		void semicolon();

		void commentStartSlash();
		void commentStart();
		void slash();

		void eos();
};
