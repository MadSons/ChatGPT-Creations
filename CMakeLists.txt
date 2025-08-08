cmake_minimum_required(VERSION 3.10)
project(PlatformerDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} include)

add_executable(platformer src/main.cpp)
target_link_libraries(platformer ${SDL2_LIBRARIES})
