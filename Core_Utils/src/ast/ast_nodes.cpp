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
			if (value != "true") {
				result += ' ';
				result += option;
				result += ' ';
				result += value;
			}
		}
		return result;
	}
	void SimpleBuiltInCommandNode::debug_print(int indent) const {
		std::cout << std::string(indent, ' ') << "Simple Built-in Command:" << program << std::endl;
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
				} else std::cout << std::string(indent + 4, ' ') << key << std::endl;
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
		result += " | ";
		result += receiver.get()->toString();
		return result;
	}
	void PipeNode::debug_print(int indent) const {
		std::cout << std::string(indent, ' ') << "Pipe: " << std::endl;
		std::cout << std::string(indent + 1, ' ') << "sender: " << std::endl;
		sender.get()->debug_print(indent + 2);
		std::cout << std::string(indent + 1, ' ') << "receiver: " << std::endl;
		receiver.get()->debug_print(indent + 2);
	}
}