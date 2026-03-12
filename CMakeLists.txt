cmake_minimum_required(VERSION 3.10)
project(LinearFormLab)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(LIB_SOURCES
    Complex.c
    Double.c
    Integer.c
    LinearForm.c
    Interface.c
)

add_executable(linearform main.c ${LIB_SOURCES})

add_executable(tests test.c ${LIB_SOURCES})

target_compile_definitions(linearform PRIVATE _CRT_SECURE_NO_WARNINGS)
target_compile_definitions(tests PRIVATE _CRT_SECURE_NO_WARNINGS)
