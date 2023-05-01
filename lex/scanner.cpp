#include "scanner.h"


[[nodiscard]] std::string typeName(Type t) {
#define PER(x) if(t==x) return #x;
    PER(variables)
    PER(keywords)
    PER(separators)
    PER(operators)
    PER(numbers)
    PER(errors)
}

int Scanner::scanWord(std::string& mask, int begin, DFA& dfa) {
    ns& src = dfa.node[0];
    int len{}, maxLen{}, n=mask.size();
    std::queue<ns*> q;
    q.push(&src);
    while(begin+len<n && !q.empty()) {
        ++len;
        int qn = q.size();
        while(qn--) {
            auto tmp = q.front();
            q.pop();
            for(int i{}; i<dfa.f.size(); ++i) {
                if(dfa.f[i].tf == mask[begin+len-1] &&
                   dfa.isEqual(dfa.f[i].l, *tmp)) {
                    q.push(&dfa.f[i].r);
                    if(dfa.isEnd(dfa.f[i].r) && len>maxLen) maxLen = len;
                }
            }
        }
    }
    return maxLen;
}


void Scanner::scanSentence(std::string sentence, int lineno, DFA& dfa) {
    std::string mask = sentence;
    int n = mask.size();
    for(int i{}; i<n; ++i) {
        if(isLetter(mask[i])) mask[i] = letterSign;
        if(isNum(mask[i])) mask[i] = numberSign;
    }

    int beg{};
    while(beg<n) {
        char cur = mask[beg];
        if(cur==' ') {
            ++beg;
            continue;
        }
        int len = scanWord(mask, beg, dfa);
        if(len) {
            auto tmpStr = sentence.substr(beg, len);
            if(cur==letterSign || cur=='_' || cur=='e') {
                if(isKeyword(tmpStr)) {
                    TokenTable.push_back(Token(std::move(tmpStr), typeName(keywords), lineno));
                }
                else {
                    TokenTable.push_back(Token(std::move(tmpStr), typeName(variables), lineno));
                }
            }
            else {
                if(isSeparator(tmpStr)) {
                    TokenTable.push_back(Token(std::move(tmpStr), typeName(separators), lineno));
                }
                else if(isOperator(tmpStr)) {
                    TokenTable.push_back(Token(std::move(tmpStr), typeName(operators), lineno));
                }
                else {
                    TokenTable.push_back(Token(std::move(tmpStr), typeName(numbers), lineno));
                }
            }
            beg = beg+len;
        }
        else {
            auto tmpStr = sentence.substr(beg, -1);
            TokenTable.push_back(Token(std::move(tmpStr), typeName(errors), lineno));
            break;
        }
    }
}

void Scanner::scanFile(std::string&& filename, DFA& dfa) {
    std::ifstream input;
    input.open(filename);
    std::string tmpline;
    int line=1;
    while(getline(input, tmpline)) {
        scanSentence(tmpline, line, dfa);
        ++line;
    }
    input.close();

    for(int i{}; i<TokenTable.size(); ++i) {
        std::cout << TokenTable[i].token << "\t\t"
                << TokenTable[i].type << "\t\t"
                << TokenTable[i].lineno<< "\n";
    }
}

void Scanner::outputTokenTable() {
    std::ofstream outfile("TokenTable", std::ios::binary | std::ios::out);
    if(!outfile) {
        std::cout << "can't access output file\n";
    }
    boost::archive::binary_oarchive oa(outfile);
    oa << TokenTable;
    outfile.close();
}
