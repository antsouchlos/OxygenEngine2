/*
 * Scanner_Lexer.h
 *
 *  Created on: Sep 23, 2015
 *      Author: andreas
 * 
 *  Updated on: Oct 04+, 2020
 */

#ifndef CSL_LEXER_H_
#define CSL_LEXER_H_

#include <types/OE_Libs.h>
#include <Carbon/CSL_Exceptions.h>

struct CSL_Char {
		int lineIndex = -1;
		int colIndex = 0;
		char cargo = '?';
};

struct CSL_Token {
	int lineIndex;
	int colIndex;
	std::string type = "eos";
	std::string cargo = "";
};

//--------------------------------------------------Scanner-------------------------------------------------
class CSL_Scanner {
    public:
    
        CSL_Scanner(std::string str);
        CSL_Char getNextChar();
    
    private:
        std::string sourceStr;

        int strLen;

        int lineIndex;
        int absoluteColIndex;
        int relativeColIndex;
};

//--------------------------------------------------Lexical Analyzer-------------------------------------------------

class CSL_Lexer {
    public:
        CSL_Lexer(std::string str);
        ~CSL_Lexer();
        CSL_Token getNextCToken();
    
        //isStringStartChar is defined publicly to be able to be accessed by the function 'isStringStartChar()' of the class 'CSL_Parser'
        bool isStringStartChar(char c);
        
    private:
        CSL_Scanner *scanner{nullptr};
        CSL_Char ch;

        int absoluteColIndex = 0;
        int maxLen;

        //Character lists
        std::string identifierStartChars;
        std::string identifierChars;

        std::string numberStartChars;
        std::string numberChars;

        std::string singleCharOperators;
        std::string doubleCharOperators;

        std::string stringStartChars;

        std::string spaceChars;

        //types
        std::string IDENTIFIER = "identifier";
        std::string STRING = "string";
        std::string NUMBER = "number";
        std::string OPERATOR = "operator";
        std::string SPACE = "space";
        std::string END = "eos";
        std::string COMMENT = "comment";

        std::vector<int> suspectedDOperatorIndex;

        void getNextCChar();

        bool isIdentStartChar(char c);
        bool isIdentChar(char c);
        bool isNumStartChar(char c);
        bool isNumChar(char c);
        bool isSingleCharOperator(char c);

        //check if character is a first character of double char operator and save the indices of these operators in the
        //list to make sure the right operators are checked when the function isDoubleCharOperator2Char() gets called
        bool isDoubleCharOperator1Char(char c);

        //check if character is a second character of double char operators that have previously been selected
        bool isDoubleCharOperator2Char(char c);
        bool isSpaceChar(char c);

        string errorMessage();

        //convert an integer into a string
        string convert(int num);
};

#endif /* CSL_LEXER_H_ */

