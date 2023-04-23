import AST;
#include <variant>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "Token.h"
#include "Scanner.h"
#include "ShlangError.h"
#include "AstNodes.h"
constexpr bool variantHas(const std::variant<Types...>& var) noexcept {
    return std::holds_alternative<Ty>(var);
}
Value PrintBuiltin(std::vector<Value> arguments) {
    for (auto const& argument : arguments) {
        if (variantHas<std::string>(argument)) {
            std::cout << std::get<std::string>(argument) << '\n';
        }
        if (variantHas<double>(argument)) {
            std::cout << std::get<double>(argument) << '\n';
        }
    }
    return NoneType();
}
Value InputBuiltin(std::vector<Value> arguments) {
    PrintBuiltin(arguments);
    std::string out;
    std::getline(std::cin, out);
    return Value(out);
};
int main(int argc, char* argv[]) {

    std::map<std::string, Value> base = {
    {"Test",2.0},
    {"PI",3.146210},
    {"print",BuiltinFunc(&PrintBuiltin,-1)},
    {"input",BuiltinFunc(&InputBuiltin,1)}
    };
    Scope env = Scope(nullptr, base);
	std::vector<Node> functionBody = {
	Assignment("as",new Node(Variable("par1"))),
	Return(new Node(BinaryNode(BinaryNode::MULTIPLY, new Node(Variable("as")), new Node(Value(2.0))))),
	};
	std::vector<Node> program = {
		Assignment("new", new Node(Value(2.0))),
		Assignment("mult2", new Node(Function(Block(functionBody), { "par1" }))),
		Assignment("aaa", new Node(Call(Variable("mult2"), { Value(1.0) }))),
		Call(Variable("print"), { Node(Variable("aaa")) })
	};
    Node programBlock = Block(program);
    try {
        Value idk = EvalNode(programBlock, env);
    }
    catch (LangError err) {
        std::cerr << err.what() << '\n';
    }
    return 0;

}
void RPL() {
    while (true) {
        std::string source;
        std::cout << "> ";
        getline(std::cin, source);
        Scanner scan = Scanner(source);
        std::vector<Token> TkStream;
        try {
            TkStream = scan.scanTokens();
        }
        catch (LangError& err) {
            std::cerr << err.what() << '\n';
        }
        for (size_t i = 0; i < TkStream.size(); i++)
        {
            std::cout << TkStream[i].toString() << '\n';
        }
    }
}