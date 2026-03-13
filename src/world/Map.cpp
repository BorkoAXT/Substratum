#include "Map.h"
#include "NoiseGen.hpp"
#include "../blocks/Block.h"
#include "../defines/Defines.h"
#include "managers/AssetManager.h"
#include <vector>
#include <cmath>
#include "../entities/player/Player.h"
#include <raymath.h>
Map::Map(NoiseGen& noiseGen)
{
    rows = ROWS;
    cols = COLS;
    cellSize = CELL_SIZE;
    blocks.resize(cols, std::vector<Block>(rows));
    std::vector<int> surfaceYs(cols, 0);
    droppedItems = {};

    for (int y = 0; y < rows; y++)
    {
        std::vector<Block> row = noiseGen.GenerateRow(y, cols, 0.08f);
        for (int x = 0; x < cols; x++)
        {
            blocks[x][y] = row[x];
            blocks[x][y].SetPos({ (float)x * cellSize, (float)y * cellSize });

            if (row[x].GetTopType() == GRASS)
                surfaceYs[x] = y;
        }
    }

    noiseGen.GenerateCaves(*this);

    for (int x = 0; x < cols; x++)
    {
        int surfaceY = surfaceYs[x];
        if (blocks[x][surfaceY].GetTopType() == GRASS)
        {
            if (rand() % 10 == 0)
            {
                noiseGen.GenerateTree(*this, x, surfaceY);
            }
        }
    }
}

void Map::Draw(Vector2 playerPos)
{
    const float screenW = (float)GetScreenWidth();
    const float screenH = (float)GetScreenHeight();

    int startX = (int)floorf((playerPos.x - screenW / 2.0f) / (float)cellSize);
    int endX   = (int)ceilf((playerPos.x + screenW / 2.0f) / (float)cellSize);
    int startY = (int)floorf((playerPos.y - screenH / 2.0f) / (float)cellSize);
    int endY   = (int)ceilf((playerPos.y + screenH / 2.0f) / (float)cellSize);

    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;
    if (endX > cols) endX = cols;
    if (endY > rows) endY = rows;

    int fadeDistance = 12;

    for (int x = startX; x < endX; x++)
    {
        int groundY = -1;
        for (int y = 0; y < rows; y++)
        {
            if (blocks[x][y].GetTopType() == GRASS) {
                groundY = y;
                break;
            }
        }

        for (int y = startY; y < endY; y++)
        {
            Block& block = blocks[x][y];

            float darkness = 0.0f;
            if (groundY != -1 && y > groundY)
            {
                darkness = (float)(y - groundY) / (float)fadeDistance;
                if (darkness > 1.0f) darkness = 1.0f;
            }
            block.darknessMeter = darkness;

            if (block.GetTopType() != AIR && block.GetTopType() != TREE_CAP)
            {
                block.Draw();
            }
        }
    }

    for (int x = startX; x < endX; x++)
    {
        for (int y = startY; y < endY; y++)
        {
            if (blocks[x][y].GetTopType() == TREE_CAP)
            {
                blocks[x][y].Draw();
            }
        }
    }
    DrawItems();
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

Block& Map::GetBlock(int col, int row)
{
    if (col < 0) col = 0;
    if (col >= cols) col = cols - 1;
    if (row < 0) row = 0;
    if (row >= rows) row = rows - 1;
    return blocks[col][row];
}

void Map::SpawnItem(ItemID id, Vector2 pos)
{
    ItemEntity item;
    item.id = id;
    item.position = pos;
    item.velocity = {(float)(rand() % 100 - 50), (float)-(rand() % 150 + 100)};
    item.active = true;
    item.age = 0.0f;
    droppedItems.push_back(item);
}
void Map::UpdateItems()
{
    float dt = GetFrameTime();
    for (auto& item : droppedItems)
    {
        if (!item.active) continue;

        item.velocity.y += 600.0f * dt;
        Vector2 nextPos = {
            item.position.x + item.velocity.x * dt,
            item.position.y + item.velocity.y * dt
        };

        int col = (int)(nextPos.x / CELL_SIZE);
        int row = (int)(nextPos.y / CELL_SIZE);

        if (GetBlock(col, row).IsSolid()) {
            item.velocity.x *= 0.9f;
            item.velocity.y = 0.0f;
        } else {
            item.position = nextPos;
        }
        item.age += dt;
    }
}

std::vector<ItemID> Map::CollectItems(Vector2 collectorPos, float radius)
{
    std::vector<ItemID> pickedUp;
    for (auto& item : droppedItems)
    {
        if (!item.active) continue;

        if (CheckCollisionCircles(item.position, 5, collectorPos, radius))
        {
            item.active = false;
            pickedUp.push_back(item.id);
        }
    }
    return pickedUp;
}
void Map::DrawItems()
{
    for (const auto& item : droppedItems)
    {
        if (!item.active) continue;

        float bob = sinf(item.age * 5.0f) * 3.0f;

        DrawTextureEx(AssetManager::GetTexture(item.id), {item.position.x, item.position.y + bob}, 0.0f, 0.6f, WHITE);
    }
}


Map::~Map()
{
}