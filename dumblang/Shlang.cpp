
#include "Scanner.h"
#include "Interpreter.h"
#include "ShlangError.h"


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
        catch (std::exception& err) {
            std::cerr << err.what() << '\n';
        }
        for (size_t i = 0; i < TkStream.size(); i++)
        {
            std::cout << TkStream[i].toString() << '\n';
        }
    }
}
void langTest() {
    Node Block1 = Block({

    Assignment("result",new Node(
        BinaryNode(
            BinaryNode::ADD,
            new Node(9.0),
            new Node(9.0)
        ))),
        Return(new Node(Variable("result")))
    });
    Block programBlock = Block({

        Assignment("var",&Block1),
        Call("print",{Variable("var")})
        
    });
    
    auto it = Interpreter(programBlock);
    try {
        it.execute();
    }
    catch (LangError& err) {
        std::cerr << err.what() << '\n';
    }
}
int main(int argc, char* argv[]) {
    langTest();
    return 0;

}