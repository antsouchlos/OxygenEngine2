#ifndef PARSER_HPP
#define PARSER_HPP


#include <string>

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
* listAssignmentTail	= "{" (value ";")* (value)? "}"				// TODO: Remove semicolon. This
* 																	// will require a rewrite of
* 																	// the blender plugin
* listAssignmentTail	= "{" "}"
* 						| "{" value "}"
* 						| "{" value (";" value)* "}"
*
* value					= NUMBER | STRING
*/

class Parser {
	public:
		Parser(std::string &input) : lexer_(input) {};
	private:
		Lexer lexer_;
};


#endif
