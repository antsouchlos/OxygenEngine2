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
		Lexer(std::string_view input) : input_(input) {};

		Token nextToken();
	private:
		const std::string_view input_;

		const std::array<char, 3> whitespaceChars_ = {{ ' ', '\t', '\n' }};
		/*const std::array<char, 53> identHeadChars = {{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
			'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
			'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
			'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '_' }};
		const std::array<char, 63> identTailChars = {{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
			'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y',
			'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
			'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '_', '0', '1', '2', '3', '4', '5',
			'6', '7', '8', '9' }};
*/
		long long index_ = 0;


		TokenType nextTokenType_;
		std::string_view nextTokenContent_;


		constexpr bool isIdentifierHeadChar(int offset = 0) const noexcept;
		constexpr bool isIdentifierTailChar(int offset = 0) const noexcept;

		constexpr bool isEOS(int offset = 0) const noexcept;

		void skipWhitespace() noexcept;

		void identifier() noexcept;

		void value() noexcept;

		void openBracket() noexcept;
		void closeBracket() noexcept;
		void eq() noexcept;
		void semicolon() noexcept;

		void commentStartSlash() noexcept;
		void commentStart() noexcept;
		void slash() noexcept;

		void eos() noexcept;
};
