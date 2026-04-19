#include "crpch.h"
#include "ast/ast_nodes.h"

namespace XS_Utils {
	// SimpleBuiltInCommandNode
	std::string SimpleBuiltInCommandNode::toString() const {
		std::string result(program);
		for (const auto& s : arguments) {
			result += ' ';
			result += s;
		}
		for (const auto& [option, value] : options) {
			result += ' ';
			result += option;
			if (value != "true") {
				result += ' ';
				result += value;
			}
		}
		result += ' ';
		return result;
	}
	void SimpleBuiltInCommandNode::debug_print(int indent) const {
		std::cout << std::string(indent, ' ') << "Simple Built-in Command: " << program << std::endl;
		if (!arguments.empty()) {
			std::cout << std::string(indent + 2, ' ') << "Arguments: " << std::endl;
			for (const auto& arg : arguments) {
				std::cout << std::string(indent + 4, ' ') << arg << std::endl;
			}
		}
		else {
			std::cout << std::string(indent + 2, ' ') << "With no argument." << std::endl;
		}
		if (!options.empty()) {
			std::cout << std::string(indent + 2, ' ') << "Options: " << std::endl;
			for (const auto& [key, value] : options) {
				if (value != "true") {
					std::cout << std::string(indent + 4, ' ') << key << ' ' << value << std::endl;
				}
				else std::cout << std::string(indent + 4, ' ') << key << std::endl;
			}
		}
		else {
			std::cout << std::string(indent + 2, ' ') << "With no option." << std::endl;
		}
	}

	// SimpleExternalCommandNode
	std::string SimpleExternalCommandNode::toString() const {
		std::string result(program);
		for (const auto& s : tokens) {
			result += ' ';
			result += s;
		}
		result += " ";
		return result;
	}
	void SimpleExternalCommandNode::debug_print(int indent) const {
		std::cout << std::string(indent, ' ') << "Simple External Command: " << program << std::endl;
		if (!tokens.empty()) {
			std::cout << std::string(indent + 2, ' ') << "With arguments: " << std::endl;
			for (const auto& token : tokens) {
				std::cout << std::string(indent + 4, ' ') << token << std::endl;
			}
		}
		else {
			std::cout << std::string(indent + 2, ' ') << "Without arguments." << std::endl;
		}
	}

	// PipeNode
	std::string PipeNode::toString() const {
		std::string result{ "" };
		result += sender.get()->toString();
		result += "| ";
		result += receiver.get()->toString();
		result += ' ';
		return result;
	}
	void PipeNode::debug_print(int indent) const {
		std::cout << std::string(indent, ' ') << "Pipe: " << std::endl;
		std::cout << std::string(indent + 1, ' ') << "sender: " << std::endl;
		sender.get()->debug_print(indent + 2);
		std::cout << std::string(indent + 1, ' ') << "receiver: " << std::endl;
		receiver.get()->debug_print(indent + 2);
	}

	// RedirectNode
	std::string RedirectNode::getRedirectSymbol() const {
		std::string redirectSymbol{ "" };
		int t = static_cast<int>(redirect_type);
		switch (t) {
		case 0:
			redirectSymbol = "<";
			break;
		case 1:
			redirectSymbol = ">";
			break;
		case 2:
			redirectSymbol = ">>";
			break;
		case 3:
			redirectSymbol = "<<";
			break;
		default:
			break;
		}
		return redirectSymbol;
	}
	std::string RedirectNode::getRedirectTypeName() const {
		int t = static_cast<int>(redirect_type);
		switch (t) {
		case 0:
			return "Input";
			break;
		case 1:
			return "Output";
			break;
		case 2:
			return "Append";
			break;
		case 3:
			return "Heredoc";
			break;
		default:
			return "Error";
			break;
		}
	}
	std::string RedirectNode::toString() const {
		std::string result{ "" };
		result += command.get()->toString();
		result += getRedirectSymbol();
		result += ' ';
		result += file;
		result += ' ';
		return result;
	}
	void RedirectNode::debug_print(int indent) const {
		std::cout << std::string(indent, ' ') << "Redirect: " << getRedirectTypeName() << " '" << getRedirectSymbol() << "'" << std::endl;
		std::cout << std::string(indent + 1, ' ') << "command: " << std::endl;
		command.get()->debug_print(indent + 2);
		std::cout << std::string(indent + 1, ' ') << "file: " << std::endl;
		std::cout << std::string(indent + 2, ' ') << file << std::endl;
	}

