
#include "AST.h"
#include "Token.h"
#include <iostream>
#include <vector>
#include <variant>
#include <exception>
static int count = 0;
enum operatorType {
	ADD,
	SUBTRACT,
	DIVIDE,
	MULTIPLY,
};
struct Variable;
struct BinaryExpr;
typedef std::variant<double, BinaryExpr, Variable> Expression;
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
	{"Test",10.0}
};

double Eval(Expression expr) {
	if (std::holds_alternative<double>(expr)) {
		return std::get<double>(expr);
	}
	else if (std::holds_alternative<Variable>(expr)) {
		Variable var = std::get<Variable>(expr);
		if (varMap.contains(var.name)) {
			return varMap[var.name];
		}
		else { 
			throw "Undeclared Variable";
		}
	}
	else {
		BinaryExpr binOp = std::get<BinaryExpr>(expr);
		double leftValue, rightValue;
		try {
			leftValue = Eval(*binOp.left);
		}
		catch (const char* msg) {
			std::cout << msg << '\n';
		}
		try {
			rightValue = Eval(*binOp.right);
		}
		catch (const char* msg) {
			std::cout << msg << '\n';
		}
		switch (binOp.type)
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
};
