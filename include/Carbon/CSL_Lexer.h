#include <iostream>
#include <string_view>
#include <string>
#include <fstream>


namespace csl {
	class token_type {
		public:
			static const unsigned int eoi				= 0;
			static const unsigned int undef				= 1;

			static const unsigned int lt				= 2;
			static const unsigned int gt				= 3;
			static const unsigned int open_brace		= 4;
			static const unsigned int close_brace		= 5;
			static const unsigned int eq				= 6;
			static const unsigned int semicolon			= 7;
			static const unsigned int comma				= 8;
			static const unsigned int slash				= 9;
			
			static const unsigned int string			= 10;
			
			static const unsigned int lt_slash			= 11;
			static const unsigned int comment			= 12;
			
			static const unsigned int identifier		= 13;
			static const unsigned int integer			= 14;
			static const unsigned int floating_point	= 15;

	};

	template<typename T, typename U>
	struct token_def {
		std::size_t type = token_type::undef;

		const T condition;
		const U action;
	};

	// Needed for compilation with clang
	template<typename T, typename U>
	token_def(int, T, U) -> token_def<T, U>;

	struct token {
		std::size_t type = token_type::undef;
		std::string_view content;
	};


	template<typename... T>
	void for_each(std::size_t &type, std::string_view::iterator &it, T... cws);

	template<typename T, typename... U>
	void for_each(std::size_t &type, std::string_view::iterator &it, T cw, U... cws) {
		if (cw.condition(*it)) {
			type = cw.type;
			it++;
			cw.action(it);
		} else {
			for_each(type, it, cws...);
		}
	}

	template<>
	void for_each(std::size_t &type, std::string_view::iterator &it) {}


	template<token_def... t_defs>
	class Generic_Lexer {
		public:
			Generic_Lexer(std::string& input)
			    : input_(input), input_it_(input_.begin()) {};

			token next_token() {
				if (input_it_ == input_.end())
					return {token_type::eoi, std::string_view(*&input_it_, 0)};

			    next_token_type_ = token_type::undef;

			    auto temp = input_it_;
			    for_each(next_token_type_, input_it_, t_defs...);

				if (next_token_type_ == token_type::undef)
					input_it_++;
					

			    return {next_token_type_, std::string_view(*&temp, input_it_ - temp)};
			}
		private:
			const std::string_view input_;
			std::string_view::iterator input_it_;

			std::size_t next_token_type_ = token_type::undef;
	};
}


bool is_identifier_head_char(char c) {
	int n = static_cast<int>(c);

	if (	( (65<=n) && (n<=90) )
		|| 	( (97<=n) && (n<=122) )
		||	( n==95 )	) {

		return true;
	} else {
		return false;
	}
}

bool is_identifier_tail_char(char c) {
	int n = static_cast<int>(c);

	if (	( (65<=n) && (n<=90) )
		|| 	( (97<=n) && (n<=122) )
		||	( (48<=n) && (n<=57) )
		||	( n==95 )	) {

		return true;
	} else {
		return false;
	}
}

bool is_number (char c) {
	int n = static_cast<int>(c);

	if ( (48<=n) && (n<=57) ) {
		return true;
	} else {
		return false;
	}
};

bool is_whitespace(char c) {
	return ( (c==' ') || (c=='\t') ||  (c=='\n') );
}

