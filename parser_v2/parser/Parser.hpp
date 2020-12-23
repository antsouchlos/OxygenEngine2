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

using CSL_GenericAssignment = std::variant<std::unique_ptr<CSL_Assignment>, std::unique_ptr<CSL_ListAssignment>>;

struct CSL_Element {
	std::string_view name;

	std::vector<CSL_GenericAssignment> attributes;

	std::vector<std::unique_ptr<CSL_Element>> elements;

	std::vector<CSL_GenericAssignment> assignments;
};

class Parser {
	public:
		Parser(std::string &input) : lexer_(input) {};

		std::unique_ptr<CSL_Element> parse();
	private:
		Lexer lexer_;

		Token token_;


//		std::string getTokenTypeStringRep() const;

		void nextToken(); 

		float parseFloat() const;
		std::size_t parseInt() const;
		
		std::unique_ptr<CSL_Element> element();
		std::pair< std::string_view, std::vector<CSL_GenericAssignment> > openTag();
		void closeTag(std::string_view tagIdentifier);

		CSL_GenericAssignment genericAssignment();
		std::unique_ptr<CSL_Assignment> singleAssignment(std::string_view name);
		std::unique_ptr<CSL_ListAssignment> listAssignment(std::string_view name);
};


#include "Parser.cpp"


#endif
