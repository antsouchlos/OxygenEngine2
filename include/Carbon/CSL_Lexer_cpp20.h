#include <iostream>
#include <string_view>
#include <string>
#include <fstream>


namespace csl {
	class token_type_def {
		public:
			static const unsigned int eoi				= 0;
			static const unsigned int undef				= 1;
			static const unsigned int skip				= 2;
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
	void for_each(std::size_t &type, std::string_view::iterator &it,
					const std::string_view::iterator& end_it, T... cws);

	template<typename T, typename... U>
	void for_each(std::size_t &type, std::string_view::iterator &it,
						const std::string_view::iterator& end_it, T cw, U... cws) {
		if (cw.condition(it, end_it)) {
			type = cw.type;
			it++;
			cw.action(it, end_it);
		} else {
			for_each(type, it, end_it, cws...);
		}
	}

	template<>
	void for_each(std::size_t &type, std::string_view::iterator &it,
									const std::string_view::iterator& end_it) {}


	template<token_def... t_defs>
	class Generic_Lexer {
		public:
			class iterator {
            	public:
					// TODO: Make this forward at some point
					using iterator_category = std::input_iterator_tag;
					using value_type        = token;
					using difference_type   = std::size_t;
					using reference         = token&;
					using pointer           = token*;

					iterator(Generic_Lexer<t_defs...> *lexer)
						: lexer_(lexer) { t_ = lexer_->next_token(); };
					iterator(token t) : t_(t) {}; 

					const token* operator->()   const { return &t_; };
					const token& operator*()    const { return t_; };

					iterator& operator++()      { t_ = lexer_->next_token(); return *this; }
					iterator operator++(int)    { iterator tmp(*this); ++(*this); return tmp; };

					bool operator==(const iterator& rhs) {
						return  (this==&rhs) || ( (t_.type==token_type_def::eoi)
												&& ((*rhs).type==token_type_def::eoi) );
					};
					bool operator!=(const iterator& rhs) { return !((*this) == rhs); }

				private:
					Generic_Lexer<t_defs...> *lexer_;
					token t_; 
			};


			Generic_Lexer(std::string& input)
			    : input_(input), input_it_(input_.begin()), end_it_(input_.end()) {};

			token next_token() {
				if (input_it_ == end_it_)
					return {token_type_def::eoi, std::string_view(*&input_it_, 0)};

			    next_token_type_ = token_type_def::undef;

			    auto temp = input_it_;
			    for_each(next_token_type_, input_it_, end_it_, t_defs...);

				if (next_token_type_ == token_type_def::undef) {
					input_it_++;
				} else if (next_token_type_ == token_type_def::skip) {
					return next_token();
				}
					

			    return {next_token_type_, std::string_view(*&temp, input_it_ - temp)};
			}

			iterator begin() {
				return iterator(this);
			}

			iterator end() {
				return iterator({token_type_def::eoi, std::string_view(*&end_it_, 0)});
			}
		private:
			const std::string_view input_;
			std::string_view::iterator input_it_;
			const std::string_view::iterator end_it_;

			std::size_t next_token_type_ = token_type_def::undef;
	};
}

