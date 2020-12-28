//#include <OE_API.h>
#include <string_view>
#include <fstream>
#include <string>
#include <iostream>

#include <unistd.h>

#define CPP20

#include <Carbon/CSL_Parser.h>


unsigned int indentation = 0;

std::string get_whitespace() {
	std::string result = "";

	for (unsigned int i = 0; i < indentation; i++)
		result += "\t";

	return result;
}

void print_attribute(std::string_view name, csl::single_assignment_t att) {
	std::cout << name << " = " << att;
}

void print_single_assignment(std::string_view name, csl::single_assignment_t as) {
	std::cout << get_whitespace() << name << " = " << as;
}

void print_list_assignment(std::string_view name, csl::list_assignment_t as) {
	std::cout << get_whitespace() << name << " = {";

	for (const auto& v : as) {
		std::cout << v << "; ";
	}

	std::cout << "};";
}

void print_element(std::string_view name, csl::element el) {
	std::cout << get_whitespace() << '<' << name;

	for (const auto& [name, att] : el.attributes) {
		std::cout << ' ';
		print_attribute(name, att);
		std::cout << ' ';
	}

	std::cout << ">\n";
	
	indentation++;
	
	for (const auto& [name, sub_el] : el.elements) {
		print_element(name, sub_el);
		std::cout << '\n';
	}


	for (const auto& [name, as] : el.single_assignments) {
		print_single_assignment(name, as);
		std::cout << ";\n";
	}

	for (const auto& [name, as] : el.list_assignments) {
		print_list_assignment(name, as);
		std::cout << '\n';
	}
	
	indentation--;

	std::cout << get_whitespace() << "</" << name << '>';
}


int main(){
/*	std::string input = "";
	std::ifstream f("csl_very_large_object_test.csl");

	std::string line;
	while(getline(f, line))
		input += line + '\n';

	f.close();

	try {
		csl::element e = csl::parse(input);

	} catch (csl::unexpected_symbol_error& e) {
		std::cout << "csl::unexpected_symbol_error:\n\t" << e.what() << std::endl;
	}*/




	return 0;
}
