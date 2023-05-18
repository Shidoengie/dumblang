#pragma once
#include <iostream>
#include <vector>
#include <variant>
#include <map>
#include <stdexcept>
#include <exception>
#include <stack>
#include <string>
#include <optional>
struct Declaration;
struct Assignment;
struct Variable;
struct BinaryNode;
struct UnaryNode;
struct Call;
struct Function;
struct BuiltinFunc;
struct Block;
struct Return;
struct BranchNode;
struct WhileNode;
struct LoopNode;
struct Break {};
struct NoneType {};
struct BlockEnd {};
using Control = std::variant< Return, Break>;
using Value = std::variant< NoneType, double,bool, std::string, Function, BuiltinFunc,Control> ;

using Node = std::variant<
	Value, BinaryNode, UnaryNode,
	Variable, Assignment, Call,
	Block,BlockEnd, BranchNode,
	WhileNode,LoopNode,Declaration
>;
using NodeStream = std::vector<Node>;
using ValueStream = std::vector<Value>;
std::string ValueToString(Value val);
static const std::vector<std::string> typeMap = { "None","double","bool","string","Function","BuiltinFunc","Control"};
struct BinaryNode {
	enum Type{
		ADD,
		SUBTRACT,
		DIVIDE,
		MULTIPLY,
		MODULO,
		AND,
		OR,
		ISEQUAL,
		ISDIFERENT,
		GREATER,
		LESSER,
		GREATER_EQUAL,
		LESSER_EQUAL,
	};
	Type type;
	Node* left;
	Node* right;
};
struct UnaryNode {
	enum Type {
		NEGATE,
		POSITIVE,
		NOT,
	};
	Type type;
	Node* object;
};
struct Declaration {
	std::string varName;
	Node* value;
	
	Declaration(std::string varName_, Node* value_) {
		varName = varName_;
		value = value_;
	}
};
struct Assignment {
	std::string varName;
	Node* value;
};
struct Variable {
	std::string name;
};
struct Block {
	NodeStream body;
};
struct Return {
	Node* object;
};
struct Call {
	Variable callee;
	NodeStream args;
	Call(std::string callee_, NodeStream args_) {
		this->callee = Variable(callee_);
		this->args = args_;
	}
	Call(Variable callee_, NodeStream args_) {
		this->callee = callee_;
		this->args = args_;
	}
};
struct Function {
	Block block;
	std::vector<std::string> args;
};
struct BuiltinFunc {
	Value(*funcPointer)(ValueStream);
	int argSize;
};
struct BranchNode {
	Node* condition;
	Block* ifBlock;
	Block* elseBlock;
};
struct LoopNode {
	Block* loopBlock;
};
struct WhileNode {
	Node* condition;
	Block* loopBlock;
};
struct Scope {
	Scope* parentScope;
	std::map<std::string, Value> varMap;
	bool isAtend();
	std::optional<Value> getVar(std::string varName);
	bool containsVar(std::string varName);
	void define(std::string varName, Value val);
	std::optional<Value> assign(std::string varName, Value val);
	Scope create_copy() { return *this; }
	std::string to_string();
	std::string structure();
};