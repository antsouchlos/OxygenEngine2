#ifndef __PARSER_H__
#define __PARSER_H__

#include <vector>
#include <stack>
#include <string>
#include <map>
#include <iostream>
#include <algorithm>


class Parser {
	public:
		Parser();

		bool recognize(std::vector<std::string> input);
	private:
		std::vector<std::string> input;

		int inputPtr;

		typedef
		std::pair<
			std::stack<std::string>, std::stack<std::string>
		> stackPairStructure;

		typedef
		std::vector<stackPairStructure> stackStructure;

		stackStructure stacks;


		typedef
		std::vector<
			std::vector<std::string>
		> ruleStructure;


		std::map<std::string, ruleStructure> grammar;
		std::vector<std::string> terminals;
		std::vector<std::string> nonTerminals;

		bool isTerminal(std::string symbol);
		bool isNonTerminal(std::string symbol);
		bool onlyTerminals();

		void printStack(std::string name, std::stack<std::string> s);
		void printRule(std::string name, ruleStructure r);
		void printParsingOverview();
};


#endif
