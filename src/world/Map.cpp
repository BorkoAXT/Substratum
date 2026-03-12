#include "Map.h"
#include "NoiseGen.hpp"
#include "../blocks/Block.h"
#include "../defines/Defines.h"
#include "managers/AssetManager.h"
#include <vector>
#include <cmath>

Map::Map(NoiseGen& noiseGen)
{
    rows = ROWS;
    cols = COLS;
    cellSize = CELL_SIZE;
    blocks.resize(cols, std::vector<Block>(rows));
    std::vector<int> surfaceYs(cols, 0);

    for (int y = 0; y < rows; y++)
    {
        std::vector<Block> row = noiseGen.GenerateRow(y, cols, 0.1f);
        for (int x = 0; x < cols; x++)
        {
            blocks[x][y] = row[x];
            if (row[x].GetType() == GRASS)
                surfaceYs[x] = y;
        }
    }

    noiseGen.GenerateCaves(*this);

    for (int x = 0; x < cols; x++)
    {
        int surfaceY = surfaceYs[x];
        if (blocks[x][surfaceY].GetType() == GRASS)
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
            if (blocks[x][y].GetType() == GRASS) {
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

            if (block.GetType() != AIR && block.GetType() != TREE_CAP)
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