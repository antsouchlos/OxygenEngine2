#include <Carbon/CSL_Exceptions.h>


extern char const CSL_IteratorElementString[] = "element";
extern char const CSL_IteratorAttributeString[] = "attribute";
extern char const CSL_IteratorVariableString[] = "variable";



const char* getTokenTypeStringRep(CSL_TokenType t) {
	switch(t) {
		case CSL_TokenType::ident:
			return "Identifier";
			break;
		case CSL_TokenType::string:
			return "String";
			break;
		case CSL_TokenType::integer:
			return "Integer";
			break;
		case CSL_TokenType::floatingPoint:
			return "Float";
			break;
		case CSL_TokenType::openTagB:
			return "<";
			break;
		case CSL_TokenType::closeTagB:
			return ">";
			break;
		case CSL_TokenType::openClosingTagB:
			return "</";
			break;
		case CSL_TokenType::openListB:
			return "{";
			break;
		case CSL_TokenType::closeListB:
			return "}";
			break;
		case CSL_TokenType::eq:
			return "=";
			break;
		case CSL_TokenType::comma:
			return ",";
			break;
		case CSL_TokenType::semicolon:
			return ";";
			break;
		case CSL_TokenType::comment:
			return "Comment";
			break;
		case CSL_TokenType::slash:
			return "/";
			break;
		case CSL_TokenType::eos:
			return "EOS";
			break;
		case CSL_TokenType::undef:
			return "Undefined Token";
			break;
		default:
			return "[Unknown Type]";
	}
}
