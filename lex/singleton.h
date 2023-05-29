#pragma once
#include <type_traits>

template<class T>
struct Singleton {
    static T &instance() {
        static T *p_instance = new T();
        return *p_instance;
    }
};
