cmake_minimum_required(VERSION 3.16)
project(lr1_lalr1 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(lr1_lalr1
    ./src/main.c
    ./src/ast.c
    ./src/motor.c
    ./src/gram_apuntadores.c
)
