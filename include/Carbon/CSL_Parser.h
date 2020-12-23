#ifndef PARSER_HPP
#define PARSER_HPP


#include <string>
#include <string_view>
#include <vector>
#include <stdexcept>
#include <variant>
#include <charconv>
#include <math.h>


#include <Carbon/Lexer.h>

// Sadly, ParseError cannon inherit from std::exception, since that would not allow the return type
// to be a string, which is necessary in this case in order to get a proper error message
// (Returning "result_ss.str().c_str()" doesn't work, since it returns a pointer to an object that
// gets destroyed a soon as the scope of "what()" is left, thereby basically returning garbage)
class ParserError {
	public:
		virtual std::string what() const throw() { return ""; };
};

class UnexpectedSymbolError : ParserError {
	public:
		UnexpectedSymbolError(std::string_view unexpected, CSL_TokenType expected,
							const std::size_t lineNum, const std::size_t colNum)
			: unexpected_(unexpected), expected_(expected), lineNum_(lineNum), colNum_(colNum) {};

		std::string what() const throw() {
			std::stringstream result_ss;
			result_ss << "Unexpetcted Symbol \"" << unexpected_ << "\" ";
		   	result_ss << "at line " << lineNum_ << ':' << (colNum_ - unexpected_.size()) << ": ";
			result_ss << "Expected \"";
			result_ss << getTokenTypeStringRep(expected_);
			result_ss << "\"";

			return result_ss.str().c_str();
		}
	private:
		std::string_view unexpected_;
		CSL_TokenType expected_;
		const std::size_t lineNum_;
		const std::size_t colNum_;
};

class SemanticError : ParserError {
	public:
		SemanticError(const char* msg) : msg_(msg) {};

		std::string what() const throw() { return msg_; };
	private:
		const char* msg_;
};


struct CSL_ListAssignment;
struct CSL_Assignment;
struct CSL_Element;

using CSL_Element_ptr			= std::unique_ptr<CSL_Element>;
using CSL_Assignment_ptr		= std::unique_ptr<CSL_Assignment>;
using CSL_ListAssignment_ptr	= std::unique_ptr<CSL_ListAssignment>;

using CSL_GenericAssignment_ptr	= std::variant<CSL_Assignment_ptr, CSL_ListAssignment_ptr>;

using CSL_OpenTagResult			= std::pair< std::string_view,
	  								std::vector<CSL_GenericAssignment_ptr> >;

/*
* -------------------------- Grammar ------------------------
*
* S						= element
*
* element				= openTag (elementContent)* closeTag		// special requirement: first
* 																	// identifiers of openTag
* 																	// and closeTag must be
* 																	// identical
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
* 						| "{" NUMBER (";" NUMBER)* "}"	    		// TODO: Remove semicolon. This
* 						    		    							// will require a rewrite of
*                                                                   // the blender plugin
* value					= NUMBER | STRING
*/


struct CSL_ListAssignment {
	std::string_view name;
	std::vector<float> elements;
};

struct CSL_Assignment {
//	CSL_Assignment(std::string_view name_init, std::string_view element_init)
//		: name(name_init), element(element_init) {};

	std::string_view name;
	std::string_view element;
};

struct CSL_Element {
	std::string_view name;
	std::vector<CSL_GenericAssignment_ptr> attributes;

	std::vector<CSL_Element_ptr> elements;
	std::vector<CSL_GenericAssignment_ptr> assignments;
};


class CSL_Parser {
	public:
		CSL_Parser(std::string &input) : lexer_(input) {};

		CSL_Element_ptr parse();
	private:
		CSL_Lexer lexer_;

		CSL_Token token_;


		void nextToken(); 

		float parseFloat() const;
		std::size_t parseInt() const;
		
		CSL_Element_ptr element();
		CSL_OpenTagResult openTag();
		void closeTag(std::string_view tagIdentifier);

		CSL_GenericAssignment_ptr genericAssignment();
		CSL_Assignment_ptr singleAssignment(std::string_view name);
		CSL_ListAssignment_ptr listAssignment(std::string_view name);
};


#endif
