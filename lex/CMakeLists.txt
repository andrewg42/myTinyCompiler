cmake_minimum_required(VERSION 3.12)
project(lex_analyzer)

find_package(Boost REQUIRED COMPONENTS serialization)

include_directories(/usr/include/boost)

add_library(nfalib STATIC nfa.cpp)
add_library(dfalib STATIC dfa.cpp)
add_library(scannerlib STATIC scanner.cpp)

add_executable(analyzer lex.cpp)

target_link_libraries(analyzer PUBLIC
    nfalib
    dfalib
    scannerlib
    Boost::serialization
)
