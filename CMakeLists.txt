cmake_minimum_required(VERSION 2.8)

project(Test CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Winvalid-pch -Wall -Wextra -Wold-style-cast -Wno-strict-aliasing -std=c++1z -fno-exceptions -fno-rtti -ffunction-sections -fdata-sections -Ofast -march=native -ffast-math")
#NOTE: only do -flto on Release

file(GLOB INCLUDES
        include/*.h
)

include_directories(include)

add_executable(test-dod src/test-array.cpp src/test-main.cpp)
target_compile_options(test-dod PRIVATE -fexceptions -frtti)

add_executable(small src/small.cpp ${INCLUDES})
