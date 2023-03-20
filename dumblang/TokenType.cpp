#include <variant>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "TokenType.h"
TokenType mapKeyword(std::string keyword) {
    std::map <std::string, TokenType> KeywordMap = GetKeywordMap();
    return KeywordMap[keyword];
}
std::map<std::string, TokenType> GetKeywordMap() {
    std::map <std::string, TokenType> KeywordMap = {
       {"print",TokenType::PRINT},
       {"input",TokenType::INPUT}
    };
    return KeywordMap;
}
std::string mapToken(TokenType tk) {
    std::map<TokenType, std::string> TokenMap = GetTokenMap();
    return TokenMap[tk];
}
std::map<TokenType, std::string> GetTokenMap() {
    std::map<TokenType, std::string> TokenMap = {
       {TokenType::PRINT,"print"},
       {TokenType::INPUT,"INPUT"},
       {TokenType::STR,"STR"},
       {TokenType::NUM,"NUM"},
       {TokenType::IDENTIFIER,"IDENTIFIER"},
       {TokenType::EOL,"EOL"},
       {TokenType::PLUS,"PLUS"},
       {TokenType::MINUS,"MINUS"},
       {TokenType::STAR,"STAR"},
       {TokenType::SLASH,"SLASH"},
       {TokenType::LPAREN,"LPAREN"},
       {TokenType::RPAREN,"RPAREN"},
       {TokenType::LBRACE,"LABRACE"},
       {TokenType::RBRACE,"RBRACE"},
       {TokenType::EQUAL,"EQUAL"},
       {TokenType::EOFL,"EOFL"}
    };
    return TokenMap;
}