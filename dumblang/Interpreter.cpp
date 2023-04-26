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
		throw InvalidOperatorError();
	}
}
Value Interpreter::CallBuiltinFunc(BuiltinFunc called, std::vector<Value> argValues) {
	if (called.argSize != -1 && argValues.size() != called.argSize) {
		throw InvalidArgumentsError(called.argSize, (int)argValues.size());
	}
	return called.funcPointer(argValues);
}
Value Interpreter::EvalAssignment(Assignment ass, std::vector<Scope>& scopes) {
	Scope& current_scope = scopes.back();
	Value assignedValue;
	try{
		assignedValue = EvalNode(*ass.value,scopes);
	}
	catch(ReturnException retval){
		assignedValue = retval.what();
	}
	current_scope.insert({ ass.varName, assignedValue });
	return assignedValue;
}

Value Interpreter::EvalVariable(Variable var, std::vector<Scope>& scopes) {
	auto currentScopes = std::views::reverse(scopes);
	auto scopeHoldsVar = std::ranges::find_if(currentScopes, [&](Scope const& scope) {
		return scope.contains(var.name);
		});
	if (scopeHoldsVar == currentScopes.end()) {
		throw UndefinedVariableError(var.name);
	}

	Value varValue = scopeHoldsVar->at(var.name);
	return varValue;
}

void Interpreter::EvalBlock(Block block) {
	std::stack<Node, std::vector<Node>> statements;
	std::vector<Scope> scopes;
	for (auto const& statement : std::views::reverse(block.body)) {
		statements.push(statement);
	}
	scopes.push_back(Scope{});

	while (!statements.empty()) {
		Node statement = statements.top();
		statements.pop();
		EvalNode(statement,scopes);
	}
};

Value Interpreter::EvalUnaryNode(UnaryNode::Type type, Value obj) {
	switch (type)
	{
	case UnaryNode::NEGATE:
		if (auto val = std::get_if<double>(&obj)) {
			return -(*val);
		}
		throw InvalidTypeError(obj, Value(0.0));
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
		throw InvalidTypeError(obj,Value(0.0));
	default:
		break;
	}
};
Value Interpreter::EvalBinaryNode(BinaryNode::Type type,Value leftValue, Value rightValue) {
	if (leftValue.index() != rightValue.index()) {
		
		if (type == BinaryNode::ISEQUAL) {
			return 0.0;
		}
		if (type == BinaryNode::ISDIFERENT) {
			return 1.0;
		}
		throw MixedTypesError({ Value(0.0),Value(0.0) }, { leftValue,rightValue });
	}
	if (variantHas<double>(leftValue)) {
		return numCalc(type, std::get<double>(leftValue), std::get<double>(rightValue));
	}
	if (variantHas<string>(leftValue)) {
		return strCalc(type, std::get<string>(leftValue), std::get<string>(rightValue));
	}
}

Value Interpreter::EvalCall(Call request, std::vector<Scope>& scopes) {
	Value getFunc = EvalNode(request.callee, scopes);
	std::vector<Value> argValues;
	for (size_t index = 0; index < request.args.size(); index++) {
		argValues.push_back(EvalNode(request.args[index], scopes));
	}
	if (variantHas<BuiltinFunc>(getFunc)) {
		return CallBuiltinFunc(std::get<BuiltinFunc>(getFunc), argValues);
	}
	if (!variantHas<Function>(getFunc)) {
		throw InvalidCallError(getFunc);
	}
	Function calledFunc = std::get<Function>(getFunc);
	if (request.args.size() != calledFunc.args.size()) {
		throw InvalidArgumentsError((int)calledFunc.args.size(), (int)request.args.size());
	}
	for (size_t index = 0; index < calledFunc.args.size(); index++) {
		string varName = calledFunc.args[index];
		Value assignVal = argValues[index];
		Scope& current_scope = scopes.back();
		current_scope.insert({ varName, assignVal });
	}
	try{
		EvalNode(calledFunc.block, scopes);
	}
	catch (ReturnException value) {
		return value.what();
	}
	return NoneType();
}
Value Interpreter::EvalBranch(BranchNode branch, std::vector<Scope>& scopes) {
	Value conditionVal = EvalNode(*branch.condition, scopes);
	if (!variantHas<double>(conditionVal)) {
		throw InvalidTypeError(conditionVal,Value(0.0));
	}
	bool condition = BoolConvert(std::get<double>(conditionVal));
	if (branch.ifBlock == nullptr) {
		throw InvalidBranchFormatError();
	}
	if (condition) {
		EvalNode(*branch.ifBlock, scopes);
		return NoneType();
	}
	else if (branch.elseBlock != nullptr) {
		EvalNode(*branch.elseBlock, scopes);
		return NoneType();
	}
	return NoneType();
}
void Interpreter::EvalWhile(WhileNode loop, std::vector<Scope>& scopes) {
	bool condition = true;
	while (true) {
		Value conditionVal = EvalNode(*loop.condition, scopes);
		if (!variantHas<double>(conditionVal)) {
			throw InvalidTypeError(conditionVal, Value(0.0));
		}
		condition = BoolConvert(std::get<double>(conditionVal));
		if (!condition) {
			break;
		}
		try {
			EvalNode(*loop.loopBlock, scopes);
		}
		catch (BreakException) {
			break;
		}

	}

}
Value Interpreter::EvalNode(Node expr, std::vector<Scope>& scopes) {
	if (auto val = std::get_if<Value>(&expr)) {
		return *val;
	}
	if (auto block = std::get_if<Block>(&expr)) {
		EvalBlock(*block);
	}
	if (auto var = std::get_if<Variable>(&expr)) {
		return EvalVariable(*var,scopes);
	}
	if (auto request = std::get_if<Call>(&expr)) {
		return EvalCall(*request, scopes);
	}
	if (auto unaryOp = std::get_if<UnaryNode>(&expr)) {
		Value obj = EvalNode(*unaryOp->object,scopes);
		return EvalUnaryNode(unaryOp->type,obj);
	}
	if (auto binOp = std::get_if<BinaryNode>(&expr)) {
		Value leftValue = EvalNode(*binOp->left,scopes);
		Value rightValue = EvalNode(*binOp->right,scopes);
		return EvalBinaryNode(binOp->type,leftValue,rightValue);
	}
	if (auto branch = std::get_if<BranchNode>(&expr)) {
		return EvalBranch(*branch, scopes);
	}
	if (auto ass = std::get_if<Assignment>(&expr)) {
		return EvalAssignment(*ass, scopes);
	}
	if (auto ret = std::get_if<Return>(&expr)) {
		throw ReturnException(EvalNode(*ret->object, scopes));
	}
	if (auto loop = std::get_if<WhileNode>(&expr)) {
		EvalWhile(*loop, scopes);
	}
	if (auto brk = std::get_if<Break>(&expr)) {
		throw BreakException();
	}
};
Interpreter::Interpreter(Block base_) {
	this->base = base_;
}
void Interpreter::execute() {
	EvalBlock(this->base);
}
