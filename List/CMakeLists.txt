cmake_minimum_required(VERSION 3.24)
project(Task1_List)

set(CMAKE_CXX_STANDARD 14)


include_directories(.)

add_library(list list.h)
set_target_properties(list PROPERTIES LINKER_LANGUAGE CXX)

add_executable(test test.cpp)
target_link_libraries(test list)

add_test(NAME test
        COMMAND test)

