cmake_minimum_required(VERSION 3.10)
project(Tlist)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(Tlist STATIC src/Tlist.c src/Titerator.c)
target_compile_options(Tlist PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(Tlist PUBLIC include)
