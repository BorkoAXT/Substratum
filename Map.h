#pragma once
#include <vector>
#include "Block.h"
#include "thirdparty/PerlinNoise.hpp"

class Map
{
public:
    Map(int rows, int cols, int cellSize, Texture2D blockTexture, Texture2D ironTexture, siv::PerlinNoise& noise);

    void Draw();
    Block& GetBlock(int row, int col);

private:
    int rows;
    int cols;
    int cellSize;
    std::vector<Block> blocks;
};
