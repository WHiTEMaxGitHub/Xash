#include "xspch.h"
#include "string_utils.h"
#include "format_print_utils.h"
#include "core/base.h"
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

#define UNIQUE_ASTNODE_PTR(node_ptr)\
		MAKE_UNIQUE_AST(utils::ASTNode, node_ptr)

	std::cout << "========================" << std::endl;
	auto* nodeptr1 = new utils::SimpleBuiltInCommandNode();
	auto* nodeptr2 = new utils::SimpleBuiltInCommandNode();
	auto* nodeptr3 = new utils::SimpleBuiltInCommandNode();
	auto* nodeptr4 = new utils::SimpleBuiltInCommandNode();
	auto* pipenodeptr = new utils::PipeNode();
	auto* redirectnodeptr = new utils::RedirectNode();
	auto* backgroundnodeptr = new utils::BackgroundNode();
	auto* subshellnodeptr = new utils::SubshellNode();
	auto* commandlistnodeptr = new utils::CommandListNode();
	auto* commandsubstnodeptr = new utils::CommandSubstNode();
	nodeptr1->setProgram("ls").addOption("-l", "true").addOption("-a", "true");
	nodeptr2->setProgram("grep").addArgument("*.cpp");
	nodeptr3->setProgram("touch").addArgument("main.cpp");
	nodeptr4->setProgram("echo").addOption("-n", "true").addArgument("\"Hello, Xash!\"");
	pipenodeptr->setSender(std::unique_ptr<utils::ASTNode>{nodeptr1}).setReceiver(std::unique_ptr<utils::ASTNode>{nodeptr2});
	redirectnodeptr->setCommand(std::unique_ptr<utils::ASTNode>{pipenodeptr}).setFile("output.txt").setRedirectType(">");
	backgroundnodeptr->setCommand(std::unique_ptr<utils::ASTNode>{redirectnodeptr});
	subshellnodeptr->setCommand(std::unique_ptr<utils::ASTNode>{nodeptr3});
	commandsubstnodeptr->setCommand(UNIQUE_ASTNODE_PTR(nodeptr4));
	commandlistnodeptr->addCommand(UNIQUE_ASTNODE_PTR(backgroundnodeptr)).addCommand(UNIQUE_ASTNODE_PTR(subshellnodeptr)).addCommand(UNIQUE_ASTNODE_PTR(commandsubstnodeptr));
	auto* root = dynamic_cast<utils::ASTNode*>(commandlistnodeptr);
	root->debug_print();
	std::cout << root->toString() << std::endl;
	return 0;
}