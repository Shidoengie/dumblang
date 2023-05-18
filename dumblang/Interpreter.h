#pragma once
#include "AstNodes.h"

using std::cout;
using std::string;
class Interpreter {
public:
	Block program_;
	void execute();
private:
	Scope current;

	Value FilterReturn(Value result);
	Value UnwrapReturnValue(Value result);
	Value EvalNode(Node expr);
	void EvalDeclaration(Declaration request);
	Value EvalAssignment(Assignment ass);
	Value EvalVariable(Variable var);
	Value EvalBlock(Block block);
	Value numCalc(BinaryNode::Type opType, Value left, Value right);
	Value strCalc(BinaryNode::Type opType, string leftValue, string rightValue);
	Value EvalUnaryNode(UnaryNode unaryOp);
	Value EvalBinaryNode(BinaryNode binOp);
	Value CallBuiltinFunc(BuiltinFunc called, ValueStream argValues);
	Value EvalCall(Call request);
	Value EvalBranch(BranchNode branch);
	Value EvalWhile(WhileNode loop);
	Value EvalLoop(LoopNode loop);
public:
	Interpreter(Block program, std::map<string, Value> base);
	Interpreter(Block program, Scope base);
	Interpreter(Block program);
};