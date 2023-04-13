
#include "AST.h"
#include "Token.h"
#include <iostream>
#include <vector>
#include <variant>
#include <exception>
#include <map>

enum BinaryType {
	ADD,
	SUBTRACT,
	DIVIDE,
	MULTIPLY,
};
enum UnaryType {
	NEGATE,
	POSITIVE
};
struct Assignment;
struct Variable;
struct BinaryExpr;
struct UnaryExpr;
struct Call;
struct Function;
struct BuiltinFunc;
using Value = std::variant<double, std::string, Function, BuiltinFunc>;
using Expression = std::variant<Value, BinaryExpr,UnaryExpr, Variable, Assignment, Call>;
bool hadError = false;
struct BinaryExpr {
	BinaryType type;
	Expression* left;
	Expression* right;
};
struct UnaryExpr {
	UnaryType type;
	Expression* object;
};
struct Assignment {
	std::string varName;
	Expression* value;
};
struct Variable {
	std::string name;
};

struct Call {
	Variable callee;
	std::vector<Expression> args;
};
struct Function {
	Expression* body;
	std::vector<std::string> args;
};
struct BuiltinFunc {
	Value(*funcPointer)(std::vector<Value>);
};
std::map<std::string, Value> varMap = {
	{"Test",2.0},
	{"PI",3.146210},
	{"print",BuiltinFunc(&print_builtin)}
};

Value Eval(Expression expr) {
	if (std::holds_alternative<Value>(expr)) {
		return std::get<Value>(expr);
	}
	if (std::holds_alternative<Assignment>(expr)) {
		Assignment ass = std::get<Assignment>(expr);
		Value assignVal = Eval(*ass.value);
		varMap.insert_or_assign(ass.varName, assignVal);
	}
	if (std::holds_alternative<Variable>(expr)) {
		Variable var = std::get<Variable>(expr);
		if (varMap.contains(var.name)) {
			return varMap[var.name];
		}
		throw "Undeclared Variable";
	}
	if (std::holds_alternative<Call>(expr)) {
		Call request = std::get<Call>(expr);
		Value getFunc = Eval(request.callee);
		if (!std::holds_alternative<Function>(getFunc)) {
			if (std::holds_alternative<BuiltinFunc>(getFunc)) {
				//do stuff
			}
			throw "Invalid Call";
		}
		Function calledFunc = std::get<Function>(getFunc);
		if (request.args.size() != calledFunc.args.size()) {
			throw "Incomplete arguments";
		}
		for (size_t index = 0; index < calledFunc.args.size(); index++) {
			std::string var = calledFunc.args[index];
			Value assignVal = Eval(request.args[index]);
			varMap.insert_or_assign(var, assignVal);
		}
		
		
	}
	if (std::holds_alternative<UnaryExpr>(expr)) {
		UnaryExpr unaryOp = std::get<UnaryExpr>(expr);
		Value obj = Eval(*unaryOp.object);
		switch (unaryOp.type)
		{
		case NEGATE:
			if (std::holds_alternative<double>(obj)) {
				return -(std::get<double>(obj));
			}
			throw "Invalid type";
		case POSITIVE:
			if (std::holds_alternative<double>(obj)) {
				return +(std::get<double>(obj));
			}
			throw "Invalid type";
			break;
		default:
			break;
		}
		
	}
	if (std::holds_alternative<BinaryExpr>(expr)) {
		BinaryExpr binOp = std::get<BinaryExpr>(expr);
		Value leftValue = Eval(*binOp.left);
		Value rightValue = Eval(*binOp.right);
		if (leftValue.index() != rightValue.index()) {
			throw "Mixed types";
		}
		if (std::holds_alternative<double>(leftValue)) {
			return numCalc(binOp.type, std::get<double>(leftValue), std::get<double>(rightValue));
		}
		if (std::holds_alternative<std::string>(leftValue)) {
			return strCalc(binOp.type, std::get<std::string>(leftValue), std::get<std::string>(rightValue));
		}
	}
};
double numCalc(BinaryType opType, double leftValue, double rightValue) {
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
std::string strCalc(BinaryType opType, std::string leftValue, std::string rightValue) {
	if (opType != ADD) {
		throw "invalid operator";
	}
	return leftValue + rightValue;
}
Value print_builtin(std::vector<Value> arguments) {
	for (auto const& argument : arguments) {
		if (std::holds_alternative<std::string>(argument)) {
			std::cout << std::get<std::string>(argument);
		}
		if (std::holds_alternative<double>(argument)) {
			std::cout << std::get<double>(argument);
		}
	}
}