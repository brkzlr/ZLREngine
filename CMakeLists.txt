cmake_minimum_required(VERSION 3.29)
project(ZLREngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(zlr ${SOURCES})

target_compile_options(zlr PRIVATE -g -pedantic-errors)
target_link_libraries(zlr PRIVATE dl SDL2)
target_include_directories(zlr PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_precompile_headers(zlr PRIVATE ${CMAKE_SOURCE_DIR}/src/precomp.h)

# Add Apple ARM brew include/library paths
if (APPLE)
	target_include_directories(zlr PRIVATE /opt/homebrew/include)
	target_link_directories(zlr PRIVATE /opt/homebrew/lib)
endif (APPLE)
