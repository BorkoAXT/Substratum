#pragma once
#include <raylib.h>
#include <vector>
#include "defines/Defines.h"

struct BlockLayer {
    TileType type;
    Texture2D texture;
    int zIndex;
};

class Block
{
public:
    Block();
    bool IsSolid();
    void SetPos(Vector2 pos);
    Vector2 GetPos();
    Vector2 GetSize();
    void Draw();

    void AddLayer(TileType type, Texture2D tex);
    void SetTypeFromItem(ItemID item, Texture2D tex);
    ItemID Hit();
    void ClearAll();

    TileType GetTopType();

    float darknessMeter;
    int durability;

private:
    float x, y;
    int size;
    std::vector<BlockLayer> layers;
    int GetPriority(TileType tileType);
};