#include <Carbon/CSL_Lexer.h>

using namespace std;

CSL_Scanner::CSL_Scanner(string str) {
    sourceStr = str;

    strLen = str.length();

    lineIndex = 0;
    absoluteColIndex = 0;
    relativeColIndex = 0;
}

CSL_Char CSL_Scanner::getNextChar() {
    CSL_Char result;

    if (!(absoluteColIndex > strLen)) {

        char curChar = sourceStr[absoluteColIndex];

        result.lineIndex = lineIndex;
        result.colIndex = relativeColIndex;
        result.cargo = curChar;

        if (curChar == '\n') {
            lineIndex++;
            relativeColIndex = 0;
        } else {
            relativeColIndex++;
        }

        absoluteColIndex++;
    }
    return result;
}

CSL_Lexer::CSL_Lexer(string str) {
    //setup 'lists' based upon which the lexer will determine the type and content of each token
    identifierStartChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_";
    identifierChars = identifierStartChars + "0123456789";

    numberStartChars = "0123456789-";
    numberChars = numberStartChars + ".";

    singleCharOperators = "!*()-=+{}[]<>%^/;.,";
    doubleCharOperators = "<//*//==!=+=-=*=/=++-->=<=";//'*/' is excluded from this list because of the way comments are processed

    stringStartChars = "\"'";

    spaceChars = "\n\t ";

    maxLen = str.length();

    scanner = new CSL_Scanner(str);

    getNextCChar();
}

CSL_Lexer::~CSL_Lexer(){
    if(scanner != nullptr)
        delete scanner;
}

CSL_Token CSL_Lexer::getNextCToken() {
    CSL_Token result;

    while (true) {
        //skip spaces
        if (isSpaceChar(ch.cargo)) {
            getNextCChar();
        }

        //handle identifiers
        else if (isIdentStartChar(ch.cargo)) {
            result.type = IDENTIFIER;
            result.colIndex = ch.colIndex;
            result.lineIndex = ch.lineIndex;

            while(isIdentChar(ch.cargo)){
                result.cargo += ch.cargo;
                getNextCChar();
            }

            break;
        }

        //hanlde numbers
        else if (isNumStartChar((int)ch.cargo)) {
            result.type = NUMBER;
            result.colIndex = ch.colIndex;
            result.lineIndex = ch.lineIndex;

            while (isNumChar(ch.cargo)) {
                result.cargo += ch.cargo;
                getNextCChar();
            }
        
            break;
        }

        //handle double char operators, and on occasion single char operators
        else if (isDoubleCharOperator1Char(ch.cargo)) {
            result.type = OPERATOR;
            result.cargo = ch.cargo;
            result.colIndex = ch.colIndex;
            result.lineIndex = ch.lineIndex;

            string op = "";
            op += ch.cargo;

            getNextCChar();

            //check if second character also matches suspected double char operator
            if (isDoubleCharOperator2Char(ch.cargo)) {
                result.cargo += ch.cargo;
                op += ch.cargo;

                getNextCChar();

                //special case when double char operator is comment of type '/*'...'*/'
                if (op == "/*") {/**/
                    result.type = COMMENT;

                    result.cargo += ch.cargo;

                    while (true){
                        if (ch.cargo == '*') {
                            getNextCChar();
                            if (ch.cargo == '/'){
                                result.cargo += ch.cargo;
                                break;
                            }
                        }
                        getNextCChar();
                        result.cargo += ch.cargo;
                    }
                    getNextCChar();
                //special case when double char operator is comment of type '//'...'\n'
                } else if (op == "//") {
                    result.type = COMMENT;

                    result.cargo += ch.cargo;

                    while (ch.cargo != '\n') {
                        result.cargo += ch.cargo;
                        getNextCChar();
                    }
                }
            }

            break;
        }

        //handle single char operators
        else if (isSingleCharOperator(ch.cargo)) {
            result.type = OPERATOR;

            result.cargo = ch.cargo;
            result.colIndex = ch.colIndex;
            result.lineIndex = ch.lineIndex;

            getNextCChar();

            break;
        }

        //handle strings
        else if(isStringStartChar(ch.cargo)) {
            char c = ch.cargo;

            result.type = STRING;
            result.colIndex = ch.colIndex;
            result.lineIndex = ch.lineIndex;

            getNextCChar();
            while (ch.cargo != c) {
                result.cargo += ch.cargo;
                getNextCChar();
            }

            getNextCChar();

            break;
        }

        //throw error if unexpected character is found
        else {
            if (absoluteColIndex <= maxLen)
                throw UnexpectedCharacterException(errorMessage());

            break;
        }
    }

    return result;
}

bool CSL_Lexer::isStringStartChar(char c) {
    bool result = false;

    for(unsigned int i = 0; i < stringStartChars.length(); i++) {
        if (c == stringStartChars[i])
            result = true;
    }

    return result;
}

void CSL_Lexer::getNextCChar() {
    ch = scanner->getNextChar();
    absoluteColIndex++;
}

bool CSL_Lexer::isIdentStartChar(char c) {
    bool result = false;

    for(unsigned int i = 0; i < identifierStartChars.length(); i++) {
        if (c == identifierStartChars[i])
            result = true;
    }

    return result;
}

bool CSL_Lexer::isIdentChar(char c) {
    bool result = false;

    for(unsigned int i = 0; i < identifierChars.length(); i++) {
        if (c == identifierChars[i])
            result = true;
    }

    return result;
}

bool CSL_Lexer::isNumStartChar(char c) {
    bool result = false;

    for(unsigned int i = 0; i < numberStartChars.length(); i++) {
        if (c == numberStartChars[i])
            result = true;
    }

    return result;
}

bool CSL_Lexer::isNumChar(char c) {
    bool result = false;

    for(unsigned int i = 0; i < numberChars.length(); i++) {
        if (c == numberChars[i])
            result = true;
    }

    return result;
}

bool CSL_Lexer::isSingleCharOperator(char c) {
    bool result = false;

    for(unsigned int i = 0; i < singleCharOperators.length(); i++) {
        if (c == singleCharOperators[i])
            result = true;
    }

    return result;
}

//check if character is a first character of double char operator and save the indices of these operators in the
//list to make sure the right operators are checked when the function isDoubleCharOperator2Char() gets called
bool CSL_Lexer::isDoubleCharOperator1Char(char c) {
    bool result = false;

    for(unsigned int i = 0; i < doubleCharOperators.length(); i++) {
        if (i%2 == 0) {
            //check t
            if (c == doubleCharOperators[i]) {
                suspectedDOperatorIndex.push_back(i + 1);
                result = true;
            }
        }
    }
    return result;
}

bool CSL_Lexer::isDoubleCharOperator2Char(char c) {
    for (unsigned int i = 0; i < suspectedDOperatorIndex.size(); i++) {
        if (c == doubleCharOperators[suspectedDOperatorIndex[i]]) {
            suspectedDOperatorIndex.clear();
            return true;
        }
    }
    suspectedDOperatorIndex.clear();
    return false;
}

bool CSL_Lexer::isSpaceChar(char c) {
    bool result = false;

    for(unsigned int i = 0; i < spaceChars.length(); i++) {
        if (c == spaceChars[i])
            result = true;
    }

    return result;
}

string CSL_Lexer::errorMessage() {
    return " at: " + convert(ch.lineIndex) + ":" + convert(ch.colIndex) + " - '" + ch.cargo + "'";
}

//convert an integer into a string
string CSL_Lexer::convert(int num) {
    ostringstream stream;
    stream << num;
    return stream.str();
}
