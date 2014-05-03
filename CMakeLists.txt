cmake_minimum_required(VERSION 2.8)

file(GLOB SRC
	"src/*.h"
	"src/*.cpp"
)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

add_executable(scrap ${SRC})
