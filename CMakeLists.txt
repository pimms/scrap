cmake_minimum_required(VERSION 2.8)

LIST( APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

FILE(GLOB SRC "src/*.h" "src/*.cpp")
FILE(GLOB TEST_SRC ${SRC} "src/tests/*.h" "src/tests/*.cpp")

ADD_EXECUTABLE(scrap ${SRC})

INCLUDE_DIRECTORIES("gtest/include")
INCLUDE_DIRECTORIES("src/")
ADD_EXECUTABLE(test ${TEST_SRC})
TARGET_LINK_LIBRARIES(test ${CMAKE_CURRENT_LIST_DIR}/gtest/build/libgtest.a)
TARGET_LINK_LIBRARIES(test "pthread")
SET_TARGET_PROPERTIES(test PROPERTIES COMPILE_FLAGS -D_SCRAP_TEST_)

MESSAGE(STATUS "Main sources: ${SRC}")
MESSAGE(STATUS "Test sources: ${TEST_SRC}")
