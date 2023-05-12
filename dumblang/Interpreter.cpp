#include "Interpreter.h"
#include "Defaults.h"
static const Value Number = Value(0.0);
Value Interpreter::FilterReturn(Value result) {
	if (!variantHas<Control>(result)) {
		return NoneType();
	}
	auto flow = std::get<Control>(result);
	if (variantHas<Return>(flow)) {
		return flow;
	}
	return NoneType();
}
Value Interpreter::UnwrapReturnValue(Value result) {
	if (!variantHas<Control>(result)) {
		return result;
	}
	auto flow = std::get<Control>(result);
	if (auto retVal = std::get_if<Return>(&flow)) {
		return EvalNode(*retVal->object);
	}
	return result;
}


void Interpreter::EvalDeclaration(Declaration request) {
	Value assVal = UnwrapReturnValue(EvalNode(*request.value));
	current.define(request.varName, EvalNode(assVal));
}
Value Interpreter::EvalAssignment(Assignment ass) {
	Value assVal = UnwrapReturnValue(EvalNode(*ass.value));
	
	std::optional<Value> hasVar = current.assign(ass.varName, assVal);
	if (hasVar) {
		return assVal;
	}
	throw UndefinedVariableError(ass.varName);

	
}
Value Interpreter::EvalVariable(Variable var) {

	if (current.containsVar(var.name)) {
		Value varRes = current.varMap[var.name];
		return varRes;
	}
	std::optional<Value> varValue = current.getVar(var.name);

	if (varValue) {
		return *varValue;
	}
	throw UndefinedVariableError(var.name);
}

