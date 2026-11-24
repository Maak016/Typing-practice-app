#include "util.h"

int string_util::read_until_char(const std::string s, char target) {
	for (int i = 0; i < s.size(); i++) {
		if (s.at(i) == target) return i;
	}

	return -1;
}

int string_util::read_until_char(const std::string s, char target, int start_idx) {
	if (start_idx >= s.length()) return -1;

	for (int i = start_idx; i < s.size(); i++) {
		if (s.at(i) == target) return i;
	}

	return -1;
}