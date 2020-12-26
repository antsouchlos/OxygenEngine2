#include <iostream>
#include <string_view>
#include <string>
#include <fstream>
#include <array>


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

//	template<class condition, class action>
	struct token_def {
		std::size_t type = token_type::undef;

		bool (*condition)(char);
		void (*action)(std::string_view::iterator&);
	};

	struct token {
		std::size_t type = token_type::undef;
		std::string_view content;
	};


	template<typename... T>
	class Generic_Lexer {
		public:
			Generic_Lexer(std::string& input, T... t_defs)
			    : input_(input), input_it_(input_.begin()), t_defs_{t_defs...} {};

			token next_token() {
				if (input_it_ == input_.end())
					return {token_type::eoi, std::string_view(*&input_it_, 0)};

			    next_token_type_ = token_type::undef;

			    auto temp = input_it_;
				for (auto t_def : t_defs_) {
					if (t_def.condition(*input_it_)) {
						next_token_type_ = t_def.type;

						input_it_++;
						t_def.action(input_it_);

						break;
					}
				}

				if (next_token_type_ == token_type::undef)
					input_it_++;
					

			    return {next_token_type_, std::string_view(*&temp, input_it_ - temp)};
			}
		private:
			const std::string_view input_;
			std::string_view::iterator input_it_;

			std::size_t next_token_type_ = token_type::undef;

			const std::array<token_def, sizeof...(T)> t_defs_;
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

