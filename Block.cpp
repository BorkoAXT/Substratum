#include "Block.h"
enum BlockType
{
    Dirt,
    Iron
};
Block::Block()
{
    x = 0;
    y = 0;
    size = 25; // cell size
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

void Block::SetTexture(Texture2D tex)
{
    texture = tex;
}

void Block::Draw()
{
    DrawTexture(texture, x, y, WHITE);
}


