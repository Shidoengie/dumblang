#pragma once
#ifndef TOKEN_H
#define TOKEN_H
#include "TokenType.h"
#include <variant>
#include <iostream>
#include <vector>
#include <map>
#include <string>
std::string stringify(std::variant<double, std::string> const& value);
struct Token
{
	enum TokenType type;
	std::variant<double, std::string> lexeme;
	int line;
	Token(TokenType _type, std::variant<double, std::string> _lexeme, int _line) {
		type = _type;
		lexeme = _lexeme;
		line = _line;
	}
	std::string toString() {
		return "type:" + mapToken(type) + " |lexeme: " + stringify(lexeme) + " |line:" + std::to_string(line);
	}
};
#endif