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
std::map <std::string, TokenType> GetKeywordMap() {
    std::map <std::string, TokenType> KeywordMap = {
       {"print",PRINT},
       {"input",INPUT}
    };
    return KeywordMap;
}
std::string mapToken(TokenType tk) {
    std::map < TokenType, std::string> TokenMap = GetTokenMap();
    return TokenMap[tk];
}
std::map <TokenType, std::string> GetTokenMap() {
    std::map < TokenType, std::string> TokenMap = {
       {PRINT,"print"},
       {INPUT,"INPUT"},
       {STR,"STR"},
       {NUM,"NUM"},
       {IDENTIFIER,"IDENTIFIER"},
       {EOL,"EOL"},
       {PLUS,"PLUS"},
       {MINUS,"MINUS"},
       {STAR,"STAR"},
       {SLASH,"SLASH"},
       {LPAREN,"LPAREN"},
       {RPAREN,"RPAREN"},
       {LBRACE,"LABRACE"},
       {RBRACE,"RBRACE"},
       {EQUAL,"EQUAL"},
       {EOFL,"EOFL"}
    };
    return TokenMap;
}