cmake_minimum_required(VERSION 3.12)
project(lex_analyzer)

find_package(Boost REQUIRED COMPONENTS serialization)

if(WIN32)
    # TODO
elseif(APPLE)
    include_directories(/opt/homebrew/Cellar/boost/1.81.0_1/include/)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    include_directories(/usr/include/boost)
endif()

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
