#include "Render.h"
#include "raylib.h"
#include "raymath.h"

void Render::DrawWorld(Map& map, Player& player, Camera2D& camera) {
    const float screenW = (float)GetScreenWidth();
    const float screenH = (float)GetScreenHeight();
    const float cellSize = (float)map.GetCellSize();

    Vector2 topLeft = GetScreenToWorld2D({ 0, 0 }, camera);
    Vector2 bottomRight = GetScreenToWorld2D({ screenW, screenH }, camera);

    int startX = (int)floorf(topLeft.x / cellSize);
    int endX   = (int)ceilf(bottomRight.x / cellSize);
    int startY = (int)floorf(topLeft.y / cellSize);
    int endY   = (int)ceilf(bottomRight.y / cellSize);

    // Boundary checks
    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;
    if (endX > map.GetCols()) endX = map.GetCols();
    if (endY > map.GetRows()) endY = map.GetRows();

    const int fadeDistance = 12;
    const float playerLightRadius = 200.0f;

    for (int x = startX; x < endX; x++) {
        int firstSolidY = -1;
        for (int y = 0; y < map.GetRows(); y++) {
            if (map.GetBlock(y, x).IsSolid()) {
                firstSolidY = y;
                break;
            }
        }

        for (int y = startY; y < endY; y++) {
            Block& block = map.GetBlock(y, x);
            if (block.GetTopType() == AIR || block.GetTopType() == TREE_CAP) continue;

            float envDarkness = 0.0f;
            if (firstSolidY != -1 && y >= firstSolidY) {
                envDarkness = (float)(y - firstSolidY) / (float)fadeDistance;
                if (envDarkness > 1.0f) envDarkness = 1.0f;
            }

            Vector2 blockPos = { x * cellSize + cellSize / 2.0f, y * cellSize + cellSize / 2.0f };
            float distToPlayer = Vector2Distance(player.position, blockPos);

            float lightBoost = 1.0f - (distToPlayer / playerLightRadius);
            if (lightBoost < 0) lightBoost = 0;

            block.darknessMeter = envDarkness * (1.0f - lightBoost);

            if (distToPlayer < 45.0f && block.darknessMeter > 0.3f) {
                block.darknessMeter = 0.3f;
            }

            block.Draw();
        }
    }

    for (int x = startX; x < endX; x++) {
        for (int y = startY; y < endY; y++) {
            if (map.GetBlock(y, x).GetTopType() == TREE_CAP) {
                map.GetBlock(y, x).Draw();
            }
        }
    }

    map.DrawItems();
}
void Render::Read()
{
}
