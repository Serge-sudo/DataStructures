add_library(queuea queuea.cpp)
set_property(TARGET queuea PROPERTY CXX_STANDARD 20)
target_include_directories(queuea PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )
install(TARGETS queuea)