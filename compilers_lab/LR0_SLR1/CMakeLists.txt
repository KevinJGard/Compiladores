cmake_minimum_required(VERSION 3.16)
project(lr0_slr1 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(lr0_slr1
    ./src/main.c
    ./src/ast.c
    ./src/motor.c
    ./src/gram_cosmolang.c
)
