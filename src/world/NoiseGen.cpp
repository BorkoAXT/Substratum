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

void NoiseGen::GenerateFullMap(Map& map)
{
    int rows = map.GetRows();
    int cols = map.GetCols();
    float cellSize = (float)map.GetCellSize();

    for (int y = 0; y < rows; y++)
    {
        std::vector<Block> rowData = GenerateRow(y, cols, 0.08f);
        for (int x = 0; x < cols; x++)
        {
            Block& mapBlock = map.GetBlock(y, x);
            mapBlock = rowData[x];
            mapBlock.SetPos({ (float)x * cellSize, (float)y * cellSize });
        }
    }

    GenerateCaves(map);

    for (int x = 0; x < cols; x++)
    {
        bool hasSurface = false;
        int firstSolidUnderAir = -1;

        for (int y = 0; y < rows; y++)
        {
            if (map.GetBlock(y, x).GetTopType() == GRASS)
            {
                hasSurface = true;
                break;
            }
            if (firstSolidUnderAir == -1 && map.GetBlock(y, x).IsSolid())
            {
                firstSolidUnderAir = y;
            }
        }

        if (!hasSurface && firstSolidUnderAir != -1)
        {
            map.GetBlock(firstSolidUnderAir, x).ClearAll();
            map.GetBlock(firstSolidUnderAir, x).AddLayer(GRASS, AssetManager::GetTexture("grass"));
        }
    }

    for (int x = 0; x < cols; x++)
    {
        int surfaceY = -1;
        for (int y = 0; y < rows; y++)
        {
            if (map.GetBlock(y, x).GetTopType() == GRASS)
            {
                surfaceY = y;
                break;
            }
        }

        if (surfaceY != -1 && surfaceY < rows)
        {
            if (rand() % 10 == 0)
            {
                GenerateTree(map, x, surfaceY);
            }
        }
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

void NoiseGen::GenerateTree(Map& map, int xSurface, int surfaceY)
{
    if (xSurface <= 1 || xSurface >= COLS - 2) return;

    int treeHeight = 3 + rand() % 2;
    int count = 1;

    map.GetBlock(surfaceY - count, xSurface).AddLayer(TREE_TRUNK, AssetManager::GetTexture("tree_trunk"));
    count++;
    for (int i = 0; i < treeHeight / 2; i++)
    {
        map.GetBlock(surfaceY - count, xSurface).AddLayer(TREE_PART, AssetManager::GetTexture("tree_part2"));
        count++;
        map.GetBlock(surfaceY - count, xSurface).AddLayer(TREE_PART, AssetManager::GetTexture("tree_part3"));
        count++;
    }
    map.GetBlock(surfaceY - count, xSurface).AddLayer(TREE_CAP, AssetManager::GetTexture("tree_cap"));
}

void NoiseGen::GenerateCaves(Map& map)
{
    float caveScale = 0.04f;
    float baseThreshold = 0.85f;
    int caveStartOffset = 15;

    for (int x = 0; x < map.GetCols(); x++)
    {
        int surface = GetSurfaceLevel(x);
        for (int y = 0; y < map.GetRows(); y++)
        {
            if (y < surface + caveStartOffset) continue;

            float depthFactor = (float)(y - surface) / (float)map.GetRows();
            float currentThreshold = baseThreshold - (depthFactor * 0.2f);

            float n = GetNoise2D(x, y, caveScale);

            if (n > currentThreshold)
            {
                map.GetBlock(y, x).ClearAll();
            }
        }
    }

    for (int x = 0; x < map.GetCols(); x++)
    {
        for (int y = 1; y < map.GetRows() - 1; y++)
        {
            Block& b = map.GetBlock(y, x);
            if (b.GetTopType() == GRASS || b.GetTopType() == DIRT)
            {
                if (map.GetBlock(y + 1, x).GetTopType() == AIR)
                {
                    b.ClearAll();
                }
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
            if (plantNoise > 0.75f) b.AddLayer(YELLOW_FLOWER, AssetManager::GetTexture("yellow_flower"));
            else if (plantNoise > 0.60f) b.AddLayer(GRASS_LEAVES_1, AssetManager::GetTexture("grass_leaves_1"));
            else if (plantNoise > 0.45f) b.AddLayer(GRASS_LEAVES_2, AssetManager::GetTexture("grass_leaves_2"));
        }
        else if (y > surface)
        {
            float oreNoise = GetNoise2D(x, y, scale);
            if (oreNoise > 0.86f && y > surface + 20) b.AddLayer(SAPPHIRE, AssetManager::GetTexture("sapphire"));
            else if (oreNoise > 0.82f && y > surface + 12) b.AddLayer(RUBY, AssetManager::GetTexture("ruby"));
            else if (oreNoise > 0.78f && y > surface + 5) b.AddLayer(COAL, AssetManager::GetTexture("coal"));
            else
            {
                if (y > surface + 5) b.AddLayer(STONE, AssetManager::GetTexture("stone"));
                else b.AddLayer(DIRT, AssetManager::GetTexture("dirt"));
            }
        }
        rowBlocks.push_back(b);
    }
    return rowBlocks;
}