cmake_minimum_required(VERSION 3.20)
project(GameOfLife)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)

# Source - https://stackoverflow.com/a/65133324
# Posted by tdashroy, modified by community. See post 'Timeline' for change history
# Retrieved 2026-05-16, License - CC BY-SA 4.0

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy_assets.cmake
)

add_executable(app 
src/main.cpp
src/core/App.cpp
src/core/Grid.cpp
src/core/HUD.cpp
src/core/Utils.cpp
)

add_dependencies(app copy_assets)

target_include_directories(app PRIVATE include)
target_link_libraries(app PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf)