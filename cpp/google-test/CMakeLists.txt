
cmake_minimum_required(VERSION 3.0.0)
project(googletest_experiment VERSION 0.1.0)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories("../opencode/googletest/googletest/include")
link_directories("../opencode/googletest/build/lib")
link_libraries("libgtest.a")
link_libraries("libgtest_main.a")
add_executable(google-test google-test.cpp)


