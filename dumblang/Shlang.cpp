
#include "Scanner.h"
#include "AST.h"


int main(int argc, char* argv[]) {
    Scope env = Scope(nullptr, defMap);
	std::vector<Node> functionBody = {
	Assignment("as",new Node(Variable("par1"))),
	Return(new Node(BinaryNode(BinaryNode::MULTIPLY, new Node(Variable("as")), new Node(Value(2.0))))),
	};
    Node func = Function(Block(functionBody), { "par1" });
	std::vector<Node> program = {
		Assignment("new", new Node(Value(2.0))),
		Assignment("mult2", &func),
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