/*
 * Advent of code common code
 *
 *
 * File      aoc.c
 * Author    Fuzes Marcell
 * Date      2021.12.11
 */

#include "aoc.h"

#include <fstream>
#include <sstream>

std::vector<std::string_view> split(std::string_view str, std::string_view delimiter) {

	size_t j = str.find(delimiter);
	if (j == std::string::npos) {
		return { str };
	}

	std::vector<std::string_view> result;
	for (;;) {
		std::string_view v = str.substr(0, j);
		result.push_back(v);
		size_t next_idx = j + delimiter.length();
		str = str.substr(next_idx, str.length());
		j = str.find(delimiter);

		if (j == std::string::npos) {
			result.push_back(str);
			break;
		}

	}

	return result;
}

std::string read_entire_file(const std::string& str) {
	std::ifstream t(str);
	std::stringstream buffer;
	buffer << t.rdbuf();
	return buffer.str();
}

int to_int(std::string_view str) {
	std::string s = std::string(str);
	return std::stoi(s);
}