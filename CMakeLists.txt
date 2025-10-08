cmake_minimum_required(VERSION 3.12)

project(TwoQCacheProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${INCLUDE_DIR})

add_executable(cache_2q ${SOURCE_DIR}/main_2q.cpp)
add_executable(cache_ideal ${SOURCE_DIR}/main_ideal.cpp)
target_include_directories(cache_2q PRIVATE ${INCLUDE_DIR})
target_include_directories(cache_ideal PRIVATE ${INCLUDE_DIR})

#check OC && compiler
message("Building on ${CMAKE_SYSTEM_NAME} with compiler ${CMAKE_CXX_COMPILER_ID}")