cmake_minimum_required(VERSION 3.10)
project(cutils)

# ----- options ----- #

option(CUTILS_BUILD_TESTS "build tests" ON)
option(CUTILS_INSTALL "install headers and libs" ON)

# ----- targets ----- #

file(GLOB_RECURSE CUTILS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
add_library(cutils_static STATIC ${CUTILS_SRC})
target_include_directories(cutils_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# ----- installations ----- #

if(CUTILS_INSTALL)
    install(DIRECTORY include DESTINATION .)
    install(TARGETS cutils_static DESTINATION lib)
endif()

# ----- tests ----- #

if(CUTILS_BUILD_TESTS)
    add_subdirectory(tests)
endif()
