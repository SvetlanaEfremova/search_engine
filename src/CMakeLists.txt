cmake_minimum_required(VERSION 3.19)
project(SearchEngine)

set(CMAKE_CXX_STANDARD 17)
set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_executable(SearchEngine main.cpp ${MY_INCLUDE_DIR}/ConverterJSON.h ConverterJSON.cpp
        ${MY_INCLUDE_DIR}/RelativeIndex.h RelativeIndex.cpp
        ${MY_INCLUDE_DIR}/Entry.h Entry.cpp
        ${MY_INCLUDE_DIR}/InvertedIndex.h InvertedIndex.cpp
        ${MY_INCLUDE_DIR}/SearchServer.h SearchServer.cpp)
target_link_libraries(SearchEngine nlohmann_json::nlohmann_json)
target_include_directories(SearchEngine PUBLIC ${MY_INCLUDE_DIR})