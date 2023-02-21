cmake_minimum_required(VERSION 3.24)
project(Task1_List)

set(CMAKE_CXX_STANDARD 14)


include_directories(.)

add_library(memoryManager mm.h)
set_target_properties(memoryManager PROPERTIES LINKER_LANGUAGE CXX)

add_executable(test test.cpp)
target_link_libraries(test memoryManager)

add_test(NAME test
        COMMAND test)

