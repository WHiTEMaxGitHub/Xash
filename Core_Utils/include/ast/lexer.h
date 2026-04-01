#pragma once
#include "ast/token.h"
#include <vector>
#include <string>
#include <iostream>
#include <functional>
namespace XS_Utils {
	class Lexer {
	public:
		Lexer(const std::string& cmd)
			: input(cmd), pos(0), line(1), column(1) {
		}
		Lexer& tokenize();
		void printTokens() const;
	private:
		std::string input;
		size_t pos;
		size_t line;
		size_t column;
		std::vector<Token> tokens;
		char current() const;
		void advance();
		char peek() const;
		void handleVariable();
		void handleWord();
		void addToken(const TokenType& type, const std::string& value);
		void addToken(const TokenType& type, const std::string& value, const size_t& _line, const size_t& _column);
	};
}

