cmake_minimum_required(VERSION 3.0.0)
project(experiment_cpp VERSION 0.1.0)

set(CMAKE_BUILD_TYPE "Debug")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MTd")
if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT> #---------|
        $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
        $<$<CONFIG:Release>:/MT> #--|
    )
endif()

add_executable(product_test main.cpp)

