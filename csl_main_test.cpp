//#include <OE_API.h>
#include <Carbon/CSL_Lexer.h>
#include <string_view>
#include <fstream>
#include <string>


int main(){
	constexpr csl::token_def lt_def				= { csl::token_type::lt,			[](char c) { return c == '<'; }, [](auto &it) {} };
	constexpr csl::token_def gt_def				= { csl::token_type::gt,			[](char c) { return c == '>'; }, [](auto &it) {} };
	constexpr csl::token_def open_brace_def		= { csl::token_type::open_brace,	[](char c) { return c == '{'; }, [](auto &it) {} };
	constexpr csl::token_def close_brace_def	= { csl::token_type::close_brace,	[](char c) { return c == '}'; }, [](auto &it) {} };
	constexpr csl::token_def eq_def				= { csl::token_type::eq,			[](char c) { return c == '='; }, [](auto &it) {} };
	constexpr csl::token_def semicolon_def		= { csl::token_type::semicolon,		[](char c) { return c == ';'; }, [](auto &it) {} };
	constexpr csl::token_def comma_def			= { csl::token_type::comma,			[](char c) { return c == ','; }, [](auto &it) {} };
	constexpr csl::token_def slash_def			= { csl::token_type::slash,			[](char c) { return c == '/'; }, [](auto &it) {} };

	constexpr csl::token_def string_def = {
		csl::token_type::string,
		[](char c) { return c == '"'; },
		[](auto &it) {
			while(*it != '"') it++;
			it++;
		}
	};
	
	constexpr csl::token_def identifier_def = {
		csl::token_type::identifier,
		[](char c) { return is_identifier_head_char(c); },
		[](auto &it) { while (is_identifier_tail_char(*it)) it++;  }
	};

	constexpr csl::token_def integer_def = {
		csl::token_type::integer,
		[](char c) { return is_number(c); },
		[](auto &it) { while(is_number(*it)) it++;  }
	};

	
	std::string input = "";
	std::ifstream f("test.csl");

	std::string line;
	while(getline(f, line))
		input += line;

	f.close();


	csl::Generic_Lexer lexer(input, lt_def, gt_def, open_brace_def, close_brace_def, eq_def, semicolon_def, comma_def, slash_def, string_def, identifier_def, integer_def, gt_def, open_brace_def);

	csl::token t = lexer.next_token();

	while (t.type != csl::token_type::eoi) {
//		std::cout << t.type << ": " << t.content << std::endl;
		t = lexer.next_token();
	}

	std::cout << "end" << std::endl;

	return 0;
}
