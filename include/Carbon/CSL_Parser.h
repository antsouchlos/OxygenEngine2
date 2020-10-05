/*
 * CSL_Parser.h
 *
 *  Created on: Sep 23, 2015
 *      Author: andreas
 */

#ifndef CSL_PARSER_H_
#define CSL_PARSER_H_

#include <Carbon/CSL_Lexer.h>

struct CSL_Node {
    
    ~CSL_Node();
    std::string type;
    std::string ID;

    std::vector<CSL_Node*> children = {};
    std::vector<std::string> args = {};

    int line = 0;
    int col = 0;
};

class CSL_Parser {
public:
    CSL_Node* parse(std::string str);
    virtual ~CSL_Parser();

    bool isStringStartChar(char c);
    
private:
    CSL_Lexer *lexer;
    CSL_Token token;

    bool definedLexer = false;

    std::vector<std::string> open_tag;

    std::string IDENTIFIER = "identifier";
    std::string NUMBER = "number";
    std::string STRING = "string";
    std::string COMMENT = "comment";

    CSL_Node *result;

    CSL_Node* currentCNode = new CSL_Node;
    std::vector<CSL_Node*> previousCNode;

    //AST types
    std::string TAG = "tag";
    std::string CLOSEDTAG = "closedtag";
    std::string ASSIGNMENT = "assignment";
    std::string LISTASSIGNMENT = "listassignment";
    std::string TAGASSIGNMENT = "tagassignment";

    bool calledEOS = false;

    void parseInternal(std::string str);
    void getNextCToken();

    /*
    * --------------------------EBNF Grammar------------------------
    * File = Block
    * Block = tags [Block]
    *
    * tags = otag {tagsContent} ctag
    *
    * tagscontent = [tags][content][tagscontent]
    *
    * otag = "<" IDENTIFIER {IDENTIFIER "=" factor} ("/>" | ">")
    *
    * ctag = "</" IDENTIFIER ">"
    *
    * content = statement [content]
    *
    * statement = IDENTIFIER "=" factor | IDENTIFIER "=" "{" [factor] {";" factor} "}"
    *
    * factor = NUMBER | STRING
    */

    void expectCargo(std::string cargo);
    void Block();
    void tags();
    void tagscontent();
    void ctag();
    void content();
    void statement();
    void factor();

    std::string errorMessage();
	string convert(int num);
};

#endif /* CSL_PARSER_H_ */
