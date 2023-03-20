#include <variant>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "Scanner.h"
#include "Token.h"
#include "TokenType.h"



Scanner::Scanner(std::string _source)
{
	source = _source;
}


bool Scanner::isAtEnd() {
	return current >= source.length();
}
char Scanner::advance() {
	return source[current++];
}
char Scanner::peek() {
	return source[current];
}
char Scanner::peekNext() {
	if (current + 1 >= source.length()) return '\0';
	return source[current + 1];
}
void Scanner::addToken(TokenType type, std::variant<double, std::string> lexeme)
{
	Token token = Token(type, lexeme, line);
	TokenStream.push_back(token);
}
void Scanner::num(char LastChar) {
	std::string numStr;
	do{
		numStr += LastChar;
		LastChar = advance();
	} while (isdigit(LastChar) || LastChar == '.');
	current--;
	double num = stod(numStr);
	addToken(TokenType::NUM,num);
}

void Scanner::str() {
	if (isAtEnd()){ 
		return;
	}
	char LastChar = advance();
	std::string endStr;
	do {
		endStr += LastChar;
		LastChar = advance();
	} while (LastChar != '"');
	addToken(TokenType::STR,endStr);
}
void Scanner::identifier(char LastChar) {
	std::string endStr;
	do {
		endStr += LastChar;
		LastChar = advance();
	} while (isalnum(LastChar));
	if (GetKeywordMap().contains(endStr)) {
		addToken(mapKeyword(endStr),endStr);
	}
	else {
		addToken(TokenType::IDENTIFIER, endStr);
	}
}
void Scanner::getToken() {

	char LastChar = advance();

	// Skip any whitespace.
	switch (LastChar)
	{
		case '+':addToken(TokenType::PLUS, "+"); break;
		case '-':addToken(TokenType::MINUS, "-"); break;
		case '*':addToken(TokenType::STAR, "*"); break;
		case '/':addToken(TokenType::SLASH, "/"); break;
		case '=':addToken(TokenType::EQUAL, "="); break;
		case '{':addToken(TokenType::LBRACE, "{"); break;
		case '}':addToken(TokenType::RBRACE, "}"); break;
		case '(':addToken(TokenType::LPAREN, "("); break;
		case ')':addToken(TokenType::RPAREN, ")"); break;
		case ';':
			addToken(TokenType::EOL, ";");
			line++;
			break;
		case ' ':break;
		case '\t':break;
		case '\r':break;
		case '"':str(); break;
	default:
		if (isdigit(LastChar))
		{
			num(LastChar);
		}
		else if (isalnum(LastChar))
		{
			identifier(LastChar);
		}
		else {
			std::cout << "Unxpected Character";
		}
		break;
	}
}
std::vector<Token> Scanner::scanTokens() {
	while (!isAtEnd())
	{
		getToken();
	}
	addToken(TokenType::EOFL, "EOF");
	return TokenStream;
};