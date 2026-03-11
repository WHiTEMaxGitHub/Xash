#pragma once
#include <vector>
#include <string>

namespace XS_Utils {
	std::vector<std::string> split_whitespace(const std::string& cmd);
	void split_whitespace(const std::string& cmd, std::vector<std::string>& tokens);
	void generate_tokens(const std::string& cmd, std::vector<std::string>& tokens);
}