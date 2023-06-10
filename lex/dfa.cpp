#include "dfa.h"


bool DFA::isEqual(ns& a, ns& b) {
    int n = a.size(), m = b.size();
    if(n!=m) return false;
    for(int i{}; i<n; ++i) {
        if(a[i]!=b[i]) return false;
    }
    return true;
}

bool DFA::isEnd(ns& a) {
    return a[a.size()-1]=='Z';
}

void DFA::getTerminator(ns& ter, NFA& nfa) {
    boost::unordered_set<char> mp;
    for(int i{}; i<nfa.f.size(); ++i) {
        char tmp = nfa.f[i].tf;
        if(mp.find(tmp)==mp.end()) {
            ter.push_back(tmp);
            mp.insert(tmp);
        }
    }
    sort(ter.begin(), ter.end());
}

void DFA::closure(ns& T, NFA& nfa) {
    ns ret;
    boost::unordered_set<char> mp;
    for(auto n:T) {
        ret.push_back(n);
        mp.insert(n);
    }
    for(int i{}; i<ret.size(); ++i) {
        for(auto ts:nfa.f) {
            if(ts.l==ret[i] && ts.tf=='@') {
                char r = ts.r;
                if(mp.find(r)==mp.end()) {
                    ret.push_back(r);
                    mp.insert(r);
                }
            }
        }
    }
    std::sort(ret.begin(), ret.end());
    T = std::move(ret);
}

void DFA::move(const ns& T, const char a, ns& ret, NFA& nfa) {
    boost::unordered_set<char> mp;
    for(auto n:T) mp.insert(n);
    for(int i{}; i<T.size(); ++i) {
        for(auto ts:nfa.f) {
            if(ts.l==T[i] && ts.tf==a)
                ret.push_back(ts.r);
        }
    }
    std::sort(ret.begin(), ret.end());
}

void DFA::createDFA(NFA& nfa) {
    ns init = {'S'}, term;
    closure(init, nfa);
    node.push_back(init);
    getTerminator(term, nfa);
    for(int i{}; i<node.size(); ++i) {
        ns& src = node[i];
        for(int j{}; j<term.size(); ++j) {
            if(term[j]=='@') continue;
            ns dst;
            move(src, term[j], dst, nfa);
            if(!dst.size()) continue;
            closure(dst, nfa);
            // judge if the collection already exists
            bool flag{};
            int k{};
            for(; k<node.size(); ++k) {
                if(isEqual(dst, node[k])) {
                    flag = true;
                    break;
                }
            }
            if(!flag) {
                node.push_back(std::move(dst));
            }
            f.push_back(tsss(node[i], node[k], term[j]));
        }
    }
    showDFA();
}

std::ostream& operator<<(std::ostream& os, const ns& s) {
    os << "{";
    for(int i{}; i<s.size(); ++i) {
        if(!i) os << s[i];
        else os << ", " << s[i];
    }
    os << "}";
    return os;
}

void DFA::showDFA() {
#ifndef NDEBUG
    std::cout << '\n';
    std::cout << "DFA: \n";
    std::cout << "nodes: \n";
    for(int i{}; i<node.size(); ++i) {
        std::cout << node[i] << "\n";
    }
    std::cout << "transition table: \n";
    for(int i{}; i<f.size(); ++i) {
        std::cout << f[i].l << "\t";
        std::cout << f[i].tf << "\t";
        std::cout << f[i].r << "\n";
    }
    std::cout << '\n';
#endif
}
