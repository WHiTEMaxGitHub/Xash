#include "xspch.h"
#include "string_utils.h"
#include "format_print_utils.h"
int main() {
	std::string s{ "Hello, world! Here's a msg from XASH!" };
	std::string cmd{ "ls -l | grep *.cpp > output.txt" };
	std::vector<std::string> tokens;
	XS_Utils::split_whitespace(cmd, tokens);
	XS_Utils::print_vector(tokens);
	return 0;
}