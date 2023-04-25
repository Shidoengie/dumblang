
#include "Scanner.h"
#include "Interpreter.h"



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
void langTest() {
    auto block2 = Block({
        Assignment("b",new Node("SCREAM IF IT PRINTS")),
        Assignment("as",new Node("b")),
        Call("print",{Variable("as")}),
        Call("print",{Variable("fg")})
    });
    auto block1 = Block({

        Assignment("as",new Node("a")),
        Call("print",{Variable("as")}),
        block2,
        Call("print",{Variable("as")}),
        
    });
    auto programBlock = Block({
        Assignment("fg",new Node("da")),
        block1,
        Call("print",{Variable("b")}),
        
    });
    auto it = Interpreter();
    try {
        Value idk = it.EvalNode(programBlock);
    }
    catch (LangError err) {
        std::cerr << err.what() << '\n';
    }
}
int main(int argc, char* argv[]) {
    langTest();
    return 0;

}