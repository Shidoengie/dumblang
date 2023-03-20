#pragma once

#include <variant>
#include <iostream>
#include <vector>
#include <map>
#include <string>

std::string enumMap[16] = { "STR", "NUM", "IDENTIFIER", "EOLN", "PLUS", "MINUS", "STAR", "SLASH", "LPAREN", "RPAREN", "EQUAL","PRINT","INPUT","EOF" };
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
    INPUT,
    EOFL
};
std::map <std::string, TokenType> KeywordMap = {
    {"print",PRINT},
    {"input",INPUT}
};


