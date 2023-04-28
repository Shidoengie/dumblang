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
public:
	Value EvalNode(Node expr);
private:
	Scope current;
	bool BoolConvert(double val);

	double numCalc(BinaryNode::Type opType, double leftValue, double rightValue);
	Value strCalc(BinaryNode::Type opType, string leftValue, string rightValue);
	Value CallBuiltinFunc(BuiltinFunc called, std::vector<Value> argValues);
	Value EvalAssignment(Assignment ass);
	Value EvalVariable(Variable var);
	Value EvalBlock(std::vector<Node> block, Scope previous);
	Value EvalUnaryNode(UnaryNode unaryOp);
	Value EvalBinaryNode(BinaryNode binOp);
	Value EvalCall(Call request);
	Value EvalBranch(BranchNode branch);
	Value EvalWhile(WhileNode loop);
public:
	Interpreter(std::map<string, Value> base);
	Interpreter(Scope base);
	Interpreter();
};