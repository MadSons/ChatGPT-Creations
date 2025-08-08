cmake_minimum_required(VERSION 3.10)
project(PlatformerDemo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Use pkg-config to locate SDL2
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

include_directories(${SDL2_INCLUDE_DIRS} include)
link_directories(${SDL2_LIBRARY_DIRS})


add_executable(platformer src/main.cpp src/Game.cpp src/Player.cpp src/Level.cpp)
target_link_libraries(platformer ${SDL2_LIBRARIES})
