#pragma once
#include "AstNodes.h"
#include "ShlangError.h"
using std::cout;
using std::string;
bool BoolConvert(double val);
Value PrintBuiltin(std::vector<Value> arguments);
Value InputBuiltin(std::vector<Value> arguments);
double numCalc(BinaryNode::Type opType, double leftValue, double rightValue);
Value strCalc(BinaryNode::Type opType, string leftValue, string rightValue);
Value CallBuiltinFunc(BuiltinFunc called, std::vector<Value> argValues);
Value EvalNode(Node expr, Scope currentScope);
Value EvalAssignment(Scope current, Assignment* ass);
Value EvalVariable(Variable var, Scope currentScope);
Value EvalBlock(std::vector<Node> block, Scope previous);
Value EvalUnaryNode(UnaryNode unaryOp, Scope current);
Value EvalBinaryNode(BinaryNode binOp, Scope current);
Value EvalCall(Call request, Scope current);
Value EvalBranch(BranchNode branch, Scope current);
Value EvalNode(Node expr, Scope currentScope);


static std::map<string, Value> defMap = {
	{"Test",2.0},
	{"PI",3.146210},
	{"print",BuiltinFunc(&PrintBuiltin,-1)},
	{"input",BuiltinFunc(&InputBuiltin,1)}
};