Value Interpreter::EvalBlock(Block block) {
	
	Scope currentCopy = current;
	auto newScope = Scope(&currentCopy,{ });
	current = newScope;
	if (block.body.size() == 0) {
		return NoneType();
	}
	NodeStream body = block.body;
	for (size_t i = 0; i < body.size(); i++)
	{

		Node statement = body[i];
		Value result = EvalNode(statement);
		if (auto control = std::get_if<Control>(&result)) {
			current = *newScope.parentScope;
			return *control;
		}
	}
	current = *newScope.parentScope;
	return NoneType();
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
		return fmod(leftValue, rightValue);
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
Value Interpreter::EvalUnaryNode(UnaryNode unaryOp) {
	Value obj = EvalNode(*unaryOp.object);
	switch (unaryOp.type)
	{
	case UnaryNode::NEGATE:
		if (auto val = std::get_if<double>(&obj)) {
			return -(*val);
		}
		throw InvalidTypeError(obj,Number);
	case UnaryNode::POSITIVE:
		if (auto val = std::get_if<double>(&obj)) {
			return +(*val);
		}
		throw InvalidTypeError(obj, Number);
		break;
	case UnaryNode::NOT:
		if (auto val = std::get_if<double>(&obj)) {
			bool out = BoolConvert(*val);
			return (double)(!out);
		}
		throw InvalidTypeError(obj, Number);
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
		throw MixedTypesError({leftValue,rightValue},{Number,Number});
	}
	if (variantHas<double>(leftValue)) {
		return numCalc(binOp.type, std::get<double>(leftValue), std::get<double>(rightValue));
	}
	if (variantHas<string>(leftValue)) {
		return strCalc(binOp.type, std::get<string>(leftValue), std::get<string>(rightValue));
	}
}
Value Interpreter::CallBuiltinFunc(BuiltinFunc called, ValueStream argValues) {
	if (called.argSize != -1 && argValues.size() != called.argSize) {
		throw InvalidArgSizeError(argValues.size(), called.argSize);
	}
	return called.funcPointer(argValues);
}
Value Interpreter::EvalCall(Call request) {
	Value getFunc = EvalNode(request.callee);

	ValueStream argValues;
	for (size_t index = 0; index < request.args.size(); index++) {
		Node argument = request.args[index];
		if (auto var = std::get_if<Variable>(&argument)) {
			argValues.push_back(EvalNode(*var));
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
		throw InvalidArgSizeError(request.args.size(), calledFunc.args.size());
	}
	NodeStream argAssignements;
	for (size_t index = 0; index < calledFunc.args.size(); index++) {
		string var = calledFunc.args[index];
		Node assignVal = argValues[index];
		argAssignements.push_back(Declaration(var, &assignVal));
	}
	argAssignements.append_range(calledFunc.block.body);
	calledFunc.block.body = argAssignements;
	Value result = EvalNode(calledFunc.block);
	if (!variantHas<Control>(result)) {
		return NoneType();
	}
	auto flow = std::get<Control>(result);
	if (auto ret = std::get_if<Return>(&flow)) {
		return EvalNode(*ret->object);
	}
}
Value Interpreter::EvalBranch(BranchNode branch) {
	Value conditionVal = EvalNode(*branch.condition);
	if (!variantHas<double>(conditionVal)) {

		throw InvalidTypeError(conditionVal, Number);
	}
	bool condition = BoolConvert(std::get<double>(conditionVal));
	if (branch.ifBlock == nullptr) {
		throw InvalidBranchFormatError();
	}
	else if (condition) {
		Value result = EvalNode(*branch.ifBlock);
		return result;
	}
	else if (branch.elseBlock != nullptr) {
		Value result = EvalNode(*branch.elseBlock);
		return result;
	}
	return NoneType();
}
Value Interpreter::EvalWhile(WhileNode loop) {
	bool condition = true;
	while (true) {
		Value conditionVal = EvalNode(*loop.condition);
		if (!variantHas<double>(conditionVal)) {
			throw InvalidTypeError(Number,conditionVal);
			break;
		}
		condition = BoolConvert(std::get<double>(conditionVal));
		if (!condition) {
			break;
		}
		Value result = EvalNode(*loop.loopBlock);
		if (!variantHas<Control>(result)) {
			continue;
		}
		auto flow = std::get<Control>(result);
		if (variantHas<Return>(flow)) {
			return flow;
		}
		if (variantHas<Break>(flow)) {			
			break;
		}

	}
	return NoneType();

}
Value Interpreter::EvalLoop(LoopNode loop) {
	while (true) {
		Value result = EvalNode(*loop.loopBlock);
		if (!variantHas<Control>(result)) {
			continue;
		}
		auto flow = std::get<Control>(result);
		if (variantHas<Return>(flow)) {
			return flow;
		}
		if (variantHas<Break>(flow)) {
			break;
		}
	}
	return NoneType();
}

Value Interpreter::EvalNode(Node expr) {
	if (auto val = std::get_if<Value>(&expr)) {
		if (!variantHas<Control>(*val)) {
			return *val;
		}
		auto flow = std::get<Control>(*val);
		if (auto ret = std::get_if<Return>(&flow)) {
			Value retValue = EvalNode(*ret->object);
			ret->object = new Node(retValue);
			return Control(*ret);
		}
		return flow;
		
	}
	else if (auto block = std::get_if<Block>(&expr)) {
		
		Value result = EvalBlock(*block);
		return result;
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
		Value result = EvalBranch(*branch);
		return result;
	}
	else if (auto declare = std::get_if<Declaration>(&expr)) {
		EvalDeclaration(*declare);
		return NoneType();
	}
	else if (auto ass = std::get_if<Assignment>(&expr)) {
		Value assVal = EvalAssignment(*ass);
		return assVal;
	}
	else if (auto loop = std::get_if<WhileNode>(&expr)) {
		Value result = EvalWhile(*loop);
		return FilterReturn(result);
	}
	else if (auto loop = std::get_if<LoopNode>(&expr)) {
		Value result = EvalLoop(*loop);
		return FilterReturn(result);
	}
	throw UnspecifiedError("IDK");
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
	Scope* baseScope = new Scope();
	baseScope->parentScope = nullptr;
	baseScope->varMap = getDefaultVarMap();
	current = *baseScope;
	program_ = program;
}
void Interpreter::execute() {
	EvalBlock(program_);
}