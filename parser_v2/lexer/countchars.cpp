#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>


int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cout << "Not enough arguments provided" << std::endl;
		return 1;
	}

	std::string content = "";
	std::ifstream f(argv[1]);

	std::string line;
	while(getline(f, line))
		content += line + '\n';

	f.close();


	std::map<char, int> chars;
	std::size_t total_count = 0;
	
	for (auto &c : content) {
		chars[c]++;
		total_count++;
	}

	std::vector<std::pair<char, std::size_t>> chars_ordered;

	for (auto &e : chars) {
		chars_ordered.push_back({e.first, e.second});
	}

	std::sort(std::begin(chars_ordered), std::end(chars_ordered), [](std::pair<char, std::size_t> p1, std::pair<char, std::size_t> p2){ return p1.second > p2.second; });

	for (auto &e : chars_ordered) {
		float perc = static_cast<float>(e.second) / total_count * 100;

		if ( (e.first != '\t') && (e.first != '\n') ) {
			std::cout << e.first << "\t|\t" << static_cast<int>(e.first) << ":\t" << e.second << '\t' << perc <<"%\n";
		} else {
			std::cout << ' ' << "\t|\t" << static_cast<int>(e.first) << ":\t" << e.second << '\t' << perc << "%\n";
		}
	}
	
	
	return 0;
}
