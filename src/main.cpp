#include <raylib.h>
#include <raymath.h>
#include <raygui.h>

#include "managers/AssetManager.h"
#include "world/Map.h"
#include "world/NoiseGen.hpp"
#include "entities/player/Player.h"
#include "defines/Defines.h"
#include "inventory/Inventory.h"

int main()
{
    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);
    InitWindow(640, 480, "Substratum");

    AssetManager::LoadAssets();

    NoiseGen noiseGen(0);
    Map map(noiseGen);
    Player player(map, 100.0f);

    Camera2D camera = { 0 };
    camera.zoom = 1.0f;

    player.SetCamera(&camera);

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        float sw = (float)GetScreenWidth();
        float sh = (float)GetScreenHeight();
        Vector2 screenCenter = { sw / 2.0f, sh / 2.0f };

        camera.offset = screenCenter;
        camera.target.x = Clamp(player.position.x, screenCenter.x, (COLS * CELL_SIZE) - screenCenter.x);
        camera.target.y = Clamp(player.position.y, screenCenter.y, (ROWS * CELL_SIZE) - screenCenter.y);

        player.Update(map);
        map.UpdateItems();


        BeginDrawing();
        ClearBackground(CYAN);


        Vector2 mouseScreen = GetMousePosition();
        Vector2 mouseWorld  = GetScreenToWorld2D(mouseScreen, camera);
        int mouseCol = static_cast<int>(mouseWorld.x / CELL_SIZE);
        int mouseRow = static_cast<int>(mouseWorld.y / CELL_SIZE);

        BeginMode2D(camera);
        map.Draw(camera.target);
        player.Draw();


        EndMode2D();

        DrawText(TextFormat("Screen Mouse: %.1f, %.1f", mouseScreen.x, mouseScreen.y),
         10, 10, 10, RED);

        DrawText(TextFormat("World Mouse: %.1f, %.1f", mouseWorld.x, mouseWorld.y),
                 10, 20, 10, RED);

        DrawText(TextFormat("Cell: %d, %d", mouseCol, mouseRow),
                 10, 30, 10, RED);

        DrawText(TextFormat("Player X: %.2f", player.position.x),
                 10, 40, 10, RED);

        DrawText(TextFormat("Player Y: %.2f", player.position.y),
                 10, 50, 10, RED);

        if (mouseCol >= 0 && mouseCol < COLS && mouseRow >= 0 && mouseRow < ROWS)
        {
            Block& block = map.GetBlock(mouseCol, mouseRow);

            DrawText(TextFormat("Block solid: %s", block.IsSolid() ? "Yes" : "No"),
                     10, 60, 10, RED);

            DrawText(TextFormat("Texture ptr: %p", (void*)block.GetTopType()),
                     10, 70, 10, RED);

            if (!block.GetTopType() == AIR)
            {
                DrawText(TextFormat("Block durability: %d", block.durability),
                     10, 80, 10, RED);
            }
            else
            {
                DrawText(TextFormat("Block durability: %d", 0),
                     10, 80, 10, RED);
            }

        }
        DrawFPS(550, 10);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}