cmake_minimum_required(VERSION 3.24)
project(Task5_AVL)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_library(avl avltree.h)
set_target_properties(avl PROPERTIES LINKER_LANGUAGE CXX)

add_library(mm mm.h)
set_target_properties(mm PROPERTIES LINKER_LANGUAGE CXX)

add_executable(Task5_AVL test.cpp)

target_link_libraries(Task5_AVL avl mm)

add_test(NAME Task5_AVL
        COMMAND Task5_AVL)


