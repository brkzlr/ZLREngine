cmake_minimum_required(VERSION 3.16)
project(ZLREngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(zlr ${SOURCES})

target_compile_options(zlr PRIVATE -g -pedantic-errors)
target_link_libraries(zlr PRIVATE dl SDL2)
