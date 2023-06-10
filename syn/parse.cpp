#include "parse.h"

std::ostream& operator<<(std::ostream& os, std::vector<char> v) {
    if(!v.size()) {
        os << "{}";
        return os;
    }
    os << "{";
    os << v[0];
    for(int i=1; i<v.size(); ++i) {
        os << ", " << v[i];
    }
    os << "}";
    return os;
}

void Parse::showVnFirst() {
#ifndef NDEBUG
    std::cout << "the FIRST set of Vn is:\n";
    for(auto it=Vn.begin(); it!=Vn.end(); ++it) {
        std::cout << *it << '\t';
        std::cout << first[*it] << '\n';
    }
    std::cout << '\n';
#endif
}

void Parse::buildVn() {
    boost::unordered_set<char> mp;
    for(int i{}; i<in->grammar.size(); ++i) {
        char tmp = in->grammar[i].l;
        if(mp.find(tmp)==mp.end()) {
            mp.insert(tmp);
            Vn.push_back(tmp);
        }
    }
}

std::vector<char> Parse::getVnFirst(char c) {
    //std::cout << "getFirst " << c << '\n';
    if(first.find(c)!=first.end()) return first[c];
    std::vector<char> ret;
    if(!isNonTerminal(c)) {
        ret.push_back(c);
        first[c] = ret;
        return ret;
    }
    for(int i{}; i<in->grammar.size(); ++i) {
        Production tmpGrammar(in->grammar[i].l, in->grammar[i].r);
        if(c==tmpGrammar.l) {
            char tmp = tmpGrammar.r[0];
            //std::cout<<"left: "<<c<<", right: "<<tmpGrammar.r<<'\n';
            if(isNonTerminal(tmp)) {
                for(int j{}; j<tmpGrammar.r.size(); ++j) {
                    char tmpc = tmpGrammar.r[j];
                    if(c==tmpc) continue; //
                    std::vector<char> tmprt = getVnFirst(tmpc);
                    for(auto it=tmprt.begin(); it!=tmprt.end(); ++it) {
                        if(*it!='@') {
                            ret.push_back(*it);
                        }
                    }
                    if(std::find(tmprt.begin(), tmprt.end(), '@')==tmprt.end()) break;
                    else if(j==tmpGrammar.r.size()-1) ret.push_back('@');
                }
            }
            else {
                ret.push_back(tmp);
                //std::cout<<"ret of "<<c<<" is: "<<ret<<'\n';
            }
        }
    }
    std::sort(ret.begin(), ret.end());
    first[c] = ret;
    return ret;
}

std::vector<char> Parse::getStrFirst(std::string str) {
    std::vector<char> ret;
    bool notEnd = true;
    int i{}, n = str.size();
    while(notEnd && i<n) {
        char c = str[i];
        notEnd = false;
        if(!isNonTerminal(c)) {
            if(c=='@') notEnd = true;
            else {
                if(std::find(ret.begin(), ret.end(), c)==ret.end()) ret.push_back(c);
            }
        }
        else {
            for(auto it=first[c].begin(); it!=first[c].end(); ++it) {
                if(*it=='@') notEnd = true;
                if(std::find(ret.begin(), ret.end(), *it)==ret.end()) ret.push_back(*it); // TODO: optimize
            }
        }
        ++i;
    }
    if(notEnd) ret.push_back('@');
    std::sort(ret.begin(), ret.end());
    return ret;
}

State Parse::closure(const State& state) {
    State ret = state;
    bool changed;
    do {
        changed = false;
        for(const auto &i:ret.items) {
            if(i.pos < i.pro.r.size() &&
               isNonTerminal(i.pro.r[i.pos])) {
                char next = i.pro.r[i.pos];
                std::string ba = i.pro.r.substr(i.pos, -1);
                ba.push_back(i.next);
                std::vector<char> bafirst = getStrFirst(ba);
                for(const auto &prod:in->grammar) {
                    if(prod.l==next) {
                        for(char c:bafirst) {
                            Production tmpPro(prod.l, prod.r);
                            int tmpPos{};
                            if(prod.r[0]=='@') tmpPos = 1;
                            item tmpItem(tmpPro, tmpPos, c);
                            bool flag = false;
                            for(auto& s:ret.items) {
                                if(s==tmpItem) {
                                    flag = true;
                                    break;
                                }
                            }
                            if(!flag) {
                                changed = true;
                                ret.items.push_back(tmpItem);
                            }
                        }
                    }
                }
            }
        }
    } while(changed);
    std::sort(ret.items.begin(), ret.items.end());
    return ret;
}

State Parse::goTo(const State& state, const char c) {
    State ret;
    for(const auto &i:state.items) {
        if(i.pos < i.pro.r.size() && 
           i.pro.r[i.pos] == c) {
            Production tmpPro(i.pro.l, i.pro.r);
            item tmpItem(tmpPro, i.pos+1, i.next);
            ret.items.push_back(tmpItem);
        }
    }
    return closure(ret);
}

void Parse::showStates() {
#ifdef NDEBUG
    for(int i{}; i<C.size(); ++i) {
        std::cout << "I_" << i << ": \n";
        for(int j{}; j<C[i].items.size(); ++j) {
            auto it = C[i].items[j];
            std::cout << it.pro.l << '\t'
                << it.pro.r << '\t'
                << it.pos << '\t'
                << it.next <<'\n';
        }
        std::cout << '\n';
    }
#endif
}

