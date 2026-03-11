#pragma once
#include "../thirdparty/PerlinNoise.hpp"
#include "../blocks/Block.h"
#include "../defines/Defines.h"
#include <bits/stdc++.h>
#include <vector>

#include "Map.h"

class Map;

class NoiseGen
{
public:
    NoiseGen(uint32_t seed = 0);

    Block GenerateBlock(int x, int y, float scale, float threshold, Texture2D texture, TileType type);


    int GetSurfaceLevel(int x, int baseHeight = 14, float scale = 0.15f, int heightRange = 10);

    std::vector<Block> GenerateRow(int y, int width, float scale, const std::vector<Texture2D>& textures);


    float GetNoise2D(int x, int y, float scale = 0.1f);

    float GetNoise1D(int x, float scale = 0.15f);

    void GenerateTree(Map& map, int xSurface);

    void GenerateCaves(Map& map);


private:
    siv::PerlinNoise noise;
};
