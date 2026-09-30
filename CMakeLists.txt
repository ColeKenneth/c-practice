cmake_minimum_required(VERSION 4.3)
project(cpractice C)

set(CMAKE_C_STANDARD 23)

add_executable(cpractice_main main.c)
add_executable(cpractice_dict dictionary/dictionary.c
        dictionary/dictionary.h
        dictionary/main.c)
