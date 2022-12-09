cmake_minimum_required(VERSION 3.14)
project(cutils-test)

add_executable(test_rbtree test_rbtree.c)
target_link_libraries(test_rbtree PRIVATE cutils_static)

add_executable(test_bitmap test_bitmap.c)
target_link_libraries(test_bitmap PRIVATE cutils_static)

add_executable(test_time_utils test_time_utils.c)
target_link_libraries(test_time_utils PRIVATE cutils_static)

add_executable(test_mt19937 test_mt19937.c)
target_link_libraries(test_mt19937 PRIVATE cutils_static)

add_executable(test_random_perf test_random_perf.c)
target_link_libraries(test_random_perf PRIVATE cutils_static)

add_executable(test_str_utils test_str_utils.c)
target_link_libraries(test_str_utils PRIVATE cutils_static)

add_executable(test_qbuf test_qbuf.c)
target_link_libraries(test_qbuf PRIVATE cutils_static)

# -------------------------------------------------------------------------- #

include(FetchContent)
set(FETCHCONTENT_QUIET OFF)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)

if(NOT xxhash_FOUND)
    set(CUTILS_TESTS_DEPS_DIR "${CMAKE_CURRENT_BINARY_DIR}/deps")
    FetchContent_Declare(xxhash
        GIT_REPOSITORY "https://github.com/Cyan4973/xxHash.git"
        GIT_TAG dev
        SOURCE_DIR ${CUTILS_TESTS_DEPS_DIR}/xxhash
        BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/xxhash-build
        SUBBUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/xxhash-subbuild)
    FetchContent_MakeAvailable(xxhash)
endif()

add_executable(test_robin_hood_hash test_robin_hood_hash.c ${xxhash_SOURCE_DIR}/xxhash.c)
target_link_libraries(test_robin_hood_hash PRIVATE cutils_static)
target_include_directories(test_robin_hood_hash PRIVATE ${xxhash_SOURCE_DIR})
