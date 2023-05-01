#pragma once

#include "para.h"

#include <string>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>


enum Type{
    variables,
    keywords,
    separators,
    operators,
    numbers,
    errors,
};

struct Token {
    std::string token;
    std::string type;
    int lineno;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & token;
        ar & type;
        ar & lineno;
    }
};
