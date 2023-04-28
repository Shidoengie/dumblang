#include "Interpreter.h"

template<class Ty, class... Types>
constexpr bool variantHas(const std::variant<Types...>& var) noexcept {
	return std::holds_alternative<Ty>(var);
}



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
Value StringToNum(std::vector<Value> arguments) {
	Value argument = arguments[0];
	if (auto val = std::get_if<string>(&argument)) {
		try {
			return std::stod(*val);
		}
		catch (std::invalid_argument) {
			throw InvalidStringFormatError(*val);
		}
	}
	if (variantHas<double>(argument)) {
		return argument;
	}

}
bool Interpreter::BoolConvert(double val) {
	bool out = (val != 0.0) ? true : false;
	return out;
};
double Interpreter::numCalc(BinaryNode::Type opType, double leftValue, double rightValue) {
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
Value Interpreter::strCalc(BinaryNode::Type opType, string leftValue, string rightValue) {
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
	}
}
Value Interpreter::CallBuiltinFunc(BuiltinFunc called, std::vector<Value> argValues) {
	if (called.argSize != -1 && argValues.size() != called.argSize) {
		throw LangError("Incomplete arguments", LangError::AST);
	}
	return called.funcPointer(argValues);
}
Value Interpreter::EvalAssignment(Assignment ass) {
	Value controlType = EvalNode(*ass.value);
	if (variantHas<NoneType>(controlType)) {
		throw LangError("Cannot assign None to a variable", LangError::AST);
	}
	current.define(ass.varName, controlType);
	return controlType;
}

Value Interpreter::EvalVariable(Variable var) {
	return current.getVar(var.name);
}

Value Interpreter::EvalBlock(std::vector<Node> block, Scope scopeBlock) {
	scopeBlock.parentScope = &current;
	if (block.size() == 0) {
		return NoneType();
	}
	for (auto& statement : block) {
		current = *scopeBlock.parentScope;
		Value val = EvalNode(statement);
	}
	return NoneType();
};

Value Interpreter::EvalUnaryNode(UnaryNode unaryOp) {
	Value obj = EvalNode(*unaryOp.object);
	switch (unaryOp.type)
	{
	case UnaryNode::NEGATE:
		if (auto val = std::get_if<double>(&obj)) {
			return -(*val);
		}
	case UnaryNode::POSITIVE:
		if (auto val = std::get_if<double>(&obj)) {
			return +(*val);
		}
		break;
	case UnaryNode::NOT:
		if (auto val = std::get_if<double>(&obj)) {
			bool out = BoolConvert(*val);
			return (double)(!out);
		}
	default:
		break;
	}
};
Value Interpreter::EvalBinaryNode(BinaryNode binOp) {
	Value leftValue = EvalNode(*binOp.left);
	Value rightValue = EvalNode(*binOp.right);
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

Value Interpreter::EvalCall(Call request) {
	Value getFunc = EvalNode(request.callee);
	std::vector<Value> argValues;
	for (size_t index = 0; index < request.args.size(); index++) {
		argValues.push_back(EvalNode(request.args[index]));
	}
	if (variantHas<BuiltinFunc>(getFunc)) {
		return CallBuiltinFunc(std::get<BuiltinFunc>(getFunc), argValues);
	}
	if (!variantHas<Function>(getFunc)) {
	}
	Function calledFunc = std::get<Function>(getFunc);
	if (request.args.size() != calledFunc.args.size()) {
	}
	for (size_t index = 0; index < calledFunc.args.size(); index++) {
		string var = calledFunc.args[index];
		Value assignVal = argValues[index];
		current.define(var, assignVal);
	}
	try {
		EvalNode(EvalNode(calledFunc.block));
	}
	catch (ReturnException value) {
		return value.what();
	}
	return NoneType();
}
Value Interpreter::EvalBranch(BranchNode branch) {
	Value conditionVal = EvalNode(*branch.condition);
	if (!variantHas<double>(conditionVal)) {

		throw LangError("Unexpected type Expected Number", LangError::AST);
	}
	bool condition = BoolConvert(std::get<double>(conditionVal));
	if (branch.ifBlock == nullptr) {
	}
	if (condition) {
		EvalNode(*branch.ifBlock);
		return NoneType();
	}
	else if (branch.elseBlock != nullptr) {
		EvalNode(*branch.elseBlock);
		return NoneType();
	}
	return NoneType();
}
Value Interpreter::EvalWhile(WhileNode loop) {
	bool condition = true;
	while (true) {
		Value conditionVal = EvalNode(*loop.condition);
		if (!variantHas<double>(conditionVal)) {
		}
		condition = BoolConvert(std::get<double>(conditionVal));
		if (!condition) {
			break;
		}
		try {
			EvalNode(*loop.loopBlock);
		}
		catch (BreakException) {
			break;
		}

	}

}
Value Interpreter::EvalNode(Node expr) {
	if (auto val = std::get_if<Value>(&expr)) {
		return *val;
	}
	if (auto block = std::get_if<Block>(&expr)) {
		return EvalBlock(block->body, Scope());
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
	if (auto ass = std::get_if<Assignment>(&expr)) {
		return EvalAssignment(*ass);
	}
	if (auto ret = std::get_if<Return>(&expr)) {
		throw ReturnException(EvalNode(*ret->object));
	}
	if (auto loop = std::get_if<WhileNode>(&expr)) {
		EvalWhile(*loop);
	}
	if (auto brk = std::get_if<Break>(&expr)) {
		throw BreakException();
	}

};

Interpreter::Interpreter(std::map<string, Value> base) {
	current = Scope(nullptr, base);
}
Interpreter::Interpreter(Scope base) {
	current = base;
}
Interpreter::Interpreter() {
	std::map<string, Value> defMap = {
		{"Test",2.0},
		{"PI",3.146210},
		{"strToNum",BuiltinFunc(&StringToNum,1)},
		{"print",BuiltinFunc(&PrintBuiltin,-1)},
		{"input",BuiltinFunc(&InputBuiltin,1)}
	};
	current = Scope(nullptr, defMap);
}