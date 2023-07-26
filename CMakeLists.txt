cmake_minimum_required(VERSION 3.21)

project(bits-lang LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(BITS_LANG_INCLUDE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set(BITS_LANG_SOURCE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set(BITS_LANG_BINARY_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set(BITS_LANG_VERSION_MAJOR 0)
set(BITS_LANG_VERSION_MINOR 0)
set(BITS_LANG_VERSION_PATCH 0)

configure_file("${BITS_LANG_SOURCE_DIRECTORY}/include/bitslangconfig.h.in"
               "${BITS_LANG_SOURCE_DIRECTORY}/include/bitslangconfig.h")

# Used to get GTest
include(FetchContent)

# ##############################################################################
# GTest setup
FetchContent_Declare(
    GTest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.13.0)

FetchContent_MakeAvailable(GTest)
# ##############################################################################

# ##############################################################################
# Bits setup
add_executable(${PROJECT_NAME} src/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE include src)

add_library(${PROJECT_NAME}.core)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}.core)
target_include_directories(${PROJECT_NAME}.core PUBLIC include)
target_sources(
    ${PROJECT_NAME}.core
    PRIVATE include/chunk.hpp include/value.hpp src/chunk.cpp src/debug.hpp
            src/debug.cpp src/value.cpp)

# ##############################################################################

# ##############################################################################
# Test target setup
add_executable(${PROJECT_NAME}.tests)
target_sources(${PROJECT_NAME}.tests PRIVATE tests/src/main.cpp)

target_link_libraries(${PROJECT_NAME}.tests PRIVATE GTest::gtest
                                                    ${PROJECT_NAME}.core)
# ##############################################################################
