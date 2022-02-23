cmake_minimum_required(VERSION 3.0)
project(minishell C CXX)

find_package(GTest REQUIRED)
set(CMAKE_CXX_STANDARD 17)
# add_compile_options(-Wall -Wextra -Werror)

file(GLOB LIBFT ../lib/libft/src/*/*.c ../lib/libft/includes/*.h)

add_library(lexer STATIC ../src/lexer/lexer.c)
add_library(parser STATIC ../src/parser/parser.c)
add_library(expansion STATIC ../src/expansion/expansion.c)
add_library(ft STATIC ${LIBFT})
include_directories(../includes)
include_directories(../lib/libft/includes)
# 
# add_executable(main main00.cpp)
# add_executable(main main01.cpp)
add_executable(main main02.cpp)

target_link_libraries(main
  PRIVATE
    GTest::GTest
    GTest::Main
    lexer
    parser
    expansion
    ft
)
