#include "Token.h"
#include <variant>
#include <iostream>
#include <vector>
#include <map>
#include <string>


std::string Token::stringify(std::variant<double, std::string> const& value) {
    if (double const* pval = get_if<double>(&value))
        return std::to_string(*pval);
    return get<std::string>(value);
}
std::string Token::toString() {
	return "type:" + mapToken(type) + " |lexeme: " + stringify(lexeme) + " |line:" + std::to_string(line);
}
std::map<std::string, Token::Type> GetKeywordMap() {
    std::map<std::string, Token::Type> KeywordMap = {
       {"print",Token::PRINT},
       {"input",Token::INPUT}
    };
    return KeywordMap;
}

std::map<Token::Type, std::string> GetTokenMap() {
    std::map<Token::Type, std::string> TokenMap = {
       {Token::PRINT,"PRINT"},
       {Token::INPUT,"INPUT"},
       {Token::STR,"STR"},
       {Token::NUM,"NUM"},
       {Token::IDENTIFIER,"IDENTIFIER"},
       {Token::EOL,"EOL"},
       {Token::PLUS,"PLUS"},
       {Token::MINUS,"MINUS"},
       {Token::STAR,"STAR"},
       {Token::SLASH,"SLASH"},
       {Token::LPAREN,"LPAREN"},
       {Token::RPAREN,"RPAREN"},
       {Token::LBRACE,"LABRACE"},
       {Token::RBRACE,"RBRACE"},
       {Token::EQUAL,"EQUAL"},
       {Token::EOFL,"EOFL"}
    };
    return TokenMap;
}
Token::Type mapKeyword(std::string keyword) {
    std::map <std::string, Token::Type> KeywordMap = GetKeywordMap();
    return KeywordMap[keyword];
}
std::string mapToken(Token::Type tk) {
    std::map < Token::Type, std::string> TokenMap = GetTokenMap();
    return TokenMap[tk];
}