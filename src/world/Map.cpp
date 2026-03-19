#include "Map.h"
#include "NoiseGen.hpp"
#include "../blocks/Block.h"
#include "../defines/Defines.h"
#include "managers/AssetManager.h"
#include <vector>
#include <cmath>
#include <algorithm>
#include "../entities/player/Player.h"
#include <raymath.h>

Map::Map(NoiseGen& noiseGen)
{
    rows = ROWS;
    cols = COLS;
    cellSize = CELL_SIZE;
    blocks.resize(cols, std::vector<Block>(rows));
    droppedItems = {};
    noiseGen.GenerateFullMap(*this);
}

int Map::GetSurfaceLevel(int col)
{
    if (col < 0) col = 0;
    if (col >= cols) col = cols - 1;
    for (int y = 0; y < rows; y++)
    {
        if (blocks[col][y].GetTopType() != AIR)
            return y;
    }
    return rows - 1;
}

Block& Map::GetBlock(int row, int col)
{
    if (col < 0) col = 0;
    if (col >= cols) col = cols - 1;
    if (row < 0) row = 0;
    if (row >= rows) row = rows - 1;
    return blocks[col][row];
}

void Map::ResolveItemInsideBlock(ItemEntity& item)
{
    int col = (int)(item.position.x / CELL_SIZE);
    int row = (int)(item.position.y / CELL_SIZE);
    while (col >= 0 && col < cols && row >= 0 && row < rows && GetBlock(row, col).IsSolid() && row > 0)
    {
        row--;
        item.position.y = row * CELL_SIZE + (CELL_SIZE / 2.0f);
    }
}

int Map::GetTreeHeight(int col, int row)
{
    int height = 0;
    int r = row;
    while (r < rows)
    {
        if (!GetBlock(r, col).IsTree())
            break;
        height++;
        r++;
    }
    return height;
}

void Map::RemoveTree(int col, int row, int height)
{
    for (int i = 0; i < height; i++)
    {
        GetBlock(row + i, col).ClearAll();
    }
}

void Map::SpawnItem(ItemID id, Vector2 pos)
{
    ItemEntity item;
    item.id = id;
    item.position = pos;
    item.velocity = {(float)(rand() % 100 - 50), (float)-(rand() % 100 + 100)};
    item.active = true;
    item.age = 0.0f;
    ResolveItemInsideBlock(item);
    droppedItems.push_back(item);
}

void Map::UpdateItems()
{
    float dt = GetFrameTime();
    for (auto& item : droppedItems)
    {
        if (!item.active) continue;
        float speed = Vector2Length(item.velocity);
        if (speed < 550.0f) item.velocity.y += 900.0f * dt;
        item.velocity.x *= (1.0f - dt * 1.5f);
        Vector2 nextPos = Vector2Add(item.position, Vector2Scale(item.velocity, dt));
        int col = (int)(nextPos.x / CELL_SIZE);
        int row = (int)(nextPos.y / CELL_SIZE);
        if (col >= 0 && col < cols && row >= 0 && row < rows)
        {
            if (!GetBlock(row, col).IsSolid()) item.position = nextPos;
            else
            {
                item.velocity.y = -item.velocity.y * 0.2f;
                item.velocity.x *= 0.5f;
                item.position.y = row * CELL_SIZE - 2.0f;
            }
        }
        else item.active = false;
        item.age += dt;
    }
    if (droppedItems.size() > 200)
    {
        droppedItems.erase(std::remove_if(droppedItems.begin(), droppedItems.end(),
            [](const ItemEntity& i) { return !i.active; }), droppedItems.end());
    }
}

std::vector<ItemID> Map::CollectItems(Vector2 collectorPos, float radius)
{
    std::vector<ItemID> pickedUp;
    for (auto& item : droppedItems)
    {
        if (!item.active) continue;
        float dist = Vector2Distance(item.position, collectorPos);
        if (dist < radius)
        {
            Vector2 dir = Vector2Normalize(Vector2Subtract(collectorPos, item.position));
            item.velocity = Vector2Scale(dir, 700.0f);
            if (dist < 15.0f)
            {
                item.active = false;
                pickedUp.push_back(item.id);
            }
        }
    }
    return pickedUp;
}

void Map::DrawItems()
{
    for (const auto& item : droppedItems)
    {
        if (!item.active) continue;
        Texture2D tex = AssetManager::GetTexture(item.id);
        float bob = sinf(item.age * 5.0f) * 4.0f;
        float scale = 0.6f;
        Vector2 drawPos = {
            item.position.x - (tex.width * scale) / 2.0f,
            item.position.y - (tex.height * scale) / 2.0f + bob
        };
        DrawTextureEx(tex, drawPos, 0.0f, scale, WHITE);
    }
}

Map::~Map() {}