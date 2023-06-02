cmake_minimum_required(VERSION 3.24)
project(Task3_Sort)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)


add_library(sort sort.h)
set_target_properties(sort PROPERTIES LINKER_LANGUAGE CXX)

add_executable(Task3_Sort test.cpp)

target_link_libraries(Task3_Sort sort)

add_test(NAME Task3_Sort
        COMMAND Task3_Sort)


