#pragma once
#include <variant>
#include <iostream>
#include <vector>
#include <map>
#include <string>

class Token
{
public:
    enum Type
    {
        // lieterals
        STR,
        NUM,
        IDENTIFIER,
        EOL,
        PLUS,
        MINUS,
        STAR,
        SLASH,
        LPAREN,
        RPAREN,
        LBRACE,
        RBRACE,
        LBRACK,
        RBRACK,
        EQUAL,
        DOT,
        LESSER,
        GREATER,
        GREATER_EQUAL,
        LESSER_EQUAL,
        COMMA,
        COLON,
        BANG,
        MODULO,
        ISEQUAL,
        ISDIFERENT,
        // keywords
        PRINT,
        INPUT,
        IF,
        ELSE,
        ELIF,
        FUNC,
        RETURN,
        LOOP,
        BREAK,
        EOFL,
        FALSE,
        TRUE,
        WHILE
    };
    Token::Type type;
    std::variant<double, std::string> lexeme;
    int line;
    
    std::string toString();
    Token(Token::Type _type, std::variant<double, std::string> _lexeme, int _line) {
        type = _type;
        lexeme = _lexeme;
        line = _line;
    }
private:
    std::string stringify(std::variant<double, std::string> const& value);
};

std::map <std::string, Token::Type> GetKeywordMap();
std::map <Token::Type, std::string> GetTokenMap();
Token::Type mapKeyword(std::string keyword);
std::string mapToken(Token::Type tk);
