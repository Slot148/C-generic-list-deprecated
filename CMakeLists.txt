cmake_minimum_required(VERSION 3.10)
project(lista)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

add_library(Tlist STATIC src/Tlist.c src/Titerator.c)

target_compile_options(Tlist PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_include_directories(Tlist PUBLIC include)

set_target_properties(Tlist PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
)