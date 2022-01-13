cmake_minimum_required(VERSION 3.10)
project(cutils)

option(CUTILS_BUILD_TESTS "build tests" ON)

file(GLOB CUTILS_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
add_library(cutils_static ${CUTILS_SRC})
target_include_directories(cutils_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(cutils_static PUBLIC pthread)

if(CUTILS_BUILD_TESTS)
    add_subdirectory(tests)
endif()
