#include <fstream>
#include <iostream>
#include <string>
#include <type_traits>

#include "Parser.hpp"


int indent = 0;

void printIndent() {
	for (int i = 0; i < indent; i++) {
		std::cout << '\t';	
	}	
}

void printAssignment(CSL_Assignment &a) {
	printIndent();
	std::cout << a.name << ": " << a.element << '\n';

}

void printListAssignment(CSL_ListAssignment &a) {
	printIndent();
	std::cout << a.name << ": { ";

	for (auto &as : a.elements)
		std::cout << as << ", ";

	std::cout << "}\n";

}

void printElement(CSL_Element &elem) {
	printIndent();
	std::cout << '<' << elem.name << ">\n";

	indent++;

	for (auto &a : elem.attributes) {
		std::visit([](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;

			if constexpr (std::is_same_v<T, CSL_Assignment>) {
				printAssignment(arg);
			} else if constexpr (std::is_same_v<T, CSL_ListAssignment>) {
				printListAssignment(arg);
			}
		}, a);
	}

	for (auto &e : elem.elements) {
		printElement(e);
	}
	
	for (auto &a : elem.assignments) {
		std::visit([](auto&& arg) {
			using T = std::decay_t<decltype(arg)>;

			if constexpr (std::is_same_v<T, CSL_Assignment>) {
				printAssignment(arg);
			} else if constexpr (std::is_same_v<T, CSL_ListAssignment>) {
				printListAssignment(arg);
			}
		}, a);
	}

	indent--;

	printIndent();
	std::cout << "</" << elem.name << ">\n";
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cout << "Not enough arguments provided" << std::endl;
		return 1;
	}


	std::string content = "";
	std::ifstream f(argv[1]);

	std::string line;
	while(getline(f, line))
		content += line;

	f.close();

	Parser parser(content);

	try {
		CSL_Element el = parser.parse();

		//printElement(el);
	} catch(LexerException &e) {
		std::cout << "LexerException occurred:\n\t" << e.what() << std::endl;
	} catch(ParserException &e) {
		std::cout << "ParserException occurred:\n\t" << e.what() << std::endl;
	}


	return 0;
}
