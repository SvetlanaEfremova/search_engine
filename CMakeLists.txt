cmake_minimum_required(VERSION 3.19)
project(search_engine)

set(CMAKE_CXX_STANDARD 17)

set(JSON_BuildTests OFF CACHE INTERNAL "")
set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(nlohmann_json)

include(FetchContent)
include(GoogleTest)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(search_engine main.cpp)

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

target_link_libraries(search_engine PRIVATE gtest_main)

gtest_discover_tests(search_engine)