#pragma once
#include <variant>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "Token.h"
#include "TokenType.cpp"
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