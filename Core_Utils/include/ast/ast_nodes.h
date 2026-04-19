#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <optional>

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
		COMMAND_SUBST,				//命令替换$()
		VARIABLE					//变量
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

		inline PipeNode& setSender(std::unique_ptr<ASTNode> nodeptr) {
			sender = std::move(nodeptr);
			return *this;
		}
		inline PipeNode& setReceiver(std::unique_ptr<ASTNode> nodeptr) {
			receiver = std::move(nodeptr);
			return *this;
		}
	private:
		std::unique_ptr<ASTNode> sender;
		std::unique_ptr<ASTNode> receiver;
	};

	// RedirectNode
	class RedirectNode : public ASTNode {
	private:
		enum class RedirectType {
			INPUT,
			OUTPUT,
			APPEND,
			HEREDOC
		};
	public:
		RedirectNode(): redirect_type(RedirectType::APPEND) {}

		inline ASTNodeType type() const override {
			return ASTNodeType::REDIRECT;
		}
		std::string toString() const override;
		void debug_print(int indent = 0) const override;

		inline RedirectNode& setCommand(std::unique_ptr<ASTNode> nodeptr) {
			command = std::move(nodeptr);
			return *this;
		}
		inline RedirectNode& setFile(const std::string& _file) {
			file = _file;
			return *this;
		}
		inline RedirectNode& setRedirectType(const std::string& symbol) {
			if (symbol == "<") redirect_type = RedirectType::INPUT;
			if (symbol == ">") redirect_type = RedirectType::OUTPUT;
			if (symbol == "<<") redirect_type = RedirectType::APPEND;
			if (symbol == ">>") redirect_type = RedirectType::HEREDOC;
			return *this;
		}
	private:
		std::unique_ptr<ASTNode> command;
		std::string file;
		RedirectType redirect_type;
		std::string getRedirectSymbol() const;
		std::string getRedirectTypeName() const;
	};

	class AndOrNode : public ASTNode {
	private:
		enum class AndOrType {
			AND,
			OR,
			NOT
		};
	public:
		std::string getSymbol() const;
		std::string getTypeName() const;
		inline ASTNodeType type() const override {
			return ASTNodeType::AND_OR;
		}
		std::string toString() const override;
		void debug_print(int indent = 0) const override;
	private:
		AndOrType andor_type;
		std::optional<std::unique_ptr<ASTNode>> left;
		std::unique_ptr<ASTNode> right;
	};

	class BackgroundNode : public ASTNode {
	private:
		std::unique_ptr<ASTNode> command;
	public:
		BackgroundNode() = default;
		inline ASTNodeType type() const override {
			return ASTNodeType::BACKGROUND;
		}
		std::string toString() const override;
		void debug_print(int indent = 0) const override;
		inline BackgroundNode& setCommand(std::unique_ptr<ASTNode> nodeptr) {
			command = std::move(nodeptr);
			return *this;
		}
	};

	class SubshellNode : public ASTNode {
	public:
		SubshellNode() = default;
		SubshellNode(std::unique_ptr<ASTNode> nodeptr): command(std::move(nodeptr)) {}
		inline SubshellNode& setCommand(std::unique_ptr<ASTNode> nodeptr) {
			command = std::move(nodeptr);
			return *this;
		}
		inline ASTNodeType type() const override {
			return ASTNodeType::SUBSHELL;
		}
		std::string toString() const override;
		void debug_print(int indent = 0) const override;
	private:
		std::unique_ptr<ASTNode> command;
	};

	class CommandListNode : public ASTNode {
	public:
		CommandListNode() = default;
		inline CommandListNode& addCommand(std::unique_ptr<ASTNode> nodeptr) {
			commands.push_back(std::move(nodeptr));
			return *this;
		}
		inline ASTNodeType type() const override {
			return ASTNodeType::SEQUENCE;
		}
		std::string toString() const override;
		void debug_print(int indent = 0) const override;
	private:
		std::vector<std::unique_ptr<ASTNode>> commands;
	};

	class CommandSubstNode : public ASTNode {
	public:
		CommandSubstNode() = default;
		inline CommandSubstNode& setCommand(std::unique_ptr<ASTNode> nodeptr) {
			command = std::move(nodeptr);
			return *this;
		}
		inline ASTNodeType type() const override {
			return ASTNodeType::COMMAND_SUBST;
		}
		std::string toString() const override;
		void debug_print(int indent = 0) const override;
	private:
		std::unique_ptr<ASTNode> command;
	};
}