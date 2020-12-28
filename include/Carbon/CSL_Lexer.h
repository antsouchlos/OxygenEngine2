#include <iostream>
#include <string_view>
#include <string>
#include <fstream>
#include <array>


namespace csl {
	using lexer_input_it = std::string_view::iterator;
	using token_type_t = unsigned int;

	class token_type_predef {
		public:
			static const token_type_t eoi	= 0;
			static const token_type_t undef	= 1;
			static const token_type_t skip	= 2;
	};

	struct token_def {
		token_type_t type = token_type_predef::undef;

		bool (*condition)(lexer_input_it&, const lexer_input_it&);
		void (*action)(lexer_input_it&, const lexer_input_it&);
	};

	struct token {
		token_type_t type = token_type_predef::undef;
		std::string_view content;
	};


	template<typename... T>
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

					iterator(Generic_Lexer<T...> *lexer)
						: lexer_(lexer) { t_ = lexer_->next_token(); };
					iterator(token t) : t_(t) {}; 

					const token* operator->()   const { return &t_; };
					const token& operator*()    const { return t_; };

					iterator& operator++()      { t_ = lexer_->next_token(); return *this; }
					iterator operator++(int)    { iterator tmp(*this); ++(*this); return tmp; };

					bool operator==(const iterator& rhs) {
						return  (this==&rhs) || ( (t_.type==token_type_predef::eoi)
												&& ((*rhs).type==token_type_predef::eoi) );
					};
					bool operator!=(const iterator& rhs) { return !((*this) == rhs); }

				private:
					Generic_Lexer<T...> *lexer_;
					token t_; 
			};


			Generic_Lexer(std::string& input, T... t_defs)
			    : input_(input), input_it_(input_.begin()), end_it_(input_.end()),
					t_defs_{t_defs...} {};

			token next_token() {
				if (input_it_ == end_it_)
					return {token_type_predef::eoi, std::string_view(*&input_it_, 0)};

			    next_token_type_ = token_type_predef::undef;

			    auto temp = input_it_;
				for (auto t_def : t_defs_) {
					if (t_def.condition(input_it_, end_it_)) {
						next_token_type_ = t_def.type;

						input_it_++;
						t_def.action(input_it_, end_it_);

						break;
					}
				}

				if (next_token_type_ == token_type_predef::undef) {
					input_it_++;
				} else if (next_token_type_ == token_type_predef::skip) {
					return next_token();
				}

			    return {next_token_type_, std::string_view(*&temp, input_it_ - temp)};
			}

			iterator begin() {
				return iterator(this);
			}

			iterator end() {
				return iterator({token_type_predef::eoi, std::string_view(*&end_it_, 0)});
			}
			
			std::size_t get_line_num() const {
				std::size_t result = 1;

				auto it = input_.begin();
				while(it != input_it_) {
					if (*it == '\n')
						result++;

					it++;
				}

				return result;
			}

			std::size_t get_col_num() const {
				std::size_t result = 1;

				auto it = input_it_;

				while( (*it != '\n') && (it != input_.begin()) ) {
					result++;
					it--;
				}


				return result;
			}
		private:
			const std::string_view input_;
			lexer_input_it input_it_;
			const lexer_input_it end_it_;

			token_type_t next_token_type_ = token_type_predef::undef;

			const std::array<token_def, sizeof...(T)> t_defs_;
	};
}



