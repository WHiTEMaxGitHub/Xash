#include "xspch.h"
#include "string_utils.h"
#include "format_print_utils.h"
#include "ast/lexer.h"
#include "ast/ast_nodes.h"

int main() {
	namespace utils = XS_Utils;
	std::string s{ "Hello, world! Here's a msg from XASH!" };
	std::cout << s << '\n';
	std::string cmd{ "ls -l | grep *.cpp > \"whitemax output.txt\"" };
	std::cout << cmd << '\n';
	std::cout.flush();
	auto* lexer = new utils::Lexer(cmd);
	lexer->tokenize();
	lexer->printTokens();
	/*std::vector<std::string> tokens;
	XS_Utils::generate_tokens(cmd, tokens);
	XS_Utils::print_vector(tokens);*/

	std::cout << "========================" << std::endl;
	auto* nodeptr1 = new utils::SimpleBuiltInCommandNode();
	auto* nodeptr2 = new utils::SimpleBuiltInCommandNode();
	auto* pipenodeptr = new utils::PipeNode();
	nodeptr1->setProgram("ls").addOption("-l", "true");
	nodeptr2->setProgram("grep").addArgument("*.cpp");
	pipenodeptr->addSender(nodeptr1).addReceiver(nodeptr2);
	auto* root = dynamic_cast<utils::ASTNode*>(pipenodeptr);
	root->debug_print();
	return 0;
}