cmake_minimum_required(VERSION 3.19)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(MakeConfigFiles main_config.cpp ${MY_INCLUDE_DIR}/MakeConfigFile.h MakeConfigFile.cpp
        ${MY_INCLUDE_DIR}/MakeRequestsFile.h MakeRequestsFile.cpp)
target_link_libraries(MakeConfigFiles nlohmann_json::nlohmann_json)
target_include_directories(MakeConfigFiles PUBLIC ${MY_INCLUDE_DIR})
