#include <Carbon/CSL_Parser.h>

using namespace std;

CSL_Node::~CSL_Node(){
    //for (auto& x: children)
    //    delete x; //IS THIS CORRECT HERE? DO WE NEED TO DO THIS? MEMORY LEAK??????!!!!!
}


CSL_Node* CSL_Parser::parse(string str) {
    lexer = new CSL_Lexer(str);

	result = currentCNode;

    getNextCToken();
    Block();

	if (result->children.size() > 0) {
        return result->children[0];
    } else {
        return result;
    }
}

CSL_Parser::~CSL_Parser() {
    delete currentCNode;
    if(lexer != nullptr)
        delete lexer;
    for(unsigned int i = 0; i < previousCNode.size(); ++i)
        delete previousCNode[i];
}

bool CSL_Parser::isStringStartChar(char c) {
    return lexer->isStringStartChar(c);
}

void CSL_Parser::parseInternal(string str) {
    CSL_Lexer *saveCLexer = lexer;
    lexer = new CSL_Lexer(str);
    getNextCToken();

    Block();
    delete(lexer);
    lexer = saveCLexer;

    calledEOS = false;
    getNextCToken();
}

void CSL_Parser::getNextCToken() {
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

 void CSL_Parser::expectCargo(string cargo) {
    if (token.type== "eos")
        getNextCToken();

    if (!(token.cargo == cargo)) {
        throw UnexpectedTokenException(errorMessage() + ", '" + cargo + "' expected");
    }

    getNextCToken();
}

void CSL_Parser::Block() {
	if (token.type != "eos")
        	tags();

    if (token.type != "eos")
        Block();
}
    
void CSL_Parser::tags() {
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
        CSL_Node *newCNode = new CSL_Node;
        newCNode->line = token.lineIndex;
        newCNode->col = token.colIndex;
        newCNode->ID = token.cargo;
        currentCNode->children.push_back(newCNode);
        previousCNode.push_back(currentCNode);
        currentCNode = newCNode;

        getNextCToken();
        while (token.type == IDENTIFIER) {
            CSL_Node *assignmentCNode = new CSL_Node;
            assignmentCNode->line = token.lineIndex;
            assignmentCNode->col = token.colIndex;
            assignmentCNode->type = TAGASSIGNMENT;
            assignmentCNode->ID = token.cargo;

            currentCNode->children.push_back(assignmentCNode);

            CSL_Node *saveCNode = currentCNode;
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

void CSL_Parser::tagscontent() {
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

void CSL_Parser::ctag() {
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

void CSL_Parser::content() {
    statement();
    getNextCToken();
    if (token.type == IDENTIFIER) {
        content();
    }
}

void CSL_Parser::statement() {
    if (token.type != IDENTIFIER) {
        throw UnexpectedTokenException(errorMessage());
    }

    CSL_Node *assignmentCNode = new CSL_Node;
    assignmentCNode->line = token.lineIndex;
	assignmentCNode->col = token.colIndex;
    assignmentCNode->ID = token.cargo;
    currentCNode->children.push_back(assignmentCNode);
    
    CSL_Node *saveCNode = currentCNode;
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

void CSL_Parser::factor() {
    if (token.type == NUMBER) {
        currentCNode->args.push_back(token.cargo);
    } else if (token.type == STRING) {
        currentCNode->args.push_back(token.cargo);
    } else {
        throw UnexpectedTokenException(errorMessage());
    }
}

string CSL_Parser::errorMessage() {
    return " at " + convert(token.lineIndex) + ":" + convert(token.colIndex) + " - '" + token.cargo + "'";
}

string CSL_Parser::convert(int num) {
    ostringstream stream;
    stream << num;
    return stream.str();
}
