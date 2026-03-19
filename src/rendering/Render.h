#pragma once
#include "world/Map.h"
#include "entities/player/Player.h"

class Render {
public:
    static void DrawWorld(Map& map, Player& player, Camera2D& camera);
    static void Read();
};