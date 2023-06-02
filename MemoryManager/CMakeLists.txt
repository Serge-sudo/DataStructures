cmake_minimum_required(VERSION 3.24)
project(Task1_List)

set(CMAKE_CXX_STANDARD 14)


include_directories(. LANGUAGES CXX)

add_library(memoryManager mm.h)
add_library(doctest doctest.h)
set_target_properties(memoryManager PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(doctest PROPERTIES LINKER_LANGUAGE CXX)

add_executable(test test.cpp)
target_link_libraries(test memoryManager doctest)

add_test(NAME test
        COMMAND test)

