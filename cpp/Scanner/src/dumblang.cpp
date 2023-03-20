#include <variant>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

string source;
int current = 0;
int line = 0;
static double numValue;
static string IdentStr;

enum TokenType
{
	// lieterals
	STR,
	NUM,
	IDENTIFIER,
	EOL,
	// operators
	PLUS,
	MINUS,
	STAR,
	SLASH,
	LPAREN,
	RPAREN,
	LBRACE,
	RBRACE,
	EQUAL,
	// keywords
	PRINT,
	INPUT
};
string enumMap[16] = { "STR", "NUM", "IDENTIFIER", "EOLN", "PLUS", "MINUS", "STAR", "SLASH", "LPAREN", "RPAREN", "EQUAL","PRINT","INPUT"};
string stringify(variant<double,string> const& value) {
	if (double const* pval = get_if<double>(&value))
		return to_string(*pval);
	return get<string>(value);
}
struct Token
{
	enum TokenType type;
	variant<double, string> lexeme;
	int line;
	Token(TokenType _type, variant<double, string> _lexeme,int _line){
		type = _type;
		lexeme = _lexeme;
		line = _line;
	}
	string toString(){
		return "type:" + enumMap[type] + " |lexeme: " + stringify(lexeme)+ " |line:" + to_string(line);
	}
};

static vector<Token> TokenStream = {};

void addToken(TokenType type, variant<double, string> lexeme)
{
	Token token = Token(type, lexeme,line);
	TokenStream.push_back(token);
}
bool isAtEnd() {
	return current >= source.length();
}
char advance() {
	return source[current++];
}
char peek() {
	return source[current];
}
char peekNext() {
	if (current + 1 >= source.length()) return '\0';
	return source[current + 1];
}
void num(char LastChar) {
	string numStr;
	do{
		numStr += LastChar;
		LastChar = advance();
	} while (isdigit(LastChar) || LastChar == '.');
	current--;
	double num = stod(numStr);
	addToken(NUM,num);
}

void str() {
	if (isAtEnd()){ 
		return;
	}
	char LastChar = advance();
	string endStr;
	do {
		endStr += LastChar;
		LastChar = advance();
	} while (LastChar != '"');
	addToken(STR,endStr);
}
void identifier() {

}
static void getToken() {

	char LastChar = advance();

	// Skip any whitespace.
	switch (LastChar)
	{
		case '+':addToken(PLUS, "+"); break;
		case '-':addToken(MINUS, "-"); break;
		case '*':addToken(STAR, "*"); break;
		case '/':addToken(SLASH, "/"); break;
		case '=':addToken(EQUAL, "="); break;
		case '{':addToken(LBRACE, "{"); break;
		case '}':addToken(RBRACE, "}"); break;
		case '(':addToken(LPAREN, "("); break;
		case ')':addToken(RPAREN, ")"); break;
		case ';':
			addToken(EOL, ";");
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
			identifier();
		}
		break;
	}
}
int main(int argc, char* argv[]) {

	while (true) {
		TokenStream.clear();
		cout << "> ";
		getline(cin, source);
		getToken();
		while (!isAtEnd())
		{
			getToken();
		}
		for (size_t i = 0; i < TokenStream.size(); i++)
		{
			cout << TokenStream[i].toString() << "\n";
		}
	}
	return 0;
	
}