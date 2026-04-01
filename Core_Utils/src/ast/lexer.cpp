#include "crpch.h"
#include "ast/lexer.h"
namespace XS_Utils {
	char Lexer::current() const {
		return pos < input.size() ? input[pos] : '\0';
	}
	void Lexer::advance() {
		if (current() == '\n') {
			++line;
			column = 1;
		}
		else ++column;
		++pos;
		return;
	}
	char Lexer::peek() const {
		return (pos + 1 < input.size()) ? input[pos + 1] : '\0';
	}
	void Lexer::addToken(const TokenType& type, const std::string& value) {
		tokens.emplace_back(type, value, line, column - value.length());
		return;
	}
	void Lexer::addToken(const TokenType& type, const std::string& value, const size_t& _line, const size_t& _column) {
		tokens.emplace_back(type, value, _line, _column);
		return;
	}
	void Lexer::handleVariable() {
		size_t start_line = line, start_column = column;
		std::string variable_name("");
		advance();
		if (current() == '{') {
			advance();
			while (pos < input.size() && current() != '}') {
				variable_name += current();
				advance();
			}
			if (current() == '}') advance();
		}
		else {
			while (isalnum(current()) || current() == '_') {
				variable_name += current();
				advance();
			}
			addToken(TokenType::SPECIAL_VARIABLE, variable_name, start_line, start_column);
		}
	}
	void Lexer::handleWord() {
		size_t start_line = line, start_column = column;
		bool escape_next = false;
		bool in_quotes = false;
		char quote_character = '\0';
		std::string current_word{ "" };
		while (pos < input.length()) {
			const char c = current();
			if (escape_next) {
				current_word += c;
				escape_next = false;
				advance();
				continue;
			}
			if (c == '\\') {
				escape_next = true;
				advance();
				continue;
			}
			if ((c == '"' || c == '\'') && !in_quotes) {
				in_quotes = true;
				quote_character = c;
				advance();
				continue;
			}
			if (c == quote_character && in_quotes) {
				in_quotes = false;
				quote_character = '\0';
				advance();
				continue;
			}
			if (!in_quotes &&
				(isspace(c) || c == '|' || c == '&' || c == '<' ||
					c == '>' || c == '$' || c == ';')) {
				break;
			}
			current_word += c;
			advance();
		}
		TokenType type = TokenType::WORD_ARGUMENT;
		if (!tokens.empty()) {
			auto tID = static_cast<int>(tokens.back().type);
			if (tID >= 4 && tID <= 8) {
				type = TokenType::WORD_COMMMAND;
			}
			else if (!in_quotes && (current_word.find('*') != std::string::npos || current_word.find('?') != std::string::npos)) {
				type = TokenType::SPECIAL_WILDCARD;
			}
			else if (current_word.length() >= 2) {
				if (current_word[0] == '-') {
					if (current_word[1] == '-') {
						type = TokenType::WORD_OPTION_LONG;
					}
					else type = TokenType::WORD_OPTION;
				}
			}
		}
		else type = TokenType::WORD_COMMMAND;

		tokens.emplace_back(type, current_word, start_line, start_column);
	}
	Lexer& Lexer::tokenize() {
		tokens.clear();
		while (pos < input.size()) {
			if (isspace(current())) {
				advance();
				continue;
			}
			char c = current();
			// &&
			if (c == '&' && peek() == '&') {
				addToken(TokenType::OPERATOR_AND_IF, "&&");
				advance();
				advance();
				continue;
			}
			// ||
			if (c == '|' && peek() == '|') {
				addToken(TokenType::OPERATOR_OR_IF, "||");
				advance();
				advance();
				continue;
			}
			// >>
			if (c == '<' && peek() == '<') {
				addToken(TokenType::REDIRECT_HEREDOC, "<<");
				advance();
				advance();
				continue;
			}
			// >>
			if (c == '>' && peek() == '>') {
				addToken(TokenType::REDIRECT_APPEND, ">>");
				advance();
				advance();
				continue;
			}
			// 其他符号
			switch (c) {
			case '<': addToken(TokenType::REDIRECT_IN, "<"); advance(); continue;
			case '>': addToken(TokenType::REDIRECT_OUT, ">"); advance(); continue;
			case '|': addToken(TokenType::OPERATOR_PIPE, "|"); advance(); continue;
			case '&': addToken(TokenType::OPERATOR_BACKGROUND, "&"); advance(); continue;
			case ';': addToken(TokenType::OPERATOR_SEMICOLON, ";"); advance(); continue;
			}
			if (c == '$') {
				handleVariable();
				continue;
			}
			//处理单词WORD
			handleWord();
		}
		return *this;
	}
	void Lexer::printTokens() const {
		for (const auto& t : tokens) {
			std::cout << t << '\n';
		}
	}
}