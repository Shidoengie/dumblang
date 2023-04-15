
#include "AST.h"
#include "Token.h"
#include <iostream>
#include <vector>
#include <variant>
#include <map>
#include <stdexcept>
#include <exception>
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
struct Block;
struct Return;
using Value = std::variant<double, std::string, Function, BuiltinFunc>;

template<class Ty, class... Types>
constexpr bool variantHas(const std::variant<Types...>& var) noexcept {
	return std::holds_alternative<Ty>(var);
}

using Expression = std::variant<
	Value, BinaryExpr, UnaryExpr, 
	Variable, Assignment, Call, 
	Block
>;
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
struct Block {
	std::vector<Expression> body;
};
struct Return {
	Expression* object;
};
struct Call {
	Variable callee;
	std::vector<Expression> args;
};
struct Function {
	std::vector<Expression> body;
	std::vector<std::string> args;
};
struct BuiltinFunc {
	Value(*funcPointer)(std::vector<Value>);
	int argSize;
};

class ReturnException : public std::exception
{
private :
	Value val_;
public:
	virtual const Value what() {
		return val_;
	}
	ReturnException(Value val) {
		val_ = val;
	}
};
class ASTerror : public std::exception
{
private:
	std::string msg;
public:
	virtual const std::string what() {
		return msg;
	}
	ASTerror(std::string message) {
		msg = message;
	}
};
Value print_builtin(std::vector<Value> arguments) {
	for (auto const& argument : arguments) {
		if (variantHas<std::string>(argument)) {
			std::cout << std::get<std::string>(argument);
		}
		if (variantHas<double>(argument)) {
			std::cout << std::get<double>(argument);
		}
	}
}
std::map<std::string, Value> varMap = {
	{"Test",2.0},
	{"PI",3.146210},
	{"print",BuiltinFunc(&print_builtin,-1)}
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
		break;
	}
}
std::string strCalc(BinaryType opType, std::string leftValue, std::string rightValue) {
	if (opType != ADD) {
		throw ASTerror("Invalid operator");
	}
	return leftValue + rightValue;
}
Value CallBuiltinFunc(BuiltinFunc called, std::vector<Value> argValues) {
	if (called.argSize != -1 && argValues.size() != called.argSize) {
		throw ASTerror("Incomplete arguments");
	}
	return called.funcPointer(argValues);
}
Value Eval(Expression expr) {
	if (auto val = std::get_if<Value>(&expr)) {
		return *val;
	}
	if (auto ass = std::get_if<Assignment>(&expr)) {
		Value assignVal = Eval(*ass->value);
		varMap.insert_or_assign(ass->varName, assignVal);
	}
	if (auto var = std::get_if<Variable>(&expr)) {
		if (varMap.contains(var->name)) {
			return varMap[var->name];
		}
		throw ASTerror("Undeclared Variable");
	}
	if (variantHas<Call>(expr)) {
		auto request = std::get<Call>(expr);
		Value getFunc = Eval(request.callee);
		std::vector<Value> argValues;
		for (size_t index = 0; index < request.args.size(); index++) {
			argValues.push_back(Eval(request.args[index]));
		}
		if (variantHas<BuiltinFunc>) {
			return CallBuiltinFunc(std::get<BuiltinFunc>(getFunc), argValues);
		}
		if (!variantHas<Function>(getFunc)) {
			throw ASTerror("Invalid Call");
		}
		Function calledFunc = std::get<Function>(getFunc);
		if (request.args.size() != calledFunc.args.size()) {
			throw ASTerror("Incomplete arguments");
		}
		for (size_t index = 0; index < calledFunc.args.size(); index++) {
			std::string var = calledFunc.args[index];
			Value assignVal = argValues[index];
			varMap.insert_or_assign(var, assignVal);
		}
		
		for (auto& statement : calledFunc.body) {
			try {
				Eval(statement);
			}
			catch (ReturnException val) {
				return val.what();
			}
		}
	}
	if (auto block = std::get_if<Block>(&expr)) {
		for (auto& statement : block->body) {
			Eval(statement);
		}
	}
	if (auto ret = std::get_if<Return>(&expr)) {
		throw ReturnException(Eval(*ret->object));
	}
	if (auto unaryOp = std::get_if<UnaryExpr>(&expr)) {
		Value obj = Eval(*unaryOp->object);
		switch (unaryOp->type)
		{
		case NEGATE:
			if (auto val = std::get_if<double>(&obj)) {
				return -(*val);
			}
			throw ASTerror("Invalid type");
		case POSITIVE:
			if (auto val = std::get_if<double>(&obj)) {
				return +(*val);
			}
			throw ASTerror("Invalid type");
			break;
		default:
			break;
		}
		
	}
	if (auto binOp = std::get_if<BinaryExpr>(&expr)) {
		Value leftValue = Eval(*binOp->left);
		Value rightValue = Eval(*binOp->right);
		if (leftValue.index() != rightValue.index()) {
			throw ASTerror("Mixed types");
		}
		if (variantHas<double>(leftValue)) {
			return numCalc(binOp->type, std::get<double>(leftValue), std::get<double>(rightValue));
		}
		if (variantHas<std::string>(leftValue)) {
			return strCalc(binOp->type, std::get<std::string>(leftValue), std::get<std::string>(rightValue));
		}
	}
};
