#include "Parser.h"
#include "Defaults.h"
static std::string source;
static std::vector<Token> stream;
int current = 0;

using Kind = Token::Type;

std::string text(Token token) {
	auto res = token.lexeme.splice(source);
	if (res) {
		return *res;
	}
	throw UnspecifiedError("idk");
}
Kind Advance() {
	current++;
	Kind token = stream[current].type;
	return token;
}
Kind Peek() {
	return stream[current].type;
}
bool CurrentHas(Kind expected) {
	return Peek() == expected;
}
Node ParseTokens() {
	auto left = Peek();
	switch (left) {

	case Token::NUM:

	default:
		break;
	}
}
void ParseNumRange(Token literal) {
	literal.lexeme.splice(source);
}