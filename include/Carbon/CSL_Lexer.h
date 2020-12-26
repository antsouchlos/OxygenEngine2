#include <iostream>
#include <string_view>
#include <string>
#include <fstream>
#include <array>


namespace csl {
	class token_type_def {
		public:
			static const unsigned int eoi	= 0;
			static const unsigned int undef	= 1;
	};

//	template<class condition, class action>
	struct token_def {
		std::size_t type = token_type_def::undef;

		bool (*condition)(std::string_view::iterator&);
		void (*action)(std::string_view::iterator&);
	};

	struct token {
		std::size_t type = token_type_def::undef;
		std::string_view content;
	};


	template<typename... T>
	class Generic_Lexer {
		public:
			Generic_Lexer(std::string& input, T... t_defs)
			    : input_(input), input_it_(input_.begin()), t_defs_{t_defs...} {};

			token next_token() {
				if (input_it_ == input_.end())
					return {token_type_def::eoi, std::string_view(*&input_it_, 0)};

			    next_token_type_ = token_type_def::undef;

			    auto temp = input_it_;
				for (auto t_def : t_defs_) {
					if (t_def.condition(input_it_)) {
						next_token_type_ = t_def.type;

						input_it_++;
						t_def.action(input_it_);

						break;
					}
				}

				if (next_token_type_ == token_type_def::undef)
					input_it_++;
					

			    return {next_token_type_, std::string_view(*&temp, input_it_ - temp)};
			}
		private:
			const std::string_view input_;
			std::string_view::iterator input_it_;

			std::size_t next_token_type_ = token_type_def::undef;

			const std::array<token_def, sizeof...(T)> t_defs_;
	};
}



