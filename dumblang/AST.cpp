#include "AST.h"
#include "Token.h"
#include <iostream>
#include <vector>

//https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl02.html

class Expression{
public:
	virtual ~Expression() = default;
};
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
	Token::Type Operator;
	std::unique_ptr<Expression> Left, Right;
	// v init list v
	BinaryExpr(Token::Type opType, std::unique_ptr<Expression> leftObj, std::unique_ptr<Expression> rightObj)
		: Operator(opType), Left(std::move(leftObj)), Right(std::move(rightObj)) {}
	// ^ init list ^
};

class UnaryExpr : public Expression {
	Token::Type Operator;
	std::unique_ptr<Expression> Object;

};