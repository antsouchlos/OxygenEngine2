#ifndef PARSER_HPP
#define PARSER_HPP


#include <string>
#include <string_view>
#include <vector>
#include <stdexcept>
#include <variant>

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

template<typename NumberType = float>
struct CSL_ListAssignment {
	std::string_view name;
	std::vector<NumberType> elements;
};

struct CSL_Assignment {
	std::string_view name;
	std::string_view element;

	bool listAssignment = false;
};

struct CSL_Attribute {
	std::string_view name;
	std::string_view value;
};

template<typename NumberType = float>
struct CSL_Element {
	std::vector<CSL_Attribute> attributes;

	std::vector<CSL_Element> children;

	std::vector<std::variant<CSL_Assignment, CSL_ListAssignment<NumberType>>> assignments;
};

template<typename NumberType = float>
class Parser {
	public:
		Parser(std::string &input) : lexer_(input) {};

		CSL_Element<NumberType> parse();
	private:
		Lexer lexer_;

		Token token_;


//		std::string getTokenTypeStringRep() const;


		void nextToken(); 

		CSL_Element<NumberType> element();
		std::string_view openTag();
		void closeTag(std::string_view tagIdentifier);

		std::variant<CSL_Assignment, CSL_ListAssignment<NumberType>> genericAssignment();
		CSL_Assignment singleAssignment();
		CSL_ListAssignment<NumberType> listAssignment();
};


#include "Parser.cpp"


#endif
