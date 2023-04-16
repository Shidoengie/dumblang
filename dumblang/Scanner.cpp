#include "Scanner.h"

#include <variant>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "Scanner.h"
#include "Token.h"
#include <stdexcept>
#include "ShlangError.h"
Scanner::Scanner(std::string _source)
{
	source = _source;
}

char8_t Scanner::advance() {
	return source[current++];
}
bool Scanner::isAtEnd() {
	return current >= source.length();
}
char8_t Scanner::peek() {
	return source[current];
}
char8_t Scanner::peekNext() {
	if (current + 1 >= source.length()) return '\0';
	return source[current + 1];
}
void Scanner::addToken(Token::Type type, std::variant<double, std::string> lexeme)
{
	Token token = Token(type, lexeme, line);
	TokenStream.push_back(token);
}
void Scanner::num(char8_t LastChar) {
	std::string numStr;
	do {
		numStr += LastChar;
		LastChar = advance();
	} while (isdigit(LastChar) || LastChar == '.');
	current--;
	double num = stod(numStr);
	addToken(Token::NUM, num);
}

void Scanner::str() {
	
	char8_t LastChar = advance();
	std::string endStr;
	do {
		endStr += LastChar;
		if (isAtEnd()) {
			throw LangError("Undetermined string", LangError::LEXER);
		}
		LastChar = advance();
	} while (LastChar != '"');
	addToken(Token::STR, endStr);
}
void Scanner::identifier(char8_t LastChar) {
	std::string endStr;
	do {
		
		endStr += LastChar;
		LastChar = advance();
	} while (isalnum(LastChar));
	current--;
	if (GetKeywordMap().contains(endStr)) {
		addToken(mapKeyword(endStr), endStr);
	}
	else {
		addToken(Token::IDENTIFIER, endStr);
	}
}
void Scanner::getToken() {
	char8_t LastChar = advance();

	// Skip any whitespace.
	switch (LastChar)
	{
	case '!':addToken(Token::BANG, "!"); break;
	case '.':addToken(Token::DOT, "."); break;
	case '+':addToken(Token::PLUS, "+"); break;
	case '-':addToken(Token::MINUS, "-"); break;
	case '*':addToken(Token::STAR, "*"); break;
	case '/':addToken(Token::SLASH, "/"); break;
	case '=':addToken(Token::EQUAL, "="); break;
	case '{':addToken(Token::LBRACE, "{"); break;
	case '}':addToken(Token::RBRACE, "}"); break;
	case '(':addToken(Token::LPAREN, "("); break;
	case ')':addToken(Token::RPAREN, ")"); break;
	case '[':addToken(Token::LBRACK, "["); break;
	case ']':addToken(Token::RBRACK, "]"); break;
	case '<':addToken(Token::LESSER, "<"); break;
	case '>':addToken(Token::GREATER, ">"); break;
	case ',':addToken(Token::COMMA, ","); break;
	case ':':addToken(Token::COLON, ":"); break;
	case ';':
		addToken(Token::EOL, ";");
		line++;
		break;
	case ' ':break;
	case '\t':break;
	case '\r':break;
	case '"': str(); break;
	default:
		if (isdigit(LastChar)) {
			num(LastChar);
		}
		else if (isalnum(LastChar)) {
			identifier(LastChar);
		}
		else {
			throw LangError("Unexpected char!", LangError::LEXER);
		}
		break;
	}
}
std::vector<Token> Scanner::scanTokens() {
	while (!isAtEnd())
	{
		getToken();
	}
	addToken(Token::EOFL, "EOF");
	return TokenStream;
};