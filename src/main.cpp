#include <raylib.h>
#include <raymath.h>

#include "managers/AssetManager.h"
#include "world/Map.h"
#include "world/NoiseGen.hpp"
#include "entities/player/Player.h"
#include "defines/Defines.h"

int main()
{
    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);
    InitWindow(640, 480, "Mineclone");

    AssetManager::LoadAssets();

    NoiseGen noiseGen(0);
    Map map(noiseGen);
    Player player(map, 100.0f);

    Camera2D camera = { 0 };
    camera.zoom = 1.0f;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        float sw = (float)GetScreenWidth();
        float sh = (float)GetScreenHeight();
        Vector2 screenCenter = { sw / 2.0f, sh / 2.0f };

        player.Update(map);

        camera.offset = screenCenter;
        camera.target.x = Clamp(player.position.x, screenCenter.x, (COLS * CELL_SIZE) - screenCenter.x);
        camera.target.y = Clamp(player.position.y, screenCenter.y, (ROWS * CELL_SIZE) - screenCenter.y);

        BeginDrawing();
        ClearBackground(CYAN);

        BeginMode2D(camera);
        map.Draw(camera.target);
        player.Draw();

        DrawText(TextFormat("X: %.2f", player.position.x), (int)player.position.x + 30, (int)player.position.y - 20, 10, RED);
        DrawText(TextFormat("Y: %.2f", player.position.y), (int)player.position.x + 30, (int)player.position.y - 10, 10, RED);
        EndMode2D();

        DrawFPS(10, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}