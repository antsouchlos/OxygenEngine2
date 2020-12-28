#ifndef PARSER_HPP
#define PARSER_HPP

#include <map>
#include <vector>
#include <variant>
#include <sstream>

#include <Carbon/CSL_Exceptions.h>

#ifdef CPP20
	#include <Carbon/CSL_Lexer_cpp20.h>
#else
	#include <Carbon/CSL_Lexer.h>
#endif


/*
* -------------------------- Grammar ------------------------
*
* S						= element
*
* element				= openTag (elementContent)* closeTag	// special requirement: first
* 																// identifiers of openTag
* 																// and closeTag must be
* 																// identical
*
* elementContent		= element | genericAssignment
*
* openTag				= "<" IDENTIFIER genericAssignment* ">" 
* closeTag				= "</" IDENTIFIER ">"
*
* genericAssignment		= IDENTIFIER "=" signleAssignmentTail
* 						| IDENTIFIER "=" listAssignmentTail
*
* singleAssignmentTail	= VALUE
* 																	
* listAssignmentTail	= "{" NUMBER "}"
* 						| "{" NUMBER (";" NUMBER)* "}"	    	// TODO: Remove semicolon. This
* 						    		    						// will require a rewrite of
*                                                               // the blender plugin
* value					= NUMBER | STRING
*/


namespace csl {

	/* 
	 *
	 * ---------------------------------
	 * Define the behaviour of the Lexer
	 * ---------------------------------
	 *
	 */

	class token_type : public token_type_predef {
		public:
			static const token_type_t lt				= 3;
			static const token_type_t gt				= 4;
			static const token_type_t open_brace		= 5;
			static const token_type_t close_brace		= 6;
			static const token_type_t eq				= 7;
			static const token_type_t semicolon			= 8;
			static const token_type_t comma				= 9;
			static const token_type_t slash				= 10;
			
			static const token_type_t string			= 11;
			
			static const token_type_t lt_slash			= 12;
			static const token_type_t comment			= 13;
			
			static const token_type_t identifier		= 14;
			static const token_type_t integer			= 15;
			static const token_type_t floating_point	= 16;

			static const token_type_t whitespace		= 17;
	};

	inline bool is_identifier_head_char(char c) {
		int n = static_cast<int>(c);

		if (	( (65<=n) && (n<=90) )
			|| 	( (97<=n) && (n<=122) )
			||	( n==95 )	) {

			return true;
		} else {
			return false;
		}
	}

