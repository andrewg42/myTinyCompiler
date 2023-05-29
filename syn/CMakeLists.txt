cmake_minimum_required(VERSION 3.12)
project(syn_analyzer)

find_package(Boost REQUIRED COMPONENTS serialization)

if(WIN32)
    # TODO
elseif(APPLE)
    include_directories(/opt/homebrew/Cellar/boost/1.81.0_1/include/)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    include_directories(/usr/include/boost)
endif()

add_library(inputlib STATIC input.cpp)
add_library(parselib STATIC parse.cpp)

add_executable(analyzer syn.cpp)

target_link_libraries(analyzer PUBLIC
    inputlib
    parselib
    Boost::serialization
)

