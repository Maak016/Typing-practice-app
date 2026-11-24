#pragma once

#include "includes.h"

class string_util {
public:
	static int read_until_char(const std::string s, char target);
	static int read_until_char(const std::string s, char target, int start_idx);
};