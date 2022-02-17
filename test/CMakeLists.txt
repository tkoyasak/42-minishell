cmake_minimum_required(VERSION 3.0)
project(minishell C CXX)

find_package(GTest REQUIRED)
set(CMAKE_CXX_STANDARD 17)
# add_compile_options(-Wall -Wextra -Werror)

add_library(lexer STATIC ../src/lexer/lexer.c)
# include_directories("../src/lexer")
include_directories(../includes)

add_executable(main main.cpp)

target_link_libraries(main
  PRIVATE
    GTest::GTest
    GTest::Main
    lexer
)
