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
	
	char8_t last = source[current];
	current++;
	return last;
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
bool Scanner::match(char8_t expected) {
	if (isAtEnd() || source[current] != expected) {
		return false;
	}
	current++;
	return true;
}
void Scanner::addToken(Token::Type type, Range lexeme)
{
	Token token = Token(type, lexeme);
	TokenStream.push_back(token);
}
void Scanner::num(char8_t LastChar) {
	size_t start = current;
	do {
		LastChar = advance();
	} while (isdigit(LastChar) || LastChar == '.');
	size_t stop = current;
	current--;
	addToken(Token::NUM, Range(size,start,stop));
}

void Scanner::str() {
	
	char8_t LastChar = advance();
	size_t start = current;
	do {
		
		if (isAtEnd()) {
			throw UndeterminedStringError();
		}
		LastChar = advance();
	} while (LastChar != '"');
	
	size_t stop = current - 1;
	addToken(Token::STR, Range(size, start, stop));
}
void Scanner::identifier(char8_t LastChar) {
	std::string endStr;
	size_t start = current;
	do {
		
		endStr += LastChar;
		LastChar = advance();
	} while (isalnum(LastChar));
	auto identSlice = Range(size, start, current);
	current--;
	
	auto splice = identSlice.splice(source);
	
	if (GetKeywordMap().contains(*splice)) {
		addToken(mapKeyword(endStr), identSlice);
	}
	else {
		addToken(Token::IDENTIFIER, identSlice);
	}
}
void Scanner::getToken() {
	int start = current;
	char8_t LastChar = advance();
	auto single = Range(source.length());
	// Skip any whitespace.
	switch (LastChar)
	{
	
	case '.':addToken(Token::DOT, single); break;
	
	case ',':addToken(Token::COMMA, single); break;
	case '{':addToken(Token::LBRACE, single); break;
	case '}':addToken(Token::RBRACE, single); break;
	case '(':addToken(Token::LPAREN, single); break;
	case ')':addToken(Token::RPAREN, single); break;
	case '[':addToken(Token::LBRACK, single); break;
	case ']':addToken(Token::RBRACK, single); break;
	case '<':
		match('=') ? addToken(Token::LESSER_EQUAL, Range(size, start, current)) : addToken(Token::LESSER, single);
		break;
	case '>':
		match('=') ? addToken(Token::GREATER_EQUAL, Range(size, start, current)) : addToken(Token::GREATER, single);
		break;
	case '+':addToken(Token::PLUS, single); break;
	case '-':addToken(Token::MINUS, single); break;
	case '*':addToken(Token::STAR, single); break;
	case '/':addToken(Token::SLASH, single); break;
	case ':':addToken(Token::COLON, single); break;
	case '%':addToken(Token::MODULO, single); break;
	case '!':
		match('=') ? addToken(Token::ISDIFERENT, Range(size, start, current)) : addToken(Token::BANG, single);
		break;
	case '=':
		match('=') ? addToken(Token::ISEQUAL, Range(size, start, current)) : addToken(Token::EQUAL, single);
		break;
	case ';':
		addToken(Token::EOL, single);
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
			throw UnexpectedCharError(LastChar);
		}
		break;
	}
}
std::vector<Token> Scanner::scanTokens() {
	size = source.length();
	while (!isAtEnd())
	{
		getToken();
	}
	addToken(Token::EOFL, Range(size,size,size));
	return TokenStream;
};