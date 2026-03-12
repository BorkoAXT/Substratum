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
    if (n > threshold) {
        b.SetType(type);
        b.SetTexture(texture);
    } else {
        b.SetType(DIRT);
    }

    return b;
}
void NoiseGen::GenerateTree(Map& map, int xSurface)
{
    if (xSurface <= 1 || xSurface >= COLS - 1) return;

    int sY = GetSurfaceLevel(xSurface);

    if (map.GetBlock(xSurface, sY).GetType() == AIR) return;

    map.GetBlock(xSurface, sY - 1).SetType(TREE_TRUNK);
    map.GetBlock(xSurface, sY - 1).SetTexture(AssetManager::GetTexture("tree_trunk"));

    map.GetBlock(xSurface, sY - 2).SetType(TREE_PART);
    map.GetBlock(xSurface, sY - 2).SetTexture(AssetManager::GetTexture("tree_part2"));

    map.GetBlock(xSurface, sY - 3).SetType(TREE_PART);
    map.GetBlock(xSurface, sY - 3).SetTexture(AssetManager::GetTexture("tree_part3"));

    map.GetBlock(xSurface + 1, sY - 2).SetType(TREE_LEAVES);
    map.GetBlock(xSurface + 1, sY - 2).SetTexture(AssetManager::GetTexture("tree_leaves_second"));

    map.GetBlock(xSurface - 1, sY - 3).SetType(TREE_LEAVES);
    map.GetBlock(xSurface - 1, sY - 3).SetTexture(AssetManager::GetTexture("tree_leaves_third"));

    map.GetBlock(xSurface, sY - 4).SetType(TREE_CAP);
    map.GetBlock(xSurface, sY - 4).SetTexture(AssetManager::GetTexture("tree_cap"));
}

void NoiseGen::GenerateCaves(Map& map)
{
    float caveScale = 0.05f;

    float caveThreshold = 0.7f;

    for (int x = 0; x < COLS; x++)
    {
        int surface = GetSurfaceLevel(x);

        for (int y = 0; y < ROWS; y++)
        {
            float n = GetNoise2D(x, y, caveScale);

            if (y > surface + 5 && n > caveThreshold)
            {
                map.GetBlock(x, y).SetTexture(AssetManager::GetTexture("stone_background"));
                map.GetBlock(x, y).SetType(BACKGROUND);
            }
            else if (y >= surface && y <= surface + 5 && n > 0.8f)
            {
                map.GetBlock(x, y).SetTexture(AssetManager::GetTexture("stone_background"));
                map.GetBlock(x, y).SetType(BACKGROUND);

                if (y > 0 && map.GetBlock(x, y - 1).GetType() == GRASS)
                    map.GetBlock(x, y).SetTexture(AssetManager::GetTexture("stone_background"));
                map.GetBlock(x, y).SetType(BACKGROUND);
            }
        }
    }
}

std::vector<Block> NoiseGen::GenerateRow(int y, int width, float scale, const std::vector<Texture2D>& textures)
{
    std::vector<Block> rowBlocks;
    rowBlocks.reserve(width);

    for (int x = 0; x < width; x++) {
        Block b;

        int surface = GetSurfaceLevel(x);
        if (y >= surface) {
            if (y == surface) {
                b.SetType(GRASS);
                b.SetTexture(textures[2]);
            } else {
                float oreNoise = GetNoise2D(x, y, scale);
                if (oreNoise > 0.75f) {
                    b.SetType(IRON);
                    b.SetTexture(textures[1]);
                } else {
                    b.SetType(DIRT);
                    b.SetTexture(textures[0]);
                }
            }
            b.SetPos({ (float)x * CELL_SIZE, (float)y * CELL_SIZE });
        }

        rowBlocks.push_back(b);
    }

    return rowBlocks;
}