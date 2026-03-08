#include <raylib.h>
#include "Map.h"
#include "thirdparty/PerlinNoise.hpp"
#include "Player.h"
#define CYAN {135, 206, 235}
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

    Camera2D camera = {0};
    camera.offset = {375, 225};
    camera.target = {0,0};
    camera.zoom = 1.0f;


    Map map(70, 80, 25, tex, irontex, perlinNoise);
    Player player;

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        player.Update();
        camera.target = player.position;
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