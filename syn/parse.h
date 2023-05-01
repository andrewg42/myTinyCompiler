#pragma once

#include "para.h"
#include "input.h"

#include <utility>
#include <memory>
#include <vector>
#include <stack>
#include <map>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

struct item{
    Production pro;
    int pos;
    char next;
    //std::vector<char> next; // cahr -> vector<char>, TODO

    item(Production pro_, int pos_, char next_): pro(pro_), pos(pos_), next(next_) {}

    item(const item &it): pro(it.pro), pos(it.pos), next(it.next)
    {}
    item &operator=(const item &it) {
        pro = it.pro;
        pos = it.pos;
        next = it.next;
        return *this; 
    }

    bool operator==(item &other) {
        return pos==other.pos && next==other.next && pro==other.pro;
    }
    bool operator<(item &other) {
        return pro==other.pro ? pos<other.pos : pro<other.pro;
    }
};

struct State {
    std::vector<item> items;
    State() = default;

    State(const State &state) {
        items = state.items;
    }
    State &operator=(const State &state) {
        items = state.items;
        return *this;
    }

    State(std::vector<item> &items_) {
        items = items_;
    }
};

enum lr1Result{
    ACCEPT,
    SHIFT,
    REDUCE,
    GOTO,
    ERROR,
};

struct parseResult {
    lr1Result ret;
    int num;
    std::string message;
};

struct Parse {
    std::unique_ptr<Input> in;

    std::vector<char> Vn;
    boost::unordered_map<char, std::vector<char>> first;

    std::vector<State> C;
    boost::unordered_map<int, std::map<char, int>> ts;

    bool isNonTerminal(const char c) { return c>='A' && c<='Z'; }
    void showVnFirst();
    void showStates();
    int findState(State&);

    void buildVn();
    std::vector<char> getVnFirst(const char);
    std::vector<char> getStrFirst(const std::string);
    State closure(const State&);
    State goTo(const State&, const char);
    void pushItems(State&);
    void items();
    parseResult ACTIONandGOTO(int, char);
    void analyze();

    void CTRL(std::unique_ptr<Input> input);
};
