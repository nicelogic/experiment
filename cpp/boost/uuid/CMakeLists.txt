
cmake_minimum_required(VERSION 3.10)

project(a_test)

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

include_directories(include)
include_directories(../../../../common/include/boost_1_72_0)

add_executable(a_test uuid_boost.cpp)

