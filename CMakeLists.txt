cmake_minimum_required (VERSION 3.1)
project (render)

set (CMAKE_CXX_STANDARD 11)
include_directories ("/usr/local/include")
link_directories ("/usr/local/lib")

add_executable (
render
	src/animation.cpp
	src/main.cpp
	src/sdlrender.cpp
	src/tilemap.cpp
)

add_executable (
mapedit
	src/tile.cpp
)

target_link_libraries (render SDL2 SDL2_image)
target_link_libraries (mapedit SDL2 SDL2_image)
