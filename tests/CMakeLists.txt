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

add_executable(test_cvector test_cvector.c)
target_link_libraries(test_cvector PRIVATE cutils_static)

add_executable(test_robin_hood_hash test_robin_hood_hash.c)
target_link_libraries(test_robin_hood_hash PRIVATE cutils_static)
target_include_directories(test_robin_hood_hash PRIVATE ${xxhash_SOURCE_DIR})

add_executable(test_avltree test_avltree.c)
target_link_libraries(test_avltree PRIVATE cutils_static)
