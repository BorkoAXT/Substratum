#include "Block.h"
Block::Block()
    : x(0), y(0), size(25), tileType(AIR), darknessMeter(0.0f)
{
    texture = {0};
}
bool Block::IsSolid()
{
    if (tileType == AIR) return false;

    if (tileType == TREE_TRUNK ||
        tileType == TREE_PART ||
        tileType == TREE_LEAVES ||
        tileType == TREE_CAP)
    {
        return false;
    }

    return true;
}
void Block::SetPos(Vector2 pos)
{
    x = pos.x;
    y = pos.y;
}

Vector2 Block::GetPos()
{
    return { x, y };
}

Vector2 Block::GetSize()
{
    return { (float)size, (float)size };
}
TileType Block::GetType()
{
    return tileType;
}
void Block::SetType(TileType type)
{
    tileType = type;
}
void Block::SetTexture(Texture2D tex)
{
    texture = tex;
}



void Block::Draw()
{
    float brightness = 1.0f - darknessMeter;
    if (brightness < 0.0f) brightness = 0.0f;
    if (brightness > 1.0f) brightness = 1.0f;

    Color tint = {
        (unsigned char)(255 * brightness),
        (unsigned char)(255 * brightness),
        (unsigned char)(255 * brightness),
        255
    };

    if (texture.id == 0) return;

    if (tileType == TREE_CAP)
    {
        int drawX = (int)x - 15;

        int drawY = (int)y - 10;

        DrawTexture(texture, drawX, drawY, tint);
    }
    else
    {
        DrawTexture(texture, (int)x, (int)y, tint);
    }
}


