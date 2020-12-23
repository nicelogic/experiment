
cmake_minimum_required(VERSION 3.10)

project(asio_test)

# option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_BUILD_TYPE "Debug")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MTd")
if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT> #---------|
    )
endif()

include_directories(../../../service/backend/common/include/boost_1_74_0)
link_directories(../../../service/backend/common/lib)
link_libraries(libboost_thread-vc142-mt-x64-1_74.lib)
link_libraries(libboost_date_time-vc142-mt-gd-x64-1_74.lib)


add_executable(asio_test asio/boost_asio.cpp)
add_executable(asio_main asio/main.cpp)

