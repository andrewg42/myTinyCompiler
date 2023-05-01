#pragma once

#include "para.h"
#include "token.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>


struct Production {
    char l;
    std::string r;

    Production(char l_, std::string r_): l(l_), r(r_) {}
    Production(const Production &pro): l(pro.l), r(pro.r) {}
    Production &operator=(const Production &pro) {
        l = pro.l;
        r = pro.r;
        return *this;
    }


    bool operator==(Production& other) {
        return l==other.l && r==other.r;
    }
    bool operator<(Production& other) {
        return l==other.l ? r<other.r : l<other.l;
    }
};


struct Input {
    std::vector<Production> grammar;
    std::vector<Token> tokenTable;
    std::map<std::string, char> mp;

    void loadGrammar(std::string &&);
    void loadTokenTable();
    void buildTokenMap();

    void showGrammar();
    void showTokenTable();
};
