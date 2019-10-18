cmake_minimum_required(VERSION 3.13)
project(blockchain_2_ud)

set(CMAKE_CXX_STANDARD 14)

add_executable(blockchain_2_uzd main.cpp Hash_Function.cpp Users.cpp blockchain.h getandset.cpp)