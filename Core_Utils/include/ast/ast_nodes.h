#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace XS_Utils {
	enum class ASTNodeType {
		SIMPLE_BUILTIN_COMMAND,		//简单内部命令
		SIMPLE_EXTERNAL_COMMAND,	//简单外部命令
		PIPE,						//管道
		REDIRECT,					//重定向
		AND_OR,						//&& ||
		SEQUENCE,					//;
		BACKGROUND,					//&
		SUBSHELL,					//子shell()
		COMMAND_SUBST				//命令替换$()
	};

	class ASTNode {
	public:
		virtual ~ASTNode() = default;
		virtual ASTNodeType type() const = 0;
		virtual std::string toString() const = 0;

		virtual void debug_print(int indent = 0) const = 0;
	};

	class SimpleBuiltInCommandNode : public ASTNode {
	public:
		SimpleBuiltInCommandNode() = default;
		SimpleBuiltInCommandNode(const std::string& prog) : program(prog) {}
		inline ASTNodeType type() const override {
			return ASTNodeType::SIMPLE_BUILTIN_COMMAND;
		}
		std::string toString() const override;

		void debug_print(int indent = 0) const override;

		inline SimpleBuiltInCommandNode& setProgram(const std::string& prog) {
			program = prog;
			return *this;
		}
		inline SimpleBuiltInCommandNode& addArgument(const std::string& argument) {
			arguments.push_back(argument);
			return *this;
		}
		inline SimpleBuiltInCommandNode& setArguments(const std::vector<std::string>& args) {
			arguments.clear();
			arguments = args;
			return *this;
		}
		inline SimpleBuiltInCommandNode& addOption(const std::string& key, const std::string& value) {
			options.insert(std::make_pair(key, value));
			return *this;
		}

	private:
		std::string program;
		std::vector<std::string> arguments;
		std::map<std::string, std::string> options;
	};

	class SimpleExternalCommandNode : public ASTNode {
	public:
		SimpleExternalCommandNode() = default;
		SimpleExternalCommandNode(const std::string& prog) : program(prog) {}
		inline ASTNodeType type() const override {
			return ASTNodeType::SIMPLE_EXTERNAL_COMMAND;
		}
		std::string toString() const override;

		void debug_print(int indent = 0) const override;

	private:
		std::string program;
		std::vector<std::string> tokens;
	};

	class PipeNode : public ASTNode {
	public:
		PipeNode() = default;
		inline ASTNodeType type() const override {
			return ASTNodeType::PIPE;
		}
		std::string toString() const override;
		
		void debug_print(int indent = 0) const override;

		inline PipeNode& addSender(ASTNode* nodeptr) {
			sender = std::unique_ptr<ASTNode>{ nodeptr };
			return *this;
		}
		inline PipeNode& addReceiver(ASTNode* nodeptr) {
			receiver = std::unique_ptr<ASTNode>{ nodeptr };
			return *this;
		}
	private:
		std::unique_ptr<ASTNode> sender;
		std::unique_ptr<ASTNode> receiver;
	};
}