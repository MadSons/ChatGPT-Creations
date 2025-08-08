file(GLOB ENGINE_SRC engine/*.cpp game/*.cpp)
add_executable(platformer ${ENGINE_SRC} main.cpp)
target_include_directories(platformer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(platformer PRIVATE SDL2::SDL2 SDL2::SDL2main)

if(SDL2_image_FOUND)
  target_link_libraries(platformer PRIVATE SDL2_image::SDL2_image)
endif()

if(SDL2_ttf_FOUND)
  target_link_libraries(platformer PRIVATE SDL2_ttf::SDL2_ttf)
endif()
