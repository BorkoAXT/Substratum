#pragma once
#include <vector>
#include "Block.h"
#include "thirdparty/PerlinNoise.hpp"
#include "Assets.h"

class Map
{
public:
    Map(siv::PerlinNoise& noise);

    void Draw(Vector2 pos);
    Block& GetBlock(int row, int col);
    virtual ~Map();

private:
    int rows;
    int cols;
    int cellSize;
    std::vector<Texture2D> textures;
    std::vector<std::vector<Block>> blocks;
};
