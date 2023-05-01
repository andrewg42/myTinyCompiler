#pragma once
#include "para.h"


struct tscc { // transition from char to char
    char l;
    char tf;
    char r;
    char padding;
    tscc(char a, char b, char c): l(a), tf(b), r(c) {}
    ~tscc() = default;
};

struct NFA { // nfa
    std::vector<char> node;
    std::vector<tscc> f;
    boost::unordered_set<char> nmp;

    NFA() {
#ifdef DEBUF
        std::cout << "NFA Create!\n";
#endif
        node.reserve(MAXN);
        node.push_back('Z');
        f.reserve(MAXN);
    }
    ~NFA() = default;

    void createNFA(std::string&);
    void showNFA();

    void loadGrammar(std::string&&);
};
