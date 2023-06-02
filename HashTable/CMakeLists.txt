cmake_minimum_required(VERSION 3.24)
project(Task4_Hash)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_library(hash hash.h)
set_target_properties(hash PROPERTIES LINKER_LANGUAGE CXX)

add_library(mm mm.h)
set_target_properties(mm PROPERTIES LINKER_LANGUAGE CXX)

add_executable(Task4_Hash test.cpp)

target_link_libraries(Task4_Hash hash mm)

add_test(NAME Task4_Hash
        COMMAND Task4_Hash)


