#pragma once
#include "AstNodes.h"
#include "ShlangError.h"
using std::cout;
using std::string;
class ReturnException
{
private:
	Value val_ = 0.0;
public:
	ReturnException(Value val) {
		this->val_ = val;
	}
	virtual const Value get() {
		return val_;
	}
};
class BreakException : public std::exception { };
Value PrintBuiltin(std::vector<Value> arguments);
Value InputBuiltin(std::vector<Value> arguments);

class Interpreter {
public:
	Block program_;
	void execute();
private:
	Value EvalNode(Node expr);
	Scope current;
	bool BoolConvert(double val);

	double numCalc(BinaryNode::Type opType, double leftValue, double rightValue);
	Value strCalc(BinaryNode::Type opType, string leftValue, string rightValue);
	Value CallBuiltinFunc(BuiltinFunc called, std::vector<Value> argValues);
	void EvalAssignment(Assignment ass);
	void EvalBlock(Block block);
	Value EvalVariable(Variable var);
	Value EvalUnaryNode(UnaryNode unaryOp);
	Value EvalBinaryNode(BinaryNode binOp);
	Value EvalCall(Call request);
	Value EvalBranch(BranchNode branch);
	Value EvalWhile(WhileNode loop);
public:
	Interpreter(Block program, std::map<string, Value> base);
	Interpreter(Block program, Scope base);
	Interpreter(Block program);
};