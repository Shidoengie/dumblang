#include "AST.h"
template<class Ty, class... Types>
constexpr bool variantHas(const std::variant<Types...>& var) noexcept {
	return std::holds_alternative<Ty>(var);
}
using std::cout;
using std::string;
static Scope current = Scope();
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
	return NoneType();
}
Value InputBuiltin(std::vector<Value> arguments) {
	PrintBuiltin(arguments);
	string out;
	std::getline(std::cin, out);
	return Value(out);
};

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
Value EvalNode(Node expr);

Value EvalAssignment(Scope current, Assignment* ass) {
	Value controlType = EvalNode(*ass->value, current);
	if (variantHas<NoneType>(controlType)) {
		throw LangError("Cannot assign None to a variable",LangError::AST);
	}
	return controlType;
}

Value EvalVariable(Variable var) {
	return current.getVar(var.name);
}

Value EvalBlock(std::vector<Node> block, Scope previous) {
	Scope newScope = Scope(&previous, {});
	if (block.size() == 0) {
		return NoneType();
	}
	for (auto& statement : block) {
		if (auto ass = std::get_if<Assignment>(&statement)) {
			newScope.define(ass->varName,EvalAssignment(newScope, ass));
		}
		Value val = EvalNode(statement, newScope);
		if (auto retVal = std::get_if<Return>(&val)) {
			return *retVal;
		}
	}
	return NoneType();
};

Value EvalUnaryNode(UnaryNode unaryOp) {
	Value obj = EvalNode(*unaryOp.object, current);
	if(auto val = std::get_if<Return>(&obj)) {
		obj = EvalNode(*val->object,current);
	}
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
Value EvalBinaryNode(BinaryNode binOp) {
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

Value EvalCall(Call request) {
	Value getFunc = EvalNode(request.callee, current);
	std::vector<Value> argValues;
	for (size_t index = 0; index < request.args.size(); index++) {
		argValues.push_back(EvalNode(request.args[index], current));
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
		current.define(var, assignVal);
	}
	Value retVal = EvalNode(calledFunc.block,current);
	if (auto val = std::get_if<Return>(&retVal)) {
		return EvalNode(*val->object, current);
	}
	return NoneType();
}
Value EvalBranch(BranchNode branch) {
	Value conditionVal = EvalNode(*branch.condition, current);
	if (!variantHas<double>(conditionVal)) {
		throw LangError("Unexpected type Expected Number", LangError::AST);
	}
	bool condition = BoolConvert(std::get<double>(conditionVal));
	if (branch.ifBlock == nullptr) {
		throw LangError("Invalid Branch", LangError::AST);
	}
	if (condition) {

		Value result = EvalNode(*branch.ifBlock, current);
		if (auto val = std::get_if<Return>(&result)) {
			return *val;
		}
		return NoneType();
	}
	else if (branch.elseBlock != nullptr) {
		Value result = EvalNode(*branch.elseBlock, current);
		if (auto val = std::get_if<Return>(&result)) {
			return *val;
		}
		return NoneType();
	}
	return NoneType();
}
Value EvalNode(Node expr) {
	if (auto val = std::get_if<Value>(&expr)) {
		return *val;
	}
	if (auto block = std::get_if<Block>(&expr)) {
		return EvalBlock(block->body,Scope());
	}
	if (auto var = std::get_if<Variable>(&expr)) {
		return EvalVariable(*var);
	}
	if (auto request = std::get_if<Call>(&expr)) {
		return EvalCall(*request);
	}
	if (auto unaryOp = std::get_if<UnaryNode>(&expr)) {
		return EvalUnaryNode(*unaryOp);
	}
	if (auto binOp = std::get_if<BinaryNode>(&expr)) {
		return EvalBinaryNode(*binOp);
	}
	if (auto branch = std::get_if<BranchNode>(&expr)) {
		return EvalBranch(*branch);
	}
};

