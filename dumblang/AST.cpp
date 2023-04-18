#include "AstNodes.h"
#include "ShlangError.h"
#include <ranges>

template<class Ty, class... Types>
constexpr bool variantHas(const std::variant<Types...>& var) noexcept {
	return std::holds_alternative<Ty>(var);
}
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
bool BoolConvert(double val) {
	bool out = (val != 0.0) ? true : false;
	return out;
};
Value PrintBuiltin(std::vector<Value> arguments) {
	for (auto const& argument : arguments) {
		if (variantHas<string>(argument)) {
			cout << std::get<string>(argument) << '\n';
		}
		if (variantHas<double>(argument)) {
			cout << std::get<double>(argument) << '\n';
		}
	}
	return 0.0;
}
Value InputBuiltin(std::vector<Value> arguments) {
	PrintBuiltin(arguments);
	string out;
	std::getline(std::cin, out);
	return Value(out);
};
std::map<string, Value> defMap = {
	{"Test",2.0},
	{"PI",3.146210},
	{"print",BuiltinFunc(&PrintBuiltin,-1)},
	{"input",BuiltinFunc(&InputBuiltin,1)}
};

void stackBlockEval(Block block) {
	std::stack<Node, std::vector<Node>> stack;
	std::stack<Scope, std::vector<Scope>> variable_scopes;
	for (auto const& expression : std::views::reverse(block.body)) {
		stack.push(expression);
	}
}

double numCalc(BinaryNode::Type opType, double leftValue, double rightValue) {
	bool leftBool = BoolConvert(leftValue);
	bool rightBool = BoolConvert(rightValue);
	switch (opType)
	{
	case BinaryNode::ADD:
		return leftValue + rightValue;
		break;
	case BinaryNode::MULTIPLY:
		return leftValue * rightValue;
		break;
	case BinaryNode::DIVIDE:
		return leftValue / rightValue;
		break;
	case BinaryNode::SUBTRACT:
		return leftValue - rightValue;
		break;
	case BinaryNode::MODULO:
		return fmod(leftValue,rightValue);
		break;
	case BinaryNode::AND:
		return (double)(leftBool && rightBool);
		break;
	case BinaryNode::OR:
		
		return (double)(leftBool || rightBool);
		break;
	case BinaryNode::ISEQUAL:
		return (double)(leftValue == rightValue);
		break;
	case BinaryNode::ISDIFERENT:
		return (double)(leftValue == rightValue);
		break;
	case BinaryNode::GREATER:
		return (double)(leftValue > rightValue);
		break;
	case BinaryNode::GREATER_EQUAL:
		return (double)(leftValue >= rightValue);
		break;
	case BinaryNode::LESSER:
		return (double)(leftValue < rightValue);
		break;
	case BinaryNode::LESSER_EQUAL:
		return (double)(leftValue <= rightValue);
		break;
	default:
		break;
	}
}
Value strCalc(BinaryNode::Type opType, string leftValue, string rightValue) {
	switch (opType)
	{
	case BinaryNode::ADD:
		return leftValue + rightValue;
		break;
	case BinaryNode::ISEQUAL:
		return (double)(leftValue == rightValue);
		break;
	case BinaryNode::ISDIFERENT:
		return (double)(leftValue != rightValue);
		break;
	case BinaryNode::GREATER:
		return (double)(leftValue > rightValue);
		break;
	case BinaryNode::GREATER_EQUAL:
		return (double)(leftValue >= rightValue);
		break;
	case BinaryNode::LESSER:
		return (double)(leftValue < rightValue);
		break;
	case BinaryNode::LESSER_EQUAL:
		return (double)(leftValue <= rightValue);
		break;
	default:
		throw LangError("Invalid operator", LangError::AST);
	}
}
Value CallBuiltinFunc(BuiltinFunc called, std::vector<Value> argValues) {
	if (called.argSize != -1 && argValues.size() != called.argSize) {
		throw LangError("Incomplete arguments", LangError::AST);
	}
	return called.funcPointer(argValues);
}
Value EvalNode(Node expr, Scope currentScope);

Scope EvalAssignment(Scope current, Assignment* ass) {
	Value assignVal = EvalNode(*ass->value, current);
	current.define(ass->varName, assignVal);
	return current;
}

