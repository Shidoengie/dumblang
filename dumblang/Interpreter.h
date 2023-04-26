#pragma once
#include "AstNodes.h"
#include "ShlangError.h"
using std::cout;
using std::string;
class ReturnException : public std::exception
{
private:
	Value val_;
public:
	virtual const Value what() {
		return val_;
	}
	ReturnException(Value val) {
		val_ = val;
	}
};
class BreakException : public std::exception { };
Value PrintBuiltin(std::vector<Value> arguments);
Value InputBuiltin(std::vector<Value> arguments);

class Interpreter {
private:
	Value EvalNode(Node expr, std::vector<Scope>& scopes);
	bool BoolConvert(double val);
	double numCalc(BinaryNode::Type opType, double leftValue, double rightValue);
	Value strCalc(BinaryNode::Type opType, string leftValue, string rightValue);
	Value CallBuiltinFunc(BuiltinFunc called, std::vector<Value> argValues);
	Value EvalAssignment(Assignment ass, std::vector<Scope>& scopes);
	Value EvalVariable(Variable var, std::vector<Scope>& scopes);
	void EvalBlock(Block block);
	void EvalWhile(WhileNode loop, std::vector<Scope>& scopes);
	Value EvalUnaryNode(UnaryNode::Type type, Value obj);
	Value EvalBinaryNode(BinaryNode::Type type, Value leftValue, Value rightValue);
	Value EvalCall(Call request, std::vector<Scope>& scopes);
	Value EvalBranch(BranchNode branch, std::vector<Scope>& scopes);
public:
	Block base;
	Interpreter(Block base_);
	void execute();
};