cmake_minimum_required(VERSION 3.21)
project(Platformer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image QUIET)
find_package(SDL2_ttf QUIET)

if(NOT SDL2_image_FOUND)
  message(STATUS "SDL2_image not found; image loading will be disabled")
endif()

if(NOT SDL2_ttf_FOUND)
  message(STATUS "SDL2_ttf not found; font rendering will be disabled")
endif()

add_subdirectory(src)