	inline bool is_identifier_tail_char(char c) {
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

	inline bool is_number (char c) {
		int n = static_cast<int>(c);

		if ( (48<=n) && (n<=57) ) {
			return true;
		} else {
			return false;
		}
	};

	inline bool is_whitespace(char c) {
		return ( (c==' ') || (c=='\t') ||  (c=='\n') );
	}


	constexpr token_def lt_def				= { token_type::lt,
			   								[](auto& it, auto& end) { return *it == '<'; },
			   								[](auto& it, auto& end) {} };
	constexpr token_def gt_def				= { token_type::gt,
			   								[](auto& it, auto& end) { return *it == '>'; },
			   								[](auto& it, auto& end) {} };
	constexpr token_def open_brace_def		= { token_type::open_brace,
			   								[](auto& it, auto& end) { return *it == '{'; },
			   								[](auto& it, auto& end) {} };
	constexpr token_def close_brace_def	= { token_type::close_brace,
			   								[](auto& it, auto& end) { return *it == '}'; },
			   								[](auto& it, auto& end) {} };
	constexpr token_def eq_def				= { token_type::eq,
			   								[](auto& it, auto& end) { return *it == '='; },
			   								[](auto& it, auto& end) {} };
	constexpr token_def semicolon_def		= { token_type::semicolon,
			   								[](auto& it, auto& end) { return *it == ';'; },
			   								[](auto& it, auto& end) {} };
	constexpr token_def comma_def			= { token_type::comma,
			   								[](auto& it, auto& end) { return *it == ','; },
			   								[](auto& it, auto& end) {} };
	constexpr token_def slash_def			= { token_type::slash,
			   								[](auto& it, auto& end) { return *it == '/'; },
			   								[](auto& it, auto& end) {} };
	constexpr token_def whitespace_def		= { token_type_predef::skip,
											[](auto& it, auto& end) {
												return is_whitespace(*it);
											},
											[](auto& it, auto& end){} };

	constexpr token_def lt_slash_def		= { token_type::lt_slash,
											[](auto& it, auto& end) {
												if (*it == '<') {
													if (*(++it) == '/')
														return true;
													else
														it--;
												}

												return false;
											},
											[](auto& it, auto& end) {} };

	constexpr token_def string_def = {
		token_type::string,
		[](auto& it, auto& end) { return *it == '"'; },
		[](auto& it, auto& end) {
			while(*it != '"') it++;
			it++;
		}
	};
	
	constexpr token_def identifier_def = {
		token_type::identifier,
		[](auto& it, auto& end) { return is_identifier_head_char(*it); },
		[](auto& it, auto& end) { while (is_identifier_tail_char(*it)) it++;  }
	};

	constexpr token_def integer_def = {
		token_type::integer,
		[](auto& it, auto& end) { return ( is_number(*it) || (*it == '-') ); },
		[](auto& it, auto& end) { while(is_number(*it)) it++;  }
	};

	// TODO: Can this somehow be combined with the integer type, in order to not have to look
	// at any character twice?
	constexpr token_def floating_point_def = {
		token_type::floating_point,
		[](auto& it, auto& end) {
			auto temp = it;

			if (*it == '-')
				it++;

			while((it != end) && is_number(*it))
				it++;

			if (*it != '.') {
				it = temp;
				return false;
			}

			it++;

			while(is_number(*it))
				it++;

			// Needed, because, in contrast to the action lambda, after the condition lambda
			// another character gets consumed by the lexer
			it--;

			return true;
		},
		[](auto& it, auto& end) {}
	};


#ifdef CPP20
	using Lexer = Generic_Lexer<whitespace_def, floating_point_def, integer_def, semicolon_def,
						eq_def, open_brace_def, close_brace_def, identifier_def, lt_slash_def,
						lt_def, gt_def, slash_def, string_def, comma_def>;
#else
	using Lexer = Generic_Lexer<token_def, token_def, token_def, token_def, token_def, token_def,
						token_def, token_def, token_def, token_def, token_def, token_def,
						token_def, token_def>;
#endif

	
	/* 
	 *
	 * --------------------------------
	 * -------- Error handling --------
	 * --------------------------------
	 *
	 */

	inline std::string get_token_type_string_rep(const token_type_t t) {
		switch(t) {
			case token_type::identifier:
				return "Identifier, ";
				break;
			case token_type::string:
				return "String, ";
				break;
			case token_type::integer:
				return "Integer, ";
				break;
			case token_type::floating_point:
				return "Float, ";
				break;
			case token_type::lt:
				return "'<', ";
				break;
			case token_type::gt:
				return "'>', ";
				break;
			case token_type::lt_slash:
				return "\"</\", ";
				break;
			case token_type::open_brace:
				return "'{', ";
				break;
			case token_type::close_brace:
				return "'}', ";
				break;
			case token_type::eq:
				return "'=', ";
				break;
			case token_type::comma:
				return "',', ";
				break;
			case token_type::semicolon:
				return "';', ";
				break;
			case token_type::comment:
				return "Comment, ";
				break;
			case token_type::slash:
				return "'/', ";
				break;
			case token_type::eoi:
				return "EOI, ";
				break;
			case token_type::undef:
				return "Undefined Token, ";
				break;
			default:
				return "[Unknown Type]";
		}
	}

	// Sadly, ParseError cannon inherit from std::exception, since that would not allow the
	// return type to be a string, which is necessary in this case in order to get a proper
	// error message (Returning "result_ss.str().c_str()" doesn't work, since it returns a
	// pointer to an object that gets destroyed a soon as the scope of "what()" is left,
	// thereby basically returning garbage)
	class parser_error {
		public:
			virtual std::string what() const throw() = 0;
			virtual ~parser_error() = default;
	};

	class unexpected_symbol_error : parser_error {
		public:
			unexpected_symbol_error(std::string unexpected, std::string expected,
									std::size_t line_num, std::size_t col_num)
				: unexpected_(unexpected), expected_(expected), line_num_(line_num),
					col_num_(col_num) {};

			std::string what() const throw() {
				std::stringstream result_ss;

				result_ss << "Unexpetcted Symbol \"" << unexpected_ << "\" ";
				result_ss << "at line " << line_num_ << ':' << col_num_ << ": ";
				result_ss << "Expected " << expected_;

				return result_ss.str();
			}
		private:
			std::string unexpected_;
			std::string expected_;

			const std::size_t line_num_;
			const std::size_t col_num_;
	};

	class semantic_error : parser_error{
		public:
			semantic_error(const char* msg) : msg_(msg){}
			std::string what() const throw() { return msg_; }
		private:
			const char* msg_;
	};


	/*
	 *
	 * ----------------------------------
	 * ----- Parser data structures -----
	 * ----------------------------------
	 *
	 */

	template<class T>
	using parser_map_t = std::map<std::string_view, T>;

	using single_assignment_t = std::string_view;
	using list_assignment_t = std::vector<std::string_view>;

	using generic_assignment_t = std::variant<single_assignment_t, list_assignment_t>;

	struct element {
		parser_map_t<element> elements;

		parser_map_t<single_assignment_t> attributes;

		parser_map_t<single_assignment_t> single_assignments;
		parser_map_t<list_assignment_t> list_assignments;
	};
	

	/*
	 *
	 * ----------------------------------
	 * ------------- Parser -------------
	 * ----------------------------------
	 *
	 */

	// TODO: See if this can be implemented with a FSM
	// TODO: How does performance change if the values aren't return but set with a reference
	// that was passed in as an argument?
	class Parser {
		public:
#ifdef CPP20
			Parser(std::string& input) : lexer_(input), token_it_(lexer_.begin()) {}
#else
			Parser(std::string& input) : lexer_(input, whitespace_def, floating_point_def,
					integer_def, semicolon_def, eq_def, open_brace_def, close_brace_def,
					identifier_def, lt_slash_def, lt_def, gt_def, slash_def, string_def,
					comma_def), token_it_(lexer_.begin()) {}
#endif
			element parse() {
				expect_token(token_type::lt);
				token_it_++;

				return parse_element();
			}
		private:
			Lexer lexer_;
			Lexer::iterator token_it_;

			element parse_element() {
				element result;


				// Parse opening tag


				expect_token(token_type::identifier);

				std::string_view el_name = token_it_->content;
				token_it_++;

				while(has_type(token_type::identifier)) {
					std::string_view att_name = token_it_->content;
					token_it_++;
					
					expect_token(token_type::eq);
					token_it_++;

					// TODO: Is emplace_back (or the equivalent for maps) applicable here?
					result.attributes[att_name] = parse_single_assignment();
				}

				expect_token(token_type::gt);
				token_it_++;


				// Parse element content


				while (token_it_ != lexer_.end()) {
					if (has_type(token_type::identifier)) {
						std::string_view as_name = token_it_->content;

						token_it_++;

						expect_token(token_type::eq);
						token_it_++;

						generic_assignment_t as_generic = parse_assignment();

						// TODO: Is access by index more efficient? This option weirdly seems to
						// not to documented as well
						// TODO: Is emplace_back (or the equivalent for maps) applicable here?
						if (std::holds_alternative<single_assignment_t>(as_generic)) {
							result.single_assignments[as_name]
								= std::get<single_assignment_t>(as_generic);
						} else if (std::holds_alternative<list_assignment_t>(as_generic)) {
							result.list_assignments[as_name]
								= std::get<list_assignment_t>(as_generic);
						}
					} else if (has_type(token_type::lt)) {
						token_it_++;
						std::string_view sub_el_name = token_it_->content;
						result.elements[sub_el_name] = parse_element();
					} else {
						break;
					}
				}


				// Parse closing tag


				expect_token(token_type::identifier, token_type::lt, token_type::lt_slash);
				token_it_++;

				expect_token(token_type::identifier);

				if (token_it_->content != el_name)
					throw semantic_error("Closing tag identifier does not match opening tag"
						   					" identifier");

				token_it_++;

				expect_token(token_type::gt);
				token_it_++;

				return result;
			}

			generic_assignment_t parse_assignment() {
				// TODO: Check where to put this in order to get more performance
				expect_token(token_type::integer, token_type::floating_point,
								token_type::identifier, token_type::open_brace, token_type::string);

				if (has_type(token_type::open_brace)) {
					token_it_++;
					return parse_list_assignment();
				} else {
					return parse_single_assignment();
				}
			}

			single_assignment_t parse_single_assignment() {
				single_assignment_t value = token_it_->content;
				token_it_++;

				return value;
			}

			list_assignment_t parse_list_assignment() {
				list_assignment_t result;

				if (is_value()) {
					// TODO: Try emplace_back
					result.push_back(token_it_->content);
					token_it_++;

					while(has_type(token_type::semicolon)) {
						token_it_++;
						expect_value();

						// TODO: Try emplace_back
						result.push_back(token_it_->content);
						token_it_++;
					}
				}

				// TODO: Not only '}' is expected
				expect_token(token_type::close_brace);
				token_it_++;
				
				return result;
			}

			template<typename... T>
			void expect_token(T... type) const {
				bool b = (has_type(type) || ...);

				if (!b) {
					// TODO: Test performance, if const char* is used here instead of std::string
					// TODO: Regardles, make all of this evaluate at compile-time
					std::string expected = (get_token_type_string_rep(type) + ...);
					expected.erase(expected.size() - 2, 2);
					std::string unexpected(token_it_->content);

					// TODO
					throw unexpected_symbol_error(unexpected, expected, lexer_.get_line_num(),
												lexer_.get_col_num() - token_it_->content.size());

				}
			}

			// TODO: Rename to match_type? Check literature
			bool has_type(token_type_t type) const {
				return (token_it_->type == type);
			}

			bool is_value() {
				return ( has_type(token_type::integer)
						|| has_type(token_type::floating_point)
						|| has_type(token_type::identifier)
					  	|| has_type(token_type::string) );
			}

			void expect_value() {
				expect_token(token_type::integer, token_type::floating_point,
								token_type::identifier, token_type::string);
			}
	};
	
	// TODO: Forwarding reference?
	inline element parse(std::string& input) {
		Parser parser(input);
		return parser.parse();
	}
}


#endif
