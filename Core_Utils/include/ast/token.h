#pragma once
#include <iostream>
#include <string>
namespace XS_Utils {

	enum class TokenType {
		WORD_COMMMAND,
		WORD_ARGUMENT,
		WORD_OPTION,
		WORD_OPTION_LONG,
		OPERATOR_PIPE,
		OPERATOR_BACKGROUND,
		OPERATOR_AND_IF,
		OPERATOR_OR_IF,
		OPERATOR_SEMICOLON,
		REDIRECT_IN,
		REDIRECT_OUT,
		REDIRECT_APPEND,
		REDIRECT_HEREDOC,
		SPECIAL_VARIABLE,
		SPECIAL_COMMAND_SUBSET,
		SPECIAL_STRING,
		SPECIAL_WILDCARD,
		UNKNOWN,
		END
	};

	struct Token {
		TokenType type;
		std::string value;
		size_t line;
		size_t column;
		Token(TokenType t = TokenType::UNKNOWN, std::string v = "", size_t l = 1, size_t c = 1)
			: type(t), value(v), line(l), column(c) {
		}
		std::string getTypeName() const;
	};
	std::ostream& operator<<(std::ostream& os, Token t);
}