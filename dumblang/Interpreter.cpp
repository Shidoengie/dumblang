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
		break;
	}
}
Value Interpreter::CallBuiltinFunc(BuiltinFunc called, std::vector<Value> argValues) {
	if (called.argSize != -1 && argValues.size() != called.argSize) {
		throw InvalidArgumentsError(argValues.size(), called.argSize);
	}
	return called.funcPointer(argValues);
}
void Interpreter::EvalAssignment(Assignment ass) {
	Value assVal; 
	try {
		assVal = EvalNode(*ass.value);
	}
	catch(ReturnException& retExcept) {
		Value retValue = retExcept.get();
		assVal = retValue;
	}
	if (variantHas<NoneType>(assVal)) {
		throw UnspecifiedError("Cannot assign None to a variable");
	}
	current.define(ass.varName, assVal);
	
}

void Interpreter::EvalBlock(Block block) {
	
	Scope currentCopy = current;
	auto newScope = Scope(&currentCopy,{ });
	current = newScope;
	if (block.body.size() == 0) {
		return;
	}
	std::vector<Node> body = block.body;
	for (size_t i = 0; i < body.size(); i++)
	{
		Node statement = body[i];
		EvalNode(statement);

	}
	current = *newScope.parentScope;
};

Value Interpreter::EvalVariable(Variable var) {

	Value varValue = current.getVar(var.name);
	if (variantHas<NoneType>(varValue)) {
		throw UndefinedVariableError(var.name);
	}
	
}

Value Interpreter::EvalUnaryNode(UnaryNode unaryOp) {
	Value obj = EvalNode(*unaryOp.object);
	switch (unaryOp.type)
	{
	case UnaryNode::NEGATE:
		if (auto val = std::get_if<double>(&obj)) {
			return -(*val);
		}
		throw InvalidTypeError(obj,Value(0.0));
	case UnaryNode::POSITIVE:
		if (auto val = std::get_if<double>(&obj)) {
			return +(*val);
		}
		throw InvalidTypeError(obj, Value(0.0));
		break;
	case UnaryNode::NOT:
		if (auto val = std::get_if<double>(&obj)) {
			bool out = BoolConvert(*val);
			return (double)(!out);
		}
		throw InvalidTypeError(obj, Value(0.0));
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
		throw MixedTypesError({leftValue,rightValue},{Value(0.0),Value(0.0)});
	}
	if (variantHas<double>(leftValue)) {
		return numCalc(binOp.type, std::get<double>(leftValue), std::get<double>(rightValue));
	}
	if (variantHas<string>(leftValue)) {
		return strCalc(binOp.type, std::get<string>(leftValue), std::get<string>(rightValue));
	}
}

Value Interpreter::EvalCall(Call request) {
	Value getFunc = current.getVar(request.callee.name);
	std::vector<Value> argValues;
	for (size_t index = 0; index < request.args.size(); index++) {
		Node argument = request.args[index];
		if (auto var = std::get_if<Variable>(&argument)) {
			argValues.push_back(current.getVar(var->name));
			continue;
		}
		argValues.push_back(EvalNode(request.args[index]));
	}
	if (variantHas<BuiltinFunc>(getFunc)) {
		return CallBuiltinFunc(std::get<BuiltinFunc>(getFunc), argValues);
	}
	if (!variantHas<Function>(getFunc)) {
		throw InvalidCallError(getFunc);
	}
	Function calledFunc = std::get<Function>(getFunc);
	if (request.args.size() != calledFunc.args.size()) {
		throw InvalidArgumentsError(request.args.size(), calledFunc.args.size());
	}
	for (size_t index = 0; index < calledFunc.args.size(); index++) {
		string var = calledFunc.args[index];
		Value assignVal = argValues[index];
		current.define(var, assignVal);
	}
	try {
		EvalNode(calledFunc.block);
	}
	catch (ReturnException value) {
		return value.get();
	}
	return NoneType();
}
Value Interpreter::EvalBranch(BranchNode branch) {
	Value conditionVal = EvalNode(*branch.condition);
	if (!variantHas<double>(conditionVal)) {

		throw InvalidTypeError(conditionVal, Value(0.0));
	}
	bool condition = BoolConvert(std::get<double>(conditionVal));
	if (branch.ifBlock == nullptr) {
		throw InvalidBranchFormatError();
	}
	else if (condition) {
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
	return NoneType();

}
Value Interpreter::EvalNode(Node expr) {
	if (auto val = std::get_if<Value>(&expr)) {
		return *val;
	}
	else if (auto block = std::get_if<Block>(&expr)) {
		
		EvalBlock(*block);
		return NoneType();
	}
	else if (auto var = std::get_if<Variable>(&expr)) {
		Value varVal = EvalVariable(*var);
		return varVal;
	}
	else if (auto request = std::get_if<Call>(&expr)) {
		return EvalCall(*request);
	}
	else if (auto unaryOp = std::get_if<UnaryNode>(&expr)) {
		return EvalUnaryNode(*unaryOp);
	}
	else if (auto binOp = std::get_if<BinaryNode>(&expr)) {
		return EvalBinaryNode(*binOp);
	}
	else if (auto branch = std::get_if<BranchNode>(&expr)) {
		return EvalBranch(*branch);
	}
	
	else if (auto ass = std::get_if<Assignment>(&expr)) {
		EvalAssignment(*ass);
		return NoneType();
	}
	else if (auto ret = std::get_if<Return>(&expr)) {
		Node returnVal = *ret->object;
		Value result = EvalNode(returnVal);
		throw ReturnException(result);
	}
	else if (auto loop = std::get_if<WhileNode>(&expr)) {
		EvalWhile(*loop);
		return NoneType();
	}
	else if (auto brk = std::get_if<Break>(&expr)) {
		throw BreakException();
	}

};

Interpreter::Interpreter(Block program, std::map<string, Value> base) {
	current = Scope(nullptr, base);
	program_ = program;
}
Interpreter::Interpreter(Block program, Scope base) {
	current = base;
	program_ = program;
}
Interpreter::Interpreter(Block program) {
	std::map<string, Value> defMap = {
		{"Test",2.0},
		{"PI",3.146210},
		{"strToNum",BuiltinFunc(&StringToNum,1)},
		{"print",BuiltinFunc(&PrintBuiltin,-1)},
		{"input",BuiltinFunc(&InputBuiltin,1)}
	};
	Scope* baseScope = new Scope();
	baseScope->parentScope = nullptr;
	baseScope->varMap = defMap;
	current = *baseScope;
	program_ = program;
}
void Interpreter::execute() {
	EvalBlock(program_);
}