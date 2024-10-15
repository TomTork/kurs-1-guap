cmake_minimum_required(VERSION 3.28)
project(opt)

set(CMAKE_CXX_STANDARD 26)

add_executable(opt main.cpp
        SQLController.cpp
        SQLController.h
        sqlite3.h
        sqlite3ext.h
        sqlite3.c)
