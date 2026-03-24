#include "crpch.h"
#include "ast/token.h"

namespace XS_Utils {
	std::ostream& operator<<(std::ostream& os, Token t) {
		os << "Token: { " << t.getTypeName() << ", " << t.value << ", line: " << t.line << ", column: " << t.column << std::endl;
	}
	std::string Token::getTypeName() const {
		static const char* names[] = {
			"COMMAND", "ARGUMENT", "OPTION", "OPTION_LONG",
			"PIPE", "BACKGROUND", "AND_IF", "OR_IF", "SEMICOLON",
			"REDIRECT_IN", "REDIRECT_OUT", "REDIRECT_APPEND", "REDIRECT_HEREDOC",
			"VARIABLE", "COMMAND_SUBST", "STRING", "WILDCARD", "UNKNOWN", "END"
		};
		return std::string(names[(size_t)type]);
	}
}