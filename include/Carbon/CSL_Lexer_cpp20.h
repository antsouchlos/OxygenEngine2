#include <iostream>
#include <string_view>
#include <string>
#include <fstream>


namespace csl {
	class token_type_def {
		public:
			static const unsigned int eoi				= 0;
			static const unsigned int undef				= 1;
	};

	template<typename T, typename U>
	struct token_def {
		std::size_t type = token_type_def::undef;

		const T condition;
		const U action;
	};

	// Needed for compilation with clang
	template<typename T, typename U>
	token_def(int, T, U) -> token_def<T, U>;

	struct token {
		std::size_t type = token_type_def::undef;
		std::string_view content;
	};


	template<typename... T>
	void for_each(std::size_t &type, std::string_view::iterator &it, T... cws);

	template<typename T, typename... U>
	void for_each(std::size_t &type, std::string_view::iterator &it, T cw, U... cws) {
		if (cw.condition(it)) {
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
					return {token_type_def::eoi, std::string_view(*&input_it_, 0)};

			    next_token_type_ = token_type_def::undef;

			    auto temp = input_it_;
			    for_each(next_token_type_, input_it_, t_defs...);

				if (next_token_type_ == token_type_def::undef)
					input_it_++;
					

			    return {next_token_type_, std::string_view(*&temp, input_it_ - temp)};
			}
		private:
			const std::string_view input_;
			std::string_view::iterator input_it_;

			std::size_t next_token_type_ = token_type_def::undef;
	};
}

