cmake_minimum_required(VERSION 3.10)
project(cutils-test)

add_executable(test_rbtree test_rbtree.c)
target_link_libraries(test_rbtree PRIVATE cutils_static)

add_executable(test_hash test_hash.c)
target_link_libraries(test_hash PRIVATE cutils_static)

add_executable(test_bitmap test_bitmap.c)
target_link_libraries(test_bitmap PRIVATE cutils_static)

add_executable(test_time_utils test_time_utils.c)
target_link_libraries(test_time_utils PRIVATE cutils_static)

add_executable(test_mt19937 test_mt19937.c)
target_link_libraries(test_mt19937 PRIVATE cutils_static)

add_executable(test_random_perf test_random_perf.c)
target_link_libraries(test_random_perf PRIVATE cutils_static)