	// AndOrNode
	std::string AndOrNode::getSymbol() const {
		switch (andor_type) {
		case XS_Utils::AndOrNode::AndOrType::AND:
			return "&&";
			break;
		case XS_Utils::AndOrNode::AndOrType::OR:
			return "||";
			break;
		case XS_Utils::AndOrNode::AndOrType::NOT:
			return "!";
			break;
		}
	}
	std::string AndOrNode::getTypeName() const {
		switch (andor_type) {
		case XS_Utils::AndOrNode::AndOrType::AND:
			return "And";
			break;
		case XS_Utils::AndOrNode::AndOrType::OR:
			return "Or";
			break;
		case XS_Utils::AndOrNode::AndOrType::NOT:
			return "Not";
			break;
		}
	}
	std::string AndOrNode::toString() const {
		std::string result{ "" };
		if (left != std::nullopt) {
			result += left.value().get()->toString();
			result += ' ';
		}
		result += getSymbol();
		result += ' ';
		result += right.get()->toString();
		return result;
	}
	void AndOrNode::debug_print(int indent) const {
		std::cout << std::string(indent, ' ') << "Logic Node: " << getTypeName() << std::endl;
		if (left != std::nullopt) {
			std::cout << std::string(indent + 1, ' ') << "left: " << std::endl;
			left.value().get()->debug_print(indent + 2);
		}
		std::cout << std::string(indent + 1, ' ') << getSymbol() << std::endl;
		std::cout << std::string(indent + 1, ' ') << "right: " << std::endl;
		right.get()->debug_print(indent + 2);
	}

	// BackgroundNode
	std::string BackgroundNode::toString() const {
		return (command.get()->toString() + " & ");
	}
	void BackgroundNode::debug_print(int indent) const {
		std::cout << std::string(indent, ' ') << "Background Node: " << std::endl;
		std::cout << std::string(indent + 1, ' ') << "command: " << std::endl;
		command.get()->debug_print(indent + 2);
	}

	// SubshellNode
	std::string SubshellNode::toString() const {
		return ("( " + command.get()->toString() + ") ");
	}
	void SubshellNode::debug_print(int indent) const {
		std::cout << std::string(indent, ' ') << "Sub-shell Node: " << std::endl;
		std::cout << std::string(indent + 1, ' ') << "command: " << std::endl;
		command.get()->debug_print(indent + 2);
	}

	// CommandListNode
	std::string CommandListNode::toString() const {
		std::string result = commands[0].get()->toString();
		for (int i = 1; i < commands.size(); ++i) {
			result += "; ";
			result += commands[i].get()->toString();
		}
		return result;
	}
	void CommandListNode::debug_print(int indent) const {
		std::cout << std::string(indent, ' ') << "Command List Node: " << std::endl;
		for (int i = 0; i < commands.size(); ++i) {
			std::cout << std::string(indent + 1, ' ') << "command[" << i << "]: " << std::endl;
			commands[i].get()->debug_print(indent + 2);
		}
	}

	// CommandSubstNode
	std::string CommandSubstNode::toString() const {
		std::string result("$( ");
		result += command.get()->toString();
		result += ") ";
		return result;
	}
	void CommandSubstNode::debug_print(int indent) const {
		std::cout << std::string(indent, ' ') << "Command Subst Node: " << std::endl;
		std::cout << std::string(indent + 1, ' ') << "command: " << std::endl;
		command.get()->debug_print(indent + 2);
	}

}