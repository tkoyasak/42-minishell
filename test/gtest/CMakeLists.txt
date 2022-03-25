cmake_minimum_required(VERSION 3.0)
project(minishell C CXX)

find_package(GTest REQUIRED)
set(CMAKE_CXX_STANDARD 17)
# add_compile_options(-Wall -Wextra -Werror)

file(GLOB LIBFT ../../lib/libft/src/*/*.c ../../lib/libft/includes/*.h)
# file(GLOB EXECUTOR ../../src/executor/*.c)
# file(GLOB ENV ../../src/env/*.c)

add_library(lexer STATIC ../../src/lexer/lexer.c)
add_library(parser STATIC ../../src/parser/parser.c)
add_library(expander STATIC ../../src/expander/expander.c)
# add_library(executor STATIC ${EXECUTOR})
# add_library(env STATIC ${ENV})
add_library(ft STATIC ${LIBFT})
include_directories(../../includes)
include_directories(../../lib/libft/includes)
#
# add_executable(main main00.cpp)
# add_executable(main main01.cpp)
add_executable(main main02.cpp)
# add_executable(main executor.cpp)

target_link_libraries(main
  PRIVATE
    GTest::GTest
    GTest::Main
    lexer
    parser
    expander
    # executor
	# env
    ft
)
