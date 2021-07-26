cmake_minimum_required(VERSION 3.19)
project(FormulaParserCPP)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(FormulaParserCPP
        ../src/Exceptions.h
        ../src/Formula.cpp
        ../src/Formula.h
        ../src/IDataStructure.h
        ../src/Nodes.cpp
        ../src/Nodes.h
        ../src/Parser.cpp
        ../src/Parser.h
        tests_SimpleNode.cpp
        ../src/Token.cpp
        ../src/Token.h)