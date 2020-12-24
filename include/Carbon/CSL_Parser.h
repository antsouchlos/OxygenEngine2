#ifndef PARSER_HPP
#define PARSER_HPP


#include <string>
#include <string_view>
#include <vector>
#include <stdexcept>
#include <variant>
#include <charconv>
#include <math.h>
#include <memory>

#include <Carbon/CSL_Exceptions.h>
#include <Carbon/CSL_Lexer.h>


struct CSL_ListAssignment;
struct CSL_Assignment;
struct CSL_Element;

using CSL_Element_ptr			= std::unique_ptr<CSL_Element>;
using CSL_Assignment_ptr		= std::unique_ptr<CSL_Assignment>;
using CSL_ListAssignment_ptr	= std::unique_ptr<CSL_ListAssignment>;

using CSL_GenericAssignment_ptr	= std::variant<CSL_Assignment_ptr, CSL_ListAssignment_ptr>;

using CSL_OpenTagResult			= std::pair< std::string_view,
	  								std::vector<CSL_GenericAssignment_ptr> >;

enum CSL_AssignmentType {singleAssignment = 0, listAssignment = 1};
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
*
* // TODO: Clean up the grammar, and make it actually match the current state of the parser
* //		(NUMBER has been split into FLOAT and INT)
*/

// TODO: Change vector of CSL_*Assignments to map and remove std::string_view name

struct CSL_ListAssignment {
	std::string_view name;
	std::vector<float> values;
};

struct CSL_Assignment {
	std::string_view name;
	std::string_view value;
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
