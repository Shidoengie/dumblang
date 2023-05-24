#include "Parser.h"
#include "Defaults.h"
static std::string source;
static std::vector<Token> stream;
static std::vector<Node> body;
int index = 0;

using Kind = Token::Type;
using TokenResult = std::optional<Token>;

std::string text(Token token) {
	auto res = token.lexeme.splice(source);
	if (res) {
		return *res;
	}
	throw UnspecifiedError("idk");
}
bool AtEnd() {
	if (index + 1 > stream.size()) {
		return true;
	}
	return false;
}
Token Peek() {
	return stream[index];
}
TokenResult PeekNext() {
	if (!AtEnd()) {
		return stream[index + 1];
	}
	return {};
}
Token Advance() {
	
	if (!AtEnd()) {
		index++;
	}
	Token next = Peek();
	return next;
}

bool matches(Kind expected) {
	auto next = PeekNext();
	if (!next || (*next).type != expected) {
		return false;
	}
	index++;
	return true;
}
Node ParseVar() {
	
	if (!matches(Token::IDENTIFIER)) {
		throw UnspecifiedError("Invalid syntax");
	}
	std::string name = text(Peek());
	if (matches(Token::EOL)) {
		NodePtr val = std::make_unique<Node>(0.0);
		Node decl = Declaration(name,std::move(val));
		return decl;
	}
	if (!matches(Token::EQUAL)) {
		throw UnspecifiedError("Invalid syntax");
	}
}
Node ParseTokens() {
	auto start = Peek();
	switch (start.type)
	{
	case Token::VAR: {
		
		
		break;
	}	
	case Token::FUNC: {
		
		break;
	}
	default:
		break;
	}
	
	return NoneType();
}

Node ParseValue(Token literal) {
	auto tokenVal = text(literal);
	switch (literal.type) {
	case Token::STR: 
		return Value(tokenVal);
	case Token::NUM: {
		double value = std::stod(tokenVal);
		Node number = Value(value);
		return number;
	}
	case Token::IDENTIFIER: {
		
	}
	default:
		break;
	}
	return NoneType();
}