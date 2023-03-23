cmake_minimum_required(VERSION 3.19)
project(SearchEngine)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

FetchContent_MakeAvailable(googletest)

set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

enable_testing()

add_subdirectory(nlohmann_json)
add_subdirectory(src)
add_subdirectory(tests)