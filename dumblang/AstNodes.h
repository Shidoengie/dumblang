#pragma once
#include <iostream>
#include <vector>
#include <variant>
#include <map>
#include <stdexcept>
#include <exception>
#include <stack>
#include <string>

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
using Value = std::variant<double, std::string, Function, BuiltinFunc>;

using Node = std::variant<
	Value, BinaryNode, UnaryNode,
	Variable, Assignment, Call,
	Block, Return,BranchNode
>;
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
struct Assignment {
	std::string varName;
	Node* value;
};
struct Variable {
	std::string name;
};
struct Block {
	std::vector<Node> body;
};
struct Return {
	Node* object;
};
struct Call {
	Variable callee;
	std::vector<Node> args;
};
struct Function {
	Block block;
	std::vector<std::string> args;
};
struct BuiltinFunc {
	Value(*funcPointer)(std::vector<Value>);
	int argSize;
};
struct BranchNode {
	Block ifBlock;
	Block elseBlock;
	Node* condition;
};
struct Scope {
	Scope* parentScope;
	std::map<std::string, Value> varMap;
	bool isAtend();
	Value getVar(std::string varName);
	bool containsVar(std::string varName);
	void define(std::string varName, Value val);
};