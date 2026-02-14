cmake_minimum_required(VERSION 4.2)
project(Constitution LANGUAGES CXX ASM_NASM)


set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ASM_NASM_COMPILER nasm)

add_executable(Constitution
        main.cpp
        main.hpp
#        func_32.asm
        func_64.asm
)
