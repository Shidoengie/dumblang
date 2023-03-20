#pragma once
#ifndef TOKENTYPE_H
#define TOKENTYPE_H
#include <variant>
#include <iostream>
#include <vector>
#include <map>
#include <string>

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
TokenType mapKeyword(std::string keyword);
std::map <std::string, TokenType> GetKeywordMap();
std::string mapToken(TokenType tk);
std::map <TokenType, std::string> GetTokenMap();
#endif