#include "input.h"


void Input::loadGrammar(std::string&& file) {
    std::ifstream grammar_file;
    grammar_file.open(file);
    if(!grammar_file.is_open()) {
        std::cout << "failed in opening grammar filei\n";
        return;
    }
    std::string str="T->S";
    grammar.push_back(Production(str[0], str.substr(3, -1)));
    while(getline(grammar_file, str)) {
        if(str[0]==annotationSign) continue;
        grammar.push_back(Production(str[0], str.substr(3, -1)));
    }
    grammar_file.close();
    showGrammar();
}

void Input::loadTokenTable() {
    std::ifstream infile("TokenTable", std::ios::binary | std::ios::in);
    if(!infile) {
        std::cout << "failed in open input file\n";
    }
    boost::archive::binary_iarchive ia(infile);
    ia >> tokenTable;
    infile.close();
    showTokenTable();
}


void Input::showGrammar() {
#ifdef DEBUG
    std::cout << '\n';
    std::cout << "CFG: \n";
    for(int i{}; i<grammar.size(); ++i) {
        std::cout << grammar[i].l << '\t' 
                << grammar[i].r << '\n';
    }
    std::cout << '\n';
#endif
}

void Input::showTokenTable() {
#ifdef DEBUG
    std::cout << '\n';
    std::cout << "TokenTable: \n";
    for(int i{}; i<tokenTable.size(); ++i) {
        std::cout << tokenTable[i].token << '\t'
            << tokenTable[i].type << '\t'
            << tokenTable[i].lineno << '\n';
    }
    std::cout << '\n';
#endif
}

void Input::buildTokenMap() {
    for(const auto &t:tokenTable) {
        if(t.type=="variables") {
            mp[t.token] = 'i';
        }
        if(t.type=="keywords") {
            if(t.token=="main") mp[t.token] = 'i';
            if(t.token=="return") mp[t.token] = 'r';
            if(t.token=="for") mp[t.token] = 'f';
            if(t.token=="while") mp[t.token] = 'w';
            if(t.token=="break") mp[t.token] = 'b';
            if(t.token=="continue") mp[t.token] = 'c';
            if(t.token=="if") mp[t.token] = 'z';
            if(t.token=="int"||t.token=="double"||t.token=="float"||
                t.token=="string"||t.token=="long"||t.token=="short"||
                t.token=="char"||t.token=="void")
                mp[t.token] = 't';
        }
        if(t.type=="separators") {
            mp[t.token] = t.token[0];
        }
        if(t.type=="operators") {
            if(t.token=="=") mp[t.token] = '=';
            if(t.token=="<=" || t.token=="==" || t.token==">="||
                t.token==">" || t.token=="<" || t.token=="!=")
                mp[t.token] = 'p';
            else mp[t.token] = 'o';
        }
        if(t.type=="numbers") {
            mp[t.token] = 'n';
        }
    }
}