int Parse::findState(State& state) {
    int n = state.items.size();
    for(int i={}; i<C.size(); ++i) { // hot but O(n^2), TODO: optimize
        if(n!=C[i].items.size()) continue;
        bool found = true;
        for(auto& tmp1:state.items) {
            bool flag = false;
            for(auto& tmp2:C[i].items) {
                if(tmp1==tmp2) {
                    flag = true;
                    break;
                }
            }
            if(!flag) {
                found = false;
                break;
            }
        }
        if(found) return i;
    }
    return -1;
}

void Parse::pushItems(State& state) {
    int stateNum = findState(state);
    if(stateNum==-1) {
        stateNum = C.size();
        C.push_back(state);
    }
    std::map<char, std::vector<item>> mp;
    for(auto i:state.items) {
        if(i.pos<i.pro.r.size()) {
            char tf = i.pro.r[i.pos];
            item newItem = i;
            ++newItem.pos;
            mp[tf].push_back(newItem);
        }
    }
    for(auto it=mp.begin(); it!=mp.end(); ++it) {
        State inState(it->second);
        State outState = closure(inState);
        int outStateNum = findState(outState);
        if(outStateNum == -1) {
            outStateNum = C.size();
            C.push_back(outState);
        }
        ts[stateNum][it->first] = outStateNum;
    }
}

void Parse::items() {
    Production oriPro(in->grammar[0].l, in->grammar[0].r);
    item oriItem(oriPro, 0, '@');
    std::vector<item> tmpItems;
    tmpItems.push_back(oriItem);
    State oriState = closure(State(tmpItems));
    pushItems(oriState);
    for(int i{}; i<C.size(); ++i) pushItems(C[i]);
}

parseResult Parse::ACTIONandGOTO(int stateNum, char c) {
    if(isNonTerminal(c)){ // GOTO
        if(ts.find(stateNum)!=ts.end() && ts[stateNum].find(c)!=ts[stateNum].end()) {
            return {GOTO, ts[stateNum][c], ""};
        }
        else {
            return {ERROR, -1, ""};
        }
    }
    else{ // ACTION
        if(ts.find(stateNum)!=ts.end() && ts[stateNum].find(c)!=ts[stateNum].end()) {
            return {SHIFT, ts[stateNum][c], ""};
        }
        for(const auto& it:C[stateNum].items) {
            if(it.pos==it.pro.r.size()) {
                if(it.pro.l=='T') return {ACCEPT, -1, ""};
                else {
                    std::string mes = it.pro.r;
                    return {REDUCE, (int)it.pro.l, mes};
                }
            }
        }
    }
    return {ERROR, -1, ""};
}

void Parse::analyze(){
    std::vector<int> states;
    states.push_back(0);
    std::vector<char> symbols;
    symbols.push_back('$');
    int step{}, tokenIdx{}, stateNum{};
    std::string inputToken;
    char inputChar;
    while(step<1000) {
        ++step;
        inputToken = in->tokenTable[tokenIdx].token;
        inputChar = in->mp[inputToken];
        stateNum = states[states.size()-1];
        for(int i{}; i<symbols.size(); ++i) std::cout << symbols[i];
        std::cout << '\t';
#ifndef NDEBUG
        for(int i{}; i<states.size(); ++i) std::cout << states[i];
        std::cout << '\t';
#endif
        std::cout << inputChar << '\n';

        parseResult tmpRet = ACTIONandGOTO(stateNum, inputChar);
        switch(tmpRet.ret) {
            case ACCEPT: {
                std::cout << "syntax analysis successfully!\n";
                return;
            }
            case SHIFT: {
                std::cout << "Shift \n";
                int tmpNum = tmpRet.num;
                states.push_back(tmpNum);
                symbols.push_back(inputChar);
                ++tokenIdx;
                break;
            }
            case REDUCE: {
                std::cout << "Reduce \n";
                char l = (char)tmpRet.num;
                int n = tmpRet.message.size();
                for(int i{}; i<n; ++i) symbols.pop_back();
                symbols.push_back(l);
                for(int i{}; i<n; ++i) states.pop_back();
                break;
            }
            case GOTO: {
                std::cout << "Goto \n";
                int num = tmpRet.num;
                if(num!=-1) {
                    states.push_back(num);
                }
                break;
            }
            case ERROR: {
                tmpRet = ACTIONandGOTO(stateNum, '@');
                if(tmpRet.ret == ERROR) {
                    std::cout << "syntax analysis error!\n";
                    return;
                }
                else {
                    std::cout << "insert @\n";
                    states.push_back(tmpRet.num);
                    symbols.push_back('@');
                    break;
                }
            }
            default: {
                std::cout << "syntax analysis error!\n";
                return;
            }
        }
    }
    std::cout << '\n';
}

void Parse::CTRL(std::unique_ptr<Input> input) {
    in = std::move(input);
    buildVn();
    for(char c:Vn) getVnFirst(c);
    showVnFirst();
    items();
    showStates();
    analyze();
}
