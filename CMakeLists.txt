cmake_minimum_required(VERSION 3.27)
project(COP3530_Project2_main)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(COP3530_Project2_main
        hashtable.cpp
        hashtable.h
        main.cpp
        trie.cpp
        trie.h)
