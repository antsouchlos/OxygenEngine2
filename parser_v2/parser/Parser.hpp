#ifndef PARSER_HPP
#define PARSER_HPP


#include <string>
#include <string_view>
#include <vector>

#include "Lexer.hpp"


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
* elementContent		= element | assignment
*
* openTag				= "<" IDENTIFIER assignment* ">" 
* closeTag				= "</" IDENTIFIER ">"
*
* assignment			= IDENTIFIER "=" signleAssignmentTail
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

struct CSL_GenericAssignment {};

template<typename T>
struct CSL_Assignment : CSL_GenericAssignment {
	std::string_view name;
	T value;
};

template<typename T>
struct CLS_ListAssignment : CSL_GenericAssignment {
	std::string_view name;
	std::vector<T> elements;
};

struct CSL_Attribute {
	std::string_view name;
	std::string_view value;
};

struct CSL_Element {
	std::vector<CSL_Attribute> attributes;

	std::vector<CSL_Element> children;

	std::vector<CSL_GenericAssignment> assignments;
};


class Parser {
	public:
		Parser(std::string &input) : lexer_(input) {};

		
	private:
		Lexer lexer_;
};


#endif
