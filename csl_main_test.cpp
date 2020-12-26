//#include <OE_API.h>
#include <string_view>
#include <fstream>
#include <string>

#ifdef CPP20
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
}

int main(){
	constexpr csl::token_def lt_def				= { csl::token_type::lt,			[](auto& it) { return *it == '<'; }, [](auto& it) {} };
	constexpr csl::token_def gt_def				= { csl::token_type::gt,			[](auto& it) { return *it == '>'; }, [](auto& it) {} };
	constexpr csl::token_def open_brace_def		= { csl::token_type::open_brace,	[](auto& it) { return *it == '{'; }, [](auto& it) {} };
	constexpr csl::token_def close_brace_def	= { csl::token_type::close_brace,	[](auto& it) { return *it == '}'; }, [](auto& it) {} };
	constexpr csl::token_def eq_def				= { csl::token_type::eq,			[](auto& it) { return *it == '='; }, [](auto& it) {} };
	constexpr csl::token_def semicolon_def		= { csl::token_type::semicolon,		[](auto& it) { return *it == ';'; }, [](auto& it) {} };
	constexpr csl::token_def comma_def			= { csl::token_type::comma,			[](auto& it) { return *it == ','; }, [](auto& it) {} };
	constexpr csl::token_def slash_def			= { csl::token_type::slash,			[](auto& it) { return *it == '/'; }, [](auto& it) {} };

	constexpr csl::token_def lt_slash_def		= { csl::token_type::lt_slash,		[](auto& it) { return true; }, [](auto& it) {} };

	constexpr csl::token_def string_def = {
		csl::token_type::string,
		[](auto &it) { return *it == '"'; },
		[](auto &it) {
			while(*it != '"') it++;
			it++;
		}
	};
	
	constexpr csl::token_def identifier_def = {
		csl::token_type::identifier,
		[](auto &it) { return is_identifier_head_char(*it); },
		[](auto &it) { while (is_identifier_tail_char(*it)) it++;  }
	};

	constexpr csl::token_def integer_def = {
		csl::token_type::integer,
		[](auto &it) { return is_number(*it); },
		[](auto &it) { while(is_number(*it)) it++;  }
	};

	
	std::string input = "";
	std::ifstream f("test.csl");

	std::string line;
	while(getline(f, line))
		input += line;

	f.close();

#ifdef CPP20
	csl::Generic_Lexer<lt_def, gt_def, open_brace_def, close_brace_def, eq_def, semicolon_def, comma_def, slash_def, string_def, identifier_def, integer_def, gt_def, open_brace_def> lexer(input);
#else
	csl::Generic_Lexer lexer(input, lt_def, gt_def, open_brace_def, close_brace_def, eq_def, semicolon_def, comma_def, slash_def, string_def, identifier_def, integer_def, gt_def, open_brace_def);
#endif

	csl::token t = lexer.next_token();

	while (t.type != csl::token_type::eoi) {
//		std::cout << t.type << ": " << t.content << std::endl;
		t = lexer.next_token();
	}

	std::cout << "end" << std::endl;

	return 0;
}
