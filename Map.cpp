#include "Map.h"

#include "thirdparty/PerlinNoise.hpp"

Map::Map(int rows, int cols, int cellSize, Texture2D blockTexture, Texture2D ironTexture, siv::PerlinNoise& noise)
{
    this->rows = rows;
    this->cols = cols;
    this->cellSize = cellSize;

    for (int x = 0; x < cols; x++)
    {
        double n = noise.noise1D(x * 0.15);
        int surface = 7 + (int)(n * 5);

        for (int y = surface; y < rows; y++)
        {
            Block b;
            b.SetPos({ (float)x * cellSize, (float)y * cellSize });

            if (y == surface)
            {
                b.SetTexture(blockTexture);
            }
            else
            {
                double oreNoise = (noise.noise2D(x * 0.1, y * 0.1) + 1.0) / 2.0;

                if (oreNoise > 0.75)
                    b.SetTexture(ironTexture);
                else
                    b.SetTexture(blockTexture);
            }

            blocks.push_back(b);
        }
    }
};

void Map::Draw()
{
    for (Block &block : blocks)
    {
        block.Draw();
    }
}

Block& Map::GetBlock(int row, int col)
{
    return blocks[row * cols + col];
}
