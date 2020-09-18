/*
 * Carbon_Parser.h
 *
 *  Created on: Sep 23, 2015
 *      Author: andreas
 */

#ifndef CARBON_PARSER_H_
#define CARBON_PARSER_H_

#include <iostream>
#include <vector>
#include <string>

#include "CScanner_CLexer.h"

using namespace std;

struct CNode {
    string type;

    string ID;

    vector<CNode> children;
    vector<string> args;
};

class Carbon_Parser {
public:
	CNode parse(string str) {
		lexer = new CLexer(str);
		currentNode = &sourceCode;

		Block();

		return sourceCode;
	}

private:
	CLexer lexer;

	CNode sourceCode;
	CNode *currentNode;

	CToken token;

	void getNextCToken() {
		token = lexer.getNextCToken();
	}
	/*--------------------------------EBNF Grammar--------------------------------
	 *Block =
	 */

	void Block() {

	}
};

#endif /* CARBON_PARSER_H_ */
