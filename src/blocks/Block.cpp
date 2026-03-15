#include "Block.h"
#include <unordered_map>
#include <algorithm>

Block::Block()
    : x(0), y(0), size(25), darknessMeter(0.0f), durability(3)
{
}

bool Block::IsSolid()
{
    if (layers.empty()) return false;

    for (const auto& layer : layers)
    {
        if (layer.type == AIR ||
            layer.type == TREE_TRUNK ||
            layer.type == TREE_PART ||
            layer.type == TREE_LEAVES ||
            layer.type == TREE_CAP ||
            layer.type == BACKGROUND ||
            layer.type == GRASS_LEAVES_1 ||
            layer.type == GRASS_LEAVES_2 ||
            layer.type == YELLOW_FLOWER)
        {
            continue;
        }
        return true;
    }
    return false;
}
bool Block::IsTree()
{
    for (auto& layer : layers)
    {
        if (layer.type == TREE_TRUNK ||
            layer.type == TREE_PART ||
            layer.type == TREE_CAP)
        {
            return true;
        }
    }

    return false;
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

TileType Block::GetTopType()
{
    if (layers.empty()) return AIR;
    return layers.back().type;
}
Texture2D Block::GetTopTexture()
{
    if (layers.empty()) return {0};
    return layers.back().texture;
}
void Block::SetLastLayerType(TileType type)
{
    if (layers.empty()) return;
    layers.back().type = type;
}
void Block::AddLayer(TileType type, Texture2D tex)
{
    layers.push_back({ type, tex, GetPriority(type) });

    std::sort(layers.begin(), layers.end(), [](const BlockLayer& a, const BlockLayer& b) {
        return a.zIndex < b.zIndex;
    });
}

void Block::ClearAll()
{
    layers.clear();
}

void Block::SetTypeFromItem(ItemID item, Texture2D tex)
{
    static const std::unordered_map<ItemID, TileType> itemToBlock = {
        { ITEM_NONE, AIR },
        { ITEM_DIRT, DIRT },
        { ITEM_STONE, STONE },
        { ITEM_IRON, IRON },
        {ITEM_WOOD, WOOD}
    };

    auto it = itemToBlock.find(item);
    TileType type = (it != itemToBlock.end()) ? it->second : AIR;

    if (type == AIR)
    {
        ClearAll();
    }
    else
    {
        AddLayer(type, tex);
    }
}

ItemID Block::Hit()
{
    if (layers.empty()) return ITEM_NONE;

    durability--;
    if (durability <= 0)
    {
        TileType type = layers.back().type;
        layers.pop_back();
        durability = 3;

        if (type == DIRT) return ITEM_DIRT;
        if (type == STONE) return ITEM_STONE;
        if (type == IRON) return ITEM_IRON;
        if (type == GRASS) return ITEM_DIRT;
        if (type == WOOD) return ITEM_WOOD;

    }
    return ITEM_NONE;
}
int Block::GetPriority(TileType type)
{
    if (type == BACKGROUND) return 0;
    if (type == DIRT || type == STONE || type == IRON || type == GRASS) return 1;
    if (type == TREE_TRUNK || type == TREE_PART) return 2;
    if (type == TREE_LEAVES) return 3;
    if (type == TREE_CAP) return 4;
    return 1;
}

void Block::Draw()
{
    if (layers.empty()) return;

    float brightness = 1.0f - darknessMeter;
    Color tint = { (unsigned char)(255 * brightness), (unsigned char)(255 * brightness), (unsigned char)(255 * brightness), 255 };

    for (const auto& layer : layers)
    {
        Color layerColor = tint;
        if (layer.type == BACKGROUND) layerColor.a = 140;

        if (layer.type == TREE_CAP)
            DrawTexture(layer.texture, (int)x - 15, (int)y - 10, layerColor);
        else
            DrawTexture(layer.texture, (int)x, (int)y, layerColor);
    }
}