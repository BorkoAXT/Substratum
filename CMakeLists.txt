cmake_minimum_required(VERSION 3.31)
project(untitled5)

set(CMAKE_CXX_STANDARD 26)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build Raylib as static libraries" FORCE)

add_subdirectory(thirdparty/raylib-5.5)

add_executable(${PROJECT_NAME} main.cpp
        grid.cpp
        grid.h
        Map.cpp
        Map.h
        Block.cpp
        Block.h
        Player.cpp
        Player.h)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib_static)
target_include_directories(${PROJECT_NAME} PRIVATE thirdparty/raylib-5.5/src)