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
		bool in_quotes = false;
		char quote_character = '\0';
		std::string current{""};
		size_t line = 1, column = 0;
		for (size_t i = 0; i < cmd.length(); ++i) {
			const char c = cmd[i];
			if (escape_next) {
				current += c;
				escape_next = false;
				continue;
			}
			if (c == '\\') {
				escape_next = true;
				continue;
			}
			if ((c == '"' || c == '\'') && !in_quotes) {
				in_quotes = true;
				quote_character = c;
				continue;
			}
			if (c == quote_character && in_quotes) {
				in_quotes = false;
				quote_character = '\0';
				continue;
			}
			if (isspace(c) && !in_quotes) {
				if (!current.empty()) {
					tokens.push_back(current);
					current.clear();
				}
			}
			else current += c;
		}
		if (!current.empty()) {
			tokens.push_back(current);
			current.clear();
		}
	}
}