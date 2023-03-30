
#include "AST.h"
#include "Token.h"
#include <iostream>
#include <vector>
#include <variant>

static int count = 0;
enum operatorType {
	ADD,
	SUBTRACT,
	DIVIDE,
	MULTIPLY,
	ASSIGN
};
struct Variable;
struct BinaryExpr;
typedef std::variant<double, BinaryExpr, Variable> Expression;

struct BinaryExpr {
	operatorType type;
	Expression* left;
	Expression* right;
};
struct Variable
{
	std::string name;
};
double eval(Expression expr) {
	if (std::holds_alternative<double>(expr)) {
		return std::get<double>(expr);
	}
	else {
		BinaryExpr binOp = std::get<BinaryExpr>(expr);
		double left_value = eval(*binOp.left);
		double right_value = eval(*binOp.right);
		switch (binOp.type)
		{
		case ADD:
			return left_value + right_value;
			break;
		case MULTIPLY:
			return left_value * right_value;
			break;
		case DIVIDE:
			return left_value / right_value;
			break;
		case SUBTRACT:
			return left_value - right_value;
			break;
		default:
			break;
		}
	}
};