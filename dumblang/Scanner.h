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
	size_t size;
	std::vector<Token> TokenStream;
	
	char8_t advance();
	bool isAtEnd();
	char8_t peek();
	char8_t peekNext();
	bool match(char8_t expected);
	void addToken(Token::Type type, Range lexeme);
	void num(char8_t LastChar);
	void str();
	void identifier(char8_t LastChar);
	void getToken();
};

