cmake_minimum_required(VERSION 2.8)
project(Test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wold-style-cast -Wno-strict-aliasing -std=c++1z -fno-exceptions -fno-rtti -ffunction-sections -fdata-sections -Wl,--gc-sections -flto -fno-use-linker-plugin -Ofast -march=native -ffast-math")

add_subdirectory(benchmark)

file(GLOB INCLUDES
	include/*.h
	Container/*.hxx
	Container/impl/*.hxx
)

include_directories(include)
include_directories(container)
add_executable(test-alloc src/test.cpp ${INCLUDES})
add_executable(benchmark-alloc src/benchmark.cpp ${INCLUDES})

target_link_libraries(test-alloc pthread)
target_link_libraries(benchmark-alloc pthread benchmark)

target_compile_options(benchmark-alloc PRIVATE -frtti)
target_compile_options(test-alloc PRIVATE -fexceptions -frtti)

add_executable(small src/small.cpp ${INCLUDES})