﻿#include <variant>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "Token.h"
#include "Scanner.h"

int main(int argc, char* argv[]) {
    while (true) {
        std::string source;
        std::cout << "> ";
        getline(std::cin, source);
        Scanner scan = Scanner(source);
        std::vector<Token> TkStream = scan.scanTokens();
        for (size_t i = 0; i < TkStream.size(); i++)
        {
            std::cout << TkStream[i].toString() << "\n";
        }
    }
    return 0;

}
