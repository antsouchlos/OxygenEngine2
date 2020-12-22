#ifndef PARSER_HPP
#define PARSER_HPP


#include <string>
#include <string_view>
#include <vector>
#include <stdexcept>

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

		CSL_Element parse();	
	private:
		Lexer lexer_;

		Token token_;


		std::string getTokenTypeStringRep() const;


		void nextToken(); 

		void element();
		std::string_view openTag();
		void closeTag(std::string_view tagIdentifier);

		void genericAssignment();
		void singleAssignment();
		void listAssignment();
};


#endif
