
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
    Block block2 = Block({
        Assignment("fail",new Node("SCREAM IF IT PRINTS")),
        Assignment("var",new Node("1")),
        Call("print",{Variable("var")}),
    });
    Block block1 = Block({
        Assignment("var",new Node("2")),
        Call("print",{Variable("var")}),
        block2,
        Call("print",{Variable("var")}),
        
    });
    Block programBlock = Block({
        
        block1,
        Call("print",{Variable("fail")})
        
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