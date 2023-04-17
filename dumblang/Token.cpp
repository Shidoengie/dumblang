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
        {"true",Token::TRUE},
        {"false",Token::FALSE},
        {"print",Token::PRINT},
        {"input",Token::INPUT},
        {"func",Token::FUNC},
        {"return",Token::RETURN},
        {"if",Token::IF},
        {"elif",Token::ELIF},
        {"else",Token::ELSE},
        {"loop",Token::LOOP},
        {"break",Token::BREAK}
    };
    return KeywordMap;
}

std::map<Token::Type, std::string> GetTokenMap() {
    std::map<Token::Type, std::string> TokenMap = {
        {Token::TRUE,"TRUE"},
        {Token::FALSE,"FALSE"},
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
        {Token::LBRACK,"LABRACK"},
        {Token::RBRACK,"RBRACK"},
        {Token::LESSER,"LESSER"},
        {Token::GREATER,"GREATER"},
        {Token::LESSER_EQUAL,"LESSER_EQUAL"},
        {Token::GREATER_EQUAL,"GREATER_EQUAL"},
        {Token::EQUAL,"EQUAL"},
        {Token::EOFL,"EOFL"},
        {Token::DOT,"DOT"},
        {Token::COMMA,"COMMA"},
        {Token::COLON,"COLON"},
        {Token::BANG,"BANG"},
        {Token::ISDIFERENT,"ISDIFERENT"},
        {Token::ISEQUAL,"ISEQUAL"},
        {Token::MODULO,"MODULO"}
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
