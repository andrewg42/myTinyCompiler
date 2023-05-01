#include "para.h"
#include "dfa.h"
#include "token.h"


struct Scanner {
    std::vector<Token> TokenTable;

    bool isKeyword(const std::string& str) {
        if(KEYWORD.find(str)==KEYWORD.end()) return false;
        return true;
    }
    bool isSeparator(const std::string& str) {
        if(SEPARATOR.find(str)==SEPARATOR.end()) return false;
        return true;
    }
    bool isOperator(const std::string& str) {
        if(OPERATOR.find(str)==OPERATOR.end()) return false;
        return true;
    }

    bool isLetter(const char c) {
        if('a'<=c && 'z'>=c || 'A'<=c && 'Z'>=c) return (c!='e' and c!='E');
        return false;
    }
    bool isNum(const char c) { return ('1'<=c && '9'>=c); }

    int scanWord(std::string&, int, DFA&);
    void scanSentence(std::string, int, DFA&);
    void scanFile(std::string&&, DFA&);

    void outputTokenTable();
};
