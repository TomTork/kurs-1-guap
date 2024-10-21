cmake_minimum_required(VERSION 3.28)
project(opt)

set(CMAKE_CXX_STANDARD 26)

set(PDCURSES_DIR "C:/vyz/kurs/PDCurses-3.9/wincon")

include_directories(${PDCURSES_DIR})

set(PDCURSES_LIB "${PDCURSES_DIR}/*")

add_executable(opt main.cpp SQLController.cpp SQLController.h sqlite3.h sqlite3ext.h sqlite3.c)

target_link_libraries(opt ${PDCURSES_LIB})

#add_executable(opt main.cpp
#        SQLController.cpp
#        SQLController.h
#        sqlite3.h
#        sqlite3ext.h
#        sqlite3.c)
