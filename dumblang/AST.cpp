#include "AST.h"
#include "Token.h"
#include <iostream>
#include <vector>

//https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl02.html


class Expression{
public:
	virtual ~Expression() = default;
};
typedef std::unique_ptr<Expression> ExprPtr;
class NumberExpr : public Expression{
	Token* tokenObj;
public:
	NumberExpr(Token* newToken) : tokenObj(newToken) {}
};
class VariableExpr : public Expression {
	std::string varName;

public:
	VariableExpr(const std::string& newName) : varName(newName) {}
};

/// BinaryExprAST - Expression class for a binary operator.
class BinaryExpr : public Expression {
	Token::Type operatorType;
	ExprPtr Left, Right;
	// v init list v
	BinaryExpr(Token::Type opType, ExprPtr leftObj, ExprPtr rightObj)
		: operatorType(opType), Left(std::move(leftObj)), Right(std::move(rightObj)) {}
	// ^ init list ^
};
class AdditionExpr : public BinaryExpr {

};
class MultExpr : public BinaryExpr {

};
class DivExpr : public BinaryExpr {

};
class SubExpr : public BinaryExpr {

};

class UnaryExpr : public Expression {
	Token::Type operatorType;
	ExprPtr Object;
	UnaryExpr(Token::Type opType, ExprPtr assignedObj)
		: operatorType(opType), Object(std::move(assignedObj)){}
};

class CallExpr : public Expression {
	std::string calleeName;
	std::vector<ExprPtr> callArgs;

public:
	CallExpr(const std::string& Callee, std::vector<ExprPtr> Args)
		: calleeName(Callee), callArgs(std::move(Args)) {}
};
class PrototypeExpr {
	std::string protoName;
	std::vector<std::string> protoArgs;

public:
	PrototypeExpr(const std::string& Name, std::vector<std::string> Args)
		: protoName(Name), protoArgs(std::move(Args)) {}

	const std::string& getName() const { return protoName; }
};

class FunctionExpr {
	typedef std::unique_ptr<PrototypeExpr> ProtoPtr;
	ProtoPtr funcProto;
	ExprPtr funcBody;

public:
	FunctionExpr(ProtoPtr Proto, ExprPtr Body)
		: funcProto(std::move(Proto)), funcBody(std::move(Body)) {}
};