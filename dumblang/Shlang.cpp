
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
    Block condBlock = Block({
        Value(Control(Break()))
    });
    Block printNumber = Block({
        Call("print",{Node(Variable("i"))})
    });
    Block printFizz = Block({
        Call("print",{Node(Value("Fizz"))})
    });
    Block printBuzz = Block({
        Call("print",{Node(Value("Buzz"))})
    });
    Block buzzBranch = Block({ Node(BranchNode(
        new Node(BinaryNode(
            BinaryNode::MODULO,
            new Node(Variable("i")),
            new Node(5.0)
        )), &printBuzz, nullptr
    )) });

    Block loopBlock = Block({

        Node(Assignment("i",
        new Node(BinaryNode(
            BinaryNode::ADD,
            new Node(Variable("i")),
            new Node(1.0)
        )))),
        Node(BranchNode(
            new Node(BinaryNode(
                BinaryNode::MODULO,
                new Node(Variable("i")),
                new Node(3.0)
            )), &printFizz,&buzzBranch
        )),
        

        Node(Call("print",{Node(Variable("i"))})),
        Node(BranchNode(
            new Node(BinaryNode(
                BinaryNode::GREATER_EQUAL,
                new Node(Variable("i")),
                new Node(100.0)
            )), &condBlock,nullptr
        ))
        
        });
    Block programBlock = Block({

        Assignment("i",new Node(0.0)),

        LoopNode(&loopBlock)
        
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