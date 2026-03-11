#pragma once
#include <raylib.h>
#include <world/Map.h>

class Entity
{
public:
    virtual ~Entity() = default;

    virtual void Update(Map& map) = 0;
    virtual void Draw() = 0;

    Vector2 position;
    Vector2 size;
};