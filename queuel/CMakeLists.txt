add_library(queuel queuel.cpp)
set_property(TARGET queuel PROPERTY CXX_STANDARD 20)
target_include_directories(queuel PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )
install(TARGETS queuel)