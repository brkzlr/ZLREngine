cmake_minimum_required(VERSION 4.0...4.4)
project(ZLREngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

find_package(SDL3 REQUIRED)
find_package(glm REQUIRED)

add_library(External STATIC
	External/glad.c
	External/stb_image.cpp
)
target_include_directories(External PUBLIC ${CMAKE_SOURCE_DIR})
target_compile_options(External PRIVATE -w)

file(GLOB_RECURSE ENGINE_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(zlr ${ENGINE_SOURCES})

target_compile_options(zlr PRIVATE -g -pedantic-errors -Wall -Wextra -Wdeprecated -Wdeprecated-declarations -Werror)
target_include_directories(zlr PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(zlr PRIVATE External SDL3::SDL3 glm::glm)
