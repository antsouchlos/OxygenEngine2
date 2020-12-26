//#include <OE_API.h>
#include <string_view>
#include <fstream>
#include <string>

#define CPP20

#define
	#include <Carbon/CSL_Lexer_cpp20.h>
#else
	#include <Carbon/CSL_Lexer.h>
#endif


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


namespace csl {
	class token_type : public token_type_def {
		public:
			static const unsigned int lt				= 3;
			static const unsigned int gt				= 4;
			static const unsigned int open_brace		= 5;
			static const unsigned int close_brace		= 6;
			static const unsigned int eq				= 7;
			static const unsigned int semicolon			= 8;
			static const unsigned int comma				= 9;
			static const unsigned int slash				= 10;
			
			static const unsigned int string			= 11;
			
			static const unsigned int lt_slash			= 12;
			static const unsigned int comment			= 13;
			
			static const unsigned int identifier		= 14;
			static const unsigned int integer			= 15;
			static const unsigned int floating_point	= 16;

			static const unsigned int whitespace		= 17;
	};
}

int main(){
	constexpr csl::token_def lt_def				= { csl::token_type::lt,			[](auto& it, auto& end) { return *it == '<'; }, [](auto& it, auto& end) {} };
	constexpr csl::token_def gt_def				= { csl::token_type::gt,			[](auto& it, auto& end) { return *it == '>'; }, [](auto& it, auto& end) {} };
	constexpr csl::token_def open_brace_def		= { csl::token_type::open_brace,	[](auto& it, auto& end) { return *it == '{'; }, [](auto& it, auto& end) {} };
	constexpr csl::token_def close_brace_def	= { csl::token_type::close_brace,	[](auto& it, auto& end) { return *it == '}'; }, [](auto& it, auto& end) {} };
	constexpr csl::token_def eq_def				= { csl::token_type::eq,			[](auto& it, auto& end) { return *it == '='; }, [](auto& it, auto& end) {} };
	constexpr csl::token_def semicolon_def		= { csl::token_type::semicolon,		[](auto& it, auto& end) { return *it == ';'; }, [](auto& it, auto& end) {} };
	constexpr csl::token_def comma_def			= { csl::token_type::comma,			[](auto& it, auto& end) { return *it == ','; }, [](auto& it, auto& end) {} };
	constexpr csl::token_def slash_def			= { csl::token_type::slash,			[](auto& it, auto& end) { return *it == '/'; }, [](auto& it, auto& end) {} };
	
	constexpr csl::token_def whitespace_def		= { csl::token_type::skip,			[](auto& it, auto& end) { return is_whitespace(*it); }, [](auto& it, auto& end){} };

	constexpr csl::token_def lt_slash_def = {
		csl::token_type::lt_slash,
		[](auto& it, auto& end) {
			if (*it == '<') {
				if (*(++it) == '/') {
					return true;
				} else {
					it--;
				}
			}

			return false;
		},
		[](auto& it, auto& end) {}
	};

	constexpr csl::token_def string_def = {
		csl::token_type::string,
		[](auto &it, auto& end) { return *it == '"'; },
		[](auto &it, auto& end) {
			while(*it != '"') it++;
			it++;
		}
	};
	
	constexpr csl::token_def identifier_def = {
		csl::token_type::identifier,
		[](auto &it, auto& end) { return is_identifier_head_char(*it); },
		[](auto &it, auto& end) { while (is_identifier_tail_char(*it)) it++;  }
	};

	constexpr csl::token_def integer_def = {
		csl::token_type::integer,
		[](auto &it, auto& end) { return is_number(*it); },
		[](auto &it, auto& end) { while(is_number(*it)) it++;  }
	};

	
	std::string input = "";
	std::ifstream f("test.csl");

	std::string line;
	while(getline(f, line))
		input += line;

	f.close();

#ifdef CPP20
	csl::Generic_Lexer<lt_slash_def, lt_def, gt_def, open_brace_def, close_brace_def, eq_def, semicolon_def, whitespace_def,
						comma_def, slash_def, string_def, identifier_def, integer_def, gt_def, open_brace_def> lexer(input);
#else
	csl::Generic_Lexer lexer(input, lt_slash_def, lt_def, gt_def, open_brace_def, close_brace_def, eq_def, semicolon_def, whitespace_def,
						comma_def, slash_def, string_def, identifier_def, integer_def, gt_def, open_brace_def);
#endif


	for (const auto& t : lexer) {
//		std::cout << t.type << ": " << t.content << std::endl;
	}

	return 0;
}
