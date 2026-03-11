#include "crpch.h"
#include "string_utils.h"

namespace XS_Utils {
	std::vector<std::string> split_whitespace(const std::string& cmd) {
		std::stringstream sstream(cmd);
		std::vector<std::string> tokens;
		std::string temp;
		while (sstream >> temp) {
			tokens.emplace_back(temp);
		}
		return tokens;
	}
	void split_whitespace(const std::string& cmd, std::vector<std::string>& tokens) {
		std::stringstream sstream(cmd);
		tokens.clear();
		std::string temp;
		while (sstream >> temp)
			tokens.push_back(temp);
	}

	void generate_tokens(const std::string& cmd, std::vector<std::string>& tokens) {
		bool escape_next = false;
		bool in_quote = false;
		char quote_char = '\0';
		std::string current{""};
		for (const char& c : cmd) {
			if (escape_next) {
				current += c;
				escape_next = false;
				continue;
			}

		}
	}
}