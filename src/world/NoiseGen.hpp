#pragma once
#include "../thirdparty/PerlinNoise.hpp"
#include "../blocks/Block.h"
#include "../defines/Defines.h"
#include <vector>
#include <cstdint>

class Map;

class NoiseGen
{
public:
    NoiseGen(uint32_t seed = 0);
    void GenerateFullMap(Map& map);
    Block GenerateBlock(int x, int y, float scale, float threshold, Texture2D texture, TileType type);
    int GetSurfaceLevel(int x, int baseHeight = 14, float scale = 0.15f, int heightRange = 10);
    std::vector<Block> GenerateRow(int y, int width, float scale);
    float GetNoise2D(int x, int y, float scale = 0.1f);
    float GetNoise1D(int x, float scale = 0.15f);
    void GenerateTree(Map& map, int xSurface, int surfaceY);
    void GenerateCaves(Map& map);

private:
    siv::PerlinNoise noise;
};