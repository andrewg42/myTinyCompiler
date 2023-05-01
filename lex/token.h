#include "para.h"
#include <boost/archive/binary_oarchive.hpp>
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

    Token(std::string&& token_, std::string&& type_, int lineno_):
        token(token_), type(type_), lineno(lineno_) {}

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & token;
        ar & type;
        ar & lineno;
    }
};
