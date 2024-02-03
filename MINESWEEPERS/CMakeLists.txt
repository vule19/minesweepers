cmake_minimum_required(VERSION 3.2)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(project3)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in ssthe CMake cache
set(SFML_DIR "C:/Users/minha/Downloads/SFML-2.5.1/SFML-2.5.1/lib/cmake/SFML")

find_package(SFML 2.5.1 COMPONENTS system window graphics audio REQUIRED)
add_executable(project3 main.cpp
        TextureManager.cpp
        TextureManager.h
        Board.h
        Tile.h
)
target_link_libraries(project3 sfml-system sfml-window sfml-graphics sfml-audio)