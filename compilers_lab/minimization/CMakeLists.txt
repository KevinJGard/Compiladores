cmake_minimum_required(VERSION 3.16)
project(minimization C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(minimization
    ./src/main.c
    ./src/dfa.c
    ./src/reachability.c
    ./src/hopcroft.c
    ./src/minimize.c
)

add_executable(reachability_test
    ./src/reachability_test.c
    ./src/dfa.c
    ./src/reachability.c
)

add_executable(minimize_test
    ./src/minimize_test.c
    ./src/dfa.c
    ./src/reachability.c
    ./src/hopcroft.c
    ./src/minimize.c
)
