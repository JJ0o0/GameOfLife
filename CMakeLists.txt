cmake_minimum_required(VERSION 3.20)
project(GameOfLife)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL3 REQUIRED)

add_executable(app 
    src/main.cpp
    src/core/App.cpp
    src/core/Grid.cpp
)

target_include_directories(app PRIVATE include)
target_link_libraries(app PRIVATE SDL3::SDL3)