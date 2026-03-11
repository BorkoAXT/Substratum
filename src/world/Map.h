#pragma once
#include <vector>
#include "blocks/Block.h"
#include "../thirdparty/PerlinNoise.hpp"

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
