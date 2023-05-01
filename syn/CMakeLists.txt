cmake_minimum_required(VERSION 3.12)
project(syn_analyzer)

find_package(Boost REQUIRED COMPONENTS serialization)

include_directories(/usr/include/boost)

add_library(inputlib STATIC input.cpp)
add_library(parselib STATIC parse.cpp)

add_executable(analyzer syn.cpp)

target_link_libraries(analyzer PUBLIC
    inputlib
    parselib
    Boost::serialization
)

