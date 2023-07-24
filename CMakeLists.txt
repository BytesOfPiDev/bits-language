cmake_minimum_required(VERSION 3.15)

project(
    bits-lang 
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} 
    PRIVATE 
        src/main.cpp
        
        src/common.hpp
        src/chunk.cpp
        src/chunk.hpp
        src/debug.cpp
        src/debug.hpp
        src/value.hpp
        src/value.cpp
)

