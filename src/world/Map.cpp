#include "Map.h"
#include <fstream>
#include "defines/Defines.h"
#include "defines/Assets.h"
#include "blocks/Block.h"
#include "../thirdparty/PerlinNoise.hpp"
#include "../thirdparty/raylib-5.5/src/raylib.h"

Map::Map(siv::PerlinNoise& noise)
{
    rows = ROWS;
    cols = COLS;
    cellSize = CELL_SIZE;

    Texture2D dirtTexture = LoadTexture(Assets::DIRT_PATH);
    Texture2D ironTexture = LoadTexture(Assets::IRON_PATH);
    Texture2D grassTexture = LoadTexture(Assets::GRASS_PATH);

    textures.push_back(dirtTexture);
    textures.push_back(ironTexture);
    textures.push_back(grassTexture);

blocks.resize(cols, std::vector<Block>(rows));

    std::ofstream stream("world/World.txt");

    for (int y = 0; y < rows; y++)
    {
        for (int x = 0; x < cols; x++)
        {
            double n = noise.noise1D(x * 0.15);
            int surface = 14 + (int)(n * 10);

            char tileChar = '0';

            if (y >= surface)
            {
                Block b;
                b.SetPos({ (float)x * cellSize, (float)y * cellSize });

                if (y == surface)
                {
                    b.SetTexture(grassTexture);
                    b.SetType(GRASS);
                    tileChar = '2';
                }
                else
                {
                    double oreNoise = (noise.noise2D(x * 0.1, y * 0.1) + 1.0) / 2.0;
                    if (oreNoise > 0.75)
                    {
                        b.SetTexture(ironTexture);
                        b.SetType(IRON);
                        tileChar = '3';
                    }
                    else
                    {
                        b.SetTexture(dirtTexture);
                        b.SetType(DIRT);
                        tileChar = '1';
                    }
                }
                blocks[x][y] = b;
            }

            stream << tileChar;
        }
        stream << "\n";
    }
    stream.close();
}

void Map::Draw(Vector2 playerPos)
{
    const float screenW = GetScreenWidth();
    const float screenH = GetScreenHeight();

    float worldLeft   = playerPos.x - (screenW / 2.0f);
    float worldRight  = playerPos.x + (screenW / 2.0f);
    float worldTop    = playerPos.y - (screenH / 2.0f);
    float worldBottom = playerPos.y + (screenH / 2.0f);

    int startX = (int)floorf(worldLeft / 25.0f);
    int endX   = (int)ceilf(worldRight / 25.0f);

    int startY = (int)floorf(worldTop / 25.0f);
    int endY   = (int)ceilf(worldBottom / 25.0f);

    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;
    if (endX > cols) endX = cols;
    if (endY > rows) endY = rows;

    int fadeDistance = 12;

    for (int x = startX; x < endX; x++)
    {
        for (int y = startY; y < endY; y++)
        {
            if (blocks[x][y].GetType() != GRASS) continue;

            for (int i = 0; i < 3; i++)
            {
                if (y + i < rows) {
                    blocks[x][y + i].darknessMeter = 0.0f;
                    blocks[x][y + i].Draw();
                }
            }

            for (int yDark = y + 3; yDark < endY; yDark++)
            {
                float blockDepth = (float)(yDark - (y + 3));
                float darknessMeter = blockDepth / (float)fadeDistance;

                if (darknessMeter > 1.0f) darknessMeter = 1.0f;
                if (darknessMeter < 0.0f) darknessMeter = 0.0f;

                blocks[x][yDark].darknessMeter = darknessMeter;
                blocks[x][yDark].Draw();
            }
            break;
        }
    }

}

Block& Map::GetBlock(int row, int col)
{
   return blocks[row][col];
}
Map::~Map()
{
    for (Texture2D& texture : textures)
    {
        UnloadTexture(texture);
    }
}
