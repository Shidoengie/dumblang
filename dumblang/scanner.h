#pragma once
#ifndef SCANNER_H
#define SCANNER_H
#include <variant>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "Token.h"
#include "TokenType.h"
class Scanner
{

public:
	Scanner(std::string _source);
	std::vector<Token> scanTokens();
private:
	std::vector<Token> TokenStream;
	std::string source;
	char advance();
	int current;
	int line;

	bool isAtEnd();
	char peek();
	char peekNext();
	void addToken(TokenType type, std::variant<double, std::string> lexeme);
	void num(char LastChar);
	void str();
	void identifier(char LastChar);
	void getToken();

};
#endif