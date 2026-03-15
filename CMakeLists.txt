cmake_minimum_required(VERSION 3.31)
project(RayLibTerraria)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(thirdparty/raylib-5.5)
add_subdirectory(thirdparty/raygui-4.0/projects/CMake)

file(GLOB_RECURSE SOURCE_FILES
        src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES}
        src/world/NoiseGen.cpp
        src/managers/AssetManager.cpp
        src/managers/AssetManager.h
        src/entities/Entity.h
        src/inventory/inventory.cpp
        src/inventory/inventory.h
        src/entities/item/ItemEntity.h
        src/rendering/Render.cpp
        src/rendering/Render.h
        src/world/FallingTree.h)

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
# target_link_libraries(${PROJECT_NAME} PRIVATE raygui)

target_include_directories(${PROJECT_NAME} PRIVATE
        src
        src/thirdparty/
        thirdparty/raylib-5.5/src
        thirdparty/raygui-4.0/src
)