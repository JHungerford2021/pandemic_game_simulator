cmake_minimum_required(VERSION 3.0.0)
project(pandemic_game_simulator VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_INCLUDE_PATH ${CMAKE_BINARY_DIR})

add_executable(pandemic_game_simulator src/main.cpp)
target_include_directories(pandemic_game_simulator PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

add_executable(pandemic_game_tests src/tests.cpp)
target_include_directories(pandemic_game_simulator PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )

message("CXX Standard: ${CMAKE_CXX_STANDARD}")
message("CMAKE_INCLUDE_PATH: ${CMAKE_INCLUDE_PATH}")