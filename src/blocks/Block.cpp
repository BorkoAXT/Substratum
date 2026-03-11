#include "Block.h"
Block::Block()
{
    x = 0;
    y = 0;
    size = 25;
}

void Block::SetPos(Vector2 pos)
{
    x = (int)pos.x;
    y = (int)pos.y;
}

Vector2 Block::GetPos()
{
    return { (float)x, (float)y };
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
    unsigned char colorValue = (unsigned char)(255 * (1.0f - darknessMeter));
    DrawTexture(texture, x, y, {colorValue, colorValue, colorValue, 255});
}



