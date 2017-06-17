cmake_minimum_required(VERSION 3.3)
project(AlphaCompiler)

set(VERSION_MAJOR "0")
set(VERSION_MINOR "2")
set(VERSION_PATCH "0")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")

set(SOURCE_FILES
  ast.cpp
  lexer.cpp
  linked_list.cpp
  parser.cpp
  symbol_table.cpp)

add_executable(AlphaCompiler ${SOURCE_FILES})
