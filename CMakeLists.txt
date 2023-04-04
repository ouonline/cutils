cmake_minimum_required(VERSION 3.10)
project(cutils)

# ----- options ----- #

option(CUTILS_BUILD_TESTS "build tests" ON)
option(CUTILS_INSTALL "install headers and libs" ON)

# ----- targets ----- #

file(GLOB_RECURSE __CUTILS_SRC__ ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
add_library(cutils_static STATIC ${__CUTILS_SRC__})
unset(__CUTILS_SRC__)

target_include_directories(cutils_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(MSVC)
    target_compile_options(cutils_static PRIVATE /W4)
else()
    target_compile_options(cutils_static PRIVATE -Wall -Wextra -Werror)
endif()

# ----- installations ----- #

if(CUTILS_INSTALL)
    install(DIRECTORY include DESTINATION .)
    install(TARGETS cutils_static DESTINATION lib)
endif()

# ----- tests ----- #

if(CUTILS_BUILD_TESTS)
    add_subdirectory(tests)
endif()
