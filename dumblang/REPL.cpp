#include <variant>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "Scanner.h"
#include "REPL.h"
int main(int argc, char* argv[]) {

	while (true) {
		std::string src;
		std::cout << "> ";
		std::getline(std::cin, src);
		Scanner scan = Scanner(src);
		std::vector<Token> tkStream = scan.scanTokens();
		for (size_t i = 0; i < tkStream.size(); i++)
		{
			std::cout << tkStream[i].toString() << "\n";
		}
	}
	return 0;

}