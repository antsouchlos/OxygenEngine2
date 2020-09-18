/*
 * CSL_Parser.h
 *
 *  Created on: Sep 23, 2015
 *      Author: andreas
 */

#ifndef CSL_PARSER_H_
#define CSL_PARSER_H_

#include "CScanner_CLexer.h"

using namespace std;

struct CNode {
    string type;

    string ID;

    vector<CNode*> children = {};
    vector<string> args = {};

    int line = 0;
    int col = 0;
};

class CSL_Parser {
public:
    CNode* parse(string str) {
        lexer = new CLexer(str);

	result = currentCNode;

        getNextCToken();
        Block();

	if (result->children.size() > 0) {
            return result->children[0];
        } else {
            return result;
        }
    }

    virtual ~CSL_Parser() {
    	delete currentCNode;

    	for(unsigned int i = 0; i < previousCNode.size(); ++i)
    	   delete previousCNode[i];
    }

    bool isStringStartChar(char c) {
    	return lexer->isStringStartChar(c);
    }
private:
    CLexer *lexer;
    CToken token;

    bool definedLexer = false;

    vector<string> open_tag;

    string IDENTIFIER = "identifier";
    string NUMBER = "number";
    string STRING = "string";
    string COMMENT = "comment";

    CNode *result;

    CNode* currentCNode = new CNode;
    vector<CNode*> previousCNode;

    //AST types
    string TAG = "tag";
    string CLOSEDTAG = "closedtag";
    string ASSIGNMENT = "assignment";
    string LISTASSIGNMENT = "listassignment";
    string TAGASSIGNMENT = "tagassignment";

    bool calledEOS = false;

    void parseInternal(string str) {
    	CLexer *saveCLexer = lexer;
	lexer = new CLexer(str);
	getNextCToken();

        Block();
        delete(lexer);
    	lexer = saveCLexer;

    	calledEOS = false;
    	getNextCToken();
    }

    void getNextCToken() {
	token = lexer->getNextCToken();

        if (token.type == COMMENT) {
        	getNextCToken();
        } else if (token.type == "eos") {
        	if (calledEOS) {
        		throw UnexpectedTokenException("unexpected end of string");
        	}

		calledEOS = true;
        }
    }

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

    void expectCargo(string cargo) {
    	if (token.type== "eos")
    		getNextCToken();

        if (!(token.cargo == cargo)) {
            	throw UnexpectedTokenException(errorMessage() + ", '" + cargo + "' expected");
        }

        getNextCToken();
    }

    void Block() {
	if (token.type != "eos")
        	tags();

        if (token.type != "eos")
            Block();
    }

    void tags() {
    	expectCargo("<");
		if (token.type != IDENTIFIER) {
			throw UnexpectedTokenException(errorMessage());
		}

		if (token.cargo == "src") {
			getNextCToken();

			expectCargo("res");
			expectCargo("=");

			if (token.type != STRING)
				throw UnexpectedTokenException(errorMessage() + ", string expected");

			string exCode = "";
			ifstream in((token.cargo).c_str());
			string line;
			while (getline(in, line))
				exCode += line + "\n";
			in.close();

			getNextCToken();

			expectCargo("/");
			if (token.cargo != ">")
				throw UnexpectedTokenException(errorMessage() + ", '" + ">" + "' expected");

			parseInternal(exCode);
		} else {
			CNode *newCNode = new CNode;
			newCNode->line = token.lineIndex;
			newCNode->col = token.colIndex;
			newCNode->ID = token.cargo;
			currentCNode->children.push_back(newCNode);
			previousCNode.push_back(currentCNode);
			currentCNode = newCNode;

			getNextCToken();
			while (token.type == IDENTIFIER) {
				CNode *assignmentCNode = new CNode;
				assignmentCNode->line = token.lineIndex;
				assignmentCNode->col = token.colIndex;
				assignmentCNode->type = TAGASSIGNMENT;
				assignmentCNode->ID = token.cargo;

				currentCNode->children.push_back(assignmentCNode);

					CNode *saveCNode = currentCNode;
					currentCNode = assignmentCNode;

					getNextCToken();

					expectCargo("=");
					factor();
					getNextCToken();

					currentCNode = saveCNode;
			}

			if (token.cargo == "/") {
				getNextCToken();
				newCNode->type = CLOSEDTAG;
				expectCargo(">");

				currentCNode = previousCNode[previousCNode.size()-1];
				previousCNode.pop_back();
			} else {
				newCNode->type = TAG;
				expectCargo(">");

				tagscontent();
				ctag();
			}
		}
    }

    void tagscontent() {
        while (token.cargo != "</") {
            if (token.type == IDENTIFIER) {
                statement();
            }

            if (token.cargo == "<") {
                tags();
            }

            if(token.cargo != "</") {
            	if (token.type != IDENTIFIER && token.cargo != "<" && token.type != "eos") throw UnexpectedTokenException(errorMessage());
            }
        }
    }

    void ctag() {
        expectCargo("</");

        if (token.type != IDENTIFIER)
        	throw UnexpectedTokenException(errorMessage());

        if (token.cargo != currentCNode->ID) {
        	throw UnexpectedTokenException(errorMessage() + ", '" + currentCNode->ID + "' expected");
        }

        getNextCToken();

        expectCargo(">");

        currentCNode = previousCNode[previousCNode.size()-1];
        previousCNode.pop_back();
    }

    void content() {
        statement();
        getNextCToken();
        if (token.type == IDENTIFIER) {
            content();
        }
    }

    void statement() {
        if (token.type != IDENTIFIER) {
            throw UnexpectedTokenException(errorMessage());
        }

		CNode *assignmentCNode = new CNode;
		assignmentCNode->line = token.lineIndex;
		assignmentCNode->col = token.colIndex;
		assignmentCNode->ID = token.cargo;
		currentCNode->children.push_back(assignmentCNode);
		CNode *saveCNode = currentCNode;
		currentCNode = assignmentCNode;

        getNextCToken();

        expectCargo("=");

        if (token.cargo == "{") {
        	assignmentCNode->type = LISTASSIGNMENT;

			getNextCToken();
			factor();
			getNextCToken();

			while (token.cargo == ";") {
				getNextCToken();
				factor();
				getNextCToken();
			}

        	expectCargo("}");
        } else {
    		assignmentCNode->type = ASSIGNMENT;

			factor();
			getNextCToken();
        }

        currentCNode = saveCNode;
    }

    void factor() {
        if (token.type == NUMBER) {
            currentCNode->args.push_back(token.cargo);
        } else if (token.type == STRING) {
            currentCNode->args.push_back(token.cargo);
        } else {
            throw UnexpectedTokenException(errorMessage());
        }
    }

    string errorMessage() {
		return " at " + convert(token.lineIndex) + ":" + convert(token.colIndex) + " - '" + token.cargo + "'";
	}

	string convert(int num) {
		ostringstream stream;
		stream << num;
		return stream.str();
	}
};

#endif /* CSL_PARSER_H_ */
