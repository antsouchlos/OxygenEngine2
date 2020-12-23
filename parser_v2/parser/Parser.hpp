#ifndef PARSER_HPP
#define PARSER_HPP


#include <string>
#include <string_view>
#include <vector>
#include <stdexcept>
#include <variant>
#include <charconv>
#include <math.h>


#include "Lexer.hpp"


class ParserException : std::exception {
	public:
		ParserException(const char *msg) : msg_(msg) {};

		const char *what() const throw() { return msg_; }; 
	private:
		const char *msg_; 
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
	std::string_view name;
	std::string_view element;
};


struct CSL_Element {
	std::string_view name;
	std::vector<CSL_GenericAssignment_ptr> attributes;

	std::vector<CSL_Element_ptr> elements;
	std::vector<CSL_GenericAssignment_ptr> assignments;
};


class Parser {
	public:
		Parser(std::string &input) : lexer_(input) {};

		CSL_Element_ptr parse();
	private:
		Lexer lexer_;

		Token token_;


//		std::string getTokenTypeStringRep() const;

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


#include "Parser.cpp"


#endif
