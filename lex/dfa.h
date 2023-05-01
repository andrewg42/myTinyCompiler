#pragma once
#include "para.h"
#include "nfa.h"


using ns = std::vector<char>; // set of nodes

struct tsss { // transition from set to set
    ns& l;
    ns& r;
    char tf;
    char padding[7];
    tsss(ns& a, ns& b, char c): l(a), r(b), tf(c) {}
    ~tsss() = default;
};

struct DFA { // dfa
    std::vector<ns> node;
    std::vector<tsss> f;

    DFA() {
        node.reserve(MAXN);
        f.reserve(MAXN);
    }
    ~DFA() = default;

    bool isEqual(ns&, ns&); // how to accelerate?
    bool isEnd(ns&);
    void getTerminator(std::vector<char>&, NFA&);
    void closure(ns&, NFA&);
    void move(const ns&, const char, ns&, NFA&);

    void createDFA(NFA&);
    void showDFA();
};
