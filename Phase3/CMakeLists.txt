cmake_minimum_required(VERSION 3.3)
project(AlphaCompiler)

set(VERSION_MAJOR "0")
set(VERSION_MINOR "3")
set(VERSION_PATCH "0")

set(SOURCE_FILES
  ast.cpp
  lexer.cpp
  linked_list.cpp
  parser.cpp
  symbol_table.cpp
  icode.cpp)

if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  include_directories(compat)
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
endif()

add_executable(AlphaCompiler ${SOURCE_FILES})

set_target_properties(AlphaCompiler PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}
)
