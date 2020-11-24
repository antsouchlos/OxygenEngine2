#include <assert.h>

#include "Parser.h"


Parser::Parser() {
	// Initialize variables

	inputPtr = 0;

	// Push start symbol onto parsing stack

	std::stack<std::string> tempParsingStack, tempAnaStack;
	tempParsingStack.push("#");
	tempParsingStack.push("S");
	stacks.push_back({tempAnaStack, tempParsingStack});

	// Define the grammar

	/*
	 * S := "a" S "b"
	 * S := "a" "b"
	 *
	 * */
/*
	grammar["S"] = { {"b","S","a"}, {"b","a"} };

	terminals = {"a", "b", "#"};
	nonTerminals = {"S"};
	*/

	/* 
	 * S := integer | integer "." integer
	 * integer := number+
	 * number := 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 
	 *
	 * --------
	 *
	 * S := integer
	 * S := integer "." integer
	 * integer := number
	 * integer := number numberTail
	 * number := 0
	 * number := 1
	 * number := 2
	 * number := 3
	 * number := 4
	 * number := 5
	 * number := 6
	 * number := 7
	 * number := 8
	 * number := 9
	 * numberTail := number numberTail
	 * numberTail := number
	 *
	 * */

	grammar["S"] = { 			{"integer"},
								{"integer", ".", "integer"} };

	grammar["integer"] = {		{"numberTail", "number"},
								{"number"} };

	grammar["number"] = {		{"0"},
								{"1"},
								{"2"},
								{"3"},
								{"4"},
								{"5"},
								{"6"},
								{"7"},
								{"8"},
								{"9"} };

	grammar["numberTail"] = { 	{"numberTail", "number"},
								{"number"} };

	terminals = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ".", "#"};
	nonTerminals = {"S", "integer", "number", "numberTail"};
}

void Parser::printStack(std::string name, std::stack<std::string> s) {
	std::cout << "[DEBUG] STACK - " << name << ": {";

	while (!s.empty()) {
		std::cout << "\""<< s.top() << "\", ";
		s.pop();
	}

	std::cout << "}" << std::endl;
}

void Parser::printRule(std::string name, ruleStructure r) {
	std::cout << "[DEBUG] RULE - " << name << ": {" << std::endl;

	for (auto &rightHandSide : r) {
		std::cout << "\t{ ";

		for (auto &symbol : rightHandSide) {
			std::cout << "\""<< symbol << "\", ";
		}

		std:: cout << " }" << std::endl;
	}

	std::cout << "}" << std::endl;
}

void Parser::printParsingOverview() {
	for (int i = 0; i < input.size(); i++) {
		if (i == inputPtr) std::cout << "|";
		std::cout << input[i];
	}

	std::cout << std::endl;

//	std::cout << "[DEBUG] stacks.size(): " << stacks.size() << std::endl;
	for (auto &stackPair : stacks) {
		std::stack<std::string> analysisStack = stackPair.first;
		std::stack<std::string> parsingStack = stackPair.second;

		std::string analysisStackString = "";
		std::string parsingStackString = "";

		while (!analysisStack.empty()) {
			analysisStackString = "" + analysisStack.top() + ", " + analysisStackString;
			analysisStack.pop();
		}

		while (!parsingStack.empty()) {
			parsingStackString = parsingStackString + "" + parsingStack.top() + ", ";
			parsingStack.pop();
		}
		
		std::cout << analysisStackString << "| " << parsingStackString << std::endl;
	}

	std::cout << std::endl;
}

bool Parser::recognize(std::vector<std::string> input) {
	this->input = input;
	this->input.push_back("#");


	while(stacks.size() && inputPtr < this->input.size()) {
		// DEBUG
//		int a;
//		std::cin >> a;
		printParsingOverview();

		stackStructure tempStacksInsert;
		stackStructure tempStacksRemove;

		bool consumable = false;

		
		if (onlyTerminals()) {
			// Match
			
			// Try matching for every sentential currently being kept track of

			for (auto &stackPair : stacks) {
				auto &analysisStack = stackPair.first;
				auto &parsingStack = stackPair.second;

				std::string currentSymbol = parsingStack.top();
				

				// Make sure the sentential form starts with a terminal in order
				// to perform the match

				if (isTerminal(currentSymbol)) {
					parsingStack.pop();
					analysisStack.push(currentSymbol);

					if ( !(this->input[inputPtr].compare(currentSymbol)) ) {
						consumable = true;
					} else {
						tempStacksRemove.push_back({analysisStack, parsingStack});
					};
				}
			}
		} else {
			// Predict

			// Try the prediction for every sentential currently being kept track of

			for (auto &stackPair : stacks) {
				auto &analysisStack = stackPair.first;
				auto &parsingStack = stackPair.second;

				std::string currentSymbol = parsingStack.top();
				

				// Make sure the sentential form starts with a non-terminal in order
				// to perform the prediction

				if (isNonTerminal(currentSymbol)) {
					parsingStack.pop();
					analysisStack.push(currentSymbol);

					ruleStructure rule = grammar[currentSymbol];

					for (auto &rightHandSide : rule) {
						std::stack<std::string> tempAnalysisStack = analysisStack;
						std::stack<std::string> tempParsingStack = parsingStack;

						for (auto &symbol: rightHandSide)
							tempParsingStack.push(symbol);

						tempStacksInsert.push_back({tempAnalysisStack, tempParsingStack});
						tempStacksRemove.push_back({analysisStack, parsingStack});
					}
				}
			}
		}


		// Remove / add sentential forms

		for (auto &s : tempStacksInsert)
			stacks.push_back(s);

		for (auto &s : tempStacksRemove) {
			auto it = std::find(stacks.begin(), stacks.end(), s);

			if (it != stacks.end()) stacks.erase(it);
		}


		// Advance the input pointer
	
		if (consumable) inputPtr++;
	}

	if (inputPtr == this->input.size()) return true;

	return false;
}

bool Parser::isTerminal(std::string symbol) {
	return std::find(terminals.begin(), terminals.end(), symbol) != terminals.end();
}

bool Parser::isNonTerminal(std::string symbol) {
	return std::find(nonTerminals.begin(), nonTerminals.end(), symbol) != nonTerminals.end();
}

bool Parser::onlyTerminals() {
	for (auto &stackPair : stacks) {
		auto &parsingStack = stackPair.second;

		if ( isNonTerminal(parsingStack.top()) )
			return false;
	}

	return true;
}
