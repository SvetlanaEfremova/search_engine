cmake_minimum_required(VERSION 3.19)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(MY_SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(SrcFiles
        ${MY_SRC_DIR}/InvertedIndex.cpp
        ${MY_SRC_DIR}/SearchServer.cpp
        ${MY_SRC_DIR}/ConverterJSON.cpp
        )

add_executable(GtestTests InvertedIndexTest.cpp SearchServerTest.cpp ConverterJSONTest.cpp ${SrcFiles})
target_link_libraries(GtestTests nlohmann_json::nlohmann_json gtest_main)
include(GoogleTest)
target_include_directories(GtestTests PUBLIC ${MY_INCLUDE_DIR})
gtest_discover_tests(GtestTests)