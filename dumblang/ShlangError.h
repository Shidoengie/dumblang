#pragma once
#include <iostream>
#include <exception>
class LangError : public std::exception {
private:
	std::string msg;
public:
	enum Type {
		AST,
		LEXER
	};
	Type type;
	int line;
	virtual const std::string what();
	LangError(std::string message, LangError::Type errorType);

};