cmake_minimum_required(VERSION 3.15)
project(bnfc)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/bnfc/*.cpp ${CMAKE_SOURCE_DIR}/parser/*.cpp)

add_executable(bnfc main.cpp LRParser.cpp Dud.cpp LR0.cpp ${SOURCE_FILES} parser/h/Token.h parser/h/Rule.h parser/h/Item.h parser/h/State.h)