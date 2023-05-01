#include "para.h"
#include "nfa.h"
#include "dfa.h"
#include "scanner.h"


int main(int argc, char* argv[]) {
    if(argc!=2) {
        std::cout << "parameter error\n";
        return 1;
    }
    NFA nfa;
    nfa.loadGrammar("grammar3.txt");
    DFA dfa;
    dfa.createDFA(nfa);
    Scanner sc;
    sc.scanFile(std::string(argv[1]), dfa);
    sc.outputTokenTable();
    return 0;
}
