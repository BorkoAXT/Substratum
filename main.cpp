#include <raylib.h>
#include "Map.h"
#include "thirdparty/PerlinNoise.hpp"
#include "Player.h"
#include <fstream>
#include <raymath.h>
#define CYAN {135, 206, 235}
#define ROWS 1800
#define COLS 6400
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

    Player player;

    Camera2D camera = {0};
    camera.offset = {375, 225};
    camera.target.x = Clamp(player.position.x, 375, 5000 - 375);
    camera.target.y = Clamp(player.position.y, 225, 2000 - 225);
    camera.zoom = 1.0f;

    // std::ofstream stream("../WorldFiles/World.txt");
    // for (int i = 0; i < ROWS; i++)
    // {
    //     for (int j = 0; j < COLS; j++)
    //     {
    //         stream << 0;
    //     }
    //     stream << std::endl;
    // }



    Map map(80, 200, 25, tex, irontex, perlinNoise);

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        player.Update();
        camera.target.x = Clamp(player.position.x, 375, 5000 - 375);
        camera.target.y = Clamp(player.position.y, 225, 2000 - 225);
        BeginDrawing();
        ClearBackground(CYAN);
        BeginMode2D(camera);

        map.Draw();
        player.Draw();

        EndMode2D();

        EndDrawing();
    }

    UnloadTexture(tex);
    CloseWindow();
}