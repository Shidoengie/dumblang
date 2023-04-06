
#include "AST.h"
#include "Token.h"
#include <iostream>
#include <vector>
#include <variant>
#include <exception>
#include <map>
enum operatorType {
	ADD,
	SUBTRACT,
	DIVIDE,
	MULTIPLY,
};
struct Variable;
struct BinaryExpr;
using Expression = std::variant<double, BinaryExpr, Variable>;
bool hadError = false;
struct BinaryExpr {
	operatorType type;
	Expression* left;
	Expression* right;
};
struct Variable {
	std::string name;
};

std::map<std::string, double> varMap = {
	{"Test",2.0},
	{"PI",3.146210}
};

double Eval(Expression expr) {
	if (std::holds_alternative<double>(expr)) {
		return std::get<double>(expr);
	}
	if (std::holds_alternative<Variable>(expr)) {
		Variable var = std::get<Variable>(expr);
		if (varMap.contains(var.name)) {
			return varMap[var.name];
		}
		throw "Undeclared Variable";
	}
	if (std::holds_alternative<BinaryExpr>(expr)) {
		BinaryExpr binOp = std::get<BinaryExpr>(expr);
		double leftValue = Eval(*binOp.left);
		double rightValue = Eval(*binOp.right);
		return binaryCalc(binOp.type, leftValue, rightValue);
	}

};
double binaryCalc(operatorType opType, double leftValue, double rightValue) {
	switch (opType)
	{
	case ADD:
		return leftValue + rightValue;
		break;
	case MULTIPLY:
		return leftValue * rightValue;
		break;
	case DIVIDE:
		return leftValue / rightValue;
		break;
	case SUBTRACT:
		return leftValue - rightValue;
		break;
	default:
		std::cout << "idk yet";
		break;
	}
}