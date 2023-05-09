#pragma once
#include "AstNodes.h"

using std::cout;
using std::string;
class Interpreter {
public:
	Block program_;
	void execute();
private:
	Value UnwrapReturn(Value result);
	Value EvalNode(Node expr);
	Scope current;
	double numCalc(BinaryNode::Type opType, double leftValue, double rightValue);
	Value strCalc(BinaryNode::Type opType, string leftValue, string rightValue);
	Value CallBuiltinFunc(BuiltinFunc called, ValueStream argValues);
	void EvalAssignment(Assignment ass);
	Value EvalBlock(Block block);
	Value EvalVariable(Variable var);
	Value EvalUnaryNode(UnaryNode unaryOp);
	Value EvalBinaryNode(BinaryNode binOp);
	Value EvalCall(Call request);
	Value EvalBranch(BranchNode branch);
	Value EvalWhile(WhileNode loop);
	Value EvalLoop(LoopNode loop);
public:
	Interpreter(Block program, std::map<string, Value> base);
	Interpreter(Block program, Scope base);
	Interpreter(Block program);
};