cmake_minimum_required(VERSION 3.17)
project(web-parser)

set(BUILD_TESTING ON)

option(BUILD_TESTING "Enable testing" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

cmake_policy(SET CMP0077 NEW) # set() overrides option()

add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>") # Define DEBUG in Debug builds

file(GLOB_RECURSE PROJECT_SOURCE src/*.hpp src/*.cpp src/*.h src/*.c)


add_library(web-parser ${PROJECT_SOURCE})

target_include_directories(web-parser PUBLIC src/)

if(BUILD_TESTING)
    #include(CTest)
    enable_testing()

    add_subdirectory(tests)
endif()