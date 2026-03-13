#include "Block.h"
#include <unordered_map>
Block::Block()
    : x(0), y(0), size(25), tileType(AIR), darknessMeter(0.0f), durability(3)
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
    if (type == AIR)
    {
        texture.id = {0};
    }
}
void Block::SetTypeFromItem(ItemID item)
{
    static const std::unordered_map<ItemID, TileType> itemToBlock = {
        { ITEM_NONE, AIR },
        { ITEM_DIRT, DIRT },
        { ITEM_STONE, STONE },
        { ITEM_IRON, IRON }
    };

    auto it = itemToBlock.find(item);
    if (it != itemToBlock.end())
        SetType(it->second);
    else
        SetType(AIR);
}

void Block::SetTexture(Texture2D tex)
{
    texture = tex;
}

void Block::Hit()
{
    durability--;
    if (durability == 0)
    {
        texture = {0};
        tileType = AIR;
    }
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
    if (tileType == BACKGROUND) {
        DrawTexture(texture, (int)x, (int)y, {255, 255, 255, 127});
    }
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



