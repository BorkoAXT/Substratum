#include "NoiseGen.hpp"
#include <chrono>
#include "Map.h"
#include "defines/Assets.h"
#include "managers/AssetManager.h"

NoiseGen::NoiseGen(uint32_t seed)
{
    if (seed == 0) {
        uint32_t randomSeed = static_cast<uint32_t>(
            std::chrono::system_clock::now().time_since_epoch().count() & 0xffffffff
        );
        noise = siv::PerlinNoise(randomSeed);
    } else {
        noise = siv::PerlinNoise(seed);
    }
}

float NoiseGen::GetNoise2D(int x, int y, float scale)
{
    double val = noise.noise2D(x * scale, y * scale);
    return static_cast<float>((val + 1.0) / 2.0);
}

float NoiseGen::GetNoise1D(int x, float scale)
{
    double val = noise.noise1D(x * scale);
    return static_cast<float>((val + 1.0) / 2.0);
}

int NoiseGen::GetSurfaceLevel(int x, int baseHeight, float scale, int heightRange)
{
    float n = GetNoise1D(x, scale);
    return baseHeight + static_cast<int>(n * heightRange);
}

Block NoiseGen::GenerateBlock(int x, int y, float scale, float threshold, Texture2D texture, TileType type)
{
    Block b;
    b.SetPos({ (float)x * CELL_SIZE, (float)y * CELL_SIZE });
    float n = GetNoise2D(x, y, scale);

    if (n > threshold)
    {
        b.AddLayer(type, texture);
    }
    else
    {
        b.AddLayer(DIRT, AssetManager::GetTexture("dirt"));
    }
    return b;
}

void NoiseGen::GenerateTree(Map& map, int xSurface, int surfaceY)
{
    if (xSurface <= 1 || xSurface >= COLS - 2) return;
    int sY = surfaceY;

    int treeHeight = 3 + rand() % 2;
    int count = 2;

    map.GetBlock(xSurface, sY - 1).AddLayer(TREE_TRUNK, AssetManager::GetTexture("tree_trunk"));

    for (int i = 0; i < treeHeight / 2; i++)
    {
        map.GetBlock(xSurface, sY - count).AddLayer(TREE_PART, AssetManager::GetTexture("tree_part2"));
        count++;
        map.GetBlock(xSurface, sY - count).AddLayer(TREE_PART, AssetManager::GetTexture("tree_part3"));
        count++;
    }

    map.GetBlock(xSurface, sY - count).AddLayer(TREE_CAP, AssetManager::GetTexture("tree_cap"));
}

void NoiseGen::GenerateCaves(Map& map)
{
    float caveScale = 0.05f;
    float caveThreshold = 0.8f;

    for (int x = 0; x < COLS; x++)
    {
        int surface = GetSurfaceLevel(x);

        for (int y = 0; y < ROWS; y++)
        {
            float n = GetNoise2D(x, y, caveScale);

            if (y >= surface && n > caveThreshold)
            {
                map.GetBlock(x, y).ClearAll();
            }
        }
    }
}

std::vector<Block> NoiseGen::GenerateRow(int y, int width, float scale)
{
    std::vector<Block> rowBlocks;
    rowBlocks.reserve(width);

    for (int x = 0; x < width; x++)
    {
        Block b;
        int surface = GetSurfaceLevel(x);

        b.SetPos({ (float)x * CELL_SIZE, (float)y * CELL_SIZE });

        if (y == surface)
        {
            b.AddLayer(GRASS, AssetManager::GetTexture("grass"));
        }
        else if (y == surface - 1)
        {
            float plantNoise = GetNoise2D(x, y, 0.25f);

            if (plantNoise > 0.75f) // increase flowers
            {
                b.AddLayer(YELLOW_FLOWER, AssetManager::GetTexture("yellow_flower"));
                b.durability = 1;
            }
            else if (plantNoise > 0.60f) // less grass
            {
                b.AddLayer(GRASS_LEAVES_1, AssetManager::GetTexture("grass_leaves_1"));
                b.durability = 1;
            }
            else if (plantNoise > 0.45f)
            {
                b.AddLayer(GRASS_LEAVES_2, AssetManager::GetTexture("grass_leaves_2"));
                b.durability = 1;
            }
        }
        else if (y > surface)
        {
            float oreNoise = GetNoise2D(x, y, scale);

            if (oreNoise > 0.86f && y > surface + 20)
                b.AddLayer(SAPPHIRE, AssetManager::GetTexture("sapphire"));
            else if (oreNoise > 0.82f && y > surface + 12)
                b.AddLayer(RUBY, AssetManager::GetTexture("ruby"));
            else if (oreNoise > 0.78f && y > surface + 5)
                b.AddLayer(COAL, AssetManager::GetTexture("coal"));
            else
            {
                if (y > surface + 5)
                    b.AddLayer(STONE, AssetManager::GetTexture("stone"));
                else
                    b.AddLayer(DIRT, AssetManager::GetTexture("dirt"));
            }
        }

        rowBlocks.push_back(b);
    }

    return rowBlocks;
}