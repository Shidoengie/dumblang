#include "Parser.h"
#include "Token.h"
#include "Interpreter.h"
static std::string source;
static std::vector<Token> stream;
int current = 0;

using Kind = Token::Type;

std::string text(Token token) {
	return *token.lexeme.splice(source);
}
Kind advance() {

	Kind last = stream[current].type;
	current++;
	return last;
}
Kind peek() {
	return stream[current].type;
}
bool current_has(Kind expected) {
	return peek() == expected;
}
void parse_tokens() {
	for (Token& token : stream)
	{
		switch (token.type){
		case Token::NUM:
			
		default:
			break;
		}
	}
}
void parse_num_range(Token literal) {
	literal.lexeme.splice(source);
}