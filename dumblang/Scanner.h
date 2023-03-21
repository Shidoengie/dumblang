#pragma once
#include <variant>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "Token.h"
class Scanner
{
public:
	Scanner(std::string _source);
	std::vector<Token> scanTokens();
private:
	std::string source;
	int current;
	int line;
	std::vector<Token> TokenStream;
	char advance();
	bool isAtEnd();
	char peek();
	char peekNext();
	void addToken(Token::Type type, std::variant<double, std::string> lexeme);
	void num(char LastChar);
	void str();
	void identifier(char LastChar);
	void getToken();
};

