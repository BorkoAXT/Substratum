cmake_minimum_required(VERSION 3.31)
project(RayLibTerraria)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(thirdparty/raylib-5.5)

file(GLOB_RECURSE SOURCE_FILES
        src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

target_include_directories(${PROJECT_NAME} PRIVATE
        src
        src/thirdparty/
        thirdparty/raylib-5.5/src
)