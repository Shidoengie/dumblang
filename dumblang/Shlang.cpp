#include <variant>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "Token.h"
std::string source;
int current = 0;
int line = 0;



static std::vector<Token> TokenStream = {};

void addToken(Token::Type type, std::variant<double, std::string> lexeme)
{
    Token token = Token(type, lexeme, line);
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
    std::string numStr;
    do {
        numStr += LastChar;
        LastChar = advance();
    } while (isdigit(LastChar) || LastChar == '.');
    current--;
    double num = std::stod(numStr);
    addToken(Token::NUM, num);
}

void str() {
    if (isAtEnd()) {
        return;
    }
    char LastChar = advance();
    std::string endStr;
    do {
        endStr += LastChar;
        LastChar = advance();
    } while (LastChar != '"');
    addToken(Token::STR, endStr);
}
void identifier(char LastChar) {
    std::string endStr;
    do {
        endStr += LastChar;
        LastChar = advance();
    } while (isalnum(LastChar));
    
    if (mapKeyword(endStr)) {
        if (mapKeyword(endStr) == Token::TRUE) {
            addToken(Token::TRUE, 1.0);
            return;
        }
        if (mapKeyword(endStr) == Token::FALSE) {
            addToken(Token::FALSE, 0.0);
            return;
        }
        addToken(mapKeyword(endStr), endStr);
    }
    else {
        addToken(Token::IDENTIFIER, endStr);
    }
}
static void getToken() {

    char LastChar = advance();

    // Skip any whitespace.
    switch (LastChar)
    {
    case '+':addToken(Token::PLUS, "+"); break;
    case '-':addToken(Token::MINUS, "-"); break;
    case '*':addToken(Token::STAR, "*"); break;
    case '/':addToken(Token::SLASH, "/"); break;
    case '=':addToken(Token::EQUAL, "="); break;
    case '{':addToken(Token::LBRACE, "{"); break;
    case '}':addToken(Token::RBRACE, "}"); break;
    case '(':addToken(Token::LPAREN, "("); break;
    case ')':addToken(Token::RPAREN, ")"); break;
    case ';':
        addToken(Token::EOL, ";");
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

            std::cout << int(LastChar);
            std::cout << "UNEXPECTED CHAR!\n";
        }
        break;
    }
}
int main(int argc, char* argv[]) {

    while (true) {
        current = 0;
        line = 0;
        TokenStream.clear();
        std::cout << "> ";
        getline(std::cin, source);
        getToken();
        while (!isAtEnd())
        {
            getToken();
        }
        for (size_t i = 0; i < TokenStream.size(); i++)
        {
            std::cout << TokenStream[i].toString() << "\n";
        }
    }
    return 0;

}