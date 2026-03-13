#pragma once
#include <raylib.h>
#include "defines/Defines.h"
class Block
{
public:
    Block();
    bool IsSolid();
    void SetPos(Vector2 pos);
    Vector2 GetPos();
    Vector2 GetSize();
    void Draw();
    void SetTexture(Texture2D tex);
    void SetType(TileType type);
    void SetTypeFromItem(ItemID item);
    void Hit();
    TileType GetType();
    float darknessMeter;
    int durability;

private:
    float x, y;
    int size;
    Texture2D texture;
    TileType tileType = AIR;
};
