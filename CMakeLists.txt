cmake_minimum_required(VERSION 3.15)
project(CarSimTest VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.0
)

FetchContent_MakeAvailable(raylib)

add_executable(CarSimTest "src/main.cpp")
target_sources(CarSimTest PRIVATE
    "src/config.h"
    "src/drawing.h"
    "src/utils.h"
    "src/setup.h" 
    "src/movement.h"
    "src/types.h"
    "src/ego.h" 
)

target_link_libraries(CarSimTest PRIVATE raylib)
