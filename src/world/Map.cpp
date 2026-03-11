#include "Map.h"
#include "NoiseGen.hpp"
#include "../blocks/Block.h"
#include "../defines/Defines.h"
#include "managers/AssetManager.h"
#include "../thirdparty/raylib-5.5/src/raylib.h"
#include <vector>
#include <cmath>

Map::Map(NoiseGen& noiseGen)
{
    rows = ROWS;
    cols = COLS;
    cellSize = CELL_SIZE;

    blocks.resize(cols, std::vector<Block>(rows));

    for (int x = 0; x < cols; x++)
    {
        int surfaceY = noiseGen.GetSurfaceLevel(x);

        for (int y = 0; y < rows; y++)
        {
            Block b;
            b.SetPos({ (float)x * cellSize, (float)y * cellSize });

            if (y > surfaceY)
            {
                float oreChance = noiseGen.GetNoise2D(x, y, 0.1f);

                if (oreChance > 0.75f)
                {
                    b.SetType(IRON);
                    b.SetTexture(AssetManager::GetTexture("iron"));
                }
                else
                {
                    b.SetType(DIRT);
                    b.SetTexture(AssetManager::GetTexture("dirt"));
                }
            }
            else if (y == surfaceY)
            {
                b.SetType(GRASS);
                b.SetTexture(AssetManager::GetTexture("grass"));
            }
            else
            {
                b.SetType(AIR);
            }

            blocks[x][y] = b;
        }
    }
    noiseGen.GenerateCaves(*this);

    for (int x = 0; x < cols; x++)
    {
        int surfaceY = GetSurfaceLevel(x);

        if (blocks[x][surfaceY].GetType() == GRASS)
        {
            if (rand() % 10 == 0)
            {
                noiseGen.GenerateTree(*this, x);
            }
        }
    }
}

void Map::Draw(Vector2 playerPos)
{
    const float screenW = (float)GetScreenWidth();
    const float screenH = (float)GetScreenHeight();

    float worldLeft   = playerPos.x - (screenW / 2.0f);
    float worldRight  = playerPos.x + (screenW / 2.0f);
    float worldTop    = playerPos.y - (screenH / 2.0f);
    float worldBottom = playerPos.y + (screenH / 2.0f);

    int startX = (int)floorf(worldLeft / (float)cellSize);
    int endX   = (int)ceilf(worldRight / (float)cellSize);
    int startY = (int)floorf(worldTop / (float)cellSize);
    int endY   = (int)ceilf(worldBottom / (float)cellSize);

    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;
    if (endX > cols) endX = cols;
    if (endY > rows) endY = rows;

    int fadeDistance = 12;

for (int x = startX; x < endX; x++)
{
    int groundY = -1;
    for (int y = startY; y < endY; y++)
    {
        Block& block = blocks[x][y];

        if (groundY == -1 && block.GetType() == GRASS)
            groundY = y;

        float darkness = 0.0f;
        if (groundY != -1 && y > groundY)
        {
            darkness = (float)(y - groundY) / (float)fadeDistance;
            if (darkness > 1.0f) darkness = 1.0f;
        }
        block.darknessMeter = darkness;

        if (block.GetType() != TREE_CAP)
        {
            block.Draw();
        }
    }
}

for (int x = startX; x < endX; x++)
{
    for (int y = startY; y < endY; y++)
    {
        if (blocks[x][y].GetType() == TREE_CAP)
        {
            blocks[x][y].Draw();
        }
    }
}
}

int Map::GetSurfaceLevel(int col)
{
    if (col < 0) col = 0;
    if (col >= cols) col = cols - 1;

    for (int y = 0; y < rows; y++)
    {
        if (blocks[col][y].GetType() != AIR)
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

Map::~Map()
{
    for (Texture2D& tex : textures)
        UnloadTexture(tex);
}