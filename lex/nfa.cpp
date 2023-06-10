#include "nfa.h"


void NFA::createNFA(std::string& str) {
    char l = str[0];
    if(nmp.find(l)==nmp.end()) {
        node.push_back(l);
        nmp.insert(l);
    }

    // first pass, record the nodes
    int idx=3;
    while(str[idx]) {
        if(str[idx]<'A' || str[idx]>'Z') {
            ++idx;
            continue;
        }
        if(nmp.find(str[idx])==nmp.end()) {
            node.push_back(str[idx]);
            nmp.insert(str[idx]);
        }
        ++idx;
    }

    // seconde pass
    idx = 3;
    std::string tmp;
    do {
        if(str[idx] && str[idx]!='|') {
            tmp += str[idx];
            continue;
        }
        if(tmp.size()==1) {
            if(tmp[0]<'A' || tmp[0]>'Z') {
                f.push_back(tscc(l, tmp[0], node[0]));
            }
            else {
                f.push_back(tscc(l, '@', tmp[0]));
            }
        }
        else { // tmp.size()==2
            f.push_back(tscc(l, tmp[0], tmp[1]));
        }
        tmp.clear();
    } while(str[idx++]);
}


void NFA::showNFA() {
#ifndef NDEBUG
    std::cout << '\n';
    std::cout << "NFA: \n";
    for(int i{}; i<f.size(); ++i) {
        std::cout << f[i].l <<"\t"
                  <<f[i].tf<<"\t"
                  <<f[i].r<<"\n";
    }
    std::cout << '\n';
#endif
}


void NFA::loadGrammar(std::string&& file) {
    std::ifstream grammar_file;
    grammar_file.open(file);
    if(!grammar_file.is_open()) {
        std::cout << "failed in openning grammar file\n";
        return;
    }
    std::string str;
    while(getline(grammar_file, str)) {
        if(str[0]==annotationSign) continue;
        createNFA(str);
    }
    showNFA();
    grammar_file.close();
}
