cmake_minimum_required(VERSION 2.8)

list( APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

file(GLOB SRC "src/*.h" "src/*.cpp")
file(GLOB TEST_SRC ${SRC} "src/tests/*.h" "src/tests/*.cpp")

add_executable(scrap ${SRC})

include_directories("gtest/include")
include_directories("src/")
add_executable(tests ${TEST_SRC})
target_link_libraries(tests ${CMAKE_CURRENT_LIST_DIR}/gtest/build/libgtest.a)
target_link_libraries(tests "pthread")
set_target_properties(tests PROPERTIES COMPILE_FLAGS -D_SCRAP_TEST_)
