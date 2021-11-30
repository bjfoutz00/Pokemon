cmake_minimum_required(VERSION 3.20)
project(Pokemon)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Pokemon
        BST.h
        BSTInterface.h
        HashMap.h
        HashMapInterface.h
        Lab09Pokemon.cpp
        Pair.h
        Set.h
        SetInterface.h)
