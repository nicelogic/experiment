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

include_directories(E:/opencode/webrtc_native/src)
include_directories(E:/opencode/webrtc_native/src/third_party/abseil-cpp)
aux_source_directory(. webrtc_source)
link_directories(E:/opencode/webrtc_native/src/out/Debug/obj)
link_libraries(webrtc.lib
                Msdmo.lib
                Winmm.lib
                dmoguids.lib
                wmcodecdspuuid.lib)
add_executable(webrtc_test ${webrtc_source})

