cmake_minimum_required(VERSION 3.0.2)
project(AlphaVM)

set(VERSION_MAJOR "0")
set(VERSION_MINOR "5")
set(VERSION_PATCH "0")

set(SOURCE_FILES
  avm.cpp
  avm_helpers.cpp
  avm_lib.cpp
  avm_memory.cpp
  avm_op.cpp
  avm_stack.cpp
  avm_table.cpp
  handlealloc32.cpp
  main.cpp)

if(WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
  include_directories(compat)
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
endif()

add_executable(AlphaVM ${SOURCE_FILES})

set_target_properties(AlphaVM PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}
)
