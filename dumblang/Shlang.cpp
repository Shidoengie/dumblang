
#include "Scanner.h"
#include "AST.h"


int main(int argc, char* argv[]) {
	std::vector<Node> functionBody = {
	Assignment("as",new Node(Variable("par1"))),
	Return(new Node(BinaryNode(BinaryNode::MULTIPLY, new Node(Variable("as")), new Node(Value(2.0))))),
	};
    Node func = Function(Block(functionBody), { "par1" });
    Block ifBlock = Block({ Call(Variable("print"),{ "true" }) });
    Block elseBlock = Block({ Call(Variable("print"),{ "false" }) });
    Block loopblock = Block(
        {
            Assignment("res",new Node(BinaryNode(BinaryNode::MULTIPLY, new Node(Variable("res")), new Node(Value(2.0))))),
            Call(Variable("print"),{Variable("res")}) 
            
        }
    );
    std::vector<Node> program = {

        Assignment("var",new Node(Call(Variable("input"),{"1-true 0-false:"}))),
        Assignment("conv",new Node(Call(Variable("strToNum"),{Variable("var")}))),
        BranchNode(new Node(Variable("conv")),&ifBlock,&elseBlock),
        Assignment("mult2",&func),
        Assignment("res",new Node(Call(Variable("mult2"),{Value(2.0)}))),
        Call(Variable("print"),{Variable("res")}),
        WhileNode(new Node(BinaryNode(
            BinaryNode::LESSER,
            new Node(Variable("res")), 
            new Node(Value(200.0)) 
        )),&loopblock),


	};
    auto it = Interpreter();
    Node programBlock = Block(program);
    try {
        Value idk = it.EvalNode(programBlock);
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