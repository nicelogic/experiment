cmake_minimum_required(VERSION 3.0.0)
project(experiment_cpp VERSION 0.1.0)

set(CMAKE_BUILD_TYPE "Debug")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MTd")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT> #---------|
        $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
        $<$<CONFIG:Release>:/MT> #--|
    )
endif()

add_executable(main main.cpp)
add_executable(functional_test c++11/function.cpp)
add_executable(smart-point-test c++11/smart-point.cpp)
add_executable(lambda-test c++11/lambda.cpp)
add_executable(class-test class/class.cpp)

