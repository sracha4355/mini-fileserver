cmake_minimum_required(VERSION 3.2)
project(CommandParser)

include_directories(include/parser/ include/tokenizer/)
file(GLOB LIB_SOURCES "sources/parser/parser.cpp" "sources/tokenizer/tokenizer.cpp")
add_library(parserlib ${LIB_SOURCES})

add_subdirectory(tests)

