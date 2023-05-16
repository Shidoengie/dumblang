#pragma once
#include <variant>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "range.h"

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
        WHILE,
        VAR
    };
    Token::Type type;
    Range lexeme;
    std::string toString();
    Token(Token::Type _type, Range _lexeme) : lexeme(_lexeme) {
        type = _type;
        lexeme = _lexeme;
    }
};

std::map <std::string, Token::Type> GetKeywordMap();
std::map <Token::Type, std::string> GetTokenMap();
Token::Type mapKeyword(std::string keyword);
std::string mapToken(Token::Type tk);
