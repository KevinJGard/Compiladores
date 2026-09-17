cmake_minimum_required(VERSION 3.16)
project(nfa_to_dfa C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(nfa_to_dfa
    ./src/main.c
    ./src/subset_ops.c
    ./src/dfa.c
    ./src/subset_construction.c
)