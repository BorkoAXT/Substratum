#include <raylib.h>
#include "Map.h"
#include "thirdparty/PerlinNoise.hpp"

int main()
{
    InitWindow(750, 450, "Map Example");

    siv::PerlinNoise perlinNoise(rand());

    Image dirt = LoadImage("../images/dirtBlock.jpg");
    Image iron = LoadImage("../images/ironOre.jpg");
    ImageResize(&dirt, 25, 25);
    ImageResize(&iron, 25, 25);
    Texture2D irontex = LoadTextureFromImage(iron);
    Texture2D tex = LoadTextureFromImage(dirt);
    UnloadImage(dirt);

    Map map(18, 30, 25, tex, irontex, perlinNoise);

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(GREEN);

        map.Draw();

        EndDrawing();
    }

    UnloadTexture(tex);
    CloseWindow();
}