Value EvalVariable(Variable var, Scope currentScope) {
	if (currentScope.containsVar(var.name)) {
		return currentScope.getVar(var.name);
	}
	else {
		if (currentScope.isAtend()) {
			throw LangError("Undeclared Variable", LangError::AST);
		}
		return EvalNode(var, *currentScope.parentScope);

	}
}
void EvalBlock(std::vector<Node> block, Scope previous) {
	Scope newScope = Scope(&previous, {});
	for (auto& statement : block) {
		EvalNode(statement, newScope);
		if (auto ass = std::get_if<Assignment>(&statement)) {
			newScope = EvalAssignment(newScope, ass);
		}
	}
};

Value EvalUnaryNode(UnaryNode unaryOp, Scope current) {
	Value obj = EvalNode(*unaryOp.object, current);
	switch (unaryOp.type)
	{
	case UnaryNode::NEGATE:
		if (auto val = std::get_if<double>(&obj)) {
			return -(*val);
		}
		throw LangError("Invalid type", LangError::AST);
	case UnaryNode::POSITIVE:
		if (auto val = std::get_if<double>(&obj)) {
			return +(*val);
		}
		throw LangError("Invalid type", LangError::AST);
		break;
	case UnaryNode::NOT:
		if (auto val = std::get_if<double>(&obj)) {
			bool out = BoolConvert(*val);
			return (double)(!out);
		}
		throw LangError("Invalid type", LangError::AST);
	default:
		break;
	}
};
Value EvalBinaryNode(BinaryNode binOp, Scope current) {
	Value leftValue = EvalNode(*binOp.left, current);
	Value rightValue = EvalNode(*binOp.right, current);
	if (leftValue.index() != rightValue.index()) {
		
		if (binOp.type == BinaryNode::ISEQUAL) {
			return 0.0;
		}
		if (binOp.type == BinaryNode::ISDIFERENT) {
			return 1.0;
		}
		throw LangError("Mixed types", LangError::AST);
	}
	if (variantHas<double>(leftValue)) {
		return numCalc(binOp.type, std::get<double>(leftValue), std::get<double>(rightValue));
	}
	if (variantHas<string>(leftValue)) {
		return strCalc(binOp.type, std::get<string>(leftValue), std::get<string>(rightValue));
	}
}

Value EvalCall(Call request, Scope currentScope) {
	Value getFunc = EvalNode(request.callee, currentScope);
	std::vector<Value> argValues;
	for (size_t index = 0; index < request.args.size(); index++) {
		argValues.push_back(EvalNode(request.args[index], currentScope));
	}
	if (variantHas<BuiltinFunc>(getFunc)) {
		return CallBuiltinFunc(std::get<BuiltinFunc>(getFunc), argValues);
	}
	if (!variantHas<Function>(getFunc)) {
		throw LangError("Invalid Call", LangError::AST);
	}
	Function calledFunc = std::get<Function>(getFunc);
	if (request.args.size() != calledFunc.args.size()) {
		throw LangError("Incomplete arguments", LangError::AST);
	}
	for (size_t index = 0; index < calledFunc.args.size(); index++) {
		string var = calledFunc.args[index];
		Value assignVal = argValues[index];
		currentScope.define(var, assignVal);
	}
	try {
		EvalNode(calledFunc.block, currentScope);
	}
	catch (ReturnException val) {
		return val.what();
	}
}
Value EvalNode(Node expr, Scope currentScope) {
	if (auto val = std::get_if<Value>(&expr)) {
		return *val;
	}
	if (auto block = std::get_if<Block>(&expr)) {
		EvalBlock(block->body,currentScope);
	}
	if (auto var = std::get_if<Variable>(&expr)) {
		return EvalVariable(*var, currentScope);
	}
	if (auto request = std::get_if<Call>(&expr)) {
		return EvalCall(*request, currentScope);
	}
	if (auto ret = std::get_if<Return>(&expr)) {
		throw ReturnException(EvalNode(*ret->object,currentScope));
	}
	if (auto unaryOp = std::get_if<UnaryNode>(&expr)) {
		return EvalUnaryNode(*unaryOp,currentScope);
	}
	if (auto binOp = std::get_if<BinaryNode>(&expr)) {
		return EvalBinaryNode(*binOp, currentScope);
	}
	if (auto branch = std::get_if<BranchNode>(&expr)) {
		Value conditionVal = EvalNode(*branch->condition,currentScope);
		if (!variantHas<double>(conditionVal)) {
			throw LangError("Unexpected type Expected Number",LangError::AST);
		}
		bool condition = BoolConvert(std::get<double>(conditionVal));
		if (condition) {
			EvalBlock(branch->ifBlock.body,currentScope);
		}
		else if(branch->elseBlock.body.size() > 0){
			
			EvalBlock(branch->elseBlock.body, currentScope);
		}
	}
};
