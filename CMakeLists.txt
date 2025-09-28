cmake_minimum_required(VERSION 3.12)

project(TwoQCacheProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${INCLUDE_DIR})

add_executable(cache_test ${SOURCE_DIR}/main.cpp)
add_executable(ideal_test ${SOURCE_DIR}/ideal_main.cpp)
target_include_directories(cache_test PRIVATE ${INCLUDE_DIR})
target_include_directories(ideal_test PRIVATE ${INCLUDE_DIR})

#check OC && compiler
message("Building on ${CMAKE_SYSTEM_NAME} with compiler ${CMAKE_CXX_COMPILER_ID}")